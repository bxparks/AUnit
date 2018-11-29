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

#include <Arduino.h>  // for declaration of 'Serial' on Teensy and others
#include "Flash.h"
#include "Verbosity.h"
#include "Printer.h"
#include "Compare.h"
#include "Test.h"

namespace aunit {

// Use a static variable inside a function to solve the static initialization
// ordering problem.
Test** Test::getRoot() {
  static Test* root;
  return &root;
}

Test::Test():
  mLifeCycle(kLifeCycleNew),
  mStatus(kStatusUnknown),
  mVerbosity(Verbosity::kNone),
  mNext(nullptr) {
}

// Resolve the status as kStatusFailed only if ok == false. Otherwise, keep the
// status as kStatusSetup to allow testing() test cases to continue.
void Test::setPassOrFail(bool ok) {
  if (!ok) {
    setStatus(kStatusFailed);
  }
}

// Insert the current test case into the singly linked list, sorted by
// getName(). This is an O(N^2) algorithm, but should be good enough for
// small N ~= 100. If N becomes bigger than that, it's probably better to insert
// using an O(N) algorithm, then sort the elements later in TestRunner::run().
// Also, we don't increment a static counter here, because that would introduce
// another static initialization ordering problem.
void Test::insert() {
  // Find the element p whose p->next sorts after the current test
  Test** p = getRoot();
  while (*p != nullptr) {
    if (getName().compareTo((*p)->getName()) < 0) break;
    p = &(*p)->mNext;
  }
  mNext = *p;
  *p = this;
}

void Test::resolve() {
  const __FlashStringHelper* const TEST_STRING = F("Test ");

  if (!isVerbosity(Verbosity::kTestAll)) return;

  Print* printer = Printer::getPrinter();
  if (mStatus == Test::kStatusPassed
      && isVerbosity(Verbosity::kTestPassed)) {
    printer->print(TEST_STRING);
    mName.print(printer);
    printer->println(F(" passed."));
  } else if (mStatus == Test::kStatusFailed
      && isVerbosity(Verbosity::kTestFailed)) {
    printer->print(TEST_STRING);
    mName.print(printer);
    printer->println(F(" failed."));
  } else if (mStatus == Test::kStatusSkipped
      && isVerbosity(Verbosity::kTestSkipped)) {
    printer->print(TEST_STRING);
    mName.print(printer);
    printer->println(F(" skipped."));
  } else if (mStatus == Test::kStatusExpired
      && isVerbosity(Verbosity::kTestExpired)) {
    printer->print(TEST_STRING);
    mName.print(printer);
    printer->println(F(" timed out."));
  }
}

}
