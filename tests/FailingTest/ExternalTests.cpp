#line 2 "ExternalTests.cpp"

#include "ExternalTests.h"

testing(slow_fail) {
  static unsigned long start = millis();
  if (millis() - start > 1000) fail();
}

testing(slow_expire) {
  static unsigned long start = millis();
  if (millis() - start > 1000) expire();
}

testingF(CustomAgainFixture, fixture_slow_fail) {
  static unsigned long start = millis();
  assertCommon(5);
  if (millis() - start > 1000) fail();
}

testingF(CustomAgainFixture, fixture_slow_expire) {
  static unsigned long start = millis();
  assertCommon(5);
  if (millis() - start > 1000) expire();
}

