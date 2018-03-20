#line 2 "advanced.ino"

// Adapted from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/advanced/advanced.ino

#define USE_AUNIT 1

#if USE_AUNIT == 1
#include <AUnit.h>
using namespace aunit;
#else
#include <ArduinoUnit.h>
#endif

test(simple1) {
  assertTrue(true);
}

test(simple2) {
  assertTrue(false);
}

class MyTestOnce: public TestOnce {
  public:
    MyTestOnce(const char *name):
        TestOnce(name) {

#if USE_AUNIT == 0
      // lightweight constructor, since
      // this test might be skipped.
      // you can adjust verbosity here
      verbosity = TEST_VERBOSITY_ALL;
#endif
    }

    virtual void setup() override {
      n = random(6);
      if (n == 0) skip();
    }

    virtual void once() override {
      for (int i = -n; i <= n; ++i) {
        for (int j = -n; j <= n; ++j) {
          assertEqual(i+j, j+i);
        }
      }
    }

  private:
    int n;
};

MyTestOnce myTestOnce1("myTestOnce1");
MyTestOnce myTestOnce2("myTestOnce2");
MyTestOnce myTestOnce3("myTestOnce3");

class MyTest: public Test {
  public:
    MyTest(const char *name):
        Test(name) {
      when = random(100, 200);
    }

    virtual void loop() override {
      assertLess(getValue() ,50L);
      if (millis() >= when) {
        pass(); // if assertion is ok
      }
    }

  private:
    long getValue() {
      return 20L;
    }

    uint16_t when;
};

MyTest myTest1("myTest1");
MyTest myTest2("myTest2");
MyTest myTest3("myTest3");

void setup() {
  Serial.begin(74880); // 74880 is default for some ESP8266 boards
  while (!Serial); // for the Arduino Leonardo/Micro only
  delay(1000); // wait for stability on some boards to prevent garbage Serial

#if USE_AUNIT == 1
  TestRunner::setVerbosity(Verbosity::kAll);
  TestRunner::exclude("myTestOnce2");
  TestRunner::exclude("myTest2");
#else
  Test::min_verbosity |= TEST_VERBOSITY_ASSERTIONS_ALL;
  Test::exclude("my*2");
#endif
}

void loop() {
#if USE_AUNIT == 1
  // Should get:
  // TestRunner summary:
  //    5 passed, 1 failed, 2 skipped, 0 timed out, out of 8 test(s).
  TestRunner::run();
#else
  // Should get:
  // Test summary: 5 passed, 1 failed, and 2 skipped, out of 8 test(s).
  Test::run();
#endif
}
