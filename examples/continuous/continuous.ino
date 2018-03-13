#line 2 "continuous.ino"

// Copied from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/continuous/continuous.ino

#define USE_AUNIT 1

#if USE_AUNIT == 1
#include <AUnit.h>
#else
#include <ArduinoUnit.h>
#endif

// test-once test named "once"
test(once)
{
  int x=1;
  assertLessOrEqual(x,1);
}

// test-until-skip-pass-or-fail test named "continuous"
testing(continuous)
{
  unsigned long t = millis();
  assertLessOrEqual(t,100UL);
  if (t >= 100) pass();
}

void setup()
{
  Serial.begin(74880); // 74880 is default for some ESP8266 boards
  while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop()
{
#if USE_AUNIT == 1
  aunit::TestRunner::run();
#else
  Test::run();
#endif
}
