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

#ifndef AUNIT_ASSERTION_H
#define AUNIT_ASSERTION_H

#include "Flash.h"
#include "Test.h"

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
    Assertion() = default;

    /** Returns true if an assertion message should be printed. */
    bool isOutputEnabled(bool ok) const;

    // Terse assertions. Prints only the argument values.

    /** Used by assertTrue() and assertFalse(). */
    bool assertionBool(
        const char* file,
        uint16_t line,
        bool arg,
        bool value);

    /** Used by assertXxx(bool, bool). */
    bool assertion(
        const char* file,
        uint16_t line,
        bool lhs,
        const char* opName,
        bool (*op)(bool lhs, bool rhs),
        bool rhs);

    /** Used by assertXxx(char, char). */
    bool assertion(
        const char* file,
        uint16_t line,
        char lhs,
        const char* opName,
        bool (*op)(char lhs, char rhs),
        char rhs);

    /** Used by assertXxx(int, int). */
    bool assertion(
        const char* file,
        uint16_t line,
        int lhs,
        const char* opName,
        bool (*op)(int lhs, int rhs),
        int rhs);

    /** Used by assertXxx(unsigned int, unsigned int). */
    bool assertion(
        const char* file,
        uint16_t line,
        unsigned int lhs,
        const char* opName,
        bool (*op)(unsigned int lhs, unsigned int rhs),
        unsigned int rhs);

    /** Used by assertXxx(long, long). */
    bool assertion(
        const char* file,
        uint16_t line,
        long lhs,
        const char* opName,
        bool (*op)(long lhs, long rhs),
        long rhs);

    /** Used by assertXxx(unsigned long, unsigned long). */
    bool assertion(
        const char* file,
        uint16_t line,
        unsigned long lhs,
        const char* opName,
        bool (*op)(unsigned long lhs, unsigned long rhs),
        unsigned long rhs);

    /** Used by assertXxx(long long, long long). */
    bool assertion(
        const char* file,
        uint16_t line,
        long long lhs,
        const char* opName,
        bool (*op)(long long lhs, long long rhs),
        long long rhs);

    /** Used by assertXxx(unsigned long long, unsigned long long). */
    bool assertion(
        const char* file,
        uint16_t line,
        unsigned long long lhs,
        const char* opName,
        bool (*op)(unsigned long long lhs, unsigned long long rhs),
        unsigned long long rhs);

    /** Used by assertXxx(double, double). */
    bool assertion(
        const char* file,
        uint16_t line,
        double lhs,
        const char* opName,
        bool (*op)(double lhs, double rhs),
        double rhs);

    /** Used by assertXxx(const void*, const void*). */
    bool assertion(
        const char* file,
        uint16_t line,
        const void* lhs,
        const char* opName,
        bool (*op)(const void* lhs, const void* rhs),
        const void* rhs);

    /** Used by assertXxx(const char*, const char*). */
    bool assertion(
        const char* file,
        uint16_t line,
        const char* lhs,
        const char* opName,
        bool (*op)(const char* lhs, const char* rhs),
        const char* rhs);

    /** Used by assertXxx(const char*, const String&). */
    bool assertion(
        const char* file,
        uint16_t line,
        const char* lhs,
        const char* opName,
        bool (*op)(const char* lhs, const String& rhs),
        const String& rhs);

    /** Used by assertXxx(const char*, const __FlashStringHelper*). */
    bool assertion(
        const char* file,
        uint16_t line,
        const char* lhs,
        const char* opName,
        bool (*op)(const char* lhs, const __FlashStringHelper* rhs),
        const __FlashStringHelper* rhs);

    /** Used by assertXxx(const String&, const char*). */
    bool assertion(
        const char* file,
        uint16_t line,
        const String& lhs,
        const char* opName,
        bool (*op)(const String& lhs, const char* rhs),
        const char* rhs);

    /** Used by assertXxx(const String&, const String&). */
    bool assertion(
        const char* file,
        uint16_t line,
        const String& lhs,
        const char* opName,
        bool (*op)(const String& lhs, const String& rhs),
        const String& rhs);

    /** Used by assertXxx(const String&, const __FlashStringHelper*). */
    bool assertion(
        const char* file,
        uint16_t line,
        const String& lhs,
        const char* opName,
        bool (*op)(const String& lhs, const __FlashStringHelper* rhs),
        const __FlashStringHelper* rhs);

    /** Used by assertXxx(const String&, const char*). */
    bool assertion(
        const char* file,
        uint16_t line,
        const __FlashStringHelper* lhs,
        const char* opName,
        bool (*op)(const __FlashStringHelper* lhs, const char* rhs),
        const char* rhs);

    /** Used by assertXxx(const __FlashStringHelper*, const String&). */
    bool assertion(
        const char* file,
        uint16_t line,
        const __FlashStringHelper* lhs,
        const char* opName,
        bool (*op)(const __FlashStringHelper* lhs, const String& rhs),
        const String& rhs);

    /**
     * Used by assertXxx(
     *   const __FlashStringHelper*,
     *   const __FlashStringHelper*).
     */
    bool assertion(
        const char* file,
        uint16_t line,
        const __FlashStringHelper* lhs,
        const char* opName,
        bool (*op)(
            const __FlashStringHelper* lhs,
            const __FlashStringHelper* rhs),
        const __FlashStringHelper* rhs);

    /** Used by assertNear(int, int). */
    bool assertionNear(
        const char* file,
        uint16_t line,
        int lhs,
        int rhs,
        int error,
        const char* opName,
        bool (*compareNear)(int lhs, int rhs, int error));

    /** Used by assertNear(unsigned int, unsigned int). */
    bool assertionNear(
        const char* file,
        uint16_t line,
        unsigned int lhs,
        unsigned int rhs,
        unsigned int error,
        const char* opName,
        bool (*compareNear)(
            unsigned int lhs, unsigned int rhs, unsigned int error));

    /** Used by assertNear(long, long). */
    bool assertionNear(
        const char* file,
        uint16_t line,
        long lhs,
        long rhs,
        long error,
        const char* opName,
        bool (*compareNear)(long lhs, long rhs, long error));

    /** Used by assertNear(unsigned long, unsigned long). */
    bool assertionNear(
        const char* file,
        uint16_t line,
        unsigned long lhs,
        unsigned long rhs,
        unsigned long error,
        const char* opName,
        bool (*compareNear)(
            unsigned long lhs, unsigned long rhs, unsigned long error));

    /** Used by assertNear(double, double). */
    bool assertionNear(
        const char* file,
        uint16_t line,
        double lhs,
        double rhs,
        double error,
        const char* opName,
        bool (*compareNear)(double lhs, double rhs, double error));

    // Verbose versions of above.

    /** Used by assertTrue() and assertFalse(). */
    bool assertionBoolVerbose(
        const char* file,
        uint16_t line,
        bool arg,
        const __FlashStringHelper* argString,
        bool value);

    /** Used by assertEqual(bool, bool). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        bool lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(bool lhs, bool rhs),
        bool rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(char, char). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        char lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(char lhs, char rhs),
        char rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(int, int). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        int lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(int lhs, int rhs),
        int rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(unsigned int, unsigned int). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        unsigned int lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(unsigned int lhs, unsigned int rhs),
        unsigned int rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(long, long). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        long lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(long lhs, long rhs),
        long rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(unsigned long, unsigned long). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        unsigned long lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(unsigned long lhs, unsigned long rhs),
        unsigned long rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(long long, long long). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        long long lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(long long lhs, long long rhs),
        long long rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(unsigned long long, unsigned long long). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        unsigned long long lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(unsigned long long lhs, unsigned long long rhs),
        unsigned long long rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(double, double). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        double lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(double lhs, double rhs),
        double rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const void*, const void*). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const void* lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const void* lhs, const void* rhs),
        const void* rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const char*, const char*). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const char* lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const char* lhs, const char* rhs),
        const char* rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const char*, const String&). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const char* lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const char* lhs, const String& rhs),
        const String& rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const char*, const __FlashStringHelper*). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const char* lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const char* lhs, const __FlashStringHelper* rhs),
        const __FlashStringHelper* rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const String&, const char*). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const String& lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const String& lhs, const char* rhs),
        const char* rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const String&, const String&). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const String& lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const String& lhs, const String& rhs),
        const String& rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const String&, const __FlashStringHelper*). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const String& lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const String& lhs, const __FlashStringHelper* rhs),
        const __FlashStringHelper* rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const String&, const char*). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const __FlashStringHelper* lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const __FlashStringHelper* lhs, const char* rhs),
        const char* rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertXxx(const __FlashStringHelper*, const String&). */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const __FlashStringHelper* lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(const __FlashStringHelper* lhs, const String& rhs),
        const String& rhs,
        const __FlashStringHelper* rhsString);

    /**
     * Used by assertXxx(
     *   const __FlashStringHelper*,
     *   const __FlashStringHelper*).
     */
    bool assertionVerbose(
        const char* file,
        uint16_t line,
        const __FlashStringHelper* lhs,
        const __FlashStringHelper* lhsString,
        const char* opName,
        bool (*op)(
            const __FlashStringHelper* lhs,
            const __FlashStringHelper* rhs),
        const __FlashStringHelper* rhs,
        const __FlashStringHelper* rhsString);

    /** Used by assertNear(int, int). */
    bool assertionNearVerbose(
        const char* file,
        uint16_t line,
        int lhs,
        const __FlashStringHelper* lhsString,
        int rhs,
        const __FlashStringHelper* rhsString,
        int error,
        const __FlashStringHelper* errorString,
        const char* opName,
        bool (*compareNear)(int lhs, int rhs, int error));

    /** Used by assertNear(unsigned int, unsigned int). */
    bool assertionNearVerbose(
        const char* file,
        uint16_t line,
        unsigned int lhs,
        const __FlashStringHelper* lhsString,
        unsigned int rhs,
        const __FlashStringHelper* rhsString,
        unsigned int error,
        const __FlashStringHelper* errorString,
        const char* opName,
        bool (*compareNear)(
            unsigned int lhs, unsigned int rhs, unsigned int error));

    /** Used by assertNear(long, long). */
    bool assertionNearVerbose(
        const char* file,
        uint16_t line,
        long lhs,
        const __FlashStringHelper* lhsString,
        long rhs,
        const __FlashStringHelper* rhsString,
        long error,
        const __FlashStringHelper* errorString,
        const char* opName,
        bool (*compareNear)(long lhs, long rhs, long error));

    /** Used by assertNear(unsigned long, unsigned long). */
    bool assertionNearVerbose(
        const char* file,
        uint16_t line,
        unsigned long lhs,
        const __FlashStringHelper* lhsString,
        unsigned long rhs,
        const __FlashStringHelper* rhsString,
        unsigned long error,
        const __FlashStringHelper* errorString,
        const char* opName,
        bool (*compareNear)(
            unsigned long lhs, unsigned long rhs, unsigned long error));

    /** Used by assertNear(double, double). */
    bool assertionNearVerbose(
        const char* file,
        uint16_t line,
        double lhs,
        const __FlashStringHelper* lhsString,
        double rhs,
        const __FlashStringHelper* rhsString,
        double error,
        const __FlashStringHelper* errorString,
        const char* opName,
        bool (*compareNear)(double lhs, double rhs, double error));

  private:
    // Disable copy-constructor and assignment operator
    Assertion(const Assertion&) = delete;
    Assertion& operator=(const Assertion&) = delete;
};

}

#endif
