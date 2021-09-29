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

/*
Design Notes:
============

Template Specialization:
-----------------------
One way to implement the compareEqual() for these types is to use template
specialization. The problem with Template specialization is that templates
use strict type matching, and does not perform the normal implicit type
conversion, including const-casting. Therefore, all of the various c-style
string types, for example:

 - char*
 - const char*
 - char[1]
 - char[N]
 - const char[1]
 - const char[N]

are considered to be different types under the C++ templating system. This
causes a combinatorial explosion of template specialization which produces
code that is difficult to understand, test and maintain.
An example can be seen in the Compare.h file of the ArduinoUnit project:
https://github.com/mmurdoch/arduinounit/blob/master/src/ArduinoUnitUtility/Compare.h

Function Overloading:
---------------------
In this project, I used function overloading instead of template
specialization. Function overloading handles c-style strings (i.e. character
arrays) naturally, in the way most users expect. For example, (char*) is
automarically cast to (const char*), and (char[N]) is autonmatically
cast to (const char*).

For the primitive value types (e.g. (char), (int), (unsigned char), etc.) I
attempted to use a generic templatized version, using sonmething like:

   template<typename T>
   compareEqual(const T& a, const T& b) { ... }

However, this template introduced this method:

   compareEqual(char* const& a, char* const& b);

that seemed to take precedence over the explicitly defined overload:

   compareEqual(const char* a, const char*b);

When the compareEqual() method is called with a (char*) or a (char[N]),
like this:

   char a[3] = {...};
   char b[4] = {...};
   compareEqual(a, b);

this calls compareEqual(char* const&, const* const&), which is the wrong
version for a c-style string. The only way I could get this to work was to
avoid templates completely and manually define all the function overloads
even for primitive integer types.

Implicit Conversions:
---------------------
For basic primitive types, I depend on some casts to avoid having to define
some functions. I assume that signed and unsigned integers smaller or equal
to (int) will be converted to an (int) to match compareEqual(int, int).

I provided an explicit compareEqual(char, char) overload because in C++, a
(char) type is distinct from (signed char) and (unsigned char).

Technically, there should be a (long long) version and an (unsigned long
long) version of compareEqual(). However, it turns out that the Arduino
Print::print() method does not have an overload for these types, so it would
not do us much good to provide an assertEqual() or compareEqual() for the
(long long) and (unsigned long long) types.

Custom Assert and Compare Functions:
------------------------------------
Another advantage of using function overloading instead of template
specialization is that the user is able to add additional function overloads
into the 'aunit' namespace. This should allow the user to define the various
comporeXxx() and assertXxx() functions for a custom class. I have not
tested this though.

Comparing Flash Strings:
------------------------
Flash memory must be read using 4-byte alignment on the ESP8266. AVR doesn't
care. Teensy-ARM fakes the flash memory API but really just uses the normal
static RAM. The following code for comparing two (__FlashStringHelper*)
against each other will work for all 3 environments.

Inlining:
--------
Even though most of these functions are one-liners, there is no advantage to
inlining them because they are almost always used through a function pointer.
*/

#include <stdint.h>
#include <string.h>
#include <math.h> // fabs()
#include <WString.h>
#include "Flash.h"
#include "Compare.h"

namespace aunit {
namespace internal {

//---------------------------------------------------------------------------
// compareString()
//---------------------------------------------------------------------------

int compareString(const char* a, const char* b) {
  if (a == b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  return strcmp(a, b);
}

int compareString(const char* a, const String& b) {
  if (a == nullptr) { return -1; }
  return strcmp(a, b.c_str());
}

int compareString(const char* a, const __FlashStringHelper* b) {
  if (a == (const char*) b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  return strcmp_P(a, (const char*) b);
}

int compareString(const String& a, const char* b) {
  return -compareString(b, a);
}

int compareString(const String& a, const String& b) {
  return strcmp(a.c_str(), b.c_str());
}

int compareString(const String& a, const __FlashStringHelper* b) {
  if (b == nullptr) { return 1; }
  return strcmp_P(a.c_str(), (const char*) b);
}

int compareString(const __FlashStringHelper* a, const char* b) {
  return -compareString(b, a);
}

int compareString(const __FlashStringHelper* a, const String& b) {
  return -compareString(b, a);
}

// On ESP8266, pgm_read_byte() already takes care of 4-byte alignment, and
// memcpy_P(s, p, 4) makes 4 calls to pgm_read_byte() anyway, so don't bother
// optimizing for 4-byte alignment here.
int compareString(const __FlashStringHelper* a, const __FlashStringHelper* b) {
  if (a == b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  const char* aa = reinterpret_cast<const char*>(a);
  const char* bb = reinterpret_cast<const char*>(b);

  while (true) {
    uint8_t ca = pgm_read_byte(aa);
    uint8_t cb = pgm_read_byte(bb);
    if (ca != cb) return (int) ca - (int) cb;
    if (ca == '\0') return 0;
    aa++;
    bb++;
  }
}

//---------------------------------------------------------------------------
// compareStringCase()
//---------------------------------------------------------------------------

int compareStringCase(const char* a, const char* b) {
  if (a == b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  return strcasecmp(a, b);
}

int compareStringCase(const char* a, const String& b) {
  if (a == nullptr) { return -1; }
  return strcasecmp(a, b.c_str());
}

int compareStringCase(const char* a, const __FlashStringHelper* b) {
  if (a == (const char*) b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  return strcasecmp_P(a, (const char*) b);
}

int compareStringCase(const String& a, const char* b) {
  return -compareStringCase(b, a);
}

int compareStringCase(const String& a, const String& b) {
  return strcasecmp(a.c_str(), b.c_str());
}

int compareStringCase(const String& a, const __FlashStringHelper* b) {
  if (b == nullptr) { return 1; }
  return strcasecmp_P(a.c_str(), (const char*) b);
}

int compareStringCase(const __FlashStringHelper* a, const char* b) {
  return -compareStringCase(b, a);
}

int compareStringCase(const __FlashStringHelper* a, const String& b) {
  return -compareStringCase(b, a);
}

// On ESP8266, pgm_read_byte() already takes care of 4-byte alignment, and
// memcpy_P(s, p, 4) makes 4 calls to pgm_read_byte() anyway, so don't bother
// optimizing for 4-byte alignment here.
int compareStringCase(const __FlashStringHelper* a,
    const __FlashStringHelper* b) {
  if (a == b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  const char* aa = reinterpret_cast<const char*>(a);
  const char* bb = reinterpret_cast<const char*>(b);

  while (true) {
    uint8_t ca = pgm_read_byte(aa);
    uint8_t cb = pgm_read_byte(bb);
    uint8_t la = tolower(ca);
    uint8_t lb = tolower(cb);
    if (la != lb) return (int) la - (int) lb;
    if (ca == '\0') return 0;
    aa++;
    bb++;
  }
}

//---------------------------------------------------------------------------
// compareStringN
//---------------------------------------------------------------------------

// We need compareStringN() to support only (const char*) and (const
// __FlashStringHelper*). And it turns out that compareStringN(a, b, N) ==
// -compareString(b, a, N).

int compareStringN(const char* a, const char* b, size_t n) {
  if (a == b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  return strncmp(a, b, n);
}

int compareStringN(const char* a, const __FlashStringHelper* b, size_t n) {
  if (a == (const char*) b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  return strncmp_P(a, (const char*) b, n);
}

int compareStringN(const __FlashStringHelper* a, const char* b, size_t n) {
  return -compareStringN(b, a, n);
}

// On ESP8266, pgm_read_byte() already takes care of 4-byte alignment, and
// memcpy_P(s, p, 4) makes 4 calls to pgm_read_byte() anyway, so don't bother
// optimizing for 4-byte alignment here.
int compareStringN(const __FlashStringHelper* a, const __FlashStringHelper* b,
    size_t n) {
  if (a == b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }
  const char* aa = reinterpret_cast<const char*>(a);
  const char* bb = reinterpret_cast<const char*>(b);

  while (n > 0) {
    uint8_t ca = pgm_read_byte(aa);
    uint8_t cb = pgm_read_byte(bb);
    if (ca != cb) return (int) ca - (int) cb;
    if (ca == '\0') return 0;
    aa++;
    bb++;
    n--;
  }
  return 0;
}

//---------------------------------------------------------------------------
// compareSubstring(haystack, needle)
//---------------------------------------------------------------------------

bool compareSubstring(const char* haystack, const char* needle) {
  return strstr(haystack, needle) != nullptr;
}

bool compareSubstring(const char* haystack, const String& needle) {
  return strstr(haystack, needle.c_str()) != nullptr;
}

bool compareSubstring(const char* haystack, const __FlashStringHelper* needle) {
  return strstr_P(haystack, (const char*) needle) != nullptr;
}

bool compareSubstring(const String& haystack, const char* needle) {
  return strstr(haystack.c_str(), needle) != nullptr;
}

bool compareSubstring(const String& haystack, const String& needle) {
  return strstr(haystack.c_str(), needle.c_str()) != nullptr;
}

bool compareSubstring(const String& haystack, const __FlashStringHelper* needle) {
  return strstr(haystack.c_str(), (const char*) needle) != nullptr;
}

// An inefficient O(M*N) implementation of strstr() for PROGMEM strings.
// The KMP algorithm
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
// is faster, but this brute force implementation is probably good enough.
bool compareSubstring(
    const __FlashStringHelper* haystack,
    const char* needle) {

  const char* hay = (const char*) haystack;
  for (uint8_t ch = pgm_read_byte(hay); ch != '\0'; hay++) {
    const char* subhay = hay;
    const char* subneedle = needle;
    while (true) {
      uint8_t chay = pgm_read_byte(subhay);
      uint8_t cneedle = *subneedle;
      if (cneedle == '\0') return true;
      if (chay == '\0') return false;
      if (chay != cneedle) break;
      subhay++;
      subneedle++;
    }
  }

  return false;
}

bool compareSubstring(
    const __FlashStringHelper* haystack,
    const String& needle) {
  return compareSubstring(haystack, needle.c_str());
}

// An inefficient O(M*N) implementation of strstr() for PROGMEM strings.
// The KMP algorithm
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
// is faster, but this brute force implementation is probably good enough.
bool compareSubstring(
    const __FlashStringHelper* haystack,
    const __FlashStringHelper* needle) {

  const char* hay = reinterpret_cast<const char*>(haystack);
  for (uint8_t ch = pgm_read_byte(hay); ch != '\0'; hay++) {
    const char* subhay = hay;
    const char* subneedle = (const char*) needle;
    while (true) {
      uint8_t chay = pgm_read_byte(subhay);
      uint8_t cneedle = pgm_read_byte(subneedle);
      if (cneedle == '\0') return true;
      if (chay == '\0') return false;
      if (chay != cneedle) break;
      subhay++;
      subneedle++;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
// compareEqual()
//---------------------------------------------------------------------------

bool compareEqual(bool a, bool b) {
  return (a == b);
}

bool compareEqual(char a, char b) {
  return (a == b);
}

bool compareEqual(int a, int b) {
  return (a == b);
}

bool compareEqual(unsigned int a, unsigned int b) {
  return (a == b);
}

bool compareEqual(long a, long b) {
  return (a == b);
}

bool compareEqual(unsigned long a, unsigned long b) {
  return (a == b);
}

bool compareEqual(long long a, long long b) {
  return (a == b);
}

bool compareEqual(unsigned long long a, unsigned long long b) {
  return (a == b);
}

bool compareEqual(double a, double b) {
  return (a == b);
}

bool compareEqual(const void* a, const void* b) {
  return (a == b);
}

bool compareEqual(const char* a, const char* b) {
  return compareString(a, b) == 0;
}

bool compareEqual(const char* a, const String& b) {
  return compareString(a, b) == 0;
}

bool compareEqual(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) == 0;
}

bool compareEqual(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) == 0;
}

bool compareEqual(const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) == 0;
}

bool compareEqual(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) == 0;
}

bool compareEqual(const String& a, const char* b) {
  return compareString(a, b) == 0;
}

bool compareEqual(const String& a, const String& b) {
  return compareString(a, b) == 0;
}

bool compareEqual(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) == 0;
}

//---------------------------------------------------------------------------
// compareLess()
//---------------------------------------------------------------------------

bool compareLess(bool a, bool b) {
  return (a < b);
}

bool compareLess(char a, char b) {
  return (a < b);
}

bool compareLess(int a, int b) {
  return (a < b);
}

bool compareLess(unsigned int a, unsigned int b) {
  return (a < b);
}

bool compareLess(long a, long b) {
  return (a < b);
}

bool compareLess(unsigned long a, unsigned long b) {
  return (a < b);
}

bool compareLess(long long a, long long b) {
  return (a < b);
}

bool compareLess(unsigned long long a, unsigned long long b) {
  return (a < b);
}

bool compareLess(double a, double b) {
  return (a < b);
}

bool compareLess(const char* a, const char* b) {
  return compareString(a, b) < 0;
}

bool compareLess(const char* a, const String& b) {
  return compareString(a, b) < 0;
}

bool compareLess(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) < 0;
}

bool compareLess(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) < 0;
}

bool compareLess(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) < 0;
}

bool compareLess(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) < 0;
}

bool compareLess(const String& a, const char* b) {
  return compareString(a, b) < 0;
}

bool compareLess(const String& a, const String& b) {
  return compareString(a, b) < 0;
}

bool compareLess(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) < 0;
}

//---------------------------------------------------------------------------
// compareMore()
//---------------------------------------------------------------------------

bool compareMore(bool a, bool b) {
  return (a > b);
}

bool compareMore(char a, char b) {
  return (a > b);
}

bool compareMore(int a, int b) {
  return (a > b);
}

bool compareMore(unsigned int a, unsigned int b) {
  return (a > b);
}

bool compareMore(long a, long b) {
  return (a > b);
}

bool compareMore(unsigned long a, unsigned long b) {
  return (a > b);
}

bool compareMore(long long a, long long b) {
  return (a > b);
}

bool compareMore(unsigned long long a, unsigned long long b) {
  return (a > b);
}

bool compareMore(double a, double b) {
  return (a > b);
}

bool compareMore(const char* a, const char* b) {
  return compareString(a, b) > 0;
}

bool compareMore(const char* a, const String& b) {
  return compareString(a, b) > 0;
}

bool compareMore(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) > 0;
}

bool compareMore(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) > 0;
}

bool compareMore(const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) > 0;
}

bool compareMore(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) > 0;
}

bool compareMore(const String& a, const char* b) {
  return compareString(a, b) > 0;
}

bool compareMore(const String& a, const String& b) {
  return compareString(a, b) > 0;
}

bool compareMore(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) > 0;
}

//---------------------------------------------------------------------------
// compareLessOrEqual
//---------------------------------------------------------------------------

bool compareLessOrEqual(bool a, bool b) {
  return (a <= b);
}

bool compareLessOrEqual(char a, char b) {
  return (a <= b);
}

bool compareLessOrEqual(int a, int b) {
  return (a <= b);
}

bool compareLessOrEqual(unsigned int a, unsigned int b) {
  return (a <= b);
}

bool compareLessOrEqual(long a, long b) {
  return (a <= b);
}

bool compareLessOrEqual(unsigned long a, unsigned long b) {
  return (a <= b);
}

bool compareLessOrEqual(long long a, long long b) {
  return (a <= b);
}

bool compareLessOrEqual(unsigned long long a, unsigned long long b) {
  return (a <= b);
}

bool compareLessOrEqual(double a, double b) {
  return (a <= b);
}

bool compareLessOrEqual(const char* a, const char* b) {
  return compareString(a, b) <= 0;
}

bool compareLessOrEqual(const char* a, const String& b) {
  return compareString(a, b) <= 0;
}

bool compareLessOrEqual(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) <= 0;
}

bool compareLessOrEqual(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) <= 0;
}

bool compareLessOrEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) <= 0;
}

bool compareLessOrEqual(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) <= 0;
}

bool compareLessOrEqual(const String& a, const char* b) {
  return compareString(a, b) <= 0;
}

bool compareLessOrEqual(const String& a, const String& b) {
  return compareString(a, b) <= 0;
}

bool compareLessOrEqual(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) <= 0;
}

//---------------------------------------------------------------------------
// compareMoreOrEqual
//---------------------------------------------------------------------------

bool compareMoreOrEqual(bool a, bool b) {
  return (a >= b);
}

bool compareMoreOrEqual(char a, char b) {
  return (a >= b);
}

bool compareMoreOrEqual(int a, int b) {
  return (a >= b);
}

bool compareMoreOrEqual(unsigned int a, unsigned int b) {
  return (a >= b);
}

bool compareMoreOrEqual(long a, long b) {
  return (a >= b);
}

bool compareMoreOrEqual(unsigned long a, unsigned long b) {
  return (a >= b);
}

bool compareMoreOrEqual(long long a, long long b) {
  return (a >= b);
}

bool compareMoreOrEqual(unsigned long long a, unsigned long long b) {
  return (a >= b);
}

bool compareMoreOrEqual(double a, double b) {
  return (a >= b);
}

bool compareMoreOrEqual(const char* a, const char* b) {
  return compareString(a, b) >= 0;
}

bool compareMoreOrEqual(const char* a, const String& b) {
  return compareString(a, b) >= 0;
}

bool compareMoreOrEqual(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) >= 0;
}

bool compareMoreOrEqual(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) >= 0;
}

bool compareMoreOrEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) >= 0;
}

bool compareMoreOrEqual(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) >= 0;
}

bool compareMoreOrEqual(const String& a, const char* b) {
  return compareString(a, b) >= 0;
}

bool compareMoreOrEqual(const String& a, const String& b) {
  return compareString(a, b) >= 0;
}

bool compareMoreOrEqual(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) >= 0;
}

//---------------------------------------------------------------------------
// compareNotEqual
//---------------------------------------------------------------------------

bool compareNotEqual(bool a, bool b) {
  return (a != b);
}

bool compareNotEqual(char a, char b) {
  return (a != b);
}

bool compareNotEqual(int a, int b) {
  return (a != b);
}

bool compareNotEqual(unsigned int a, unsigned int b) {
  return (a != b);
}

bool compareNotEqual(long a, long b) {
  return (a != b);
}

bool compareNotEqual(unsigned long a, unsigned long b) {
  return (a != b);
}

bool compareNotEqual(long long a, long long b) {
  return (a != b);
}

bool compareNotEqual(unsigned long long a, unsigned long long b) {
  return (a != b);
}

bool compareNotEqual(double a, double b) {
  return (a != b);
}

bool compareNotEqual(const void* a, const void* b) {
  return (a != b);
}

bool compareNotEqual(const char* a, const char* b) {
  return compareString(a, b) != 0;
}

bool compareNotEqual(const char* a, const String& b) {
  return compareString(a, b) != 0;
}

bool compareNotEqual(const char* a, const __FlashStringHelper* b) {
  return compareString(a, b) != 0;
}

bool compareNotEqual(const __FlashStringHelper* a, const char* b) {
  return compareString(a, b) != 0;
}

bool compareNotEqual(
    const __FlashStringHelper* a, const __FlashStringHelper* b) {
  return compareString(a, b) != 0;
}

bool compareNotEqual(const __FlashStringHelper* a, const String& b) {
  return compareString(a, b) != 0;
}

bool compareNotEqual(const String& a, const char* b) {
  return compareString(a, b) != 0;
}

bool compareNotEqual(const String& a, const String& b) {
  return compareString(a, b) != 0;
}

bool compareNotEqual(const String& a, const __FlashStringHelper* b) {
  return compareString(a, b) != 0;
}

//---------------------------------------------------------------------------
// compareStringCaseEqual()
//---------------------------------------------------------------------------

bool compareStringCaseEqual(const char* a, const char* b) {
  return compareStringCase(a, b) == 0;
}

bool compareStringCaseEqual(const char* a, const String& b) {
  return compareStringCase(a, b) == 0;
}

bool compareStringCaseEqual(const char* a, const __FlashStringHelper* b) {
  return compareStringCase(a, b) == 0;
}

bool compareStringCaseEqual(const __FlashStringHelper* a, const char* b) {
  return compareStringCase(a, b) == 0;
}

bool compareStringCaseEqual(const __FlashStringHelper* a,
    const __FlashStringHelper* b) {
  return compareStringCase(a, b) == 0;
}

bool compareStringCaseEqual(const __FlashStringHelper* a, const String& b) {
  return compareStringCase(a, b) == 0;
}

bool compareStringCaseEqual(const String& a, const char* b) {
  return compareStringCase(a, b) == 0;
}

bool compareStringCaseEqual(const String& a, const String& b) {
  return compareStringCase(a, b) == 0;
}

bool compareStringCaseEqual(const String& a, const __FlashStringHelper* b) {
  return compareStringCase(a, b) == 0;
}

//---------------------------------------------------------------------------
// compareStringCaseNotEqual()
//---------------------------------------------------------------------------

bool compareStringCaseNotEqual(const char* a, const char* b) {
  return compareStringCase(a, b) != 0;
}

bool compareStringCaseNotEqual(const char* a, const String& b) {
  return compareStringCase(a, b) != 0;
}

bool compareStringCaseNotEqual(const char* a, const __FlashStringHelper* b) {
  return compareStringCase(a, b) != 0;
}

bool compareStringCaseNotEqual(const __FlashStringHelper* a, const char* b) {
  return compareStringCase(a, b) != 0;
}

bool compareStringCaseNotEqual(const __FlashStringHelper* a,
    const __FlashStringHelper* b) {
  return compareStringCase(a, b) != 0;
}

bool compareStringCaseNotEqual(const __FlashStringHelper* a, const String& b) {
  return compareStringCase(a, b) != 0;
}

bool compareStringCaseNotEqual(const String& a, const char* b) {
  return compareStringCase(a, b) != 0;
}

bool compareStringCaseNotEqual(const String& a, const String& b) {
  return compareStringCase(a, b) != 0;
}

bool compareStringCaseNotEqual(const String& a, const __FlashStringHelper* b) {
  return compareStringCase(a, b) != 0;
}

//---------------------------------------------------------------------------
// compareNear()
//---------------------------------------------------------------------------

bool compareNear(int a, int b, int error) {
  return abs(a - b) <= error;
}

bool compareNear(unsigned int a, unsigned int b, unsigned int error) {
  return (unsigned int) abs((int)(a - b)) <= error;
}

bool compareNear(long a, long b, long error) {
  return abs(a - b) <= error;
}

bool compareNear(unsigned long a, unsigned long b, unsigned long error) {
  return (unsigned long) abs((long)(a - b)) <= error;
}

bool compareNear(double a, double b, double error) {
  return fabs(a - b) <= error;
}

bool compareNotNear(int a, int b, int error) {
  return !compareNear(a, b, error);
}

bool compareNotNear(unsigned int a, unsigned int b, unsigned int error) {
  return !compareNear(a, b, error);
}

bool compareNotNear(long a, long b, long error) {
  return !compareNear(a, b, error);
}

bool compareNotNear(unsigned long a, unsigned long b, unsigned long error) {
  return !compareNear(a, b, error);
}

bool compareNotNear(double a, double b, double error) {
  return !compareNear(a, b, error);
}

}
}
