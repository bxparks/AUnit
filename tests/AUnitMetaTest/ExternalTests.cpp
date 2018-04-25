#line 2 "ExternalTests.cpp"

/*
 * Define a bunch of tests in a file outside of AUnitTest to verify that
 * externTest(), externTesting(), externTestF() and externTestingF() work as
 * expected.
 */

#include <Arduino.h>
#include "AUnitMetaTest.h"

test(external) {}

testing(slow_pass) {
  static unsigned long start = millis();
  if (millis() - start > 1000) pass();
}

testing(slow_fail) {
  static unsigned long start = millis();
  if (millis() - start > 1000) fail();
}

testing(slow_skip) {
  static unsigned long start = millis();
  if (millis() - start > 1000) skip();
}

#if USE_AUNIT == 1
testing(slow_expire) {
  static unsigned long start = millis();
  if (millis() - start > 1000) expire();
}
#endif

#if USE_AUNIT == 1

// These are external slow tests using AUnit's testF() and testingF() macro.

testF(CustomOnceFixture, fixture_external) {}

testingF(CustomAgainFixture, fixture_slow_pass) {
  static unsigned long start = millis();
  assertCommon(5);
  if (millis() - start > 1000) pass();
}

testingF(CustomAgainFixture, fixture_slow_fail) {
  static unsigned long start = millis();
  assertCommon(5);
  if (millis() - start > 1000) fail();
}

testingF(CustomAgainFixture, fixture_slow_skip) {
  static unsigned long start = millis();
  assertCommon(5);
  if (millis() - start > 1000) skip();
}

testingF(CustomAgainFixture, fixture_slow_expire) {
  static unsigned long start = millis();
  assertCommon(5);
  if (millis() - start > 1000) expire();
}

#endif
