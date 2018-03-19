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

#include <Arduino.h>  // definition of Print
#include "Printer.h"
#include "TestRunner.h"

// Meta tests, same syntax as ArduinoUnit for compatibility.
// The checkTestXxx() macros return a boolean, and execution continues.

#define checkTestDone(name) (test_##name##_instance.isDone())

#define checkTestNotDone(name) (test_##name##_instance.isNotDone())

#define checkTestPass(name) (test_##name##_instance.isPassed())

#define checkTestNotPass(name) (test_##name##_instance.isNotPassed())

#define checkTestFail(name) (test_##name##_instance.isFailed())

#define checkTestNotFail(name) (test_##name##_instance.isNotFailed())

#define checkTestSkip(name) (test_##name##_instance.isSkipped())

#define checkTestNotSkip(name) (test_##name##_instance.isNotSkipped())

#define checkTestExpire(name) (test_##name##_instance.isExpired())

#define checkTestNotExpire(name) (test_##name##_instance.isNotExpired())

// If the assertTestXxx() macros fail, they generate an optional output, calls
// fail(), and returns from the current test case.

#define assertTestDone(name) assertTestStatus(name, isDone)

#define assertTestNotDone(name) assertTestStatus(name, isNotDone)

#define assertTestPass(name) assertTestStatus(name, isPassed)

#define assertTestNotPass(name) assertTestStatus(name, isNotPassed)

#define assertTestFail(name) assertTestStatus(name, isFailed)

#define assertTestNotFail(name) assertTestStatus(name, isNotFailed)

#define assertTestSkip(name) assertTestStatus(name, isSkipped)

#define assertTestNotSkip(name) assertTestStatus(name, isNotSkipped)

#define assertTestExpire(name) assertTestStatus(name, isExpired)

#define assertTestNotExpire(name) assertTestStatus(name, isNotExpired)

#define assertTestStatus(name,method) do {\
  if (!aunit::assertionTestStatus(\
      __FILE__,__LINE__,#name,#method,test_##name##_instance.method()))\
    return;\
} while (false)

namespace aunit {

/**
 * Set the status of the current test based on 'ok, and  print assertion
 * message if requested.
 */
bool assertionTestStatus(const char* file, uint16_t line, 
    const char* testName, const char* statusName, bool ok);

}

#endif
