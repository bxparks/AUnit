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

namespace aunit {

/**
 * Base class of all test cases. The test() and testing() macros define
 * subclasses of Test or TestOnce (respectively), and allow the code following
 * the macros in '{}' to become the body of the loop() and once() methods of
 * the two classes (respectively).
 */
class Test {
  public:
    // The LifeCycle states are used by TestRunner to determine what a Test
    // should do. Unlike the assertion Status, the LfeCycle is mostly hidden
    // from client code. The state transition diagram looks like this:
    //
    //        include()/exclude()
    //      .---------------------> Excluded -----------.
    //      v                                           v
    //    New                                        Finished -> (out of list)
    //      \ setup()      assertion()       teardown() ^
    //       -------> Setup -------> Asserted ----------'

    /** Test is new, needs to be setup. */
    static const uint8_t kLifeCycleNew = 0;

    /**
     * Test is Excluded by an exclude() method. The setup() and teardown()
     * methods are bypassed and the test goes directly to kLifeCycleFinished.
     * For reporting purposes, an excluded test is counted as a "skipped" test.
     * The include() method puts the test back into the kLifeCycleNew state.
     */
    static const uint8_t kLifeCycleExcluded = 1;

    /**
     * Test has been set up by calling setup() and ready to execute the test
     * code. TestOnce tests (i.e. test() or testF()) should be in Setup state
     * only for a single iteration. TestAgain tests (i.e. testing() or
     * testingF()) will stay in Setup state until explicitly moved to a
     * different state by the testing code (or the test times out).
     */
    static const uint8_t kLifeCycleSetup = 2;

    /**
     * Test is asserted (using pass(), fail(), expired() or skipped()) and the
     * getStatus() has been determined. The teardown() method should be called.
     */
    static const uint8_t kLifeCycleAsserted = 3;

    /**
     * The test has completed its life cycle. It should be resolved using
     * resolve() and removed from the linked list. Note that this is different
     * than isDone() (i.e. kStatusDone) which indicates that an assertion about
     * the test has been made.
     */
    static const uint8_t kLifeCycleFinished = 4;

    // The assertion Status is the result of an "assertion()". In addition to
    // the usual pass() and fail(), there are meta-assertions such as skip()
    // and expire(). When the Status is changed from kStatusUnknown, the
    // lifeCycle state changes to kLifeCycleAsserted.

    /** Test status is unknown. */
    static const uint8_t kStatusUnknown = 0;

    /** Test has passed, or pass() was called. */
    static const uint8_t kStatusPassed = 1;

    /** Test has failed, or fail() was called. */
    static const uint8_t kStatusFailed = 2;

    /** Test is skipped through the exclude() method or skip() was called. */
    static const uint8_t kStatusSkipped = 3;

    /** Test has timed out, or expire() called. */
    static const uint8_t kStatusExpired = 4;

    /**
     * Get the pointer to the root pointer. Implemented as a function static so
     * fixes the C++ static initialization problem making it safe to use this in
     * other static contexts.
     */
    static Test** getRoot();

    /** Empty constructor. The name will be set later. */
    Test();

    // NOTE: Don't create a virtual destructor. That's the normal best practice
    // for classes that will be used polymorphically. However, this class will
    // never be deleted polymorphically (i.e. through its pointer) so it
    // doesn't need a virtual destructor. In fact, adding it causes flash and
    // static memory to increase dramatically because each test() and testing()
    // macro creates a new subclass. AceButtonTest flash memory increases from
    // 18928 to 20064 bytes, and static memory increases from 917 to 1055
    // bytes.

    /**
     * Optional method that performs any initialization. The assertXxx() macros,
     * as well as  pass(), fail() and skip() functions can be called in here.
     * Subclasses that override this should call the parent's setup() method in
     * the first line so that the setup() methods in the inheritance tree are
     * properly chained.
     */
    virtual void setup() {}

    /**
     * Optional method that performs any clean up after the test ends for any
     * reasons, either passing or otherwise. Subclasses that override this
     * should call the parent's teardown() method in the last line before
     * returning, so that the teardown() methods in the inheritance tree are
     * properly chained.
     */
    virtual void teardown() {}

    /**
     * The user-provided test case function. Each call to Test::run() makes one
     * call to this loop() method. The assertXxx() macros, as well as pass(),
     * fail() and skip() functions can be called in here.
     */
    virtual void loop() = 0;

    /** Print out the summary of the current test. */
    void resolve();

    /** Get the name of the test. */
    const internal::FCString& getName() const { return mName; }

    /** Get the life cycle state of the test. */
    uint8_t getLifeCycle() const { return mLifeCycle; }

    void setLifeCycle(uint8_t state) { mLifeCycle = state; }

    /** Get the status of the test. */
    uint8_t getStatus() const { return mStatus; }

    /**
     * Set the status of the test. All changes to getStatus() should happen
     * through this method because it also changes the getLifeCycle() of the
     * test.
     */
    void setStatus(uint8_t status) {
      if (status != kStatusUnknown) {
        setLifeCycle(kLifeCycleAsserted);
      }
      mStatus = status;
    }

    /** Set the status to Passed or Failed depending on ok. */
    void setPassOrFail(bool ok);

    /**
     * Return the next pointer as a pointer to the pointer, similar to
     * getRoot(). This makes it much easier to manipulate a singly-linked list.
     * Also makes setNext() method unnecessary.
     */
    Test** getNext() { return &mNext; }

    /**
     * Return true if test has been asserted. Note that this is different than
     * the internal LifeCycleFinished state. The name isDone() is a carry-over
     * from ArduinoUnit and might have been named isAsserted() if this library
     * had been built from scratch.
     */
    bool isDone() const { return mStatus != kStatusUnknown; }

    /** Return true if test is not has been asserted. */
    bool isNotDone() const { return !isDone(); }

    /** Return true if test is passed. */
    bool isPassed() const { return mStatus == kStatusPassed; }

    /** Return true if test is not passed. */
    bool isNotPassed() const { return !isPassed(); }

    /** Return true if test is failed. */
    bool isFailed() const { return mStatus == kStatusFailed; }

    /** Return true if test is not failed. */
    bool isNotFailed() const { return !isFailed(); }

    /** Return true if test is skipped. */
    bool isSkipped() const { return mStatus == kStatusSkipped; }

    /** Return true if test is not skipped. */
    bool isNotSkipped() const { return !isSkipped(); }

    /** Return true if test is expired. */
    bool isExpired() const { return mStatus == kStatusExpired; }

    /** Return true if test is not expired. */
    bool isNotExpired() const { return !isExpired(); }

    /**
     * Mark the test as skipped. Use the skipTestNow() macro in a unit test to
     * print a diagnostic message and exit immediately.
     */
    void skip() { setStatus(kStatusSkipped); }

    /**
     * Mark the test as expired (i.e. timed out). Use the expireTestNow() macro
     * in a unit test to print a diagnostic message and exit immediately.
     */
    void expire() { setStatus(kStatusExpired); }

    /** Enable the given verbosity of the current test. */
    void enableVerbosity(uint8_t verbosity) { mVerbosity |= verbosity; }

    /** Disable the given verbosity of the current test. */
    void disableVerbosity(uint8_t verbosity) { mVerbosity &= ~verbosity; }

  protected:
    /**
     * Mark the test as failed. Use the failTestNow() macro in a unit test to
     * print a diagnostic message and exit immediately.
     */
    void fail() { setStatus(kStatusFailed); }

    /**
     * Mark the test as passed. Often used to terminate a testing() looping
     * test. The passTestNow() macro can be used in a unit test to print a
     * diagnostic message and exit immediately. It is expected that pass() will
     * be used more often.
     */
    void pass() { setStatus(kStatusPassed); }

    void init(const char* name) {
      mName = internal::FCString(name);
      mLifeCycle = kLifeCycleNew;
      mStatus = kStatusUnknown;
      mVerbosity = 0;
      insert();
    }

    void init(const __FlashStringHelper* name) {
      mName = internal::FCString(name);
      mLifeCycle = kLifeCycleNew;
      mStatus = kStatusUnknown;
      mVerbosity = 0;
      insert();
    }

    /** Determine if any of the given verbosity is enabled. */
    bool isVerbosity(uint8_t verbosity) const { return mVerbosity & verbosity; }

    /** Get the verbosity. */
    uint8_t getVerbosity() const { return mVerbosity; }

  private:
    // Disable copy-constructor and assignment operator
    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;

    /** Insert into the linked list. */
    void insert();

    internal::FCString mName;
    uint8_t mLifeCycle;
    uint8_t mStatus;
    uint8_t mVerbosity;
    Test* mNext;
};

}

#endif
