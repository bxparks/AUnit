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
 * @file TestMacros.h
 *
 * Various macros (test(), testing(), externTest(), externTesting()) are
 * defined in this header.
 */

#ifndef AUNIT_TEST_MACROS_H
#define AUNIT_TEST_MACROS_H

#include <stdint.h>
#include <Arduino.h> // F() macro
#include "Flash.h"
#include "FCString.h"
#include "TestOnce.h"
#include "TestAgain.h"

// On the ESP8266 platform, The F() string cannot be placed in an inline
// context, because it interferes with other PROGMEM strings. See
// https://github.com/esp8266/Arduino/issues/3369. The solution was to move the
// constructor definition out from an inline function into a normal function
// defined outside of the class declaration..

/** Macro to define a test that will be run only once. */
#define test(name) struct test_ ## name : aunit::TestOnce {\
  test_ ## name();\
  virtual void once() override;\
} test_ ## name ## _instance;\
test_ ## name :: test_ ## name() {\
  init(F(#name)); \
}\
void test_ ## name :: once()

/**
 * Macro to define a test that will run repeatly upon each iteration of the
 * global loop() method, stopping when the something calls Test::pass(),
 * Test::fail() or Test::skip().
 */
#define testing(name) struct test_ ## name : aunit::TestAgain {\
  test_ ## name();\
  virtual void again() override;\
} test_ ## name ## _instance;\
test_ ## name :: test_ ## name() {\
  init(F(#name));\
}\
void test_ ## name :: again()

/**
 * Create an extern reference to a test() test case object defined elsewhere.
 * This is only necessary if you use assertTestXxx() or checkTestXxx() when the
 * test is in another file (or defined after the assertion on it).
 */
#define externTest(name) struct test_ ## name : aunit::TestOnce {\
  test_ ## name();\
  void once();\
};\
extern test_##name test_##name##_instance

/**
 * Create an extern reference to a testing() test case object defined
 * elsewhere.  This is only necessary if you use assertTestXxx() or
 * checkTestXxx() when the test is in another file (or defined after the
 * assertion on it).
 */
#define externTesting(name) struct test_ ## name : aunit::TestAgain {\
  test_ ## name();\
  void again();\
};\
extern test_##name test_##name##_instance

/**
 * Create a test that is derived from a custom TestOnce class.
 * The name of the instance is prefixed by '{test_class}_' to avoid
 * name collisions with similarly named tests using other fixtures.
 */
#define testF(test_class, name) \
struct test_class ## _ ## name : test_class {\
  test_class ## _ ## name();\
  virtual void once() override;\
} test_class ## _ ## name ## _instance;\
test_class ## _ ## name :: test_class ## _ ## name() {\
  init(F(#test_class "_" #name));\
}\
void test_class ## _ ## name :: once()

/**
 * Create a test that is derived from a custom TestAgain class.
 * The name of the instance is prefixed by '{test_class}_' to avoid
 * name collisions with similarly named tests using other fixtures.
 */
#define testingF(test_class, name) \
struct test_class ## _ ## name : test_class {\
  test_class ## _ ## name();\
  virtual void again() override;\
} test_class ## _ ## name ## _instance;\
test_class ## _ ## name :: test_class ## _ ## name() {\
  init(F(#test_class "_" #name));\
}\
void test_class ## _ ## name :: again()

/**
 * Create an extern reference to a testF() test case object defined elsewhere.
 * This is only necessary if you use assertTestXxx() or checkTestXxx() when the
 * test is in another file (or defined after the assertion on it).
 */
#define externTestF(test_class, name) \
struct test_class ## _ ## name : test_class {\
  test_class ## _ ## name();\
  virtual void once() override;\
};\
extern test_class ## _ ## name test_class##_##name##_instance

/**
 * Create an extern reference to a testingF() test case object defined
 * elsewhere.  This is only necessary if you use assertTestXxx() or
 * checkTestXxx() when the test is in another file (or defined after the
 * assertion on it).
 */
#define externTestingF(test_class, name) \
struct test_class ## _ ## name : test_class {\
  test_class ## _ ## name();\
  virtual void again() override;\
};\
extern test_class ## _ ## name test_class##_##name##_instance

#endif
