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
  void setup() override {
    TestOnce::setup();
    Print* printer = Printer::getPrinter();
    printer->print("CustomOnce::setup(): ");
    getName().println(printer);
  }

  void teardown() override {
    Print* printer = Printer::getPrinter();
    printer->print("CustomOnce::teardown(): ");
    getName().println(printer);
    TestOnce::teardown();
  }
    
};

// Verify that no matter how the test ends, the teardown() gets called.
testF(CustomOnce, pass) { pass(); }
testF(CustomOnce, fail) { fail(); }
testF(CustomOnce, skip) { skip(); }
testF(CustomOnce, expire) { expire(); }
testF(CustomOnce, excluded) { fail(); }

class CustomAgain: public TestAgain {
  void setup() override {
    TestAgain::setup();
    Print* printer = Printer::getPrinter();
    printer->print("CustomAgain::setup(): ");
    getName().println(printer);
  }

  void teardown() override {
    Print* printer = Printer::getPrinter();
    printer->print("CustomAgain::teardown(): ");
    getName().println(printer);
    TestAgain::teardown();
  }
    
};

// Verify that if the test executes, setup() and teardown() get called.
// But if the test is excluded, neither setup() nor teardown() get called,
// and the test is reported as "skipped".
testingF(CustomAgain, pass) { pass(); }
testingF(CustomAgain, fail) { fail(); }
testingF(CustomAgain, skip) { skip(); }
testingF(CustomAgain, expire) { expire(); }
testingF(CustomAgain, excluded) { fail(); }

void setup() {
#ifdef ARDUINO
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while (! SERIAL_PORT_MONITOR); // Wait until Serial is ready - Leonardo/Micro

  SERIAL_PORT_MONITOR.println(F("This test should produce the following:"));
  SERIAL_PORT_MONITOR.println(
    F("2 passed, 2 failed, 4 skipped, 2 timed out, out of 10 test(s).")
  );
  SERIAL_PORT_MONITOR.println(F("----"));

  TestRunner::exclude("CustomOnce", "excluded");
  TestRunner::exclude("CustomAgain", "excluded");
  TestRunner::list();
}

void loop() {
  TestRunner::run();
}
