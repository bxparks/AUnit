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

#include <WString.h>

#if USE_AUNIT == 1

  #if defined(__AVR__)
    #include <AUnit.h>
  #else
    #include <AUnitVerbose.h>
  #endif
  using namespace aunit;
  using namespace aunit::internal;

#else

  #include <ArduinoUnit.h>

#endif

// Defined in ESP8266, not defined in AVR or Teensy, broken in ESP32.
#if !defined(ESP8266)
  #undef FPSTR
  #define FPSTR(pstr_pointer) \
      (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif

signed char sc = 4;
signed char sd = 5;
unsigned char usc = 4;
unsigned char usd = 5;
char c = 'a';
char d = 'b';
const char a[] = "a";
const char b[] = "b";
char aa[] = "a";
char bb[] = "b";
String s = "a";
String t = "b";
const char F_PROGMEM[] PROGMEM = "a";
const char G_PROGMEM[] PROGMEM = "b";
const __FlashStringHelper* f = FPSTR(F_PROGMEM);
const __FlashStringHelper* g = FPSTR(G_PROGMEM);
const char FF_PROGMEM[] PROGMEM = "abcde";
const char GG_PROGMEM[] PROGMEM = "abcdef";
const char HH_PROGMEM[] PROGMEM = "abcdeg";
const __FlashStringHelper* ff = FPSTR(FF_PROGMEM);
const __FlashStringHelper* gg = FPSTR(GG_PROGMEM);
const __FlashStringHelper* hh = FPSTR(HH_PROGMEM);

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

#if USE_AUNIT == 1

// We use if-statements instead of assertXxx() because compareXxx() is used by
// the assertXxx() methods. Strictly speaking, it's not necessary because one
// is a string compare and the other is an integer compare, but this feels
// conceptually cleaner.
test(compareString) {
  if (!(compareString(a, a) == 0)) { fail(); }
  if (!(compareString(a, f) == 0)) { fail(); }
  if (!(compareString(a, s) == 0)) { fail(); }

  if (!(compareString(f, a) == 0)) { fail(); }
  if (!(compareString(f, f) == 0)) { fail(); }
  if (!(compareString(f, s) == 0)) { fail(); }

  if (!(compareString(s, a) == 0)) { fail(); }
  if (!(compareString(s, f) == 0)) { fail(); }
  if (!(compareString(s, s) == 0)) { fail(); }

  if (!(compareString(a, b) < 0)) { fail(); }
  if (!(compareString(a, g) < 0)) { fail(); }
  if (!(compareString(a, t) < 0)) { fail(); }

  if (!(compareString(f, b) < 0)) { fail(); }
  if (!(compareString(f, g) < 0)) { fail(); }
  if (!(compareString(f, t) < 0)) { fail(); }

  if (!(compareString(s, b) < 0)) { fail(); }
  if (!(compareString(s, g) < 0)) { fail(); }
  if (!(compareString(s, t) < 0)) { fail(); }
}

// We use if-statements instead of assertXxx() because compareXxx() is used by
// the assertXxx() methods. Strictly speaking, it's not necessary because one
// is a string compare and the other is an integer compare, but this feels
// conceptually cleaner.
test(compareStringN) {
  if (!(compareStringN(ff, "abcde", 5) == 0)) { fail(); }
  if (!(compareStringN("abcde", ff, 5) == 0)) { fail(); }

  if (!(compareStringN(ff, "abcd", 5) > 0)) { fail(); }
  assertLess(compareStringN("abcd", ff, 5), 0);

  if (!(compareStringN(ff, "abcd", 4) == 0)) { fail(); }
  if (!(compareStringN("abcd", ff, 4) == 0)) { fail(); }

  if (!(compareStringN(ff, "", 1) > 0)) { fail(); }
  if (!(compareStringN("", ff, 1) < 0)) { fail(); }

  if (!(compareStringN(ff, "", 0) == 0)) { fail(); }
  if (!(compareStringN("", ff, 0) == 0)) { fail(); }

  if (!(compareStringN(gg, ff, 5) == 0)) { fail(); }

  if (!(compareStringN(gg, ff, 6) > 0)) { fail(); }
}

#endif

test(assertEqual) {
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

test(assertTrue) {
  assertTrue(true);
  assertFalse(false);
}

#if USE_AUNIT == 1
test(verbosity_assertionFailed_only) {
  enableVerbosity(Verbosity::kAssertionPassed);
  disableVerbosity(Verbosity::kTestPassed);
  assertTrue(true);
}

test(verbosity_testFailed_only) {
  enableVerbosity(Verbosity::kTestFailed);
  disableVerbosity(Verbosity::kAssertionFailed);
  assertTrue(false);
}
#endif

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

testing(timeout_after_10_seconds) {
  static unsigned long startTime  = millis();

  // complete the test in 20 seconds.
  unsigned long now = millis();
  if (now >= startTime + 20000) {
    pass();
  }
}

// Each of the following should print a message and exit immediately.
test(failTestNow) { failTestNow(); }
test(passTestNow) { passTestNow(); }
test(skipTestNow) { skipTestNow(); }
test(expireTestNow) { expireTestNow(); }

#if USE_AUNIT == 1

// -------------------------------------------------------------------------
// Validate the testF() and testingF() macros.
// -------------------------------------------------------------------------

class CustomOnceFixture: public TestOnce {
  protected:
    virtual void setup() override {
      TestOnce::setup();
      subject = 6;
    }

    virtual void teardown() override {
      TestOnce::teardown();
    }

    void assertCommon(int m) {
      assertLess(m, subject);
    }

    void assertFailing() {
      assertEqual(1, 2);
    }

    int subject;
};

testF(CustomOnceFixture, common) {
  assertCommon(5);
  assertEqual(6, subject);
}

testF(CustomOnceFixture, failing) {
  assertFailing();
  assertEqual(6, subject); // should bail out early because of prev failure
  assertTrue(false); // this should not execute
}

class CustomAgainFixture: public TestAgain {
  protected:
    virtual void setup() override {
      TestAgain::setup();
      subject = 6;
    }

    virtual void teardown() override {
      TestAgain::teardown();
    }

    void assertCommon(int m) {
      assertLess(m, subject);
    }

    int subject;
};

testingF(CustomAgainFixture, common) {
  assertCommon(5);
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
  assertCommon();
}
#endif

#if 0
// Test a testF() macro with a TestAgain class. Should get compiler error
// because testF() overrides a once() method which doesn't exist in TestAgain.
testF(CustomAgainFixture, crossedAgain) {
  assertCommon();
}
#endif

#else

// -------------------------------------------------------------------------
// Test creating custom parent classes manually as supported by ArduinoUnit,
// Not supported in AUnit.
// -------------------------------------------------------------------------

class CustomTestOnce: public TestOnce {
  public:
    CustomTestOnce(const char *name):
        TestOnce(name) {
    }

  protected:
    virtual void setup() override {
      n = 6;
    }

    virtual void once() override {
      assertLessOrEqual(5, 6);
    }

  private:
    int n;
};

CustomTestOnce myTestOnce1("customTestOnce1");
CustomTestOnce myTestOnce2("customTestOnce2");

#endif

// ------------------------------------------------------
// The main body.
// ------------------------------------------------------

void setup() {
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro

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
  //    15 passed, 3 failed, 1 skipped, 2 timed out, out of 21 test(s).
  TestRunner::run();
#else
  // Should get something like:
  // Test summary: 12 passed, 0 failed, and 0 skipped, out of 12 test(s).
  Test::run();
#endif
}
