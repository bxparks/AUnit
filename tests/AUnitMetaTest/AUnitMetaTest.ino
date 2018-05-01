#line 2 "AUnitMetaTest.ino"

/*
 * These tests examine the various "meta" assertion functions, in other words,
 * statements about the status of other tests. They use the following
 * macros:
 *    externTest()
 *    externTestF()
 *    externTesting()
 *    externTestingF()
 *    assertTest*()
 *    assertTest*F()
 *    checkTest*()
 *    checkTest*F()
 *
 * These tests were extracted from AUnitTest.ino because AUnitTest became too
 * big to fit in an Arduino Micro (max flash: 28672).
 */

#include "AUnitMetaTest.h"

// -------------------------------------------------------------------------
// Test externTest() and externTesting() macros and various meta assertions.
// -------------------------------------------------------------------------

externTest(external);

testing(external_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start > 1000) {
    assertTestDone(external);
    assertTrue(checkTestDone(external));

    assertTestPass(external);
    assertTrue(checkTestPass(external));

    assertTestNotFail(external);
    assertTrue(checkTestNotFail(external));

    assertTestNotSkip(external);
    assertTrue(checkTestNotSkip(external));

    assertTestNotExpire(external);
    assertTrue(checkTestNotExpire(external));

    pass();
  }
}

externTesting(slow_pass);

testing(slow_pass_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start < 900) {
    assertTestNotDone(slow_pass);
    assertTrue(checkTestNotDone(slow_pass));

    assertTestNotPass(slow_pass);
    assertTrue(checkTestNotPass(slow_pass));

    assertTestNotFail(slow_pass);
    assertTrue(checkTestNotFail(slow_pass));

    assertTestNotSkip(slow_pass);
    assertTrue(checkTestNotSkip(slow_pass));

    assertTestNotExpire(slow_pass);
    assertTrue(checkTestNotExpire(slow_pass));
  }
  if (now - start > 2100) {
    assertTestDone(slow_pass);
    assertTrue(checkTestDone(slow_pass));

    assertTestPass(slow_pass);
    assertTrue(checkTestPass(slow_pass));

    assertTestNotFail(slow_pass);
    assertTrue(checkTestNotFail(slow_pass));

    assertTestNotSkip(slow_pass);
    assertTrue(checkTestNotSkip(slow_pass));

    assertTestNotExpire(slow_pass);
    assertTrue(checkTestNotExpire(slow_pass));

    pass();
  }
}

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

externTesting(slow_skip);

testing(slow_skip_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start < 900) {
    assertTestNotDone(slow_skip);
    assertTrue(checkTestNotDone(slow_skip));

    assertTestNotPass(slow_skip);
    assertTrue(checkTestNotPass(slow_skip));

    assertTestNotFail(slow_skip);
    assertTrue(checkTestNotFail(slow_skip));

    assertTestNotSkip(slow_skip);
    assertTestNotExpire(slow_skip);
  }
  if (now - start > 2100) {
    assertTestDone(slow_skip);
    assertTrue(checkTestDone(slow_skip));

    assertTestNotPass(slow_skip);
    assertTrue(checkTestNotPass(slow_skip));

    assertTestNotFail(slow_skip);
    assertTrue(checkTestNotFail(slow_skip));

    assertTestSkip(slow_skip);
    assertTrue(checkTestSkip(slow_skip));

    assertTestNotExpire(slow_skip);
    pass();
  }
}

#if USE_AUNIT == 1
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

// -------------------------------------------------------------------------
// Test externTestF() and externTestingF() macros and various meta assertions.
// -------------------------------------------------------------------------

externTestF(CustomOnceFixture, fixture_external);

testing(fixture_external_monitor) {
  // this will loop forever unless explicitly passed
  assertTestDoneF(CustomOnceFixture, fixture_external);
  pass();
}

externTestingF(CustomAgainFixture, fixture_slow_pass);

testing(fixture_slow_pass_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start < 900) {
    assertTestNotDoneF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestNotDoneF(CustomAgainFixture, fixture_slow_pass));

    assertTestNotPassF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestNotPassF(CustomAgainFixture, fixture_slow_pass));

    assertTestNotFailF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestNotFailF(CustomAgainFixture, fixture_slow_pass));

    assertTestNotSkipF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestNotSkipF(CustomAgainFixture, fixture_slow_pass));

    assertTestNotExpireF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestNotExpireF(CustomAgainFixture, fixture_slow_pass));
  }
  if (now - start > 2100) {
    assertTestDoneF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestDoneF(CustomAgainFixture, fixture_slow_pass));

    assertTestPassF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestPassF(CustomAgainFixture, fixture_slow_pass));

    assertTestNotFailF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestNotFailF(CustomAgainFixture, fixture_slow_pass));

    assertTestNotSkipF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestNotSkipF(CustomAgainFixture, fixture_slow_pass));

    assertTestNotExpireF(CustomAgainFixture, fixture_slow_pass);
    assertTrue(checkTestNotExpireF(CustomAgainFixture, fixture_slow_pass));

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

externTestingF(CustomAgainFixture, fixture_slow_skip);

testing(fixture_slow_skip_monitor) {
  static unsigned long start = millis();

  unsigned long now = millis();
  if (now - start < 900) {
    assertTestNotDoneF(CustomAgainFixture, fixture_slow_skip);
    assertTrue(checkTestNotDoneF(CustomAgainFixture, fixture_slow_skip));

    assertTestNotPassF(CustomAgainFixture, fixture_slow_skip);
    assertTrue(checkTestNotPassF(CustomAgainFixture, fixture_slow_skip));

    assertTestNotFailF(CustomAgainFixture, fixture_slow_skip);
    assertTrue(checkTestNotFailF(CustomAgainFixture, fixture_slow_skip));

    assertTestNotSkipF(CustomAgainFixture, fixture_slow_skip);
    assertTestNotExpireF(CustomAgainFixture, fixture_slow_skip);
  }
  if (now - start > 2100) {
    assertTestDoneF(CustomAgainFixture, fixture_slow_skip);
    assertTrue(checkTestDoneF(CustomAgainFixture, fixture_slow_skip));

    assertTestNotPassF(CustomAgainFixture, fixture_slow_skip);
    assertTrue(checkTestNotPassF(CustomAgainFixture, fixture_slow_skip));

    assertTestNotFailF(CustomAgainFixture, fixture_slow_skip);
    assertTrue(checkTestNotFailF(CustomAgainFixture, fixture_slow_skip));

    assertTestSkipF(CustomAgainFixture, fixture_slow_skip);
    assertTrue(checkTestSkipF(CustomAgainFixture, fixture_slow_skip));

    assertTestNotExpireF(CustomAgainFixture, fixture_slow_skip);
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

  // If set to 0, meaning infinite timeout, some testing() may accidentally run
  // forever. Default is 10s, let's set it to 5s to verify that it can be
  // changed.
  TestRunner::setTimeout(5);
#else
  //Test::min_verbosity = TEST_VERBOSITY_ALL;
#endif
}

void loop() {
#if USE_AUNIT == 1
  // Should get something like:
  // TestRunner summary:
  //    14 passed, 2 failed, 2 skipped, 2 timed out, out of 20 test(s).
  TestRunner::run();
#else
  // Should get something like:
  // Test summary: 6 passed, 1 failed, and 1 skipped, out of 8 test(s).
  Test::run();
#endif
}
