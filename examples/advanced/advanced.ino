#line 2 "advanced.ino"

// Adapted from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/advanced/advanced.ino

#include <AUnit.h>
using namespace aunit;

test(simple1) {
  assertTrue(true);
}

test(simple2) {
  assertTrue(false);
}

class MyTestOnce: public TestOnce {
  protected:
    virtual void setup() override {
      n = random(6);
      if (n == 0) skip();
    }

    int n;
};

testF(MyTestOnce, simple1) {
  for (int i = -n; i <= n; ++i) {
    for (int j = -n; j <= n; ++j) {
      assertEqual(i+j, j+i);
    }
  }
}

class MyTestAgain: public TestAgain {
  protected:
    virtual void setup() override {
      TestAgain::setup();
      when = random(100, 200);
    }

    long getValue() {
      return 20L;
    }

    uint16_t when;
};

testingF(MyTestAgain, again) {
  assertLess(getValue() ,50L);
  if (millis() >= when) {
    pass(); // if assertion is ok
  }
}

void setup() {
  Serial.begin(74880); // 74880 is default for some ESP8266 boards
  while (!Serial); // for the Arduino Leonardo/Micro only
  delay(1000); // wait for stability on some boards to prevent garbage Serial

  TestRunner::setVerbosity(Verbosity::kAll);
}

void loop() {
  // Should get:
  // TestRunner summary:
  //    5 passed, 1 failed, 2 skipped, 0 timed out, out of 8 test(s).
  TestRunner::run();
}
