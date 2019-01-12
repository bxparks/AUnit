#line 2 "FilterTest.ino"

/*
MIT License

Copyright (c) 2018 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
 * This unit test caught a bug with TestRunner::include() which incorrectly set
 * the status of the test to kStatusSetup instead of kStatusNew.
 */

#include <AUnit.h>
using namespace aunit;

test(configure) {}
test(display) {}

class CustomOnce: public TestOnce {
  void setup() override {
  }

  void teardown() override {
  }
};

testF(CustomOnce, configure) {}
testF(CustomOnce, display) {}

class CustomAgain: public TestAgain {
  void setup() override {
  }

  void teardown() override {
  }
};

testingF(CustomAgain, configure) { pass(); }
testingF(CustomAgain, display) { pass(); }

/**
 * Assert that the given test has the expected lifecycle state. We can't use
 * the various assertXxx() macros because these need to be used before the
 * AUnit framework is brought up. So we need to create special assertion macros
 * just for this test.
 */
void assertionLifeCycle(uint8_t expected, const Test& instance, uint16_t line) {
  if (expected != instance.getLifeCycle()) {
    Serial.print(F("FAILED: " __FILE__ " on line "));
    Serial.println(line);
  }
}

/** Macro that automatically inserts the __LINE__. */
#define assertLifeCycle(expected, instance) \
  assertionLifeCycle(expected, instance, __LINE__)

void setup() {
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (! Serial); // Wait until Serial is ready - Leonardo

  // Verify that the names of these tests don't collide and can be
  // independently selected. Name of test is "{testClass}_{name}", but we can
  // use the new 2-argument versions of include(testClass, pattern) and
  // exclude(testClass, pattern) instead.

  TestRunner::exclude("*");
  assertLifeCycle(Test::kLifeCycleExcluded, test_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, test_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomAgain_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomAgain_display_instance);

  TestRunner::include("configure");
  assertLifeCycle(Test::kLifeCycleNew, test_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, test_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomAgain_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomAgain_display_instance);

  TestRunner::include("CustomAgain", "*");
  assertLifeCycle(Test::kLifeCycleNew, test_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, test_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_display_instance);
  assertLifeCycle(Test::kLifeCycleNew, CustomAgain_configure_instance);
  assertLifeCycle(Test::kLifeCycleNew, CustomAgain_display_instance);

  TestRunner::exclude("CustomAgain", "*");
  assertLifeCycle(Test::kLifeCycleNew, test_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, test_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomAgain_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomAgain_display_instance);

  TestRunner::include("CustomAgain", "display");
  assertLifeCycle(Test::kLifeCycleNew, test_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, test_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomAgain_configure_instance);
  assertLifeCycle(Test::kLifeCycleNew, CustomAgain_display_instance);

  TestRunner::include("CustomOnce", "dis*");
  assertLifeCycle(Test::kLifeCycleNew, test_configure_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, test_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomOnce_configure_instance);
  assertLifeCycle(Test::kLifeCycleNew, CustomOnce_display_instance);
  assertLifeCycle(Test::kLifeCycleExcluded, CustomAgain_configure_instance);
  assertLifeCycle(Test::kLifeCycleNew, CustomAgain_display_instance);
}

void loop() {
  // Should get something like:
  // TestRunner summary:
  //    3 passed, 0 failed, 3 skipped, 0 timed out, out of 6 test(s).
  //
  // Verify that excluded tests do not execute setup() and teardown(). They
  // go directly into the final Skipped state.
  TestRunner::run();
}
