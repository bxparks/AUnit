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
 * On the ESP8266 platform, flash strings are implemented, and the F(),
 * PSTR() and __FlashStringHelper are defined, but the implementation is
 * brittle and can fail with obscure errors messages. For a single compilation
 * unit, a flash string cannot be defined in both an inline and non-inline
 * contexts (see https://github.com/esp8266/Arduino/issues/3369). In some cases
 * (e.g. TestMacros.h), we were able to move the F() macro into a non-inline
 * context. But in other cases (e.g. AssertVerboseMacros.h), the end-user can
 * choose to use an assertXxx() macro inside an inline function, which breaks
 * the compiler. Therefore, I chose to use normal (const char*) strings instead
 * of flash strings in those assertXxx() macros. In addition, the ESP8266
 * platform defines a useful FPSTR() macro which converts a (const char*)
 * returned by PSTR() into a (const __FlashStringHelper*) pointer.
 *
 * On the ESP32, flash strings are *not* implemented, but the various F(),
 * PSTR() and __FlashStringHelper symbols are defined for compatibility,
 * similar to Teensy-ARM. However, the implementation of FPSTR() is incorrect,
 * see https://github.com/espressif/arduino-esp32/issues/1371. That macro
 * should return a (const __FlashStringHelper*) pointer, but is defined to
 * return a (const char*) pointer.
 *
 * To make AUnit work under all of the above platforms, I chose to support
 * flash strings only on the AVR. I create custom versions of the F() and
 * FPSTR() macros below to accomplish this.
 */

#ifndef AUNIT_FLASH_H
#define AUNIT_FLASH_H

class __FlashStringHelper;

/**
 * The FPSTR() macro is defined on ESP8266, not defined on Teensy and AVR, and
 * broken on ESP32. We define our onw version to make this work on all 4
 * platforms. We might be able to use just FPSTR() if
 * https://github.com/espressif/arduino-esp32/issues/1371 is fixed.
 */
#define AUNIT_FPSTR(pstr_pointer) \
    (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))

#if defined(__AVR__) || defined(__arm__)
  #include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <pgmspace.h>
#elif defined(__linux__) or defined(__APPLE__)
  #include <pgmspace.h>
#else
  #error Unsupported platform
#endif

#if defined(__AVR__)
  #define AUNIT_F(x) F(x)
#elif defined(ESP8266) || defined(ESP32) || defined(__arm__)
  #define AUNIT_F(x) AUNIT_FPSTR(x)
#elif defined(__linux__) or defined(__APPLE__)
  #define AUNIT_F(x) F(x)
#else
  #error Unsupported platform
#endif

#endif
