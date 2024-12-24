/**
 * @file IS32FL3237_Example.ino
 * @author your name (you@domain.com)
 * @brief Example with D1 mini. D4 connected to RESET pin of IS32FL3237
 * @version 0.1.1
 * @date 2024-12-22
 *
 * @copyright Copyright (c) 2024
 */
#include "is32fl3237.h"

IS32FL3237 led_drv;
uint8_t counter = 0;
bool setupOK = false;

void setup() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);  // Hold LED driver in reset

  led_drv.begin(AD_GND, PWM_16BIT, OSC_16MHZ, false);  // No auto-update

  led_drv.writeRegister(GCC, 0xFF);        // Global current control to max
  led_drv.writeRegister(SSR, 0b00000000);  // PWM enabled, spread spectrum disabled
  led_drv.setAllScaleFactors(0xA0);
  for (uint8_t i = 0; i < 12; i++) led_drv.setScaleFactor(3 * i + 2, 0x40);  // On my prototype board, these channels are very bright green LEDs and need to be scaled down

  digitalWrite(D4, HIGH);      // Release reset pin
  led_drv.setShutdown(false);  // Disable software shutdown

  // Serial.printf("CTL:0x%02x SSR:0x%02x GCC:0x%02x\n", led_drv.readRegister(CONTROL), led_drv.readRegister(SSR), led_drv.readRegister(GCC));
  // Serial.printf("PWML01:0x%02x PWMH01:0x%02x SL1:0x%02x\n", led_drv.readRegister(PWML01), led_drv.readRegister(PWMH01), led_drv.readRegister(SL01));

  Serial.println("Setup done");

  delay(10);

  setupOK = true;
}

void loop() {
  static unsigned long updateTime;

  if (!setupOK)
    return;

  if (millis() - updateTime > 500) {
    counter++;

    // Cycle red LEDs
    led_drv.setBrightness(1, counter % 15 == 0 ? 0xffff : 0);
    led_drv.setBrightness(4, counter % 15 == 1 ? 0xffff : 0);
    led_drv.setBrightness(7, counter % 15 == 2 ? 0xffff : 0);
    led_drv.setBrightness(10, counter % 15 == 3 ? 0xffff : 0);
    led_drv.setBrightness(13, counter % 15 == 4 ? 0xffff : 0);
    led_drv.setBrightness(16, counter % 15 == 5 ? 0xffff : 0);
    led_drv.setBrightness(19, counter % 15 == 6 ? 0xffff : 0);
    led_drv.setBrightness(22, counter % 15 == 7 ? 0xffff : 0);
    led_drv.setBrightness(25, counter % 15 == 8 ? 0xffff : 0);
    led_drv.setBrightness(28, counter % 15 == 9 ? 0xffff : 0);
    led_drv.setBrightness(31, counter % 15 == 10 ? 0xffff : 0);
    led_drv.setBrightness(34, counter % 15 == 11 ? 0xffff : 0);

    // Cycle green LEDs
    led_drv.setBrightness(2, counter % 15 == 1 ? 0xffff : 0);
    led_drv.setBrightness(5, counter % 15 == 2 ? 0xffff : 0);
    led_drv.setBrightness(8, counter % 15 == 3 ? 0xffff : 0);
    led_drv.setBrightness(11, counter % 15 == 4 ? 0xffff : 0);
    led_drv.setBrightness(14, counter % 15 == 5 ? 0xffff : 0);
    led_drv.setBrightness(17, counter % 15 == 6 ? 0xffff : 0);
    led_drv.setBrightness(20, counter % 15 == 7 ? 0xffff : 0);
    led_drv.setBrightness(23, counter % 15 == 8 ? 0xffff : 0);
    led_drv.setBrightness(26, counter % 15 == 9 ? 0xffff : 0);
    led_drv.setBrightness(29, counter % 15 == 10 ? 0xffff : 0);
    led_drv.setBrightness(32, counter % 15 == 11 ? 0xffff : 0);
    led_drv.setBrightness(35, counter % 15 == 12 ? 0xffff : 0);

    // Cycle blue LEDs
    led_drv.setBrightness(3, counter % 15 == 2 ? 0xffff : 0);
    led_drv.setBrightness(6, counter % 15 == 3 ? 0xffff : 0);
    led_drv.setBrightness(9, counter % 15 == 4 ? 0xffff : 0);
    led_drv.setBrightness(12, counter % 15 == 5 ? 0xffff : 0);
    led_drv.setBrightness(15, counter % 15 == 6 ? 0xffff : 0);
    led_drv.setBrightness(18, counter % 15 == 7 ? 0xffff : 0);
    led_drv.setBrightness(21, counter % 15 == 8 ? 0xffff : 0);
    led_drv.setBrightness(24, counter % 15 == 9 ? 0xffff : 0);
    led_drv.setBrightness(27, counter % 15 == 10 ? 0xffff : 0);
    led_drv.setBrightness(30, counter % 15 == 11 ? 0xffff : 0);
    led_drv.setBrightness(33, counter % 15 == 12 ? 0xffff : 0);
    led_drv.setBrightness(36, counter % 15 == 13 ? 0xffff : 0);

    // This is necessary to apply updated brightnesses
    // Auto-update makes setBrightness() call this function every time but that is wasteful in this case
    // so I don't enable it
    led_drv.updateLEDs();

    updateTime = millis();
  }
}
