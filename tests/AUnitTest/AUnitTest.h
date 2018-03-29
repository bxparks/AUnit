#ifndef AUNIT_AUNIT_TEST_H
#define AUNIT_AUNIT_TEST_H

// If ArduinoUnit is used, this unit test no longer fits in a 32kB
// Arduino UNO or Nano.
#define USE_AUNIT 1

#if USE_AUNIT == 1

#include <Arduino.h> // random()
#include <AUnit.h>
using namespace aunit;

class CustomOnceFixture: public TestOnce {
  protected:
    virtual void setup() override {
      n = random(6);
    }

    void assertCommon() {
      assertLess(n, 6);
    }

  private:
    int n;
};

class CustomLoopFixture: public TestAgain {
  protected:
    virtual void setup() override {
      n = random(6);
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
