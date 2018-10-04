#line 2 "AUnitMetaTest.h"

#ifndef AUNIT_AUNIT_META_TEST_H
#define AUNIT_AUNIT_META_TEST_H

// If ArduinoUnit is used, this unit test no longer fits in a 32kB
// Arduino UNO or Nano.
#define USE_AUNIT 1

#if USE_AUNIT == 1

#if defined(__AVR__)
  #include <AUnit.h>
#else
  #include <AUnitVerbose.h>
#endif

using namespace aunit;

class CustomOnceFixture: public TestOnce {
  protected:
    void setup() override {
      TestOnce::setup();
      subject = 6;
    }

    void teardown() override {
      TestOnce::teardown();
    }

    void assertCommon(int m) {
      assertLess(m, subject);
    }

    void assertFailing() {
      assertEqual(1, 2);
    }

    int subject;
};

class CustomAgainFixture: public TestAgain {
  protected:
    void setup() override {
      TestAgain::setup();
      subject = 6;
    }

    void teardown() override {
      TestAgain::teardown();
    }

    void assertCommon(int m) {
      assertLess(m, subject);
    }

    int subject;
};

#else

#include <ArduinoUnit.h>

// These are available only in AUnit so make them disappear in ArduinoUnit.
#define assertTestExpire(x)
#define assertTestNotExpire(x)
#define checkTestExpire(x) true
#define checkTestNotExpire(x) true

#endif

#endif
