#ifndef _IS32FL3237_H
#define _IS32FL3237_H

#include <Arduino.h>
#include <Wire.h>

#define PWML01 0x01
#define PWMH01 0x02
#define SL01   0x4A
#define PWML02 0x03
#define PWMH02 0x04
#define SL02   0x4B
#define PWML03 0x05
#define PWMH03 0x06
#define SL03   0x4C
#define PWML04 0x07
#define PWMH04 0x08
#define SL04   0x4D
#define PWML05 0x09
#define PWMH05 0x0A
#define SL05   0x4E
#define PWML06 0x0B
#define PWMH06 0x0C
#define SL06   0x4F
#define PWML07 0x0D
#define PWMH07 0x0E
#define SL07   0x50
#define PWML08 0x0F
#define PWMH08 0x10
#define SL08   0x51
#define PWML09 0x11
#define PWMH09 0x12
#define SL09   0x52
#define PWML10 0x13
#define PWMH10 0x14
#define SL10   0x53
#define PWML11 0x15
#define PWMH11 0x16
#define SL11   0x54
#define PWML12 0x17
#define PWMH12 0x18
#define SL12   0x55
#define PWML13 0x19
#define PWMH13 0x1A
#define SL13   0x56
#define PWML14 0x1B
#define PWMH14 0x1C
#define SL14   0x57
#define PWML15 0x1D
#define PWMH15 0x1E
#define SL15   0x58
#define PWML16 0x1F
#define PWMH16 0x20
#define SL16   0x59
#define PWML17 0x21
#define PWMH17 0x22
#define SL17   0x5A
#define PWML18 0x23
#define PWMH18 0x24
#define SL18   0x5B
#define PWML19 0x25
#define PWMH19 0x26
#define SL19   0x5C
#define PWML20 0x27
#define PWMH20 0x28
#define SL20   0x5D
#define PWML21 0x29
#define PWMH21 0x2A
#define SL21   0x5E
#define PWML22 0x2B
#define PWMH22 0x2C
#define SL22   0x5F
#define PWML23 0x2D
#define PWMH23 0x2E
#define SL23   0x60
#define PWML24 0x2F
#define PWMH24 0x30
#define SL24   0x61
#define PWML25 0x31
#define PWMH25 0x32
#define SL25   0x62
#define PWML26 0x33
#define PWMH26 0x34
#define SL26   0x63
#define PWML27 0x35
#define PWMH27 0x36
#define SL27   0x64
#define PWML28 0x37
#define PWMH28 0x38
#define SL28   0x65
#define PWML29 0x39
#define PWMH29 0x3A
#define SL29   0x66
#define PWML30 0x3B
#define PWMH30 0x3C
#define SL30   0x67
#define PWML31 0x3D
#define PWMH31 0x3E
#define SL31   0x68
#define PWML32 0x3F
#define PWMH32 0x40
#define SL32   0x69
#define PWML33 0x41
#define PWMH33 0x42
#define SL33   0x6A
#define PWML34 0x43
#define PWMH34 0x44
#define SL34   0x6B
#define PWML35 0x45
#define PWMH35 0x46
#define SL35   0x6C
#define PWML36 0x47
#define PWMH36 0x48
#define SL36   0x6D

#define CONTROL 0x00
#define UPDATE  0x49
#define GCC     0x6E
#define OSDE    0x71
#define OSD1    0x72
#define OSD2    0x73
#define OSD3    0x74
#define OSD4    0x75
#define OSD5    0x76
#define TEMP    0x77
#define SSR     0x78
#define RESET   0x7F

#define _CTL_SSD_MASK   0x01
#define _CTL_PMS_MASK   0x06
#define _CTL_OSC_MASK   0x70

#define _PHASE_PDE_MASK 0x80
#define _PHASE_PSS_MASK 0x3F

#define _TEMP_TROF_MASK 0xC0
#define _TEMP_FLAG_MASK 0x10
#define _TEMP_TS_MASK   0x03

#define _SSR_DCPWM_MASK 0xE0
#define _SSR_SSP_MASK   0x10
#define _SSR_RNG_MASK   0x0C
#define _SSR_CLT_MASK   0x03

 // Sets the 7-bit I2C address depending where AD (pin 40) is connected. See datasheet
 // AD connected to:         2 LS bits of I2C address
 // GND                      00
 // VCC                      11
 // SCL                      01
 // SDA                      10
typedef enum {
    AD_GND  =   0,
    AD_VCC  =   3,
    AD_SCL  =   1,
    AD_SDA  =   2
} ad_conn_t;

// Sets the resolution of the PWM brightness control. See datasheet
typedef enum {
    PWM_8BIT,
    PWM_10BIT,
    PWM_12BIT,
    PWM_16BIT
} pwm_res_t;

// Sets the frequency of the PWM signal. See datasheet for important info
// about combinations of PWM resolution and osc. frequency
typedef enum {
    OSC_16MHZ,
    OSC_8MHZ,
    OSC_1MHZ,
    OSC_500KHZ,
    OSC_250KHZ,
    OSC_125KHZ,
    OSC_62KHZ,
    OSC_31KHZ
} osc_freq_t;

class IS32FL3237
{
private:
    uint8_t m_i2c_addr;
    bool m_autoUpdate;

public:
    void begin(ad_conn_t ad, pwm_res_t res, osc_freq_t of, bool autoUpdate);
    void setShutdown(bool x);
    void enablePWM(bool x);

    // getBrightness8(n) - read 8-bit brightness value for LED #n
    uint8_t getBrightness8(uint8_t n);
    // getBrightness(n) - read 16-bit brightness value for LED #n
    uint16_t getBrightness(uint8_t n);
    // setBrightness8(n,v) - write 8-bit brightness value for LED #n
    void setBrightness8(uint8_t n, uint8_t v);
    // setBrightness(n,v) - write 16-bit brightness value for LED #n
    void setBrightness(uint8_t n, uint16_t v);
    // setAllBrightness8(v) - write 8-bit brightness value for all LEDs
    void setAllBrightness8(uint8_t v);
    // setAllBrightness(v) - write 16-bit brightness value for all LEDs
    void setAllBrightness(uint16_t v);

    // If using PWM, you need to call this to apply any updated brightness settings
    void updateLEDs();

    // getScaleFactor(n) - read Scaling Factor for LED #n
    uint8_t getScaleFactor(uint8_t n);
    // setScaleFactor(n,v) - write Scaling Factor for LED #n
    void setScaleFactor(uint8_t n, uint8_t v);
    // setAllScaleFactors(v) - write Scaling Factor for all LEDs
    void setAllScaleFactors(uint8_t v);

    uint8_t readRegister(uint8_t addr);
    void writeRegister(uint8_t addr, uint8_t val);
};

#endif //#ifndef _IS32FL3237_H
