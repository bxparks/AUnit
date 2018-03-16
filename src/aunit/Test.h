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

class __FlashStringHelper;

// On the ESP8266 platform, The F() string cannot be placed in an inline
// context, because it interferes with other PROGMEM strings. See
// https://github.com/esp8266/Arduino/issues/3369. The solution was to move the
// constructor definition out from an inline function into a normal function
// defined outside of the class declaration..

// Macro to define a test that will be run only once.
#define test(name) struct test_ ## name : aunit::TestOnce {\
  test_ ## name();\
  virtual void once() override;\
} test_ ## name ## _instance;\
test_ ## name :: test_ ## name() : TestOnce(F(#name)) {}\
void test_ ## name :: once()

// Macro to define a test that will run repeatly upon each iteration of the
// global loop() method, stopping when the something calls Test::pass(),
// Test::fail() or Test::skip().
#define testing(name) struct test_ ## name : aunit::Test {\
  test_ ## name();\
  virtual void loop() override;\
} test_ ## name ## _instance;\
test_ ## name :: test_ ## name() : Test(F(#name)) {}\
void test_ ## name :: loop()

namespace aunit {

class Test {
  public:
    static const uint8_t kStatusNew = 0;
    static const uint8_t kStatusSetup = 1;
    static const uint8_t kStatusPassed = 2;
    static const uint8_t kStatusFailed = 3;
    static const uint8_t kStatusSkipped = 4;
    static const uint8_t kStatusExpired = 5;

    /**
     * Get the pointer to the root pointer. Implemented as a function static so
     * fixes the C++ static initialization problem making it safe to use this in
     * other static contexts.
     */
    static Test** getRoot();

    /**
     * Constructor taking the name of the given test case. Also performs
     * self-registration into the linked list of all test cases defined by
     * Test::getRoot().
     */
    explicit Test(const char* name);

    /**
     * Constructor taking the name of the given test case. Also performs
     * self-registration into the linked list of all test cases defined by
     * Test::getRoot()..
     */
    explicit Test(const __FlashStringHelper* name);

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

    /** Mark the test as skipped. */
    void skip() { mStatus = kStatusSkipped; }

    /** Mark the test as expired (i.e. timed out). */
    void expire() { mStatus = kStatusExpired; }

  protected:
    /** Mark the test as failed. */
    void fail() { mStatus = kStatusFailed; }

    /** Mark the test as passed. */
    void pass() { mStatus = kStatusPassed; }

  private:
    // Disable copy-constructor and assignment operator
    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;

    /** Insert into the linked list. */
    void insert();

    const FCString mName;
    uint8_t mStatus;
    Test* mNext;
};

/** Similar to Test but performs the loop() method only once. */
class TestOnce: public Test {
  public:
    /** Constructor. */
    explicit TestOnce(const char* name):
        Test(name) {}

    /** Constructor. */
    explicit TestOnce(const __FlashStringHelper* name):
        Test(name) {}

    /**
     * Calls the user-provided once() method. If no other assertXxx() macros set
     * the internal status, then this calls pass() to make sure that this test
     * case will be called only once from Test::run().
     */
    virtual void loop() override;

    /** User-provided test case. */
    virtual void once() = 0;

  private:
    // Disable copy-constructor and assignment operator
    TestOnce(const TestOnce&) = delete;
    TestOnce& operator=(const TestOnce&) = delete;
};

}
#endif
