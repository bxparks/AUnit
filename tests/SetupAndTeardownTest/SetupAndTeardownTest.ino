#line 2 "SetupAndTeardownTest.ino"

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

#include <AUnit.h>
using namespace aunit;

class CustomOnce: public TestOnce {
  virtual void setup() override {
    TestOnce::setup();
    Printer::getPrinter()->print("CustomOnce::setup(): ");
    Printer::println(getName());
  }

  virtual void teardown() override {
    Printer::getPrinter()->print("CustomOnce::teardown(): ");
    Printer::println(getName());
    TestOnce::teardown();
  }
    
};

// Verify that no matter how the test ends, the teardown() gets called.
testF(CustomOnce, pass) { pass(); }
testF(CustomOnce, fail) { fail(); }
testF(CustomOnce, skip) { skip(); }
testF(CustomOnce, expire) { expire(); }

class CustomAgain: public TestAgain {
  virtual void setup() override {
    TestAgain::setup();
    Printer::getPrinter()->print("CustomAgain::setup(): ");
    Printer::println(getName());
  }

  virtual void teardown() override {
    Printer::getPrinter()->print("CustomAgain::teardown(): ");
    Printer::println(getName());
    TestAgain::teardown();
  }
    
};

// Verify that no matter how the test ends, the teardown() gets called.
testingF(CustomAgain, pass) { pass(); }
testingF(CustomAgain, fail) { fail(); }
testingF(CustomAgain, skip) { skip(); }
testingF(CustomAgain, expire) { expire(); }

void setup() {
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (! Serial); // Wait until Serial is ready - Leonardo

  TestRunner::list();
}

void loop() {
  // Should get something like:
  // TestRunner summary:
  //     2 passed, 2 failed, 2 skipped, 2 timed out, out of 8 test(s).
  TestRunner::run();
}
