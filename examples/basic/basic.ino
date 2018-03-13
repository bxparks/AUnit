#line 2 "basic.ino"

// Copied from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/basic/basic.ino

#define USE_AUNIT 1

#if USE_AUNIT == 1
#include <AUnit.h>
#else
#include <ArduinoUnit.h>
#endif

test(correct)
{
  int x=1;
  assertEqual(x,1);
}

test(incorrect)
{
  int x=1;
  assertNotEqual(x,1);
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
