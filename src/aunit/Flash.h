/*
MIT License

Copyright (c) 2018 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 * @file Flash.h
 *
 * Various macros to smooth over the differences among the various platforms
 * with regards to their support for flash strings and the various macros used
 * to create and access them.
 *
 * On AVR, flash strings are fully supported through the F() and PSTR() macros,
 * and the (const __FlashStringHelper*) pointer. However, the useful FPSTR()
 * macro is not defined.
 *
 * Seeeduino SAMD core has a broken implementation of FPSTR(). This file
 * clobbers that implementation.
 *
 * Adafruit's SAMD core fixes the broken FPSTR(), so we don't have to do
 * anything.
 *
 * Arduino's SAMD core breaks backwards compatibility after version >= 1.8.10.
 * AUnit does not support the ArduinoCore-API, so Arduino-branded SAMD boards
 * are explicitly blacklisted.
 *
 * STM32duino seems to have forked from Teensyduino, so it too has F() and
 * PSTR(), but no FPSTR() macro.
 *
 * The ESP8266 platform used to have flash strings which were buggy and brittle,
 * and often failed with obscure errors messages, see for example,
 * https://github.com/esp8266/Arduino/issues/3369. This bug was fixed in Dec
 * 2018, so the ESP8266 is fully supported.
 *
 * The ESP32 had a broken implementation of FPSTR()
 * (https://github.com/espressif/arduino-esp32/issues/1371), but the bug was
 * fixed in ESP32 Core 1.0.3 around Sept 2019. ESP32 is fully supported.
 *
 * On Teensy-ARM, flash strings are *not* supported, but F(), PSTR() and (const
 * __FlashStringHelper*) are defined. The useful FPSTR() macro is not defined.
 *
 * The megaAVR had a broken implementation of the F() macro (does not return a
 * const __FlashStringHelper*), but returns a (const char*)). But more
 * importantly, megaAVR uses the ArrduinoCore-api which is blacklisted by AUnit,
 * so the megaAVR (e.g. Nano Every) is not supported.
 */

#ifndef AUNIT_FLASH_H
#define AUNIT_FLASH_H

class __FlashStringHelper;

/**
 * The FPSTR() macro is defined on ESP8266 and ESP32, but not on other
 * platforms (e.g. AVR, SAMD, Teensyduino, and STM32duino). We define our own
 * version to make this work on the various platforms.
 */
#define AUNIT_FPSTR(pstr) reinterpret_cast<const __FlashStringHelper *>(pstr)

#if defined(ARDUINO_ARCH_AVR)

  #include <avr/pgmspace.h>
  #define AUNIT_F(x) F(x)

// Seeeduino SAMD21 Core is buggy, so we have to hack around it. Unfortunately,
// the Seeeduino core does not define an identifier for the following boards, so
// they are not supported:
//  * Wio lite MG126
//  * Wio GPS Board
//  * Wio LTE CAT.1
#elif defined(SEEED_XIAO_M0) \
  || defined(SEEEDUINO_ZERO) \
  || defined(SEEED_FEMTO_M0) \
  || defined(SEEEDUINO_LORAWAN) \
  || defined(SEEED_WIO_TERMINAL) \
  || defined(SEEED_GROVE_UI_WIRELESS)

  #include <avr/pgmspace.h>

  // Seeeduino (as of 1.8.4) provides an incorrect definition of FPSTR()
  // so we have to clobber it.
  #undef FPSTR
  #define FPSTR(p) (reinterpret_cast<const __FlashStringHelper *>(p))
  #define AUNIT_F(x) F(x)

// The following should work for Adafruit SAMD core, and other third party SAMD
// (SparkFun?) cores which are less buggy. The Arduino SAMD Core breaks
// backwards compatibility for versions >= 1.8.10. It is blacklisted by the time
// we get here.
#elif defined(ARDUINO_ARCH_SAMD)

  #include <avr/pgmspace.h>
  #define AUNIT_F(x) F(x)

#elif defined(ESP8266)

  #include <pgmspace.h>
  #define AUNIT_F(x) F(x)

#elif defined(ESP32)

  #include <pgmspace.h>
  #define AUNIT_F(x) F(x)

  // ESP32 cores don't seem to define SERIAL_PORT_MONITOR
  #if ! defined(SERIAL_PORT_MONITOR)
    #define SERIAL_PORT_MONITOR Serial
  #endif

#elif defined(ARDUINO_ARCH_STM32)

  #include <avr/pgmspace.h>
  #define AUNIT_F(x) F(x)

#elif defined(EPOXY_DUINO)

  #include <pgmspace.h>
  #define AUNIT_F(x) F(x)

#elif defined(TEENSYDUINO)

  #include <pgmspace.h>
  #define AUNIT_F(x) F(x)

#else

  #warning Untested platform, AUnit may still work...

  #include <avr/pgmspace.h>
  #define AUNIT_F(x) F(x)

  #if ! defined(SERIAL_PORT_MONITOR)
    #define SERIAL_PORT_MONITOR Serial
  #endif

#endif

#endif // AUNIT_FLASH_H
