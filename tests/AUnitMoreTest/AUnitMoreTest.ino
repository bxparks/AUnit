#line 2 "AUnitTestMore.ino"
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
 * These have been moved from AUnitTest into here because they overflowed the
 * flash memory limit of an Arduino Pro Micro/Leonardo. They pull in floating
 * point code from <math.h> which increases flash memory consumption by 2644
 * bytes.
 */

#if defined(__AVR__)
  #include <AUnit.h>
#else
  #include <AUnitVerbose.h>
#endif
using namespace aunit;

test(assertNear) {
  assertNear((char) 1, (char) 2, (char) 1);
  assertNear(1, 2, 1);
  assertNear(1U, 2U, 1U);
  assertNear(1L, 2L, 1L);
  assertNear(1UL, 2UL, 1UL);
  assertNear(1.0f, 1.1f, 0.2f);
  assertNear(1.0, 1.1, 0.2);
}

test(assertNotNear) {
  assertNotNear((char) 4, (char) 2, (char) 1);
  assertNotNear(4, 2, 1);
  assertNotNear(4U, 2U, 1U);
  assertNotNear(4L, 2L, 1L);
  assertNotNear(4UL, 2UL, 1UL);
  assertNotNear(4.0f, 1.1f, 0.2f);
  assertNotNear(4.0, 1.1, 0.2);
}

test(longLong) {
  long long a = -1234567890123456789LL;
  long long b = -1234567890123456788LL;
  assertEqual(a, a);
  assertLess(a, b);
  assertMore(b, a);
  assertNotEqual(a, b);
  assertLessOrEqual(a, b);
  assertMoreOrEqual(b, a);
}

test(unsignedLongLong) {
  unsigned long long a = 1234567890123456789ULL;
  unsigned long long b = 1234567890123456790ULL;
  assertEqual(a, a);
  assertLess(a, b);
  assertMore(b, a);
  assertNotEqual(a, b);
  assertLessOrEqual(a, b);
  assertMoreOrEqual(b, a);
}

// ------------------------------------------------------
// The main body.
// ------------------------------------------------------

void setup() {
#ifdef ARDUINO
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while (! SERIAL_PORT_MONITOR); // Wait until Serial is ready - Leonardo/Micro
}

void loop() {
  TestRunner::run();
}
