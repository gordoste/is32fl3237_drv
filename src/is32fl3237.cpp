#include "is32fl3237.h"

#include <stdarg.h>

#define bool_str(b) ((b) ? "true" : "false")

void IS32FL3237::begin(
  ad_conn_t ad,
  pwm_res_t res,
  osc_freq_t of,
  bool autoUpdate) {

  log(1, "begin(addr: %d, pwm_res: %d, freq: %d, autoupdate: %s)\n",
      static_cast<int>(ad),
      static_cast<int>(res),
      static_cast<int>(of),
      bool_str(autoUpdate));

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
  log(1, "setShutdown(%s)\n", bool_str(x));

  uint8_t i = readRegister(CONTROL);
  uint8_t j = x ? (i & (~_CTL_SSD_MASK)) : (i | _CTL_SSD_MASK);
  if (j != i)
    writeRegister(CONTROL, j);
}

void IS32FL3237::enablePWM(bool x) {
  log(1, "enablePWM(%s)\n", bool_str(x));

  uint8_t i = readRegister(SSR);
  uint8_t j = x ? (i & (~_SSR_DCPWM_MASK)) : (i | _SSR_DCPWM_MASK);
  if (j != i)
    writeRegister(SSR, j);
}

void IS32FL3237::updateLEDs() {
  log(1, "updateLEDs()\n");

  writeRegister(UPDATE, 0);
}

uint8_t IS32FL3237::getBrightness8(uint8_t n) {
  const uint8_t brightness = (validate_led_number(n)
                                ? readRegister((n << 1) - 1)
                                : 0);

  log(1, "getBrightness8(%u) => %u\n",
      static_cast<unsigned>(n),
      static_cast<unsigned>(brightness));
  return brightness;
}

uint16_t IS32FL3237::getBrightness(uint8_t n) {
  const uint16_t brightness = (validate_led_number(n)
                                 ? (readRegister(n << 1) << 8)
                                     & readRegister((n << 1) - 1)
                                 : 0);

  log(1, "getBrightness(%u) => %u\n",
      static_cast<unsigned>(n),
      static_cast<unsigned>(brightness));
  return brightness;
}

void IS32FL3237::setBrightness8(uint8_t n, uint8_t v) {
  log(1, "setBrightness8(%u, %u)\n",
      static_cast<unsigned>(n),
      static_cast<unsigned>(v));

  if (validate_led_number(n)) {
    writeRegister((n << 1) - 1, v);
    if (m_autoUpdate)
      writeRegister(UPDATE, 0);
  }
}

void IS32FL3237::setBrightness(uint8_t n, uint16_t v) {
  log(1, "setBrightness(%u, %u)\n",
      static_cast<unsigned>(n),
      static_cast<unsigned>(v));

  if (validate_led_number(n)) {
    Wire.beginTransmission(m_i2c_addr);
    Wire.write((n << 1) - 1);
    Wire.write((uint8_t)(v & 0xFF));
    Wire.write((uint8_t)(v >> 8));
    Wire.endTransmission();
    if (m_autoUpdate)
      writeRegister(UPDATE, 0);
  }
}

void IS32FL3237::setAllBrightness8(uint8_t v) {
  log(1, "setAllBrightness8(%u)\n", static_cast<unsigned>(v));

  Wire.beginTransmission(m_i2c_addr);
  Wire.write(PWML01);
  for (uint8_t l = 1; l <= 36; l++) {
    Wire.write(v);
    // Overwrite the high byte so we can do it in one I2C txn
    Wire.write(0);
  }
  Wire.endTransmission();
}

void IS32FL3237::setAllBrightness(uint16_t v) {
  log(1, "setAllBrightness(%u)\n", static_cast<unsigned>(v));

  Wire.beginTransmission(m_i2c_addr);
  Wire.write(PWML01);
  for (uint8_t l = 1; l <= 36; l++) {
    Wire.write((uint8_t)(v & 0xFF));
    Wire.write((uint8_t)(v >> 8));
  }
  Wire.endTransmission();
}

uint8_t IS32FL3237::getScaleFactor(uint8_t n) {
  const uint8_t scale_factor = (validate_led_number(n)
                                  ? readRegister(0x49 + n)
                                  : 0);

  log(1, "getScaleFactor(%u) =>\n",
      static_cast<unsigned>(n),
      static_cast<unsigned>(scale_factor));

  return scale_factor;
}

void IS32FL3237::setScaleFactor(uint8_t n, uint8_t v) {
  log(1, "setScaleFactor(%u, %u)\n",
      static_cast<unsigned>(n),
      static_cast<unsigned>(v));

  if (validate_led_number(n))
    writeRegister(0x49 + n, v);
}

void IS32FL3237::setAllScaleFactors(uint8_t v) {
  log(1, "setAllScaleFactors(%u)\n", static_cast<unsigned>(v));

  Wire.beginTransmission(m_i2c_addr);
  Wire.write(SL01);
  for (uint8_t l = SL01; l <= SL36; l++)
    Wire.write(v);  // Write all in one I2C txn
  Wire.endTransmission();
}

void IS32FL3237::writeRegister(uint8_t addr, uint8_t val) {
  log(2, "writeRegister(0x%02X, 0x%02X)\n",
      static_cast<unsigned>(addr),
      static_cast<unsigned>(val));

  Wire.beginTransmission(m_i2c_addr);
  Wire.write(addr);
  Wire.write(val);
  Wire.endTransmission();
}

uint8_t IS32FL3237::readRegister(uint8_t addr) {
  uint8_t value = 0;

  Wire.beginTransmission(m_i2c_addr);
  Wire.write(addr);
  Wire.endTransmission();
  unsigned long t = millis();
  Wire.requestFrom(m_i2c_addr, (uint8_t)1);
  while (millis() - t < 1000 && !Wire.available());
  if (Wire.available()) {
    value = Wire.read();
  } else {
    error("i2c read timeout");
  }
  log(2, "readRegister(0x%02X) => 0x%02X\n",
      static_cast<unsigned>(addr),
      static_cast<unsigned>(value));
  return value;
}

/**
 * @brief Validates if the provided LED number is within the acceptable range.
 *
 * This function checks whether the given LED number is valid based on the
 * total number of LEDs available. Emits a log message if the LED number is
 * invalid.
 *
 * @param ledNumber The LED number to validate.
 * @return true if the LED number is valid, false otherwise.
 */
bool IS32FL3237::validate_led_number(uint8_t n) {
  const bool valid = n >= 1 && n <= 36;

  if (!valid) {
    error("Invalid LED number %u", static_cast<unsigned>(n));
  }
  return valid;
}

#ifndef NDEBUG
/**
 * @brief Logs a formatted message using a variadic argument list.
 *
 * This function generates a formatted log message and sends it to the specified
 * logging output stream. If the message exceeds the size of the internal buffer,
 * it is handled dynamically by allocating memory.
 *
 * @param fmt Format of the log message.
 * @param args Variadic argument list containing the values to insert into the format string.
 */
void IS32FL3237::vlog(const char *fmt, va_list args) {
  char buffer[64];
  va_list copy;

  m_log.print(m_name);
  m_log.print(": ");

  // Attempt to write the message into the buffer
  va_copy(copy, args);
  int len = vsnprintf(buffer, sizeof(buffer), fmt, copy);
  va_end(copy);
  if (len < 0) {  // If vsnprintf fails
    return;
  }

  if (len < static_cast<int>(sizeof(buffer))) {
    m_log.print(buffer);
  } else {
    // If the formatted message exceeds the size of the internal buffer,
    // allocate memory dynamically to handle larger messages.
    char *temp = static_cast<char *>(malloc(len + 1));
    if (temp == nullptr) {
      return;
    }
    vsnprintf(temp, len + 1, fmt, args);
    m_log.print(temp);
    free(temp);
  }
}

/**
 * @brief Logs a formatted message with a specified verbosity level.
 *
 * This function checks if the specified verbosity level is less than or equal
 * to the system's configured verbosity level. If so, it generates and logs the
 * message.
 *
 * @param verbosity Verbosity level of the message.
 * @param fmt Format of the log message.
 * @param ... Variadic arguments to insert into the format string.
 */
void IS32FL3237::log(int verbosity, const char *fmt, ...) {
  if (verbosity <= m_verbosity) {
    va_list args;

    va_start(args, fmt);
    vlog(fmt, args);
    va_end(args);
  }
}

/**
 * @brief Logs an error message.
 *
 * This function generates and logs an error message, always prefixed with
 * "ERROR: ".
 *
 * @param fmt Format of the error message.
 * @param ... Variadic arguments to insert into the format string.
 */
void IS32FL3237::error(const char *fmt, ...) {
  va_list args;

  m_log.print("ERROR: ");

  va_start(args, fmt);
  vlog(fmt, args);
  va_end(args);
}
#endif  // NDEBUG
