#line 2 "meta_asserts.ino"

// Copied from the README.md in https://github.com/mmurdoch/arduinounit/
// with some fixes.

#define USE_AUNIT 1

#if USE_AUNIT == 1
#include <AUnit.h>
using namespace aunit;
#else
#include <ArduinoUnit.h>
#endif

test(ok) { pass(); }

// This should always fail.
test(bad) { fail(); }

testing(slow) { if (millis() > 1000) pass(); }

testing(passed)
{
  if (checkTestDone(ok)) {
    pass();
  }
}

testing(too_slow)
{
  if (millis() > 2000) {
    assertTestDone(slow);
    pass();
  }
}

void setup()
{
  Serial.begin(74880); // 74880 is default for some ESP8266 boards
  while(!Serial); // for the Arduino Leonardo/Micro only
}

// We should get 4 passes and 1 failure out of 5 tests.
void loop()
{
#if USE_AUNIT == 1
  TestRunner::run();
#else
  Test::run();
#endif
}
