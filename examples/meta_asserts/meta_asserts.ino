#line 2 "meta_asserts.ino"

// Adapted from code fragment in the README.md in
// https://github.com/mmurdoch/arduinounit/

#include <AUnit.h>
using aunit::TestRunner;
using aunit::Verbosity;

testing(slow_pass) { if (millis() > 1000) pass(); }

testing(slow_fail) { if (millis() > 1000) fail(); }

testing(slow_skip) { if (millis() > 1000) skip(); }

testing(slow_expire) { if (millis() > 1000) expire(); }

testing(slow_pass_monitor) {
  unsigned long now = millis();
  if (now < 1000) {
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
  if (now > 2000) {
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

testing(slow_fail_monitor) {
  unsigned long now = millis();
  if (now < 1000) {
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
  if (now > 2000) {
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

testing(slow_skip_monitor) {
  unsigned long now = millis();
  if (now < 1000) {
    assertTestNotDone(slow_skip);
    assertTrue(checkTestNotDone(slow_skip));

    assertTestNotPass(slow_skip);
    assertTrue(checkTestNotPass(slow_skip));

    assertTestNotFail(slow_skip);
    assertTrue(checkTestNotFail(slow_skip));

    assertTestNotSkip(slow_skip);
    assertTrue(checkTestNotSkip(slow_skip));

    assertTestNotExpire(slow_skip);
    assertTrue(checkTestNotExpire(slow_skip));
  }
  if (now > 2000) {
    assertTestDone(slow_skip);
    assertTrue(checkTestDone(slow_skip));

    assertTestNotPass(slow_skip);
    assertTrue(checkTestNotPass(slow_skip));

    assertTestNotFail(slow_skip);
    assertTrue(checkTestNotFail(slow_skip));

    assertTestSkip(slow_skip);
    assertTrue(checkTestSkip(slow_skip));

    assertTestNotExpire(slow_skip);
    assertTrue(checkTestNotExpire(slow_skip));

    pass();
  }
}

testing(slow_expire_monitor) {
  unsigned long now = millis();
  if (now < 1000) {
    assertTestNotDone(slow_expire);
    assertTrue(checkTestNotDone(slow_expire));

    assertTestNotPass(slow_expire);
    assertTrue(checkTestNotPass(slow_expire));

    assertTestNotFail(slow_expire);
    assertTrue(checkTestNotFail(slow_expire));

    assertTestNotSkip(slow_expire);
    assertTrue(checkTestNotSkip(slow_expire));

    assertTestNotExpire(slow_expire);
    assertTrue(checkTestNotExpire(slow_expire));
  }
  if (now > 2000) {
    assertTestDone(slow_expire);
    assertTrue(checkTestDone(slow_expire));

    assertTestNotPass(slow_expire);
    assertTrue(checkTestNotPass(slow_expire));

    assertTestNotFail(slow_expire);
    assertTrue(checkTestNotFail(slow_expire));

    assertTestNotSkip(slow_expire);
    assertTrue(checkTestNotSkip(slow_expire));

    assertTestExpire(slow_expire);
    assertTrue(checkTestExpire(slow_expire));

    pass();
  }
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while(!Serial); // for the Arduino Leonardo/Micro only

  Serial.println(F("This test should produce the following:"));
  Serial.println(
    F("5 passed, 1 failed, 1 skipped, 1 timed out, out of 8 test(s).")
  );
  Serial.println(F("----"));
}

void loop() {
  TestRunner::run();
}
