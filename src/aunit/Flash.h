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
 * On Teensy-ARM, flash strings are *not* supported, but F(), PSTR() and (const
 * __FlashStringHelper*) are defined. The useful FPSTR() macro is not defined.
 *
 * STM32duino seems to have forked from Teensyduino, so it too has F() and
 * PSTR(), but no FPSTR() macro.
 *
 * On the ESP8266 platform, flash strings are implemented, and the F(), PSTR()
 * and __FlashStringHelper are defined, but the implementation used to be
 * brittle and fail with obscure errors messages. For a single compilation
 * unit, a flash string could not be defined in both an inline and non-inline
 * contexts (see https://github.com/esp8266/Arduino/issues/3369). This bug was
 * fixed in Dec 2018, so we can use normal F(), PSTR(), and FPSTR() macros on
 * the ESP8266.
 *
 * On the ESP32, flash strings are not implemented, but the various F(), PSTR()
 * and __FlashStringHelper symbols are defined for compatibility, similar to
 * Teensy-ARM. Unfortunately, the implementation of FPSTR() was incorrectly
 * defined (https://github.com/espressif/arduino-esp32/issues/1371), but the
 * bug was fixed in ESP32 Core 1.0.3 around Sept 2019. Therefore, ESP32 can now
 * use normal F(), PSTR() and FPSTR() macros.
 *
 * On megaAVR, the F() does not return a (const __FlashStringHelper*), but
 * returns a (const char*). Unfortunately, this breaks AUnit
 * (https://github.com/bxparks/AUnit/issues/56) because a lot of code is
 * generated through macros, which assume that the F() macro returns the
 * correct type.
 *
 * Previously, AUnit used the F() only for the AVR platforms. But with the
 * various fixes, I think it can be activated for ESP8266 and other platforms.
 * Except for megaAVR whose F() returns the wrong type.
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

#elif defined(ARDUINO_ARCH_MEGAAVR)

  #error MegaAVR not supported, https://github.com/bxparks/AUnit/issues/56

#else

  #warning Untested platform, AUnit may still work...

  #include <avr/pgmspace.h>
  #define AUNIT_F(x) F(x)

  #if ! defined(SERIAL_PORT_MONITOR)
    #define SERIAL_PORT_MONITOR Serial
  #endif

#endif

// Define SERIAL_PORT_MONITOR consistently. We should also rename this file to
// something like "compat.h".
#if defined(ARDUINO_SAMD_ZERO)
  // If have a real Arduino Zero and using the "Arduino/Genuino Zero (Native
  // USB Port)" configuration on the Arduino IDE,  you may need to uncomment
  // the following to clobber SERIAL_PORT_MONITOR to point to the correct
  // SerialUSB.
  //
  // On the other hand, if you are using a SparkFun breakout board, or one of
  // the "SAMD21 M0 Mini" clones, you should be using the SparkFun SAMD Boards,
  // and selecting the "SparkFun SAMD21 Dev Breakout" or the "SparkFun SAMD21
  // Mini Breakout" settings, which will set the SERIAL_PORT_MONITOR macro
  // correctly to SerialUSB.
  #if 0
    #undef SERIAL_PORT_MONITOR
    #define SERIAL_PORT_MONITOR SerialUSB
  #endif

#endif // ARDUINO_SAMD_ZERO

#endif // AUNIT_FLASH_H
