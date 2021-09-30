#line 2 "CompareTest.ino"
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

#if defined(__AVR__)
  #include <AUnit.h>
#else
  #include <AUnitVerbose.h>
#endif
#include <aunit/string_util.h>

using namespace aunit;
using namespace aunit::internal;

// Defined in ESP8266 and ESP32, not defined in AVR or Teensy.
#ifndef FPSTR
  #define FPSTR(pstr) (reinterpret_cast<const __FlashStringHelper *>(pstr))
#endif

const char a[] = "a";
const char b[] = "b";
const char A[] = "A";
const char B[] = "B";

char aa[] = "a";
char bb[] = "b";
char AA[] = "A";
char BB[] = "B";

String s = "a";
String t = "b";
String S = "A";
String T = "B";

const char F_PROGMEM[] PROGMEM = "a";
const char G_PROGMEM[] PROGMEM = "b";
const __FlashStringHelper* f = FPSTR(F_PROGMEM);
const __FlashStringHelper* g = FPSTR(G_PROGMEM);

const char F_UPPER_PROGMEM[] PROGMEM = "A";
const char G_UPPER_PROGMEM[] PROGMEM = "B";
const __FlashStringHelper* F = FPSTR(F_UPPER_PROGMEM);
const __FlashStringHelper* G = FPSTR(G_UPPER_PROGMEM);

const char FF_PROGMEM[] PROGMEM = "abcde";
const char GG_PROGMEM[] PROGMEM = "abcdef";
const char HH_PROGMEM[] PROGMEM = "abcdeg";
const __FlashStringHelper* ff = FPSTR(FF_PROGMEM);
const __FlashStringHelper* gg = FPSTR(GG_PROGMEM);
const __FlashStringHelper* hh = FPSTR(HH_PROGMEM);

const char FF_UPPER_PROGMEM[] PROGMEM = "ABCDE";
const char GG_UPPER_PROGMEM[] PROGMEM = "ABCDEF";
const char HH_UPPER_PROGMEM[] PROGMEM = "ABCDEG";
const __FlashStringHelper* FF = FPSTR(FF_UPPER_PROGMEM);
const __FlashStringHelper* GG = FPSTR(GG_UPPER_PROGMEM);
const __FlashStringHelper* HH = FPSTR(HH_UPPER_PROGMEM);

//----------------------------------------------------------------------------
// We use if-statements instead of assertXxx() because compareXxx() are
// lower-level functions that the assertXxx() methods depend on.
//----------------------------------------------------------------------------

test(compareString) {
  if (!(compareString(a, a) == 0)) { failTestNow(); }
  if (!(compareString(a, f) == 0)) { failTestNow(); }
  if (!(compareString(a, s) == 0)) { failTestNow(); }

  if (!(compareString(f, a) == 0)) { failTestNow(); }
  if (!(compareString(f, f) == 0)) { failTestNow(); }
  if (!(compareString(f, s) == 0)) { failTestNow(); }

  if (!(compareString(s, a) == 0)) { failTestNow(); }
  if (!(compareString(s, f) == 0)) { failTestNow(); }
  if (!(compareString(s, s) == 0)) { failTestNow(); }

  if (!(compareString(a, b) < 0)) { failTestNow(); }
  if (!(compareString(a, g) < 0)) { failTestNow(); }
  if (!(compareString(a, t) < 0)) { failTestNow(); }

  if (!(compareString(f, b) < 0)) { failTestNow(); }
  if (!(compareString(f, g) < 0)) { failTestNow(); }
  if (!(compareString(f, t) < 0)) { failTestNow(); }

  if (!(compareString(s, b) < 0)) { failTestNow(); }
  if (!(compareString(s, g) < 0)) { failTestNow(); }
  if (!(compareString(s, t) < 0)) { failTestNow(); }
}

test(compareString_WithNulls) {
  const char* const NULL_CSTRING = (const char*) nullptr;
  const __FlashStringHelper* const NULL_FSTRING =
      (const __FlashStringHelper*) nullptr;

  if (!(compareString(NULL_CSTRING, NULL_CSTRING) == 0)) { failTestNow(); }
  if (!(compareString(a, NULL_CSTRING) > 0)) { failTestNow(); }
  if (!(compareString(NULL_CSTRING, a) < 0)) { failTestNow(); }

  if (!(compareString(NULL_FSTRING, NULL_FSTRING) == 0)) { failTestNow(); }
  if (!(compareString(f, NULL_FSTRING) > 0)) { failTestNow(); }
  if (!(compareString(NULL_FSTRING, f) < 0)) { failTestNow(); }

  if (!(compareString(NULL_FSTRING, NULL_FSTRING) == 0)) { failTestNow(); }
  if (!(compareString(s, NULL_FSTRING) > 0)) { failTestNow(); }
  if (!(compareString(NULL_FSTRING, s) < 0)) { failTestNow(); }

  if (!(compareString(NULL_CSTRING, NULL_FSTRING) == 0)) { failTestNow(); }
  if (!(compareString(NULL_FSTRING, NULL_CSTRING) == 0)) { failTestNow(); }
}

// Same as test(compareString) but case-insensitive.
test(compareStringCase) {
  if (!(compareStringCase(a, A) == 0)) { failTestNow(); }
  if (!(compareStringCase(A, f) == 0)) { failTestNow(); }
  if (!(compareStringCase(a, S) == 0)) { failTestNow(); }

  if (!(compareStringCase(f, A) == 0)) { failTestNow(); }
  if (!(compareStringCase(F, f) == 0)) { failTestNow(); }
  if (!(compareStringCase(f, S) == 0)) { failTestNow(); }

  if (!(compareStringCase(s, A) == 0)) { failTestNow(); }
  if (!(compareStringCase(S, f) == 0)) { failTestNow(); }
  if (!(compareStringCase(s, S) == 0)) { failTestNow(); }

  if (!(compareStringCase(a, B) < 0)) { failTestNow(); }
  if (!(compareStringCase(A, g) < 0)) { failTestNow(); }
  if (!(compareStringCase(a, T) < 0)) { failTestNow(); }

  if (!(compareStringCase(f, B) < 0)) { failTestNow(); }
  if (!(compareStringCase(F, g) < 0)) { failTestNow(); }
  if (!(compareStringCase(f, T) < 0)) { failTestNow(); }

  if (!(compareStringCase(s, B) < 0)) { failTestNow(); }
  if (!(compareStringCase(S, g) < 0)) { failTestNow(); }
  if (!(compareStringCase(s, T) < 0)) { failTestNow(); }
}

test(compareStringCase_WithNulls) {
  const char* const NULL_CSTRING = (const char*) nullptr;
  const __FlashStringHelper* const NULL_FSTRING =
      (const __FlashStringHelper*) nullptr;

  if (!(compareStringCase(NULL_CSTRING, NULL_CSTRING) == 0)) { failTestNow(); }
  if (!(compareStringCase(a, NULL_CSTRING) > 0)) { failTestNow(); }
  if (!(compareStringCase(NULL_CSTRING, a) < 0)) { failTestNow(); }

  if (!(compareStringCase(NULL_FSTRING, NULL_FSTRING) == 0)) { failTestNow(); }
  if (!(compareStringCase(f, NULL_FSTRING) > 0)) { failTestNow(); }
  if (!(compareStringCase(NULL_FSTRING, f) < 0)) { failTestNow(); }

  if (!(compareStringCase(NULL_FSTRING, NULL_FSTRING) == 0)) { failTestNow(); }
  if (!(compareStringCase(s, NULL_FSTRING) > 0)) { failTestNow(); }
  if (!(compareStringCase(NULL_FSTRING, s) < 0)) { failTestNow(); }

  if (!(compareStringCase(NULL_CSTRING, NULL_FSTRING) == 0)) { failTestNow(); }
  if (!(compareStringCase(NULL_FSTRING, NULL_CSTRING) == 0)) { failTestNow(); }
}

// We use if-statements instead of assertXxx() because compareXxx() is used by
// the assertXxx() methods. Strictly speaking, it's not necessary because one
// is a string compare and the other is an integer compare, but this feels
// conceptually cleaner.
test(compareStringN) {
  if (!(compareStringN(ff, "abcde", 5) == 0)) { failTestNow(); }
  if (!(compareStringN("abcde", ff, 5) == 0)) { failTestNow(); }

  if (!(compareStringN(ff, "abcd", 5) > 0)) { failTestNow(); }
  if (!(compareStringN("abcd", ff, 5) < 0)) { failTestNow(); }

  if (!(compareStringN(ff, "abcd", 4) == 0)) { failTestNow(); }
  if (!(compareStringN("abcd", ff, 4) == 0)) { failTestNow(); }

  if (!(compareStringN(ff, "", 1) > 0)) { failTestNow(); }
  if (!(compareStringN("", ff, 1) < 0)) { failTestNow(); }

  if (!(compareStringN(ff, "", 0) == 0)) { failTestNow(); }
  if (!(compareStringN("", ff, 0) == 0)) { failTestNow(); }

  if (!(compareStringN(gg, ff, 5) == 0)) { failTestNow(); }

  if (!(compareStringN(gg, ff, 6) > 0)) { failTestNow(); }
}

test(compareStringN_WithNulls) {
  const char* const NULL_CSTRING = (const char*) nullptr;
  const __FlashStringHelper* const NULL_FSTRING =
      (const __FlashStringHelper*) nullptr;

  if (!(compareStringN(NULL_CSTRING, NULL_CSTRING, 4) == 0)) { failTestNow(); }
  if (!(compareStringN(a, NULL_CSTRING, 4) > 0)) { failTestNow(); }
  if (!(compareStringN(NULL_CSTRING, a, 4) < 0)) { failTestNow(); }

  if (!(compareStringN(NULL_FSTRING, NULL_FSTRING, 4) == 0)) { failTestNow(); }
  if (!(compareStringN(f, NULL_FSTRING, 4) > 0)) { failTestNow(); }
  if (!(compareStringN(NULL_FSTRING, f, 4) < 0)) { failTestNow(); }

  if (!(compareString(NULL_CSTRING, NULL_FSTRING) == 0)) { failTestNow(); }
  if (!(compareString(NULL_FSTRING, NULL_CSTRING) == 0)) { failTestNow(); }
}

test(compareSubstring) {
  if (!compareSubstring("abc", "")) { failTestNow(); }
  if (!compareSubstring("abc", "bc")) { failTestNow(); }
  if (compareSubstring("abc", "dc")) { failTestNow(); }

  if (!compareSubstring("abc", String(""))) { failTestNow(); }
  if (!compareSubstring("abc", String("bc"))) { failTestNow(); }
  if (compareSubstring("abc", String("dc"))) { failTestNow(); }

  if (!compareSubstring("abc", F(""))) { failTestNow(); }
  if (!compareSubstring("abc", F("bc"))) { failTestNow(); }
  if (compareSubstring("abc", F("dc"))) { failTestNow(); }

  if (!compareSubstring(String("abc"), "")) { failTestNow(); }
  if (!compareSubstring(String("abc"), "bc")) { failTestNow(); }
  if (compareSubstring(String("abc"), "dc")) { failTestNow(); }

  if (!compareSubstring(String("abc"), String(""))) { failTestNow(); }
  if (!compareSubstring(String("abc"), String("bc"))) { failTestNow(); }
  if (compareSubstring(String("abc"), String("dc"))) { failTestNow(); }

  if (!compareSubstring(String("abc"), F(""))) { failTestNow(); }
  if (!compareSubstring(String("abc"), F("bc"))) { failTestNow(); }
  if (compareSubstring(String("abc"), F("dc"))) { failTestNow(); }

  if (!compareSubstring(F("abc"), "")) { failTestNow(); }
  if (!compareSubstring(F("abc"), "bc")) { failTestNow(); }
  if (compareSubstring(F("abc"), "dc")) { failTestNow(); }

  if (!compareSubstring(F("abc"), String(""))) { failTestNow(); }
  if (!compareSubstring(F("abc"), String("bc"))) { failTestNow(); }
  if (compareSubstring(F("abc"), String("dc"))) { failTestNow(); }

  if (!compareSubstring(F("abc"), F(""))) { failTestNow(); }
  if (!compareSubstring(F("abc"), F("bc"))) { failTestNow(); }
  if (compareSubstring(F("abc"), F("dc"))) { failTestNow(); }
}

//----------------------------------------------------------------------------

void setup() {
#ifndef EPOXY_DUINO
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while (! SERIAL_PORT_MONITOR); // Wait until Serial is ready - Leonardo/Micro
}

void loop() {
  TestRunner::run();
}
