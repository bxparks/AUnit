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
// https://github.com/mmurdoch/arduinounit/blob/master/src/ArduinoUnitUtility/Compare.h

/**
 * @file Compare.h
 *
 * This file provides overloaded compareXxx(a, b) functions which are used by
 * the various assertXxx(a, b) macros. We wanted to allow users to use the
 * assertXxx() macros with all combinations of the 3 types of strings available
 * in the Arduino platform:
 *
 *  - (const char*)
 *  - (String&)
 *  - (const __FlashStringHelper*)
 *
 * Clearly, there are 9 binary combinations these string types, so we define 9
 * compareString(a, b) functions.
 *
 * For other primitive types, I depend on implicit conversion rules to reduce
 * the overloaded types to 7:
 *
 *  - (bool, bool)
 *  - (char, char)
 *  - (int, int)
 *  - (unsigned int, unsigned int)
 *  - (long, long)
 *  - (unsigned long, unsigned long)
 *  - (double, double)
 *
 * Therefore, there are 16 overloaded versions of each of the compareXxx(a, b)
 * functions.
 *
 * There are 6 compareXxx() functions corresponding to (==, !=, <, >, <=, >=).
 * Many of them are implemented using the primitive logical operators for
 * the respective primitive types. The compare functions for string types are
 * implemented using compareString().
 *
 * All versions of compareString() and compareStringN() accept nullptr
 * arguments (in constrast to strcmp() and strncmp() where their behavior for
 * nullptr are undefined by the C standard.) If both arguments are nullptr,
 * then the strings are considered equal (returns 0). Otherwise, the nullptr is
 * arbitrarily defined to be less than all non-null strings, including the
 * empty string.
 */

#ifndef AUNIT_COMPARE_H
#define AUNIT_COMPARE_H

#include <stddef.h> // size_t

class String;
class __FlashStringHelper;

namespace aunit {
namespace internal {

//---------------------------------------------------------------------------
// compareString()
//---------------------------------------------------------------------------

int compareString(const char* a, const char* b);

int compareString(const char* a, const String& b);

int compareString(const char* a, const __FlashStringHelper* b);

int compareString(const String& a, const char* b);

int compareString(const String& a, const String& b);

int compareString(const String& a, const __FlashStringHelper* b);

int compareString(const __FlashStringHelper* a, const char* b);

int compareString(const __FlashStringHelper* a, const __FlashStringHelper* b);

int compareString(const __FlashStringHelper* a, const String& b);

//---------------------------------------------------------------------------
// compareStringCase() - case insensitive versions of compareString()
//---------------------------------------------------------------------------

int compareStringCase(const char* a, const char* b);

int compareStringCase(const char* a, const String& b);

int compareStringCase(const char* a, const __FlashStringHelper* b);

int compareStringCase(const String& a, const char* b);

int compareStringCase(const String& a, const String& b);

int compareStringCase(const String& a, const __FlashStringHelper* b);

int compareStringCase(const __FlashStringHelper* a, const char* b);

int compareStringCase(const __FlashStringHelper* a,
    const __FlashStringHelper* b);

int compareStringCase(const __FlashStringHelper* a, const String& b);

//---------------------------------------------------------------------------
// compareStringN()
//
// These functions are used to implement the TestRunner::exclude() and
// TestRunner::include() features.
//---------------------------------------------------------------------------

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const char* a, const char* b, size_t n);

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const char* a, const __FlashStringHelper* b, size_t n);

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const __FlashStringHelper* a, const char* b, size_t n);

/** Compare only the first n characters of 'a' or 'b'. */
int compareStringN(const __FlashStringHelper* a, const __FlashStringHelper* b,
    size_t n);

//---------------------------------------------------------------------------
// compareSubstring(haystack, needle)
//---------------------------------------------------------------------------

bool compareSubstring(const char* haystack, const char* needle);

bool compareSubstring(const char* haystack, const String& needle);

bool compareSubstring(const char* haystack, const __FlashStringHelper* needle);

bool compareSubstring(const String& haystack, const char* needle);

bool compareSubstring(const String& haystack, const String& needle);

bool compareSubstring(
    const String& haystack, const __FlashStringHelper* needle);

/**
 * An inefficient O(M*N) implementation of strstr() for PROGMEM strings.
 * The KMP algorithm
 * https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
 * is faster, but this brute force implementation is probably good enough.
 */
bool compareSubstring(
    const __FlashStringHelper* haystack,
    const char* needle);

bool compareSubstring(
    const __FlashStringHelper* haystack,
    const String& needle);

/**
 * An inefficient O(M*N) implementation of strstr() for PROGMEM strings.
 * The KMP algorithm
 * https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
 * is faster, but this brute force implementation is probably good enough.
 */
bool compareSubstring(
    const __FlashStringHelper* haystack,
    const __FlashStringHelper* needle);

//---------------------------------------------------------------------------
// compareEqual()
//---------------------------------------------------------------------------

bool compareEqual(bool a, bool b);

bool compareEqual(char a, char b);

bool compareEqual(int a, int b);

bool compareEqual(unsigned int a, unsigned int b);

bool compareEqual(long a, long b);

bool compareEqual(unsigned long a, unsigned long b);

bool compareEqual(long long a, long long b);

bool compareEqual(unsigned long long a, unsigned long long b);

bool compareEqual(double a, double b);

bool compareEqual(const void* a, const void* b);

bool compareEqual(const char* a, const char* b);

bool compareEqual(const char* a, const String& b);

bool compareEqual(const char* a, const __FlashStringHelper* b);

bool compareEqual(const __FlashStringHelper* a, const char* b);

bool compareEqual( const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareEqual(const __FlashStringHelper* a, const String& b);

bool compareEqual(const String& a, const char* b);

bool compareEqual(const String& a, const String& b);

bool compareEqual(const String& a, const __FlashStringHelper* b);

//---------------------------------------------------------------------------
// compareLess()
//---------------------------------------------------------------------------

bool compareLess(bool a, bool b);

bool compareLess(char a, char b);

bool compareLess(int a, int b);

bool compareLess(unsigned int a, unsigned int b);

bool compareLess(long a, long b);

bool compareLess(unsigned long a, unsigned long b);

bool compareLess(long long a, long long b);

bool compareLess(unsigned long long a, unsigned long long b);

bool compareLess(double a, double b);

bool compareLess(const char* a, const char* b);

bool compareLess(const char* a, const String& b);

bool compareLess(const char* a, const __FlashStringHelper* b);

bool compareLess(const __FlashStringHelper* a, const char* b);

bool compareLess(const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareLess(const __FlashStringHelper* a, const String& b);

bool compareLess(const String& a, const char* b);

bool compareLess(const String& a, const String& b);

bool compareLess(const String& a, const __FlashStringHelper* b);

//---------------------------------------------------------------------------
// compareMore()
//---------------------------------------------------------------------------

bool compareMore(bool a, bool b);

bool compareMore(char a, char b);

bool compareMore(int a, int b);

bool compareMore(unsigned int a, unsigned int b);

bool compareMore(long a, long b);

bool compareMore(unsigned long a, unsigned long b);

bool compareMore(long long a, long long b);

bool compareMore(unsigned long long a, unsigned long long b);

bool compareMore(double a, double b);

bool compareMore(const char* a, const char* b);

bool compareMore(const char* a, const String& b);

bool compareMore(const char* a, const __FlashStringHelper* b);

bool compareMore(const __FlashStringHelper* a, const char* b);

bool compareMore(const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareMore(const __FlashStringHelper* a, const String& b);

bool compareMore(const String& a, const char* b);

bool compareMore(const String& a, const String& b);

bool compareMore(const String& a, const __FlashStringHelper* b);

//---------------------------------------------------------------------------
// compareLessOrEqual
//---------------------------------------------------------------------------

bool compareLessOrEqual(bool a, bool b);

bool compareLessOrEqual(char a, char b);

bool compareLessOrEqual(int a, int b);

bool compareLessOrEqual(unsigned int a, unsigned int b);

bool compareLessOrEqual(long a, long b);

bool compareLessOrEqual(unsigned long a, unsigned long b);

bool compareLessOrEqual(long long a, long long b);

bool compareLessOrEqual(unsigned long long a, unsigned long long b);

bool compareLessOrEqual(double a, double b);

bool compareLessOrEqual(const char* a, const char* b);

bool compareLessOrEqual(const char* a, const String& b);

bool compareLessOrEqual(const char* a, const __FlashStringHelper* b);

bool compareLessOrEqual(const __FlashStringHelper* a, const char* b);

bool compareLessOrEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareLessOrEqual(const __FlashStringHelper* a, const String& b);

bool compareLessOrEqual(const String& a, const char* b);

bool compareLessOrEqual(const String& a, const String& b);

bool compareLessOrEqual(const String& a, const __FlashStringHelper* b);

//---------------------------------------------------------------------------
// compareMoreOrEqual
//---------------------------------------------------------------------------

bool compareMoreOrEqual(bool a, bool b);

bool compareMoreOrEqual(char a, char b);

bool compareMoreOrEqual(int a, int b);

bool compareMoreOrEqual(unsigned int a, unsigned int b);

bool compareMoreOrEqual(long a, long b);

bool compareMoreOrEqual(unsigned long a, unsigned long b);

bool compareMoreOrEqual(long long a, long long b);

bool compareMoreOrEqual(unsigned long long a, unsigned long long b);

bool compareMoreOrEqual(double a, double b);

bool compareMoreOrEqual(const char* a, const char* b);

bool compareMoreOrEqual(const char* a, const String& b);

bool compareMoreOrEqual(const char* a, const __FlashStringHelper* b);

bool compareMoreOrEqual(const __FlashStringHelper* a, const char* b);

bool compareMoreOrEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareMoreOrEqual(const __FlashStringHelper* a, const String& b);

bool compareMoreOrEqual(const String& a, const char* b);

bool compareMoreOrEqual(const String& a, const String& b);

bool compareMoreOrEqual(const String& a, const __FlashStringHelper* b);

//---------------------------------------------------------------------------
// compareNotEqual
//---------------------------------------------------------------------------

bool compareNotEqual(bool a, bool b);

bool compareNotEqual(char a, char b);

bool compareNotEqual(int a, int b);

bool compareNotEqual(unsigned int a, unsigned int b);

bool compareNotEqual(long a, long b);

bool compareNotEqual(unsigned long a, unsigned long b);

bool compareNotEqual(long long a, long long b);

bool compareNotEqual(unsigned long long a, unsigned long long b);

bool compareNotEqual(double a, double b);

bool compareNotEqual(const void* a, const void* b);

bool compareNotEqual(const char* a, const char* b);

bool compareNotEqual(const char* a, const String& b);

bool compareNotEqual(const char* a, const __FlashStringHelper* b);

bool compareNotEqual(const __FlashStringHelper* a, const char* b);

bool compareNotEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b);

bool compareNotEqual(const __FlashStringHelper* a, const String& b);

bool compareNotEqual(const String& a, const char* b);

bool compareNotEqual(const String& a, const String& b);

bool compareNotEqual(const String& a, const __FlashStringHelper* b);

//---------------------------------------------------------------------------
// compareStringCaseEqual
//---------------------------------------------------------------------------

bool compareStringCaseEqual(const char* a, const char* b);

bool compareStringCaseEqual(const char* a, const String& b);

bool compareStringCaseEqual(const char* a, const __FlashStringHelper* b);

bool compareStringCaseEqual(const __FlashStringHelper* a, const char* b);

bool compareStringCaseEqual( const __FlashStringHelper* a,
    const __FlashStringHelper* b);

bool compareStringCaseEqual(const __FlashStringHelper* a, const String& b);

bool compareStringCaseEqual(const String& a, const char* b);

bool compareStringCaseEqual(const String& a, const String& b);

bool compareStringCaseEqual(const String& a, const __FlashStringHelper* b);

//---------------------------------------------------------------------------
// compareStringCaseNotEqual
//---------------------------------------------------------------------------

bool compareStringCaseNotEqual(const char* a, const char* b);

bool compareStringCaseNotEqual(const char* a, const String& b);

bool compareStringCaseNotEqual(const char* a, const __FlashStringHelper* b);

bool compareStringCaseNotEqual(const __FlashStringHelper* a, const char* b);

bool compareStringCaseNotEqual( const __FlashStringHelper* a,
    const __FlashStringHelper* b);

bool compareStringCaseNotEqual(const __FlashStringHelper* a, const String& b);

bool compareStringCaseNotEqual(const String& a, const char* b);

bool compareStringCaseNotEqual(const String& a, const String& b);

bool compareStringCaseNotEqual(const String& a, const __FlashStringHelper* b);

//---------------------------------------------------------------------------
// compareNear
//---------------------------------------------------------------------------

bool compareNear(int a, int b, int error);

bool compareNear(unsigned int a, unsigned int b, unsigned int error);

bool compareNear(long a, long b, long error);

bool compareNear(unsigned long a, unsigned long b, unsigned long error);

bool compareNear(double a, double b, double error);

//---------------------------------------------------------------------------
// compareNotNear
//---------------------------------------------------------------------------

bool compareNotNear(int a, int b, int error);

bool compareNotNear(unsigned int a, unsigned int b, unsigned int error);

bool compareNotNear(long a, long b, long error);

bool compareNotNear(unsigned long a, unsigned long b, unsigned long error);

bool compareNotNear(double a, double b, double error);

}
}

#endif
