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

size_t Test::maxLength = 0;

// Use a static variable inside a function to solve the static initialization
// ordering problem.
Test** Test::getRoot() {
  static Test* root;
  return &root;
}

Test::Test():
  mLifeCycle(LifeCycle::New),
  mStatus(Status::Unknown),
  mVerbosity(Verbosity::kNone),
  mNext(nullptr) {
}

// Resolve the status as Failed only if ok == false. Otherwise, keep the
// status as kStatusSetup to allow testing() test cases to continue.
void Test::setPassOrFail(bool ok) {
  if (!ok) {
    setStatus(Status::Failed);
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
  std::string result;

  if (!isVerbosity(Verbosity::kTestAll)) return;

  Print* printer = Printer::getPrinter();
  switch (mStatus)
  {
    case Test::Status::Passed:
      if (isVerbosity(Verbosity::kTestPassed)) result = "\033[32m passed\033[37m.";
      break;
    case Test::Status::Failed:
      if (isVerbosity(Verbosity::kTestFailed)) result = "\033[31m failed\033[37m.";
      break;
    case Test::Status::Skipped:
      if (isVerbosity(Verbosity::kTestSkipped)) result = " skipped.";
      break;
    case Test::Status::Expired:
       if (isVerbosity(Verbosity::kTestExpired)) result = "\033[33m failed\033[37m.";
       break;
    case Test::Status::Unknown:
       break;
  }

  if (result.length())
  {
    printer->print(F("Test"));
    int spc = maxLength - mName.length() + 1;

    while(spc > 0)
    {
      printer->print(' ');
      spc--;
    }

    mName.print(printer);
    printer->println(result.c_str());
  }
}

}
