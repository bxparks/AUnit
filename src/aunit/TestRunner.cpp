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

#include <Arduino.h>  // import 'Serial'
#include <string.h>

#include "FCString.h"
#include "Compare.h"
#include "Printer.h"
#include "Verbosity.h"
#include "Test.h"
#include "TestRunner.h"

namespace aunit {

// Use a function static singleton to avoid the static initialization ordering
// problem. It's probably not an issue right now, since TestRunner is expected
// to be called only after all static initialization, but future refactoring
// could change that so this is defensive programming.
TestRunner* TestRunner::getRunner() {
  static TestRunner singletonRunner;
  return &singletonRunner;
}

void TestRunner::setPrinter(Print* printer) {
  Printer::setPrinter(printer);
}

void TestRunner::setStatusMatchingPattern(const char* pattern, uint8_t status) {
  size_t length = strlen(pattern);
  if (length > 0 && pattern[length - 1] == '*') {
    // prefix match
    length--;
  } else {
    // exact match
    length++;
  }

  for (Test** p = Test::getRoot(); *p != nullptr; p = (*p)->getNext()) {
    if (compareStringN((*p)->getName(), pattern, length) == 0) {
      (*p)->setStatus(status);
    }
  }
}

TestRunner::TestRunner():
    mCurrent(nullptr),
    mIsResolved(false),
    mIsSetup(false),
    mVerbosity(Verbosity::kDefault),
    mCount(0),
    mPassedCount(0),
    mFailedCount(0),
    mSkippedCount(0) {}

void TestRunner::runTest() {
  setupRunner();

  // If no more test cases, then print out summary of run.
  if (*Test::getRoot() == nullptr) {
    if (!mIsResolved) {
      resolveRun();
    }
    return;
  }

  // If reached the end and there are still test cases left, start from the
  // beginning again.
  if (*mCurrent == nullptr) {
    mCurrent = Test::getRoot();
  }

  // Implement a finite state machine that calls the (*mCurrent)->setup() or
  // (*mCurrent)->loop(), then changes the test case's mStatus.
  switch ((*mCurrent)->getStatus()) {
    case Test::kStatusNew:
      (*mCurrent)->setup();

      // support assertXxx() statements inside the setup() method
      if ((*mCurrent)->getStatus() == Test::kStatusNew) {
        (*mCurrent)->setStatus(Test::kStatusSetup);
      }
      break;
    case Test::kStatusSetup:
      (*mCurrent)->loop();

      // If test status is unresolved (i.e. still in kStatusSetup state) after
      // loop(), then this is a continuous testing() test case, so skip to the
      // next test. Otherwise, stay on the current test so that the next
      // iteration can resolve the current test.
      if ((*mCurrent)->getStatus() == Test::kStatusSetup) {
        // skip to the next one, but keep current test in the list
        mCurrent = (*mCurrent)->getNext();
      }
      break;
    case Test::kStatusSkipped:
      mSkippedCount++;
      resolveTest((*mCurrent));
      // skip to the next one by taking current test out of the list
      *mCurrent = *(*mCurrent)->getNext();
      break;
    case Test::kStatusPassed:
      mPassedCount++;
      resolveTest((*mCurrent));
      // skip to the next one by taking current test out of the list
      *mCurrent = *(*mCurrent)->getNext();
      break;
    case Test::kStatusFailed:
      mFailedCount++;
      resolveTest((*mCurrent));
      // skip to the next one by taking current test out of the list
      *mCurrent = *(*mCurrent)->getNext();
      break;
  }
}

void TestRunner::setupRunner() {
  if (!mIsSetup) {
    mIsSetup = true;
    mCount = countTests();
    mCurrent = Test::getRoot();
  }
}

// Count the number of tests in TestRunner instead of Test::insert() to avoid
// another C++ static initialization ordering problem.
uint16_t TestRunner::countTests() {
  uint16_t count = 0;
  for (Test** p = Test::getRoot(); *p != nullptr; p = (*p)->getNext()) {
    count++;
  }
  return count;
}

void TestRunner::resolveTest(Test* testCase) {
  bool isOutput = isVerbosity(
      Verbosity::kTestFailed | Verbosity::kTestSkipped |
      Verbosity::kTestPassed);
  if (!isOutput) return;

  Printer::getPrinter()->print(F("Test "));
  const FCString& name = testCase->getName();
  if (name.getType() == FCString::kCStringType) {
    Printer::getPrinter()->print(name.getCString());
  } else {
    Printer::getPrinter()->print(name.getFString());
  }
  if (testCase->getStatus() == Test::kStatusSkipped) {
    Printer::getPrinter()->println(F(" skipped."));
  } else if (testCase->getStatus() == Test::kStatusFailed) {
    Printer::getPrinter()->println(F(" failed."));
  } else if (testCase->getStatus ()== Test::kStatusPassed) {
    Printer::getPrinter()->println(F(" passed."));
  }
}

void TestRunner::resolveRun() {
  if (!isVerbosity(Verbosity::kTestRunSummary)) return;

  Printer::getPrinter()->print(F("Test summary: "));
  Printer::getPrinter()->print(mPassedCount);
  Printer::getPrinter()->print(F(" passed, "));
  Printer::getPrinter()->print(mFailedCount);
  Printer::getPrinter()->print(F(" failed, and "));
  Printer::getPrinter()->print(mSkippedCount);
  Printer::getPrinter()->print(F(" skipped, out of "));
  Printer::getPrinter()->print(mCount);
  Printer::getPrinter()->println(F(" test(s)."));

  mIsResolved = true;
}

void TestRunner::listTests() {
  setupRunner();

  Printer::getPrinter()->print("Test count: ");
  Printer::getPrinter()->println(mCount);
  for (Test** p = Test::getRoot(); (*p) != nullptr; p = (*p)->getNext()) {
    Printer::getPrinter()->print(F("Test "));
    const FCString& name = (*p)->getName();
    if (name.getType() == FCString::kCStringType) {
      Printer::getPrinter()->print(name.getCString());
    } else {
      Printer::getPrinter()->print(name.getFString());
    }
    Printer::getPrinter()->println(F(" found."));
  }
}

}
