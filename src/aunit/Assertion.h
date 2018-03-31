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
 * @file Assertion.h
 *
 * Various assertXxx() macros are defined in this header. They all go through
 * another helper macro called assertOp(), eventually calling one of the
 * methods on the Assertion class.
 */

#ifndef AUNIT_ASSERTION_H
#define AUNIT_ASSERTION_H

#include "Test.h"

/** Assert that arg1 is equal to arg2. */
#define assertEqual(arg1,arg2) assertOp(arg1,aunit::compareEqual,"==",arg2)

/** Assert that arg1 is not equal to arg2. */
#define assertNotEqual(arg1,arg2) \
    assertOp(arg1,aunit::compareNotEqual,"!=",arg2)

/** Assert that arg1 is less than arg2. */
#define assertLess(arg1,arg2) assertOp(arg1,aunit::compareLess,"<",arg2)

/** Assert that arg1 is more than arg2. */
#define assertMore(arg1,arg2) assertOp(arg1,aunit::compareMore,">",arg2)

/** Assert that arg1 is less than or equal to arg2. */
#define assertLessOrEqual(arg1,arg2) \
    assertOp(arg1,aunit::compareLessOrEqual,"<=",arg2)

/** Assert that arg1 is more than or equal to arg2. */
#define assertMoreOrEqual(arg1,arg2) \
    assertOp(arg1,aunit::compareMoreOrEqual,">=",arg2)

/** Assert that arg is true. */
#define assertTrue(arg) assertEqual(arg,true)

/** Assert that arg is false. */
#define assertFalse(arg) assertEqual(arg,false)

/** Internal helper macro, shouldn't be called directly by users. */
#define assertOp(arg1,op,opName,arg2) do {\
  if (!assertion(__FILE__,__LINE__,(arg1),opName,op,(arg2)))\
    return;\
} while (false)

class __FlashStringHelper;
class String;

namespace aunit {

/**
 * An Assertion class is a subclass of Test and provides various overloaded
 * assertion() functions. Having this class inherit from Test allows it to
 * have access to the mVerbosity setting, as well as the test's current
 * mStatus. (An earlier implementation inverted the class hierarchy between
 * Assertion and Test). That allows every assertion() method to bail out early
 * if it detects the result of a previous assertion() in mStatus. This delayed
 * bailout may happen if the assertXxx() macro was called from inside a helper
 * method of a fixture class used by testF() or testingF() macros.
 *
 * For the same reason as the compareXxx() methods, we use explicit overloaded
 * functions, instead of using template specialization. And just as before, I
 * was unable to use a template function for primitive integer types, because
 * it interfered with the resolution of assertion(char*, char*). The wrong
 * function would be called.
 *
 * The assertion() methods are internal helpers, they should not be called
 * directly by users.
 */
class Assertion: public Test {
  protected:
    /** Empty constructor. */
    Assertion() {}

    /** Returns true if an assertion message should be printed. */
    bool isOutputEnabled(bool ok);

    // NOTE: Don't create a virtual destructor. That's the normal best practice
    // for classes that will be used polymorphically. However, this class will
    // never be deleted polymorphically (i.e. through its pointer) so it
    // doesn't need a virtual destructor. In fact, adding it causes flash and
    // static memory to increase dramatically because each test() and testing()
    // macro creates a new subclass. AceButtonTest flash memory increases from
    // 18928 to 20064 bytes, and static memory increases from 917 to 1055
    // bytes.

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
        bool (*op)(const __FlashStringHelper* lhs,
        const __FlashStringHelper* rhs),
        const __FlashStringHelper* rhs);

  private:
    // Disable copy-constructor and assignment operator
    Assertion(const Assertion&) = delete;
    Assertion& operator=(const Assertion&) = delete;
};

}

#endif
