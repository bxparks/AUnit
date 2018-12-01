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
#define checkTestDone(...)\
  get_checkTestDone(__VA_ARGS__, checkTestDone2, checkTestDone1)\
      (__VA_ARGS__)
#define get_checkTestDone(_1, _2, NAME, ...) NAME
#define checkTestDone1(name) (test_##name##_instance.isDone())
#define checkTestDone2(testSuite, name) (testSuite##_##name##_instance.isDone())

/** Return true if test 'name' is not done. */
#define checkTestNotDone(...)\
  get_checkTestNotDone(__VA_ARGS__, checkTestNotDone2, checkTestNotDone1)\
      (__VA_ARGS__)
#define get_checkTestNotDone(_1, _2, NAME, ...) NAME
#define checkTestNotDone1(name) (test_##name##_instance.isNotDone())
#define checkTestNotDone2(testSuite, name) (testSuite##_##name##_instance.isNotDone())

/** Return true if test 'name' has passed. */
#define checkTestPass(...)\
  get_checkTestPass(__VA_ARGS__, checkTestPass2, checkTestPass1)\
      (__VA_ARGS__)
#define get_checkTestPass(_1, _2, NAME, ...) NAME
#define checkTestPass1(name) (test_##name##_instance.isPassed())
#define checkTestPass2(testSuite, name) (testSuite##_##name##_instance.isPassed())

/** Return true if test 'name' has not passed. */
#define checkTestNotPass(...)\
  get_checkTestNotPass(__VA_ARGS__, checkTestNotPass2, checkTestNotPass1)\
      (__VA_ARGS__)
#define get_checkTestNotPass(_1, _2, NAME, ...) NAME
#define checkTestNotPass1(name) (test_##name##_instance.isNotPassed())
#define checkTestNotPass2(testSuite, name) (testSuite##_##name##_instance.isNotPassed())

/** Return true if test 'name' has failed. */
#define checkTestFail(...)\
  get_checkTestFail(__VA_ARGS__, checkTestFail2, checkTestFail1)\
      (__VA_ARGS__)
#define get_checkTestFail(_1, _2, NAME, ...) NAME
#define checkTestFail1(name) (test_##name##_instance.isFailed())
#define checkTestFail2(testSuite, name) (testSuite##_##name##_instance.isFailed())

/** Return true if test 'name' has not failed. */
#define checkTestNotFail(...)\
  get_checkTestNotFail(__VA_ARGS__, checkTestNotFail2, checkTestNotFail1)\
      (__VA_ARGS__)
#define get_checkTestNotFail(_1, _2, NAME, ...) NAME
#define checkTestNotFail1(name) (test_##name##_instance.isNotFailed())
#define checkTestNotFail2(testSuite, name) (testSuite##_##name##_instance.isNotFailed())

/** Return true if test 'name' has been skipped. */
#define checkTestSkip(...)\
  get_checkTestSkip(__VA_ARGS__, checkTestSkip2, checkTestSkip1)\
      (__VA_ARGS__)
#define get_checkTestSkip(_1, _2, NAME, ...) NAME
#define checkTestSkip1(name) (test_##name##_instance.isSkipped())
#define checkTestSkip2(testSuite, name) (testSuite##_##name##_instance.isSkipped())

/** Return true if test 'name' has not been skipped. */
#define checkTestNotSkip(...)\
  get_checkTestNotSkip(__VA_ARGS__, checkTestNotSkip2, checkTestNotSkip1)\
      (__VA_ARGS__)
#define get_checkTestNotSkip(_1, _2, NAME, ...) NAME
#define checkTestNotSkip1(name) (test_##name##_instance.isNotSkipped())
#define checkTestNotSkip2(testSuite, name) (testSuite##_##name##_instance.isNotSkipped())

/** Return true if test 'name' has timed out. */
#define checkTestExpire(...)\
  get_checkTestExpire(__VA_ARGS__, checkTestExpire2, checkTestExpire1)\
      (__VA_ARGS__)
#define get_checkTestExpire(_1, _2, NAME, ...) NAME
#define checkTestExpire1(name) (test_##name##_instance.isExpired())
#define checkTestExpire2(testSuite, name) (testSuite##_##name##_instance.isExpired())

/** Return true if test 'name' has not timed out. */
#define checkTestNotExpire(...)\
  get_checkTestNotExpire(__VA_ARGS__, checkTestNotExpire2, checkTestNotExpire1)\
      (__VA_ARGS__)
#define get_checkTestNotExpire(_1, _2, NAME, ...) NAME
#define checkTestNotExpire1(name) (test_##name##_instance.isNotExpired())
#define checkTestNotExpire2(testSuite, name) (testSuite##_##name##_instance.isNotExpired())

// If the assertTestXxx() macros fail, they generate an optional output, calls
// fail(), and returns from the current test case.

/** Assert that test 'name' is done. */
#define assertTestDone(...)\
  get_assertTestDone(__VA_ARGS__, assertTestDone2, assertTestDone1)\
      (__VA_ARGS__)
#define get_assertTestDone(_1, _2, NAME, ...) NAME
#define assertTestDone1(name) \
  assertTestStatusInternal1(name, isDone, kMessageDone)
#define assertTestDone2(testSuite, name) \
  assertTestStatusInternal2(testSuite, name, isDone, kMessageDone)

/** Assert that test 'name' is not done. */
#define assertTestNotDone(...)\
  get_assertTestNotDone(__VA_ARGS__, assertTestNotDone2, assertTestNotDone1)\
      (__VA_ARGS__)
#define get_assertTestNotDone(_1, _2, NAME, ...) NAME
#define assertTestNotDone1(name) \
    assertTestStatusInternal1(name, isNotDone, kMessageNotDone)
#define assertTestNotDone2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isNotDone, kMessageNotDone)

/** Assert that test 'name' has passed. */
#define assertTestPass(...)\
  get_assertTestPass(__VA_ARGS__, assertTestPass2, assertTestPass1)\
      (__VA_ARGS__)
#define get_assertTestPass(_1, _2, NAME, ...) NAME
#define assertTestPass1(name) \
    assertTestStatusInternal1(name, isPassed, kMessagePassed)
#define assertTestPass2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isPassed, kMessagePassed)

/** Assert that test 'name' has not passed. */
#define assertTestNotPass(...)\
  get_assertTestNotPass(__VA_ARGS__, assertTestNotPass2, assertTestNotPass1)\
      (__VA_ARGS__)
#define get_assertTestNotPass(_1, _2, NAME, ...) NAME
#define assertTestNotPass1(name) \
    assertTestStatusInternal1(name, isNotPassed, kMessageNotPassed)
#define assertTestNotPass2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isNotPassed, kMessageNotPassed)

/** Assert that test 'name' has failed. */
#define assertTestFail(...)\
  get_assertTestFail(__VA_ARGS__, assertTestFail2, assertTestFail1)\
      (__VA_ARGS__)
#define get_assertTestFail(_1, _2, NAME, ...) NAME
#define assertTestFail1(name) \
    assertTestStatusInternal1(name, isFailed, kMessageFailed)
#define assertTestFail2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isFailed, kMessageFailed)

/** Assert that test 'name' has not failed. */
#define assertTestNotFail(...)\
  get_assertTestNotFail(__VA_ARGS__, assertTestNotFail2, assertTestNotFail1)\
      (__VA_ARGS__)
#define get_assertTestNotFail(_1, _2, NAME, ...) NAME
#define assertTestNotFail1(name) \
    assertTestStatusInternal1(name, isNotFailed, kMessageNotFailed)
#define assertTestNotFail2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isNotFailed, kMessageNotFailed)

/** Assert that test 'name' has been skipped. */
#define assertTestSkip(...)\
  get_assertTestSkip(__VA_ARGS__, assertTestSkip2, assertTestSkip1)\
      (__VA_ARGS__)
#define get_assertTestSkip(_1, _2, NAME, ...) NAME
#define assertTestSkip1(name) \
    assertTestStatusInternal1(name, isSkipped, kMessageSkipped)
#define assertTestSkip2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isSkipped, kMessageSkipped)

/** Assert that test 'name' has not been skipped. */
#define assertTestNotSkip(...)\
  get_assertTestNotSkip(__VA_ARGS__, assertTestNotSkip2, assertTestNotSkip1)\
      (__VA_ARGS__)
#define get_assertTestNotSkip(_1, _2, NAME, ...) NAME
#define assertTestNotSkip1(name) \
    assertTestStatusInternal1(name, isNotSkipped, kMessageNotSkipped)
#define assertTestNotSkip2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isNotSkipped, kMessageNotSkipped)

/** Assert that test 'name' has timed out. */
#define assertTestExpire(...)\
  get_assertTestExpire(__VA_ARGS__, assertTestExpire2, assertTestExpire1)\
      (__VA_ARGS__)
#define get_assertTestExpire(_1, _2, NAME, ...) NAME
#define assertTestExpire1(name) \
    assertTestStatusInternal1(name, isExpired, kMessageExpired)
#define assertTestExpire2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isExpired, kMessageExpired)

/** Assert that test 'name' has not timed out. */
#define assertTestNotExpire(...)\
  get_assertTestNotExpire(__VA_ARGS__, assertTestNotExpire2, assertTestNotExpire1)\
      (__VA_ARGS__)
#define get_assertTestNotExpire(_1, _2, NAME, ...) NAME
#define assertTestNotExpire1(name) \
    assertTestStatusInternal1(name, isNotExpired, kMessageNotExpired)
#define assertTestNotExpire2(testSuite, name) \
    assertTestStatusInternal2(testSuite, name, isNotExpired, kMessageNotExpired)

/** Internal helper macro, shouldn't be called directly by users. */
#define assertTestStatusInternal1(name,method,message) do {\
  if (!assertionTestStatus(\
      __FILE__,__LINE__,#name,AUNIT_FPSTR(message),\
          test_##name##_instance.method()))\
    return;\
} while (false)

#define assertTestStatusInternal2(testSuite,name,method,message) do {\
  if (!assertionTestStatus(\
      __FILE__,__LINE__,#testSuite "_" #name,AUNIT_FPSTR(message),\
          testSuite##_##name##_instance.method()))\
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
