#include "is32fl3237.h"

void IS32FL3237::begin(ad_conn_t ad)
{
    Wire.begin();
    i2c_addr = 0b0110100 | (ad & 0b11);
}

void IS32FL3237::setShutdown(bool x) {
    uint8_t i = readRegister(CONTROL);
    uint8_t j = i ^ (x?_CTL_SSD_MASK:0x00);
    if (j != i) writeRegister(CONTROL, j);
}

void IS32FL3237::writePWM8(uint8_t n, uint8_t v)
{
    if (n<1 || n>36) return;
    Wire.beginTransmission(i2c_addr);
    Wire.write((n << 1) - 1);
    Wire.write(v);
    Wire.endTransmission();
}

void IS32FL3237::writePWM16(uint8_t n, uint16_t v)
{
    if (n<1 || n>36) return;
    Wire.beginTransmission(i2c_addr);
    Wire.write((n << 1) - 1);
    Wire.write((uint8_t)(v & 0xFF));
    Wire.write((uint8_t)(v >> 8));
    Wire.endTransmission();
}

void IS32FL3237::writeScaleFactor(uint8_t n, uint8_t v)
{
    if (n<1 || n>36) return;
    Wire.beginTransmission(i2c_addr);
    Wire.write(0x49+n);
    Wire.write(v);
    Wire.endTransmission();
}

void IS32FL3237::writeRegister(uint8_t addr, uint8_t val)
{
    Wire.beginTransmission(i2c_addr);
    Wire.write(addr);
    Wire.write(val);
    Wire.endTransmission();
}

uint8_t IS32FL3237::readRegister(uint8_t addr)
{
    Wire.beginTransmission(i2c_addr);
    Wire.write(addr);
    Wire.endTransmission();
    unsigned long t = millis();
    Wire.requestFrom(i2c_addr, 1);
    while (millis() - t < 1000 && !Wire.available())
        ;
    if (!Wire.available())
    {
        Serial.println("i2c read timeout");
        return 0;
    }
    return Wire.read();
}