#line 2 "continuous.ino"

// Adapted from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/continuous/continuous.ino

#include <AUnit.h>

// test-once test named "once"
test(once) {
  int x = 1;
  assertLessOrEqual(x, 1);
}

// test-until-skip-pass-or-fail test named "continuous"
testing(continuous) {
  static unsigned long startTime = millis();

  unsigned long t = millis();
  assertLessOrEqual(t, startTime + 100UL);
  if (t >= 100UL + startTime) pass();
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
    F("2 passed, 0 failed, 0 skipped, 0 timed out, out of 2 test(s).")
  );
  Serial.println(F("----"));
}

void loop() {
  aunit::TestRunner::run();
}
