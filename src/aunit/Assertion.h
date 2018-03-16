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

#ifndef AUNIT_ASSERTION_H
#define AUNIT_ASSERTION_H

#include <Arduino.h>  // definition of Print
#include "Printer.h"
#include "Verbosity.h"
#include "TestRunner.h"

// Various assertXxx() macros, implemented using the assertOp() macro.

#define assertEqual(arg1,arg2) assertOp(arg1,aunit::compareEqual,"==",arg2)

#define assertNotEqual(arg1,arg2) \
    assertOp(arg1,aunit::compareNotEqual,"!=",arg2)

#define assertLess(arg1,arg2) assertOp(arg1,aunit::compareLess,"<",arg2)

#define assertMore(arg1,arg2) assertOp(arg1,aunit::compareMore,">",arg2)

#define assertLessOrEqual(arg1,arg2) \
    assertOp(arg1,aunit::compareLessOrEqual,"<=",arg2)

#define assertMoreOrEqual(arg1,arg2) \
    assertOp(arg1,aunit::compareMoreOrEqual,">=",arg2)

#define assertTrue(arg) assertEqual(arg,true)

#define assertFalse(arg) assertEqual(arg,false)

#define assertOp(arg1,op,opName,arg2) do {\
  if (!aunit::assertion(__FILE__,__LINE__,(arg1),opName,op,(arg2)))\
    return;\
} while (false)

// Meta tests, copied from ArduinoUnit for compatibility.

// The checkTestXxx() macros returns a boolean. Execution continues.

#define checkTestDone(name) \
    (test_##name##_instance.getStatus() >= Test::kStatusPassed)

#define checkTestNotDone(name) \
    (test_##name##_instance.getStatus() < Test::kStatusPassed)

#define checkTestPass(name) \
    (test_##name##_instance.getStatus() == Test::kStatusPassed)

#define checkTestNotPass(name) \
    (test_##name##_instance.getStatus() != Test::kStatusPassed)

#define checkTestFail(name) \
    (test_##name##_instance.getStatus() == Test::kStatusFailed)

#define checkTestNotFail(name) \
    (test_##name##_instance.getStatus() != Test::kStatusFailed)

#define checkTestSkip(name) \
    (test_##name##_instance.getStatus() == Test::kStatusSkipped)

#define checkTestNotSkip(name) \
    (test_##name##_instance.getStatus() != Test::kStatusSkipped)

// If the assertTestXxx() macros fail, they generate an optional output, calls
// fail(), and returns from the current test case.

#define assertTestDone(name) \
    assertMoreOrEqual(test_##name##_instance.getStatus(), Test::kStatusPassed)

#define assertTestNotDone(name) \
    assertLess(test_##name##_instance.getStatus(), Test::kStatusPassed)

#define assertTestPass(name) \
    assertEqual(test_##name##_instance.getStatus(), Test::kStatusPassed)

#define assertTestNotPass(name) \
    assertNotEqual(test_##name##_instance.getStatus(), Test::kStatusPassed)

#define assertTestFail(name) \
    assertEqual(test_##name##_instance.getStatus(), Test::kStatusFailed)

#define assertTestNotFail(name) \
    assertNotEqual(test_##name##_instance.getStatus(), Test::kStatusFailed)

#define assertTestSkip(name) \
    assertEqual(test_##name##_instance.getStatus(), Test::kStatusSkipped)

#define assertTestNotSkip(name) \
    assertNotEqual(test_##name##_instance.getStatus(), Test::kStatusSkipped)

namespace aunit {

// This can be a template function because it is accessed only through the
// various assertXxx() methods. Those assertXxx() methods are explicitly
// overloaded for the various types that we want to support.
//
// Prints something like the following:
// Assertion failed: (5) == (6), file Test.ino, line 820.
// Assertion passed: (6) == (6), file Test.ino, line 820.
template <typename A, typename B>
void printAssertionMessage(bool ok, const char* file, uint16_t line,
    const A& lhs, const char *opName, const B& rhs) {

  bool isOutput =
      (ok && TestRunner::isVerbosity(Verbosity::kAssertionPassed)) ||
      (!ok && TestRunner::isVerbosity(Verbosity::kAssertionFailed));
  if (!isOutput) return;

  // Don't use F() strings here because flash memory strings are not deduped by
  // the compiler, so each template instantiation of this method causes a
  // duplication of all the strings below. See
  // https://github.com/mmurdoch/arduinounit/issues/70
  // for more info.
  Printer::getPrinter()->print("Assertion ");
  Printer::getPrinter()->print(ok ? "passed" : "failed");
  Printer::getPrinter()->print(": (");
  Printer::getPrinter()->print(lhs);
  Printer::getPrinter()->print(") ");
  Printer::getPrinter()->print(opName);
  Printer::getPrinter()->print(" (");
  Printer::getPrinter()->print(rhs);
  Printer::getPrinter()->print("), file ");
  Printer::getPrinter()->print(file);
  Printer::getPrinter()->print(", line ");
  Printer::getPrinter()->print(line);
  Printer::getPrinter()->println('.');
}

// For the same reason as the compareXxx() methods, we use explicit overloaded
// functions, instead of using template specialization. And just as before, I
// was unable to use a template function for primitive integer types, because it
// interfered with the resolution of assertion(char*, char*). The wrong function
// would be called.

bool assertion(const char* file, uint16_t line, bool lhs,
    const char* opName, bool (*op)(bool lhs, bool rhs),
    bool rhs);

bool assertion(const char* file, uint16_t line, char lhs,
    const char* opName, bool (*op)(char lhs, char rhs),
    char rhs);

bool assertion(const char* file, uint16_t line, int lhs,
    const char* opName, bool (*op)(int lhs, int rhs),
    int rhs);

bool assertion(const char* file, uint16_t line, unsigned int lhs,
    const char* opName, bool (*op)(unsigned int lhs, unsigned int rhs),
    unsigned int rhs);

bool assertion(const char* file, uint16_t line, long lhs,
    const char* opName, bool (*op)(long lhs, long rhs),
    long rhs);

bool assertion(const char* file, uint16_t line, unsigned long lhs,
    const char* opName, bool (*op)(unsigned long lhs, unsigned long rhs),
    unsigned long rhs);

bool assertion(const char* file, uint16_t line, double lhs,
    const char* opName, bool (*op)(double lhs, double rhs),
    double rhs);

bool assertion(const char* file, uint16_t line, const char* lhs,
    const char* opName, bool (*op)(const char* lhs, const char* rhs),
    const char* rhs);

bool assertion(const char* file, uint16_t line, const char* lhs,
    const char *opName, bool (*op)(const char* lhs, const String& rhs),
    const String& rhs);

bool assertion(const char* file, uint16_t line, const char* lhs,
    const char *opName,
    bool (*op)(const char* lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs);

bool assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName, bool (*op)(const String& lhs, const char* rhs),
    const char* rhs);

bool assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName, bool (*op)(const String& lhs, const String& rhs),
    const String& rhs);

bool assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName,
    bool (*op)(const String& lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs);

bool assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const char* rhs),
    const char* rhs);

bool assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const String& rhs),
    const String& rhs);

bool assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs);

}
#endif
