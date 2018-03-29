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

class BaseTest: public TestOnce {
};

class ATest: public BaseTest {
};

class BTest: public ATest {
};

testF(ATest, setPattern_setBrightness) {}
testF(ATest, writeDigitPin) {}
testF(ATest, writeSegmentPin) {}
testF(ATest, configure) {}
testF(ATest, displayCurrentField_one_dark) {}
testF(ATest, displayCurrentField_repeated_segment_pattern) {}
testF(BTest, mod_configure) {}
testF(BTest, mod_displayCurrentField_one_dark) {}
test(incrementMod) {}
test(calcPulseFraction) {}
test(calcBlinkState) {}

void setup() {
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
  Serial.begin(74880); // 74880 is the default for some ESP8266 boards
  while (! Serial); // Wait until Serial is ready - Leonardo

  TestRunner::list();
  TestRunner::exclude("*");
  TestRunner::list();
  TestRunner::include("mod_*");
  TestRunner::list();
}

void loop() {
  TestRunner::run();
}
