#ifndef _IS32FL3237_H
#define _IS32FL3237_H

#include <Arduino.h>
#include <Wire.h>

class IS32FL3237
{
private:
    uint8_t i2c_addr;

public:
    void begin();

    // Set the 7-bit I2C address. Only two bits can be varied, and AD (pin 40) must be connected appropriately. See datasheet
    // AD connected to:         2 LS bits of I2C address
    // GND                      00
    // VCC                      11
    // SCL                      01
    // SDA                      10
    void setI2CAddr(uint8_t addr);

    uint8_t readRegister(uint8_t addr);
    void writeRegister(uint8_t addr, uint8_t val);
};

#endif //#ifndef _IS32FL3237_H
