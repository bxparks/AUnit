#line 2 "AUnitTest.ino"
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

// Core AUnit features are tested here. Meta assertion tests have been moved to
// MetaAssertionTest because they make this sketch no longer fit in a Arduino
// Micro (ATmega32U4, max flash: 28672).

// If ArduinoUnit is used, this unit test no longer fits in a 32kB
// Arduino UNO or Nano.
#define USE_AUNIT 1

#if USE_AUNIT == 1

  #if defined(__AVR__)
    #include <AUnit.h>
  #else
    #include <AUnitVerbose.h>
  #endif
  #include <aunit/string_util.h>
  using namespace aunit;
  using namespace aunit::internal;

#else

  #include <ArduinoUnit.h>

#endif

// Defined in ESP8266 and ESP32, not defined in AVR or Teensy.
#ifndef FPSTR
  #define FPSTR(pstr) (reinterpret_cast<const __FlashStringHelper *>(pstr))
#endif

signed char sc = 4;
signed char sd = 5;
unsigned char usc = 4;
unsigned char usd = 5;
char c = 'a';
char d = 'b';

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

// ------------------------------------------------------
// Test the test() macro.
// ------------------------------------------------------

test(type_mismatch) {
  unsigned short ushortValue = 5;
  unsigned int uintValue = 5;
  unsigned long ulongValue = 5;

  // Works because both (signed char) and (unsigned char) are promoted to (int)
  assertEqual(sc, usc);

  // compiler error on AVR because both (short) and (int) are 16-bits
  //assertEqual(5, ushortValue);

  // compiler error on all platforms
  //assertEqual(5U, ushortValue);

  // works on all platforms
  assertEqual((unsigned short) 5, ushortValue);

  // compiler error on all platforms
  //assertEqual(5, uintValue);

  // works on all platforms
  assertEqual(5U, uintValue);

  // compiler error on all platforms
  //assertEqual(5, ulongValue);

  // compiler error on all platforms
  //assertEqual(5U, ulongValue);

  // works on all platforms
  assertEqual(5UL, ulongValue);
}

// ------------------------------------------------------
// Test FCString
// ------------------------------------------------------

test(FCStringTest, compareTo) {
  FCString n;
  FCString a("a");
  FCString b("b");
  FCString fa(F("a"));
  FCString fb(F("b"));

  assertEqual(n.compareTo(n), 0);
  assertLess(n.compareTo(a), 0);
  assertLess(n.compareTo(fa), 0);

  assertEqual(a.compareTo(fa), 0);
  assertEqual(fb.compareTo(b), 0);

  assertLess(a.compareTo(b), 0);
  assertLess(a.compareTo(fb), 0);
  assertMore(fb.compareTo(a), 0);
  assertMore(fb.compareTo(fa), 0);
}

test(FCStringTest, compareToN) {
  FCString a("a");
  FCString fa(F("a"));

  assertEqual(a.compareToN("aa", 1), 0);
  assertLess(a.compareToN("aa", 2), 0);
  assertEqual(a.compareToN(F("aa"), 1), 0);
  assertLess(a.compareToN(F("aa"), 2), 0);

  assertEqual(fa.compareToN("aa", 1), 0);
  assertLess(fa.compareToN("aa", 2), 0);
  assertEqual(fa.compareToN(F("aa"), 1), 0);
  assertLess(fa.compareToN(F("aa"), 2), 0);
}

test(FCStringTest, hasSubstring) {
  FCString f("abc");

  assertTrue(f.hasSubstring(""));
  assertTrue(f.hasSubstring("bc"));
  assertFalse(f.hasSubstring("dc"));
}

// ------------------------------------------------------
// Test the various assertXxx() macros.
// ------------------------------------------------------

test(assertEqual) {
  assertEqual(true, true);
  assertEqual(true, true);
  assertEqual(c, c);
  assertEqual(sc, sc);
  assertEqual(usc, usc);
  assertEqual((short)4, (short)4);
  assertEqual((unsigned short)4, (unsigned short)4);
  assertEqual(4, 4);
  assertEqual(4U, 4U);
  assertEqual(4L, 4L);
  assertEqual(4UL, 4UL);
  assertEqual(4.0f, 4.0f);
  assertEqual(4.0, 4.0);
  assertEqual((const char*) nullptr, (const char*) nullptr);
  assertEqual(a, a);
  assertEqual(aa, aa);
  assertEqual(f, f);
  assertEqual(s, s);
  assertEqual(a, s);
  assertEqual(s, a);
  assertEqual(a, f);
  assertEqual(f, a);
  assertEqual(f, s);
  assertEqual(s, f);
}

test(assertLess) {
  assertLess(false, true);
  assertLess(c, d);
  assertLess(sc, sd);
  assertLess(usc, usd);
  assertLess((short)3, (short)4);
  assertLess((unsigned short)3, (unsigned short)4);
  assertLess(3, 4);
  assertLess(3U, 4U);
  assertLess(3L, 4L);
  assertLess(3UL, 4UL);
  assertLess(3.0f, 4.0f);
  assertLess(3.0, 4.0);
  assertLess((const char*) nullptr, b);
  assertLess(a, b);
  assertLess(aa, bb);
  assertLess(f, g);
  assertLess(s, t);
  assertLess(a, t);
  assertLess(s, b);
  assertLess(a, g);
  assertLess(f, b);
  assertLess(f, t);
  assertLess(s, g);
}

test(assertMore) {
  assertMore(true, false);
  assertMore(d, c);
  assertMore(sd, sc);
  assertMore(usd, usc);
  assertMore((uint8_t)4, (uint8_t)3);
  assertMore((short)4, (short)3);
  assertMore(4, 3);
  assertMore(4U, 3U);
  assertMore(4L, 3L);
  assertMore(4UL, 3UL);
  assertMore(4.0f, 3.0f);
  assertMore(4.0, 3.0);
  assertMore(a, (const char*) nullptr);
  assertMore(b, a);
  assertMore(bb, aa);
  assertMore(g, f);
  assertMore(t, s);
  assertMore(t, a);
  assertMore(b, s);
  assertMore(g, a);
  assertMore(b, f);
  assertMore(t, f);
  assertMore(g, s);
}

test(assertLessOrEqual) {
  assertLessOrEqual(false, true);
  assertLessOrEqual(c, d);
  assertLessOrEqual(sc, sd);
  assertLessOrEqual(usc, usd);
  assertLessOrEqual((short)3, (short)4);
  assertLessOrEqual((unsigned short)3, (unsigned short)4);
  assertLessOrEqual(3, 4);
  assertLessOrEqual(3U, 4U);
  assertLessOrEqual(3L, 4L);
  assertLessOrEqual(3UL, 4UL);
  assertLessOrEqual(4.0f, 4.0f);
  assertLessOrEqual(4.0, 4.0);
  assertLessOrEqual((const char*) nullptr, b);
  assertLessOrEqual(a, b);
  assertLessOrEqual(aa, bb);
  assertLessOrEqual(f, g);
  assertLessOrEqual(s, t);
  assertLessOrEqual(a, t);
  assertLessOrEqual(s, b);
  assertLessOrEqual(a, g);
  assertLessOrEqual(f, b);
  assertLessOrEqual(f, t);
  assertLessOrEqual(s, g);
}

test(assertMoreOrEqual) {
  assertMoreOrEqual(true, false);
  assertMoreOrEqual(d, c);
  assertMoreOrEqual(sd, sc);
  assertMoreOrEqual(usd, usc);
  assertMoreOrEqual((short)4, (short)3);
  assertMoreOrEqual((unsigned short)4, (unsigned short)3);
  assertMoreOrEqual(4, 3);
  assertMoreOrEqual(4U, 3U);
  assertMoreOrEqual(4L, 3L);
  assertMoreOrEqual(4UL, 3UL);
  assertMoreOrEqual(4.0f, 4.0f);
  assertMoreOrEqual(4.0, 4.0);
  assertMoreOrEqual(a, (const char*) nullptr);
  assertMoreOrEqual(b, a);
  assertMoreOrEqual(bb, aa);
  assertMoreOrEqual(g, f);
  assertMoreOrEqual(t, s);
  assertMoreOrEqual(t, a);
  assertMoreOrEqual(b, s);
  assertMoreOrEqual(g, a);
  assertMoreOrEqual(b, f);
  assertMoreOrEqual(t, f);
  assertMoreOrEqual(g, s);
}

test(assertNotEqual) {
  assertNotEqual(true, false);
  assertNotEqual(d, c);
  assertNotEqual(sc, sd);
  assertNotEqual(usc, usd);
  assertNotEqual((short)4, (short)3);
  assertNotEqual((unsigned short)4, (unsigned short)3);
  assertNotEqual(4, 3);
  assertNotEqual(4U, 3U);
  assertNotEqual(4L, 3L);
  assertNotEqual(4UL, 3UL);
  assertNotEqual(4.0f, 3.0f);
  assertNotEqual(4.0, 3.0);
  assertNotEqual(a, (const char*) nullptr);
  assertNotEqual(b, a);
  assertNotEqual(bb, aa);
  assertNotEqual(g, f);
  assertNotEqual(t, s);
  assertNotEqual(t, a);
  assertNotEqual(b, s);
  assertNotEqual(g, a);
  assertNotEqual(b, f);
  assertNotEqual(t, f);
  assertNotEqual(g, s);
}

test(assertStringCaseEqual) {
  assertStringCaseEqual((const char*) nullptr, (const char*) nullptr);
  assertStringCaseEqual(a, A);
  assertStringCaseEqual(AA, aa);
  assertStringCaseEqual(f, F);
  assertStringCaseEqual(S, s);
  assertStringCaseEqual(a, S);
  assertStringCaseEqual(S, a);
  assertStringCaseEqual(a, F);
  assertStringCaseEqual(F, a);
  assertStringCaseEqual(f, S);
  assertStringCaseEqual(s, f);
}

test(assertStringCaseNotEqual) {
  assertNotEqual(A, (const char*) nullptr);
  assertNotEqual(b, A);
  assertNotEqual(BB, aa);
  assertNotEqual(g, F);
  assertNotEqual(T, s);
  assertNotEqual(t, A);
  assertNotEqual(B, s);
  assertNotEqual(g, A);
  assertNotEqual(B, f);
  assertNotEqual(t, F);
  assertNotEqual(G, s);
}

test(assertTrue) {
  assertTrue(true);
  assertFalse(false);
}

test(flashString) {
  assertEqual(ff, ff);
  assertEqual(gg, gg);
  assertEqual(hh, hh);
  assertNotEqual(ff, gg);
  assertNotEqual(ff, hh);
  assertNotEqual(gg, hh);
  assertLess(ff, gg);
  assertLess(ff, hh);
  assertLess(gg, hh);
  assertMore(gg, ff);
  assertMore(hh, ff);
  assertMore(hh, gg);
  assertLessOrEqual(ff, gg);
  assertLessOrEqual(ff, hh);
  assertLessOrEqual(gg, hh);
  assertMoreOrEqual(gg, ff);
  assertMoreOrEqual(hh, ff);
  assertMoreOrEqual(hh, gg);
}

// ------------------------------------------------------
// Test assertEqual() for pointers.
// ------------------------------------------------------

test(voidPointer) {
  const int aa[] = {1, 2};
  const long bb[] = {1, 2};
  const char cc[] = "ab";
  const int* dd = nullptr;

  assertTrue(compareEqual(aa, aa));
  assertFalse(compareEqual(aa, bb));
  assertFalse(compareEqual(aa, cc));
  assertFalse(compareEqual(aa, dd));

  assertEqual(aa, aa);
  assertNotEqual(aa, bb);
  assertNotEqual(aa, cc);
  assertNotEqual(aa, dd);
}

test(nullPointer) {
  const int aa[] = {1, 2};
  const long bb[] = {1, 2};
  // const char cc[] = "ab";
  const int* dd = nullptr;

  assertFalse(compareEqual(aa, nullptr));
  assertFalse(compareEqual(bb, nullptr));
  // assertFalse(compareEqual(cc, nullptr)); // ambiguous
  assertTrue(compareEqual(dd, nullptr));

  assertNotEqual(aa, nullptr);
  assertNotEqual(bb, nullptr);
  // assertNotEqual(cc, nullptr); // ambiguous
  assertEqual(dd, nullptr);
}

// -------------------------------------------------------------------------
// Test the string_join() method.
// -------------------------------------------------------------------------

test(string_join) {
  const uint8_t SIZE = 10;
  char dest[SIZE];

  assertTrue(internal::string_join(dest, SIZE, '_', "a", "b"));
  assertEqual("a_b", dest);

  assertTrue(internal::string_join(dest, SIZE, '_', "12345", "678"));
  assertEqual("12345_678", dest);

  assertFalse(internal::string_join(dest, SIZE, '_', "12345", "6789"));
  assertEqual("", dest);
}

#if USE_AUNIT == 1

// -------------------------------------------------------------------------
// Validate the testF() and testingF() macros.
// -------------------------------------------------------------------------

class CustomOnceFixture: public TestOnce {
  protected:
    void setup() override {
      TestOnce::setup();
      subject = 6;
    }

    void teardown() override {
      TestOnce::teardown();
    }

    void testCommon(int m) {
      assertLess(m, subject);
    }

    void assertFailing() {
      assertEqual(1, 2);
    }

    int subject;
};

testF(CustomOnceFixture, common) {
  assertNoFatalFailure(testCommon(5));
  assertEqual(6, subject);
}

class CustomAgainFixture: public TestAgain {
  protected:
    void setup() override {
      TestAgain::setup();
      subject = 6;
    }

    void teardown() override {
      TestAgain::teardown();
    }

    void testCommon(int m) {
      assertLess(m, subject);
    }

    int subject;
};

testingF(CustomAgainFixture, common) {
  assertNoFatalFailure(testCommon(5));
  assertEqual(6, subject);
  pass();
}

// Check that in the case of picking the wrong class for the testF() or the
// testingF() macros, the compiler gives an error.

#if 0
// Test a testingF() macro with a TestOnce class. Should get compiler error
// because testingF() overrides an again() method which doesn't exist in
// TestOnce.
testingF(CustomOnceFixture, crossedOnce) {
  testCommon();
}
#endif

#if 0
// Test a testF() macro with a TestAgain class. Should get compiler error
// because testF() overrides a once() method which doesn't exist in TestAgain.
testF(CustomAgainFixture, crossedAgain) {
  testCommon();
}
#endif

#else

// -------------------------------------------------------------------------
// Test creating custom parent classes manually as supported by ArduinoUnit,
// Not supported in AUnit.
// -------------------------------------------------------------------------

class CustomTestOnce: public TestOnce {
  public:
    CustomTestOnce(const char* name):
        TestOnce(name) {
    }

  protected:
    void setup() override {
      n = 6;
    }

    void once() override {
      assertLessOrEqual(5, 6);
    }

  private:
    int n;
};

CustomTestOnce myTestOnce1("customTestOnce1");
CustomTestOnce myTestOnce2("customTestOnce2");

#endif // USE_AUNIT

// ------------------------------------------------------
// The main body.
// ------------------------------------------------------

void setup() {
#ifdef ARDUINO
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while (! SERIAL_PORT_MONITOR); // Wait until Serial is ready - Leonardo/Micro

#if USE_AUNIT == 1
  // These are useful for debugging.
  //TestRunner::setVerbosity(Verbosity::kAll);
  //TestRunner::setVerbosity(Verbosity::kTestRunSummary);
  //TestRunner::list();
#else
  //Test::min_verbosity = TEST_VERBOSITY_ALL;
#endif
}

void loop() {
#if USE_AUNIT == 1
  // Should get something like:
  // TestRunner summary:
  //    21 passed, 0 failed, 0 skipped, 0 timed out, out of 23 test(s).
  TestRunner::run();
#else
  // Should get something like:
  // Test summary: 12 passed, 0 failed, and 0 skipped, out of 12 test(s).
  Test::run();
#endif
}
