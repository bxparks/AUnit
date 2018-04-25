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
 * Flash strings (using F() macro) on the ESP8266 platform cannot be placed in
 * an inline context, because it interferes with other PROGMEM strings in
 * non-inline contexts. See https://github.com/esp8266/Arduino/issues/3369. In
 * some cases (e.g. TestMacros.h), we were able to move the F() macro into a
 * non-inline context. But in other cases (e.g. AssertVerboseMacros.h) it is
 * impossible to move these out of inline contexts because we want to support
 * assertXxx() statements inside inlined methods. We use normal (const char*)
 * strings instead of flash strings in those places instead.
 */

#ifndef AUNIT_FLASH_H
#define AUNIT_FLASH_H

class __FlashStringHelper;

#if (defined ESP8266 || defined ESP32)
  #include <pgmspace.h>
#else
  #include <avr/pgmspace.h>
#endif

// Defined in ESP8266, not defined in AVR or Teensy
#ifndef FPSTR
  #define FPSTR(pstr_pointer) \
      (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif

// These are used only in AssertionVerboseMacros.h and the "Verbose" methods of
// Assertion.h because ESP8266 cannot handle F() strings in both inline and
// non-inlnie contexts. So don't use F() strings on ESP8266.
#ifdef ESP8266
  #define AUNIT_F(x) (x)
  namespace aunit {
  namespace internal {
  typedef const char* FlashStringType;
  }
  }
#else
  #define AUNIT_F(x) F(x)
  namespace aunit {
  namespace internal {
  typedef const __FlashStringHelper* FlashStringType;
  }
  }
#endif

#endif
