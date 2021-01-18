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
 * Various macros (test(), testF(), testing(), testingF(), externTest(),
 * externTestF(), externTesting(), externTestingF()) are defined in this
 * header.
 */

#ifndef AUNIT_TEST_MACROS_H
#define AUNIT_TEST_MACROS_H

#include <stdint.h>
#include <Arduino.h> // F() macro
#include "Flash.h" // AUNIT_F() macro
#include "FCString.h"
#include "TestOnce.h"
#include "TestAgain.h"

/**
 * Macro to define a test that will be run only once.
 *
 * Two versions are supported: test(name) and test(suiteName, name).
 * The 2-argument test(suiteName, name) is a convenience macro which is
 * identical to test(suiteName_name).
 */
#define test(...) \
    GET_TEST(__VA_ARGS__, TEST2, TEST1)(__VA_ARGS__)

#define GET_TEST(_1, _2, NAME, ...) NAME

#define TEST1(name) \
class test_##name : public aunit::TestOnce {\
public:\
  test_##name();\
  void once() override;\
} test_##name##_instance;\
test_##name :: test_##name() {\
  init(AUNIT_F(#name)); \
}\
void test_##name :: once()

#define TEST2(suiteName, name) \
class suiteName##_##name : public aunit::TestOnce {\
public:\
  suiteName##_##name();\
  void once() override;\
} suiteName##_##name##_instance;\
suiteName##_##name :: suiteName##_##name() {\
  init(AUNIT_F(#suiteName "_" #name)); \
}\
void suiteName##_##name :: once()

/**
 * Macro to define a test that will run repeatly upon each iteration of the
 * global loop() method, stopping when the something calls Test::pass(),
 * Test::fail() or Test::skip().
 *
 * Two versions are supported: testing(name) and testing(suiteName, name).
 * The 2-argument testing(suiteName, name) is a convenience macro which is
 * identical to testing(suiteName_name).
 */
#define testing(...) \
    GET_TESTING(__VA_ARGS__, TESTING2, TESTING1)(__VA_ARGS__)

#define GET_TESTING(_1, _2, NAME, ...) NAME

#define TESTING1(name) \
class test_##name : public aunit::TestAgain {\
public:\
  test_##name();\
  void again() override;\
} test_##name##_instance;\
test_##name :: test_##name() {\
  init(AUNIT_F(#name));\
}\
void test_##name :: again()

#define TESTING2(suiteName, name) \
class suiteName##_##name : public aunit::TestAgain {\
public:\
  suiteName##_##name();\
  void again() override;\
} suiteName##_##name##_instance;\
suiteName##_##name :: suiteName##_##name() {\
  init(AUNIT_F(#suiteName "_" #name));\
}\
void suiteName##_##name :: again()

/**
 * Create an extern reference to a test() test case object defined elsewhere.
 * This is only necessary if you use assertTestXxx() or checkTestXxx() when the
 * test is in another file (or defined after the assertion on it).
 *
 * Two versions are supported: externTest(name) and externTest(suiteName, name).
 * The 2-argument externTest(suiteName, name) is a convenience macro which is
 * identical to externTest(suiteName_name).
 */
#define externTest(...) \
    GET_EXTERN_TEST(__VA_ARGS__, EXTERN_TEST2, EXTERN_TEST1)(__VA_ARGS__)

#define GET_EXTERN_TEST(_1, _2, NAME, ...) NAME

#define EXTERN_TEST1(name) \
class test_##name : public aunit::TestOnce {\
public:\
  test_##name();\
  void once();\
};\
extern test_##name test_##name##_instance

#define EXTERN_TEST2(suiteName, name) \
class suiteName##_##name : public aunit::TestOnce {\
public:\
  suiteName##_##name();\
  void once();\
};\
extern suiteName##_##name suiteName##_##name##_instance

/**
 * Create an extern reference to a testing() test case object defined
 * elsewhere.  This is only necessary if you use assertTestXxx() or
 * checkTestXxx() when the test is in another file (or defined after the
 * assertion on it).
 *
 * Two versions are supported: externTesting(name) and externTesting(suiteName,
 * name).
 */
#define externTesting(...) \
    GET_EXTERN_TESTING(__VA_ARGS__, EXTERN_TESTING2, EXTERN_TESTING1)(__VA_ARGS__)

#define GET_EXTERN_TESTING(_1, _2, NAME, ...) NAME

#define EXTERN_TESTING1(name) \
class test_ ## name : public aunit::TestAgain {\
public:\
  test_ ## name();\
  void again();\
};\
extern test_##name test_##name##_instance

#define EXTERN_TESTING2(suiteName, name) \
class suiteName##_ ## name : public aunit::TestAgain {\
public:\
  suiteName##_ ## name();\
  void again();\
};\
extern suiteName##_##name suiteName##_##name##_instance

/**
 * Create a test that is derived from a custom TestOnce class.
 * The name of the instance is prefixed by '{testClass}_' to avoid
 * name collisions with similarly named tests using other fixtures.
 */
#define testF(testClass, name) \
class testClass ## _ ## name : public testClass {\
public:\
  testClass ## _ ## name();\
  void once() override;\
} testClass ## _ ## name ## _instance;\
testClass ## _ ## name :: testClass ## _ ## name() {\
  init(AUNIT_F(#testClass "_" #name));\
}\
void testClass ## _ ## name :: once()

/**
 * Create a test that is derived from a custom TestAgain class.
 * The name of the instance is prefixed by '{testClass}_' to avoid
 * name collisions with similarly named tests using other fixtures.
 *
 * Note that test(suiteName, name) is different than testF(className, name),
 * but there will be a name collision if suiteName is the same as className.
 */
#define testingF(testClass, name) \
class testClass ## _ ## name : public testClass {\
public:\
  testClass ## _ ## name();\
  void again() override;\
} testClass ## _ ## name ## _instance;\
testClass ## _ ## name :: testClass ## _ ## name() {\
  init(AUNIT_F(#testClass "_" #name));\
}\
void testClass ## _ ## name :: again()

/**
 * Create an extern reference to a testF() test case object defined elsewhere.
 * This is only necessary if you use assertTestXxx() or checkTestXxx() when the
 * test is in another file (or defined after the assertion on it).
 */
#define externTestF(testClass, name) \
class testClass ## _ ## name : public testClass {\
public:\
  testClass ## _ ## name();\
  void once() override;\
};\
extern testClass ## _ ## name testClass##_##name##_instance

/**
 * Create an extern reference to a testingF() test case object defined
 * elsewhere.  This is only necessary if you use assertTestXxx() or
 * checkTestXxx() when the test is in another file (or defined after the
 * assertion on it).
 */
#define externTestingF(testClass, name) \
class testClass ## _ ## name : public testClass {\
public:\
  testClass ## _ ## name();\
  void again() override;\
};\
extern testClass ## _ ## name testClass##_##name##_instance

#endif
