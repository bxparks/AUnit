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
    void setup() override {
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
    void setup() override {
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

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (!Serial); // for the Arduino Leonardo/Micro only

  //TestRunner::setVerbosity(Verbosity::kAll);

  Serial.println(F("This test should produce the following:"));
  Serial.println(
    F("3 passed, 1 failed, 0 skipped, 0 timed out, out of 4 test(s).")
  );
  Serial.println(F("----"));
}

void loop() {
  TestRunner::run();
}
