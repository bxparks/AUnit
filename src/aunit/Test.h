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

#ifndef AUNIT_TEST_H
#define AUNIT_TEST_H

#include <stdint.h>
#include "FCString.h"
#include "Verbosity.h"

// Defined in ESP8266, not defined in AVR or Teensy
#ifndef FPSTR
#define FPSTR(pstr_pointer) \
    (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif

namespace aunit {

/**
 * Base class of all test cases. The test() and testing() macros define
 * subclasses of Test or TestOnce (respectively), and allow the code following
 * the macros in '{}' to become the body of the loop() and once() methods of
 * the two classes (respectively).
 */
class Test {
  public:
    // Don't change the order of Passed, Failed, Skipped or Expired without
    // looking at the isDone() method.

    /** Test is new, needs to be setup. */
    static const uint8_t kStatusNew = 0;

    /** Test is set up. */
    static const uint8_t kStatusSetup = 1;

    /** Test has passed, or pass() was called. */
    static const uint8_t kStatusPassed = 2;

    /** Test has failed, or failed() was called. */
    static const uint8_t kStatusFailed = 3;

    /** Test is skipped, through the exclude() method or skip() was called. */
    static const uint8_t kStatusSkipped = 4;

    /** Test has timed out, or expire() called. */
    static const uint8_t kStatusExpired = 5;

    /**
     * Get the pointer to the root pointer. Implemented as a function static so
     * fixes the C++ static initialization problem making it safe to use this in
     * other static contexts.
     */
    static Test** getRoot();

    /** Empty constructor. The name will be set later. */
    Test();

    /**
     * Optional method that performs any initialization. The assertXxx() macros,
     * as well as  pass(), fail() and skip() functions can be called in here.
     */
    virtual void setup() {}

    /**
     * The user-provided test case function. EEach call to Test::run() makes one
     * call to this loop() method. The assertXxx() macros, as well as pass(),
     * fail() and skip() functions can be called in here.
     */
    virtual void loop() = 0;

    /** Print out the summary of the current test. */
    void resolve();

    /** Get the name of the test. */
    const FCString& getName() { return mName; }

    /** Get the status of the test. */
    uint8_t getStatus() { return mStatus; }

    /** Set the status of the test. */
    void setStatus(uint8_t status) { mStatus = status; }

    /** Set the status to Passed or Failed depending on ok. */
    void setPassOrFail(bool ok);

    /**
     * Return the next pointer as a pointer to the pointer, similar to
     * getRoot(). This makes it much easier to manipulate a singly-linked list.
     * Also makes setNext() method unnecessary.
     */
    Test** getNext() { return &mNext; }

    /** Return true if test is done (passed, failed, skipped, expired). */
    bool isDone() { return mStatus >= kStatusPassed; }

    /** Return true if test is done (passed, failed, skipped, expired). */
    bool isNotDone() { return !isDone(); }

    /** Return true if test is passed. */
    bool isPassed() { return mStatus == kStatusPassed; }

    /** Return true if test is passed. */
    bool isNotPassed() { return !isPassed(); }

    /** Return true if test is failed. */
    bool isFailed() { return mStatus == kStatusFailed; }

    /** Return true if test is failed. */
    bool isNotFailed() { return !isFailed(); }

    /** Return true if test isNot skipped. */
    bool isSkipped() { return mStatus == kStatusSkipped; }

    /** Return true if test isNot skipped. */
    bool isNotSkipped() { return !isSkipped(); }

    /** Return true if test is expired. */
    bool isExpired() { return mStatus == kStatusExpired; }

    /** Return true if test is expired. */
    bool isNotExpired() { return !isExpired(); }

    /** Mark the test as skipped. */
    void skip() { mStatus = kStatusSkipped; }

    /** Mark the test as expired (i.e. timed out). */
    void expire() { mStatus = kStatusExpired; }

    /** Enable the given verbosity of the current test. */
    void enableVerbosity(uint8_t verbosity) { mVerbosity |= verbosity; }

    /** Disable the given verbosity of the current test. */
    void disableVerbosity(uint8_t verbosity) { mVerbosity &= ~verbosity; }

  protected:
    /** Mark the test as failed. */
    void fail() { mStatus = kStatusFailed; }

    /** Mark the test as passed. */
    void pass() { mStatus = kStatusPassed; }

    void init(const char* name) {
      mName = FCString(name);
      mStatus = kStatusNew;
      mVerbosity = 0;
      insert();
    }

    void init(const __FlashStringHelper* name) {
      mName = FCString(name);
      mStatus = kStatusNew;
      mVerbosity = 0;
      insert();
    }

    /** Determine if any of the given verbosity is enabled. */
    bool isVerbosity(uint8_t verbosity) { return mVerbosity & verbosity; }

    /** Get the verbosity. */
    uint8_t getVerbosity() { return mVerbosity; }

  private:
    // Disable copy-constructor and assignment operator
    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;

    /** Insert into the linked list. */
    void insert();

    FCString mName;
    uint8_t mStatus;
    uint8_t mVerbosity;
    Test* mNext;
};

}

#endif
