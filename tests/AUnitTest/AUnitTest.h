#line 2 "AUnitTest.h"

#ifndef AUNIT_AUNIT_TEST_H
#define AUNIT_AUNIT_TEST_H

// If ArduinoUnit is used, this unit test no longer fits in a 32kB
// Arduino UNO or Nano.
#define USE_AUNIT 1

#if USE_AUNIT == 1

#include <Arduino.h> // random()

// AVR:
//    AUnit.h: flash/static: 29186/1369
//    AUnitVerbose.h: flash/static: 37352/1373 (too big for ATmega328P)
// ESP8266:
//    AUnit.h: flash/static: 276112/33476
//    AUnitVerbose.h: flash/static: 281464/36100
// Teensy 3.2:
//    AUnit.h: flash/static: 43328/5440
//    AUnitVerbose.h: flash/static: 49820/5440
#if defined(__AVR__)
  #include <AUnit.h>
#else
  #include <AUnitVerbose.h>
#endif

using namespace aunit;

class CustomOnceFixture: public TestOnce {
  protected:
    virtual void setup() override {
      TestOnce::setup();
      n = random(6);
    }

    virtual void teardown() override {
      TestOnce::teardown();
    }

    void assertCommon() {
      assertLess(n, 6);
    }

    void assertFailing() {
      assertEqual(1, 2);
    }

  private:
    int n;
};

class CustomAgainFixture: public TestAgain {
  protected:
    virtual void setup() override {
      TestAgain::setup();
      n = random(6);
    }

    virtual void teardown() override {
      TestAgain::teardown();
    }

    void assertCommon() {
      assertLess(n, 6);
    }

  private:
    int n;
};

#else

#include <ArduinoUnit.h>

// These are available only in AUnit so make them disappear in ArduinoUnit.
#define assertTestExpire(x)
#define assertTestNotExpire(x)
#define checkTestExpire(x) true
#define checkTestNotExpire(x) true

// Defined in ESP8266, not defined in AVR or Teensy
#ifndef FPSTR
#define FPSTR(pstr_pointer) \
    (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif

#endif

#endif
