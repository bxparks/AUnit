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
class Verbosity {
  public:
    /** Print assertXxx() passed message. */
    static const uint8_t kAssertionPassed = 0x01;

    /** Print assertXxx() failed message. */
    static const uint8_t kAssertionFailed = 0x02;

    /** Print test passed  message. */
    static const uint8_t kTestPassed = 0x04;

    /** Print test failed message. */
    static const uint8_t kTestFailed = 0x08;

    /** Print test skipped message. */
    static const uint8_t kTestSkipped = 0x10;

    /** Print test timed out message. */
    static const uint8_t kTestExpired = 0x20;

    /** Print TestRunner summary message. */
    static const uint8_t kTestRunSummary = 0x40;

    // compound flags
    /** Print all assertXxx() messages. */
    static const uint8_t kAssertionAll = (kAssertionPassed | kAssertionFailed);

    /** Print all test status messages. */
    static const uint8_t kTestAll =
        (kTestPassed | kTestFailed | kTestSkipped | kTestExpired);

    /** The default verbosity. */
    static const uint8_t kDefault =
        (kAssertionFailed | kTestAll | kTestRunSummary);

    /** Print all messages. */
    static const uint8_t kAll = 0xFF;

    /** Print no messages. */
    static const uint8_t kNone = 0x00;

  private:
    // Disable constructor, copy-constructor and assignment operator
    Verbosity() = delete;
    Verbosity(const Verbosity&) = delete;
    Verbosity& operator=(const Verbosity&) = delete;
};

}

#endif
