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
  virtual void setup() override {
    Serial.println(F("CustomOnce::setup()"));
  }

  virtual void teardown() override {
    Serial.println(F("CustomOnce::teardown()"));
  }
};

testF(CustomOnce, configure) {}
testF(CustomOnce, display) {}

class CustomAgain: public TestAgain {
  virtual void setup() override {
    Serial.println(F("CustomAgain::setup()"));
  }

  virtual void teardown() override {
    Serial.println(F("CustomAgain::teardown()"));
  }
};

testingF(CustomAgain, configure) { pass(); }
testingF(CustomAgain, display) { pass(); }

void setup() {
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (! Serial); // Wait until Serial is ready - Leonardo

  // Verify that the names of these tests don't collide and can be
  // independently selected. Name of test is "{testClass}_{name}", but we can
  // use the new 2-argument versions of include(testClass, pattern) and
  // exclude(testClass, pattern) instead.

  TestRunner::list();

  Serial.println("exclude(\"*\")");
  TestRunner::exclude("*");
  TestRunner::list();

  Serial.println("include(\"configure*\")");
  TestRunner::include("configure");
  TestRunner::list();

  Serial.println("include(\"CustomAgain*\")");
  TestRunner::include("CustomAgain*");
  TestRunner::list();

  Serial.println("exclude(\"CustomAgain\", \"*\")");
  TestRunner::exclude("CustomAgain", "*");
  TestRunner::list();

  Serial.println("include(\"CustomAgain\", \"display\")");
  TestRunner::include("CustomAgain", "display");
  TestRunner::list();

  Serial.println("include(\"CustomOnce_dis*\")");
  TestRunner::include("CustomOnce_dis*");
  TestRunner::list();
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
