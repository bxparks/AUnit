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

/**
 * @file MetaAssertMacros.h
 *
 * Various assertTestXxx(), checkTestXxx(), assertTestXxxF() and 
 * checkTestXxxF() macros are defined in this header.
 */

#ifndef AUNIT_META_ASSERT_MACROS_H
#define AUNIT_META_ASSERT_MACROS_H

#include "Flash.h"

// Meta tests, same syntax as ArduinoUnit for compatibility.
// The checkTestXxx() macros return a boolean, and execution continues.

/** Return true if test 'name' is done. */
#define checkTestDone(name) (test_##name##_instance.isDone())

/** Return true if test 'name' is not done. */
#define checkTestNotDone(name) (test_##name##_instance.isNotDone())

/** Return true if test 'name' has passed. */
#define checkTestPass(name) (test_##name##_instance.isPassed())

/** Return true if test 'name' has not passed. */
#define checkTestNotPass(name) (test_##name##_instance.isNotPassed())

/** Return true if test 'name' has failed. */
#define checkTestFail(name) (test_##name##_instance.isFailed())

/** Return true if test 'name' has not failed. */
#define checkTestNotFail(name) (test_##name##_instance.isNotFailed())

/** Return true if test 'name' has been skipped. */
#define checkTestSkip(name) (test_##name##_instance.isSkipped())

/** Return true if test 'name' has not been skipped. */
#define checkTestNotSkip(name) (test_##name##_instance.isNotSkipped())

/** Return true if test 'name' has timed out. */
#define checkTestExpire(name) (test_##name##_instance.isExpired())

/** Return true if test 'name' has not timed out. */
#define checkTestNotExpire(name) (test_##name##_instance.isNotExpired())

// If the assertTestXxx() macros fail, they generate an optional output, calls
// fail(), and returns from the current test case.

/** Assert that test 'name' is done. */
#define assertTestDone(name) \
  assertTestStatusInternal(name, isDone, kMessageDone)

/** Assert that test 'name' is not done. */
#define assertTestNotDone(name) \
    assertTestStatusInternal(name, isNotDone, kMessageNotDone)

/** Assert that test 'name' has passed. */
#define assertTestPass(name) \
    assertTestStatusInternal(name, isPassed, kMessagePassed)

/** Assert that test 'name' has not passed. */
#define assertTestNotPass(name) \
    assertTestStatusInternal(name, isNotPassed, kMessageNotPassed)

/** Assert that test 'name' has failed. */
#define assertTestFail(name) \
    assertTestStatusInternal(name, isFailed, kMessageFailed)

/** Assert that test 'name' has not failed. */
#define assertTestNotFail(name) \
    assertTestStatusInternal(name, isNotFailed, kMessageNotFailed)

/** Assert that test 'name' has been skipped. */
#define assertTestSkip(name) \
    assertTestStatusInternal(name, isSkipped, kMessageSkipped)

/** Assert that test 'name' has not been skipped. */
#define assertTestNotSkip(name) \
    assertTestStatusInternal(name, isNotSkipped, kMessageNotSkipped)

/** Assert that test 'name' has timed out. */
#define assertTestExpire(name) \
    assertTestStatusInternal(name, isExpired, kMessageExpired)

/** Assert that test 'name' has not timed out. */
#define assertTestNotExpire(name) \
    assertTestStatusInternal(name, isNotExpired, kMessageNotExpired)

/** Internal helper macro, shouldn't be called directly by users. */
#define assertTestStatusInternal(name,method,message) do {\
  if (!assertionTestStatus(\
      __FILE__,__LINE__,#name,AUNIT_FPSTR(message),test_##name##_instance.method()))\
    return;\
} while (false)

// Meta tests for testF() and testingF() are slightly different because
// the name of the fixture class is appended to the instance name.

/** Return true if test 'name' is done. */
#define checkTestDoneF(testClass,name) \
    (testClass##_##name##_instance.isDone())

/** Return true if test 'name' is not done. */
#define checkTestNotDoneF(testClass,name) \
    (testClass##_##name##_instance.isNotDone())

/** Return true if test 'name' has passed. */
#define checkTestPassF(testClass,name) \
    (testClass##_##name##_instance.isPassed())

/** Return true if test 'name' has not passed. */
#define checkTestNotPassF(testClass,name) \
    (testClass##_##name##_instance.isNotPassed())

/** Return true if test 'name' has failed. */
#define checkTestFailF(testClass,name) \
    (testClass##_##name##_instance.isFailed())

/** Return true if test 'name' has not failed. */
#define checkTestNotFailF(testClass,name) \
    (testClass##_##name##_instance.isNotFailed())

/** Return true if test 'name' has been skipped. */
#define checkTestSkipF(testClass,name) \
    (testClass##_##name##_instance.isSkipped())

/** Return true if test 'name' has not been skipped. */
#define checkTestNotSkipF(testClass,name) \
    (testClass##_##name##_instance.isNotSkipped())

/** Return true if test 'name' has timed out. */
#define checkTestExpireF(testClass,name) \
    (testClass##_##name##_instance.isExpired())

/** Return true if test 'name' has not timed out. */
#define checkTestNotExpireF(testClass,name) \
    (testClass##_##name##_instance.isNotExpired())

// If the assertTestXxx() macros fail, they generate an optional output, calls
// fail(), and returns from the current test case.

/** Assert that test 'name' is done. */
#define assertTestDoneF(testClass,name) \
  assertTestStatusInternalF(testClass, name, isDone, kMessageDone)

/** Assert that test 'name' is not done. */
#define assertTestNotDoneF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isNotDone, kMessageNotDone)

/** Assert that test 'name' has passed. */
#define assertTestPassF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isPassed, kMessagePassed)

/** Assert that test 'name' has not passed. */
#define assertTestNotPassF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isNotPassed, kMessageNotPassed)

/** Assert that test 'name' has failed. */
#define assertTestFailF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isFailed, kMessageFailed)

/** Assert that test 'name' has not failed. */
#define assertTestNotFailF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isNotFailed, kMessageNotFailed)

/** Assert that test 'name' has been skipped. */
#define assertTestSkipF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isSkipped, kMessageSkipped)

/** Assert that test 'name' has not been skipped. */
#define assertTestNotSkipF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isNotSkipped, \
        kMessageNotSkipped)

/** Assert that test 'name' has timed out. */
#define assertTestExpireF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isExpired, kMessageExpired)

/** Assert that test 'name' has not timed out. */
#define assertTestNotExpireF(testClass,name) \
    assertTestStatusInternalF(testClass, name, isNotExpired, \
        kMessageNotExpired)

/** Internal helper macro, shouldn't be called directly by users. */
#define assertTestStatusInternalF(testClass,name,method,message) do {\
  if (!assertionTestStatus(__FILE__, __LINE__, #name, AUNIT_FPSTR(message),\
      testClass##_##name##_instance.method()))\
    return;\
} while (false)

// Methods that sets the status of the current test, prints a message, and
// exits immediately.

/**
 * Fail the current test, return immediately, and print a status message.
 * Similar to Test::fail() except that this prints a status message.
 */
#define failTestNow() do {\
  setStatusNow(__FILE__, __LINE__, kStatusFailed, AUNIT_FPSTR(kMessageFailed));\
  return;\
} while (false)

/**
 * Pass the current test, print a status message, and return immediately.
 * Similar to Test::pass() except that this prints a status message.
 */
#define passTestNow() do {\
  setStatusNow(__FILE__, __LINE__, kStatusPassed, AUNIT_FPSTR(kMessagePassed));\
  return;\
} while (false)

/**
 * Skip the current test, print a status message, and return immediately.
 * Similar to Test::skip() except that this prints a status message.
 */
#define skipTestNow() do {\
  setStatusNow(__FILE__, __LINE__, kStatusSkipped,\
      AUNIT_FPSTR(kMessageSkipped));\
  return;\
} while (false)

/**
 * Expire the current test, print a status message, and return immediately.
 * Similar to Test::expire() except that this prints a status message.
 */
#define expireTestNow() do {\
  setStatusNow(__FILE__, __LINE__, kStatusExpired,\
      AUNIT_FPSTR(kMessageExpired));\
  return;\
} while (false)

#endif
