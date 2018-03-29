#line 2 "ExternalTests.cpp"

#include "AUnitTest.h"

test(external) {}

testing(slow_pass) { if (millis() > 1000) pass(); }

testing(slow_fail) { if (millis() > 1000) fail(); }

testing(slow_skip) { if (millis() > 1000) skip(); }

#if USE_AUNIT == 1
testing(slow_expire) { if (millis() > 1000) expire(); }
#endif

#if USE_AUNIT == 1

// These are external slow tests using AUnit's testF() and testingF() macro.

testF(CustomOnceFixture, fixture_external) {}

testingF(CustomAgainFixture, fixture_slow_pass) {
  assertCommon();
  if (millis() > 1000) pass();
}

testingF(CustomAgainFixture, fixture_slow_fail) {
  assertCommon();
  if (millis() > 1000) fail();
}

testingF(CustomAgainFixture, fixture_slow_skip) {
  assertCommon();
  if (millis() > 1000) skip();
}

testingF(CustomAgainFixture, fixture_slow_expire) {
  assertCommon();
  if (millis() > 1000) expire();
}

#endif
