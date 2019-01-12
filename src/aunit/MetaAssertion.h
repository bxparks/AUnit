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

#ifndef AUNIT_META_ASSERTION_H
#define AUNIT_META_ASSERTION_H

#include "Printer.h"
#include "Assertion.h"

class __FlashStringHelper;

namespace aunit {

/**
 * Class that extends the Assertion class to support the checkTestXxx() and
 * assertTestXxx() macros that look at the status of the named test.
 */
class MetaAssertion: public Assertion {
  protected:
    // Human-readable strings for various meta-asssertion messages.
    // They need to be protected, not private, because they are used by
    // subclasses through the test() and testing() macros.
    static const char kMessageDone[];
    static const char kMessageNotDone[];
    static const char kMessagePassed[];
    static const char kMessageNotPassed[];
    static const char kMessageFailed[];
    static const char kMessageNotFailed[];
    static const char kMessageSkipped[];
    static const char kMessageNotSkipped[];
    static const char kMessageExpired[];
    static const char kMessageNotExpired[];

    /** Empty constructor. */
    MetaAssertion() {}

    /**
     * Set the status of the current test using the 'ok' status from another
     * test, and print the assertion message if requested.
     */
    bool assertionTestStatus(const char* file, uint16_t line,
        const char* testName, const __FlashStringHelper* statusMessage,
        bool ok);

    /** Return true if setting of status should print a message. */
    bool isOutputEnabledForStatus(uint8_t status) const;

    /** Set the status of the current test to 'status' and print a message. */
    void setStatusNow(const char* file, uint16_t line, uint8_t status,
        const __FlashStringHelper* statusString);

  private:
    // Disable copy-constructor and assignment operator
    MetaAssertion(const MetaAssertion&) = delete;
    MetaAssertion& operator=(const MetaAssertion&) = delete;

};

}

#endif
