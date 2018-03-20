#line 2 "basic.ino"

// Adapted from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/basic/basic.ino

#define USE_AUNIT 1

#if USE_AUNIT == 1
#include <AUnit.h>
#else
#include <ArduinoUnit.h>
#endif

test(correct) {
  int x = 1;
  assertEqual(x, 1);
}

test(incorrect) {
  int x = 1;
  assertNotEqual(x, 1);
}

void setup() {
  Serial.begin(74880); // 74880 is default for some ESP8266 boards
  while(!Serial); // for the Arduino Leonardo/Micro only
  delay(1000); // wait for stability on some boards to prevent garbage Serial
}

void loop() {
#if USE_AUNIT == 1
  // Should get:
  // TestRunner summary:
  //    1 passed, 1 failed, 0 skipped, 0 timed out, out of 2 test(s).
  aunit::TestRunner::run();
#else
  // Should get:
  // Test summary: 1 passed, 1 failed, and 0 skipped, out of 2 test(s).
  Test::run();
#endif
}
