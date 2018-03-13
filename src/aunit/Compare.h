#ifndef AUNIT_COMPARE_H
#define AUNIT_COMPARE_H

#ifdef ESP8266
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

#include <WString.h>
#include <string.h>

class FCString;

/*
 * This file provides overloaded compareXxx(a, b) functions which are used by
 * the various assertXxx() macros. A primary goal of this file is to allow users
 * to use the assertXxx() macros with all combinations of the 3 types of strings
 * available in the Arduino platform:
 *
 *  - (const char *)
 *  - (String&)
 *  - (const __FlashStringHelper*)
 *
 * Clearly, there are 9 binary combinations these string types.
 *
 * Template Specialization:
 * -----------------------
 * One way to implement the compareEqual() for these types is to use template
 * specialization. The problem with Template specialization is that templates
 * use strict type matching, and does not perform the normal implicit type
 * conversion, including const-casting. Therefore, all of the various c-style
 * string types, for example:
 *
 *  - char*
 *  - const char*
 *  - char[1]
 *  - char[N]
 *  - const char[1]
 *  - const char[N]
 *
 * are considered to be different types under the C++ templating system. This
 * causes a combinatorial explosion of template specialization which produces
 * code that is difficult to understand, test and maintain.
 * An example can be seen in the Compare.h file of the ArduinoUnit project:
 * https://github.com/mmurdoch/arduinounit/blob/master/src/ArduinoUnitUtility/Compare.h
 *
 * Function Overloading:
 * ---------------------
 * In this project, I used function overloading instead of template
 * specialization. Functional overloading handles c-style strings (i.e.
 * character arrays) naturally, in the way users expect. For example, (char*) is
 * automarically cast to (const char*), and (char[N]) is autonmatically
 * converted to (const char*).
 *
 * Since all the primitive value types (e.g. (char), (int), (unsigned char),
 * etc.) can be implemented using the exact same code, I attempted to use a
 * generic templatized version, using sonmething like:
 *
 *    template<typename T>
 *    compareEqual(const T& a, const T& b) { ... }
 *
 * However, this template introduced this method:
 *
 *    compareEqual(char* const& a, char* const& b);
 *
 * that seemed to take precedence over the explicitly defined overload:
 *
 *    compareEqual(const char* a, const char*b);
 *
 * When the compareEqual() method is called with a (char*) or a (char[N]):
 *
 *    char a[3] = {...};
 *    char b[4] = {...};
 *    compareEqual(a, b);
 *
 * This calls compareEqual(char* const&, const* const&), which is the wrong
 * version for a c-style string. The only way I could get this to work was to
 * avoid templates completely and manually define all the function overloads
 * even for primitive integer types.
 *
 * Implicit Conversions:
 * ---------------------
 * For basic primitive types, I depend on some casts to avoid having to define
 * some functions. I assume that signed and unsigned intergers smaller or equal
 * to (int) will be converted to an (int) to match compareEqual(int, int).
 *
 * I provided an explicit compareEqual(char, char) overload because in C++, a
 * (char) type is distinct from (signed char) and (unsigned char).
 *
 * Technically, there should be a (long long) version and an (unsigned long
 * long) version of compareEqual(). However, it turns out that the Arduino
 * Print::print() method does not have an overload for these types, so it would
 * not do us much good to provide an assertEqual() or compareEqual() for the
 * (long long) and (unsigned long long) types.
 *
 * Custom Assert and Compare Functions:
 * ------------------------------------
 * Another advantage of using function overloading instead of template
 * specialization is that the user is able to add additional function overloads
 * into the 'aunit' namespace. This should allow the user to define the various
 * comporeXxx() and assertXxx() functions for a custom class. I have not
 * tested this though.
 */
namespace aunit {

// compareString()

inline int compareString(const char* a, const char* b) {
  return strcmp(a, b);
}

inline int compareString(const char* a, const String& b) {
  return strcmp(a, b.c_str());
}

inline int compareString(const char* a, const __FlashStringHelper* b) {
  return strcmp_P(a, (const char*)b);
}

inline int compareString(const String& a, const char* b) {
  return strcmp(a.c_str(), b);
}

inline int compareString(const String& a, const String& b) {
  return a.compareTo(b);
}

inline int compareString(const String& a, const __FlashStringHelper* b) {
  return strcmp_P(a.c_str(), (const char*)b);
}

inline int compareString(const __FlashStringHelper* a, const char* b) {
  return -strcmp_P(b, (const char*) a);
}

int compareString(const __FlashStringHelper* a, const __FlashStringHelper* b);

inline int compareString(const __FlashStringHelper* a, const String& b) {
  return -strcmp_P(b.c_str(), (const char*)a);
}

int compareString(const FCString& a, const FCString& b);

// compareStringN()
// These methods are used to implement the TestRunner::exclude() and
// TestRunner::include() features.

/** Compare only the first n characters of 'b'. */
int compareStringN(const __FlashStringHelper* a, const __FlashStringHelper* b,
    uint16_t n);

/** Compare only the first n characters of 'b'. */
int compareStringN(const __FlashStringHelper* a, const char* b, uint16_t n);

/** Compare only the first n characters of 'b'. */
inline int compareStringN(const char* a, const char* b, uint16_t n) {
  return strncmp(a, b, n);
}

/** Compare only the first n characters of 'b'. */
inline int compareStringN(const char* a, const __FlashStringHelper* b,
    uint16_t n) {
  return strncmp_P(a, (const char*)b, n);
}

/** Compare only the first n characters of 'b'. */
int compareStringN(const FCString& a, const __FlashStringHelper* b, uint16_t n);

/** Compare only the first n characters of 'b'. */
int compareStringN(const FCString& a, const char* b, uint16_t n);

// compareEqual()

inline bool compareEqual(bool a, bool b) {
  return (a == b);
}

inline bool compareEqual(char a, char b) {
  return (a == b);
}

inline bool compareEqual(int a, int b) {
  return (a == b);
}

inline bool compareEqual(unsigned int a, unsigned int b) {
  return (a == b);
}

inline bool compareEqual(long a, long b) {
  return (a == b);
}

inline bool compareEqual(unsigned long a, unsigned long b) {
  return (a == b);
}

inline bool compareEqual(double a, double b) {
  return (a == b);
}

inline bool compareEqual(const char* a, const char* b) {
  return compareString(a, b) == 0;
}

inline bool compareEqual(const char* a, const String& b) {
  return compareString(a, b) == 0;
}

inline bool compareEqual(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) == 0;
}

inline bool compareEqual(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) == 0;
}

inline bool compareEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) == 0;
}

inline bool compareEqual(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) == 0;
}

inline bool compareEqual(const String& a, const char* b) {
  return compareString(a, b) == 0;
}

inline bool compareEqual(const String& a, const String& b) {
  return compareString(a, b) == 0;
}

inline bool compareEqual(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) == 0;
}

// compareLess()

inline bool compareLess(bool a, bool b) {
  return (a < b);
}

inline bool compareLess(char a, char b) {
  return (a < b);
}

inline bool compareLess(int a, int b) {
  return (a < b);
}

inline bool compareLess(unsigned int a, unsigned int b) {
  return (a < b);
}

inline bool compareLess(long a, long b) {
  return (a < b);
}

inline bool compareLess(unsigned long a, unsigned long b) {
  return (a < b);
}

inline bool compareLess(double a, double b) {
  return (a < b);
}

inline bool compareLess(const char* a, const char* b) {
  return compareString(a, b) < 0;
}

inline bool compareLess(const char* a, const String& b) {
  return compareString(a, b) < 0;
}

inline bool compareLess(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) < 0;
}

inline bool compareLess(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) < 0;
}

inline bool compareLess(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) < 0;
}

inline bool compareLess(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) < 0;
}

inline bool compareLess(const String& a, const char* b) {
  return compareString(a, b) < 0;
}

inline bool compareLess(const String& a, const String& b) {
  return compareString(a, b) < 0;
}

inline bool compareLess(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) < 0;
}

// compareMore()

inline bool compareMore(bool a, bool b) {
  return (a > b);
}

inline bool compareMore(char a, char b) {
  return (a > b);
}

inline bool compareMore(int a, int b) {
  return (a > b);
}

inline bool compareMore(unsigned int a, unsigned int b) {
  return (a > b);
}

inline bool compareMore(long a, long b) {
  return (a > b);
}

inline bool compareMore(unsigned long a, unsigned long b) {
  return (a > b);
}

inline bool compareMore(double a, double b) {
  return (a > b);
}

inline bool compareMore(const char* a, const char* b) {
  return compareString(a, b) > 0;
}

inline bool compareMore(const char* a, const String& b) {
  return compareString(a, b) > 0;
}

inline bool compareMore(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) > 0;
}

inline bool compareMore(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) > 0;
}

inline bool compareMore(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) > 0;
}

inline bool compareMore(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) > 0;
}

inline bool compareMore(const String& a, const char* b) {
  return compareString(a, b) > 0;
}

inline bool compareMore(const String& a, const String& b) {
  return compareString(a, b) > 0;
}

inline bool compareMore(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) > 0;
}

// compareLessOrEqual

inline bool compareLessOrEqual(bool a, bool b) {
  return (a <= b);
}

inline bool compareLessOrEqual(char a, char b) {
  return (a <= b);
}

inline bool compareLessOrEqual(int a, int b) {
  return (a <= b);
}

inline bool compareLessOrEqual(unsigned int a, unsigned int b) {
  return (a <= b);
}

inline bool compareLessOrEqual(long a, long b) {
  return (a <= b);
}

inline bool compareLessOrEqual(unsigned long a, unsigned long b) {
  return (a <= b);
}

inline bool compareLessOrEqual(double a, double b) {
  return (a <= b);
}

inline bool compareLessOrEqual(const char* a, const char* b) {
  return compareString(a, b) <= 0;
}

inline bool compareLessOrEqual(const char* a, const String& b) {
  return compareString(a, b) <= 0;
}

inline bool compareLessOrEqual(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) <= 0;
}

inline bool compareLessOrEqual(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) <= 0;
}

inline bool compareLessOrEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) <= 0;
}

inline bool compareLessOrEqual(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) <= 0;
}

inline bool compareLessOrEqual(const String& a, const char* b) {
  return compareString(a, b) <= 0;
}

inline bool compareLessOrEqual(const String& a, const String& b) {
  return compareString(a, b) <= 0;
}

inline bool compareLessOrEqual(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) <= 0;
}

// compareMoreOrEqual

inline bool compareMoreOrEqual(bool a, bool b) {
  return (a >= b);
}

inline bool compareMoreOrEqual(char a, char b) {
  return (a >= b);
}

inline bool compareMoreOrEqual(int a, int b) {
  return (a >= b);
}

inline bool compareMoreOrEqual(unsigned int a, unsigned int b) {
  return (a >= b);
}

inline bool compareMoreOrEqual(long a, long b) {
  return (a >= b);
}

inline bool compareMoreOrEqual(unsigned long a, unsigned long b) {
  return (a >= b);
}

inline bool compareMoreOrEqual(double a, double b) {
  return (a >= b);
}

inline bool compareMoreOrEqual(const char* a, const char* b) {
  return compareString(a, b) >= 0;
}

inline bool compareMoreOrEqual(const char* a, const String& b) {
  return compareString(a, b) >= 0;
}

inline bool compareMoreOrEqual(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) >= 0;
}

inline bool compareMoreOrEqual(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) >= 0;
}

inline bool compareMoreOrEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) >= 0;
}

inline bool compareMoreOrEqual(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) >= 0;
}

inline bool compareMoreOrEqual(const String& a, const char* b) {
  return compareString(a, b) >= 0;
}

inline bool compareMoreOrEqual(const String& a, const String& b) {
  return compareString(a, b) >= 0;
}

inline bool compareMoreOrEqual(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) >= 0;
}

// compareNotEqual

inline bool compareNotEqual(bool a, bool b) {
  return (a != b);
}

inline bool compareNotEqual(char a, char b) {
  return (a != b);
}

inline bool compareNotEqual(int a, int b) {
  return (a != b);
}

inline bool compareNotEqual(unsigned int a, unsigned int b) {
  return (a != b);
}

inline bool compareNotEqual(long a, long b) {
  return (a != b);
}

inline bool compareNotEqual(unsigned long a, unsigned long b) {
  return (a != b);
}

inline bool compareNotEqual(double a, double b) {
  return (a != b);
}

inline bool compareNotEqual(const char* a, const char* b) {
  return compareString(a, b) != 0;
}

inline bool compareNotEqual(const char* a, const String& b) {
  return compareString(a, b) != 0;
}

inline bool compareNotEqual(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) != 0;
}

inline bool compareNotEqual(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) != 0;
}

inline bool compareNotEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) != 0;
}

inline bool compareNotEqual(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) != 0;
}

inline bool compareNotEqual(const String& a, const char* b) {
  return compareString(a, b) != 0;
}

inline bool compareNotEqual(const String& a, const String& b) {
  return compareString(a, b) != 0;
}

inline bool compareNotEqual(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) != 0;
}

}

#endif
