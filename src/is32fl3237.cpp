#include "is32fl3237.h"

void IS32FL3237::begin()
{
    Wire.begin();
}

void IS32FL3237::setI2CAddr(uint8_t addr)
{
    i2c_addr = 0b0110100 | (addr & 0b11);
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