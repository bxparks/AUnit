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

#ifndef AUNIT_VERBOSITY_H
#define AUNIT_VERBOSITY_H

#include <stdint.h>

namespace aunit {

  /**
   * Utility class to hold the Verbosity constants. Current used only by
   * TestRunner but potentially could be used by Test and TestOnce in the future,
   * so it seemed better to pull these out into a separate file.
   */
  enum class Verbosity : uint8_t
  {
    /** Print assertXxx() passed message. */
    kAssertionPassed = 0x01,

    /** Print assertXxx() failed message. */
    kAssertionFailed = 0x02,

    /** Print test passed  message. */
    kTestPassed = 0x04,

    /** Print test failed message. */
    kTestFailed = 0x08,

    /** Print test skipped message. */
    kTestSkipped = 0x10,

    /** Print test timed out message. */
    kTestExpired = 0x20,

    /** Print TestRunner summary message. */
    kTestRunSummary = 0x40,

    // compound flags
    /** Print all assertXxx() messages. */
    kAssertionAll = (kAssertionPassed | kAssertionFailed),

    /** Print all test status messages. */
    kTestAll = (kTestPassed | kTestFailed | kTestSkipped | kTestExpired),

    /** The default verbosity. */
    kDefault = (kAssertionFailed | kTestAll | kTestRunSummary),

    /** Print all messages. */
    kAll = 0xFF,

    /** Print no messages. */
    kNone = 0x00,
  };

  inline Verbosity operator&(Verbosity left, const Verbosity right)
  { return static_cast<Verbosity>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right)); }

  inline Verbosity operator|(Verbosity left, const Verbosity right)
  { return static_cast<Verbosity>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right)); }

  inline Verbosity operator~(Verbosity v) { return static_cast<Verbosity>(~(static_cast<uint8_t>(v))); }

  inline Verbosity& operator|=(Verbosity& left, const Verbosity& right) { return left = left | right; }
  inline Verbosity& operator&=(Verbosity& left, const Verbosity& right) { return left = left & right; }

}

#endif
