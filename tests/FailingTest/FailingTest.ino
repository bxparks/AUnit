#line 2 "FailingTest.ino"

/*
 * These are tests which (for the most part) are expected to fail because they
 * are testing whether the AUnit framework actually will detect the failure.
 * Because these tests will always fail, they can't be run in the AUniter
 * continuous integration builds. I have to remember to manually run them
 * before each release.
 */

#if defined(__AVR__)
  #include <AUnit.h>
#else
  #include <AUnitVerbose.h>
#endif
using namespace aunit;

#include "ExternalTests.h"

// -------------------------------------------------------------------------
// Test early bailout upon assert failure.
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

    void assertCommon(int m) {
      assertLess(m, subject);
    }

    void assertFailing() {
      assertEqual(1, 2);
    }

    int subject;
};

testF(CustomOnceFixture, failing_causes_early_bailout) {
  assertFailing();
  assertEqual(6, subject); // should bail out early because of prev failure
  assertTrue(false); // this should not execute
}

// -------------------------------------------------------------------------
// Test immediate termination macros: failTestNow(), passTestNow(),
// skipTestNow(), expireTestNow().
// -------------------------------------------------------------------------

// Each of the following should print a message and exit immediately.
test(failTestNow) { failTestNow(); }
test(passTestNow) { passTestNow(); }
test(skipTestNow) { skipTestNow(); }
test(expireTestNow) { expireTestNow(); }

// -------------------------------------------------------------------------
// Test enableVerbosity() and disableVerbosity()
// -------------------------------------------------------------------------

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

// -------------------------------------------------------------------------
// Test 10 timeout global timeout.
// -------------------------------------------------------------------------
testing(timeout_after_10_seconds) {
  static unsigned long startTime  = millis();

  // complete the test in 20 seconds.
  unsigned long now = millis();
  if (now >= startTime + 20000) {
    pass();
  }
}

// -------------------------------------------------------------------------
// Test externTest() and externTesting() macros and various meta assertions
// in failing conditions.
// -------------------------------------------------------------------------

externTesting(slow_fail);

testing(slow_fail_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start < 900) {
    assertTestNotDone(slow_fail);
    assertTrue(checkTestNotDone(slow_fail));

    assertTestNotPass(slow_fail);
    assertTrue(checkTestNotPass(slow_fail));

    assertTestNotFail(slow_fail);
    assertTrue(checkTestNotFail(slow_fail));

    assertTestNotSkip(slow_fail);
    assertTrue(checkTestNotSkip(slow_fail));

    assertTestNotExpire(slow_fail);
    assertTrue(checkTestNotExpire(slow_fail));
  }
  if (now - start > 2100) {
    assertTestDone(slow_fail);
    assertTrue(checkTestDone(slow_fail));

    assertTestNotPass(slow_fail);
    assertTrue(checkTestNotPass(slow_fail));

    assertTestFail(slow_fail);
    assertTrue(checkTestFail(slow_fail));

    assertTestNotSkip(slow_fail);
    assertTrue(checkTestNotSkip(slow_fail));

    assertTestNotExpire(slow_fail);
    assertTrue(checkTestNotExpire(slow_fail));

    pass();
  }
}

externTesting(slow_expire);

testing(slow_expire_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start < 900) {
    assertTestNotDone(slow_expire);
    assertTrue(checkTestNotDone(slow_expire));

    assertTestNotPass(slow_expire);
    assertTrue(checkTestNotPass(slow_expire));

    assertTestNotFail(slow_expire);
    assertTrue(checkTestNotFail(slow_expire));

    assertTestNotSkip(slow_expire);
    assertTrue(checkTestNotSkip(slow_expire));

    assertTestNotExpire(slow_expire);
  }
  if (now - start > 2100) {
    assertTestDone(slow_expire);
    assertTrue(checkTestDone(slow_expire));

    assertTestNotPass(slow_expire);
    assertTrue(checkTestNotPass(slow_expire));

    assertTestNotFail(slow_expire);
    assertTrue(checkTestNotFail(slow_expire));

    assertTestNotSkip(slow_expire);
    assertTrue(checkTestNotSkip(slow_expire));

    assertTestExpire(slow_expire);
    pass();
  }
}

externTestingF(CustomAgainFixture, fixture_slow_fail);

testing(fixture_slow_fail_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start < 900) {
    assertTestNotDoneF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestNotDoneF(CustomAgainFixture, fixture_slow_fail));

    assertTestNotPassF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestNotPassF(CustomAgainFixture, fixture_slow_fail));

    assertTestNotFailF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestNotFailF(CustomAgainFixture, fixture_slow_fail));

    assertTestNotSkipF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestNotSkipF(CustomAgainFixture, fixture_slow_fail));

    assertTestNotExpireF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestNotExpireF(CustomAgainFixture, fixture_slow_fail));
  }
  if (now - start > 2100) {
    assertTestDoneF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestDoneF(CustomAgainFixture, fixture_slow_fail));

    assertTestNotPassF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestNotPassF(CustomAgainFixture, fixture_slow_fail));

    assertTestFailF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestFailF(CustomAgainFixture, fixture_slow_fail));

    assertTestNotSkipF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestNotSkipF(CustomAgainFixture, fixture_slow_fail));

    assertTestNotExpireF(CustomAgainFixture, fixture_slow_fail);
    assertTrue(checkTestNotExpireF(CustomAgainFixture, fixture_slow_fail));

    pass();
  }
}

externTestingF(CustomAgainFixture, fixture_slow_expire);

testing(fixture_slow_expire_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start < 900) {
    assertTestNotDoneF(CustomAgainFixture, fixture_slow_expire);
    assertTrue(checkTestNotDoneF(CustomAgainFixture, fixture_slow_expire));

    assertTestNotPassF(CustomAgainFixture, fixture_slow_expire);
    assertTrue(checkTestNotPassF(CustomAgainFixture, fixture_slow_expire));

    assertTestNotFailF(CustomAgainFixture, fixture_slow_expire);
    assertTrue(checkTestNotFailF(CustomAgainFixture, fixture_slow_expire));

    assertTestNotSkipF(CustomAgainFixture, fixture_slow_expire);
    assertTrue(checkTestNotSkipF(CustomAgainFixture, fixture_slow_expire));

    assertTestNotExpireF(CustomAgainFixture, fixture_slow_expire);
  }
  if (now - start > 2100) {
    assertTestDoneF(CustomAgainFixture, fixture_slow_expire);
    assertTrue(checkTestDoneF(CustomAgainFixture, fixture_slow_expire));

    assertTestNotPassF(CustomAgainFixture, fixture_slow_expire);
    assertTrue(checkTestNotPassF(CustomAgainFixture, fixture_slow_expire));

    assertTestNotFailF(CustomAgainFixture, fixture_slow_expire);
    assertTrue(checkTestNotFailF(CustomAgainFixture, fixture_slow_expire));

    assertTestNotSkipF(CustomAgainFixture, fixture_slow_expire);
    assertTrue(checkTestNotSkipF(CustomAgainFixture, fixture_slow_expire));

    assertTestExpireF(CustomAgainFixture, fixture_slow_expire);
    pass();
  }
}

// -------------------------------------------------------------------------

void setup() {
#ifdef ARDUINO
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while (! SERIAL_PORT_MONITOR); // Wait until Serial is ready - Leonardo/Micro

  SERIAL_PORT_MONITOR.println(F("This test should produce the following:"));
  SERIAL_PORT_MONITOR.println(
    F("6 passed, 5 failed, 1 skipped, 4 timed out, out of 16 test(s).")
  );
  SERIAL_PORT_MONITOR.println(F("----"));
}

void loop() {
  TestRunner::run();
}
