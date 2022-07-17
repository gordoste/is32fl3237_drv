#include "is32fl3237.h"

void IS32FL3237::begin(ad_conn_t ad, pwm_res_t res, osc_freq_t of, bool autoUpdate)
{
    Wire.begin();
    m_i2c_addr = 0b0110100 | (ad & 0b11);
    uint8_t ctl = 0;
    switch (res) {
        case PWM_8BIT: break;
        case PWM_10BIT: ctl |= 0x02; break;
        case PWM_12BIT: ctl |= 0x04; break;
        case PWM_16BIT: ctl |= 0x06; break;
    }
    switch (of) {
        case OSC_16MHZ: break;
        case OSC_8MHZ: ctl |= 0x10; break;
        case OSC_1MHZ: ctl |= 0x20; break;
        case OSC_500KHZ: ctl |= 0x30; break;
        case OSC_250KHZ: ctl |= 0x40; break;
        case OSC_125KHZ: ctl |= 0x50; break;
        case OSC_62KHZ: ctl |= 0x60; break;
        case OSC_31KHZ: ctl |= 0x70; break;
    }
    writeRegister(CONTROL, ctl);
    m_autoUpdate = autoUpdate;
}

void IS32FL3237::setShutdown(bool x) {
    uint8_t i = readRegister(CONTROL);
    uint8_t j = x?(i&(~_CTL_SSD_MASK)):(i|_CTL_SSD_MASK);
    if (j != i) writeRegister(CONTROL, j);
}

void IS32FL3237::enablePWM(bool x) {
    uint8_t i = readRegister(SSR);
    uint8_t j = x?(i&(~_SSR_DCPWM_MASK)):(i|_SSR_DCPWM_MASK);
    if (j != i) writeRegister(SSR, j);
}

void IS32FL3237::updateLEDs() {
    writeRegister(UPDATE, 0);
}

uint8_t IS32FL3237::getBrightness8(uint8_t n)
{
    if (n<1 || n>36) return 0;
    return readRegister((n << 1) - 1);
}

uint16_t IS32FL3237::getBrightness(uint8_t n) {
    if (n<1 || n>36) return 0;
    return (readRegister(n << 1) << 8) & readRegister((n << 1) - 1);
}

void IS32FL3237::setBrightness8(uint8_t n, uint8_t v)
{
    if (n<1 || n>36) return;
    writeRegister((n << 1) - 1, v);
    if (m_autoUpdate) writeRegister(UPDATE, 0);
}

void IS32FL3237::setBrightness(uint8_t n, uint16_t v)
{
    if (n<1 || n>36) return;
    Wire.beginTransmission(m_i2c_addr);
    Wire.write((n << 1) - 1);
    Wire.write((uint8_t)(v & 0xFF));
    Wire.write((uint8_t)(v >> 8));
    Wire.endTransmission();
    if (m_autoUpdate) writeRegister(UPDATE, 0);
}

void IS32FL3237::setAllBrightness8(uint8_t v)
{
    Wire.beginTransmission(m_i2c_addr);
    Wire.write(PWML01);
    for (uint8_t l = 1; l <= 36; l++) {
        Wire.write(v);
        // Overwrite the high byte so we can do it in one I2C txn
        Wire.write(0); 
    }
    Wire.endTransmission();
}

void IS32FL3237::setAllBrightness(uint16_t v)
{
    Wire.beginTransmission(m_i2c_addr);
    Wire.write(PWML01);
    for (uint8_t l = 1; l <= 36; l++) {
        Wire.write((uint8_t)(v & 0xFF));
        Wire.write((uint8_t)(v >> 8));
    }
    Wire.endTransmission();
}

uint8_t IS32FL3237::getScaleFactor(uint8_t n) {
    if (n<1 || n>36) return 0;
    return readRegister(0x49+n);
}

void IS32FL3237::setScaleFactor(uint8_t n, uint8_t v)
{
    if (n<1 || n>36) return;
    writeRegister(0x49+n, v);
}

void IS32FL3237::setAllScaleFactors(uint8_t v)
{

    Wire.beginTransmission(m_i2c_addr);
    Wire.write(SL01);
    for (uint8_t l = SL01; l<=SL36; l++) Wire.write(v); // Write all in one I2C txn
    Wire.endTransmission();
}

void IS32FL3237::writeRegister(uint8_t addr, uint8_t val)
{
    Wire.beginTransmission(m_i2c_addr);
    Wire.write(addr);
    Wire.write(val);
    Wire.endTransmission();
}

uint8_t IS32FL3237::readRegister(uint8_t addr)
{
    Wire.beginTransmission(m_i2c_addr);
    Wire.write(addr);
    Wire.endTransmission();
    unsigned long t = millis();
    Wire.requestFrom(m_i2c_addr, (uint8_t)1);
    while (millis() - t < 1000 && !Wire.available())
        ;
    if (!Wire.available())
    {
        Serial.println("i2c read timeout");
        return 0;
    }
    return Wire.read();
}