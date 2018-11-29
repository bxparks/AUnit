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

// Significant portions of the design and implementation of this file came from
// https://github.com/mmurdoch/arduinounit/blob/master/src/ArduinoUnit.h

#ifndef AUNIT_TEST_ONCE_H
#define AUNIT_TEST_ONCE_H

#include <stdint.h>
#include "FCString.h"
#include "MetaAssertion.h"

class __FlashStringHelper;

namespace aunit {

/** Similar to TestAgain but performs user-defined test only once. */
class TestOnce: public MetaAssertion {
  public:
    /** Constructor. */
    TestOnce() {}

    /**
     * Calls the user-provided once() method. If no other assertXxx() macros set
     * the internal status, then this calls pass() to make sure that this test
     * case will be called only once from Test::run().
     */
    void loop() override;

    /** User-provided test case. */
    virtual void once() = 0;

  private:
    // Disable copy-constructor and assignment operator
    TestOnce(const TestOnce&) = delete;
    TestOnce& operator=(const TestOnce&) = delete;
};

}

#endif
