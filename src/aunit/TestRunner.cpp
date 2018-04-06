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

#include <Arduino.h>  // definition of 'Serial'
#include <string.h>
#include <stdint.h>
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

void TestRunner::setStatusMatchingPattern(const char* testClass,
    const char* pattern, uint8_t status) {
  String fullPattern(testClass);
  fullPattern.concat('_');
  fullPattern.concat(pattern);

  setStatusMatchingPattern(fullPattern.c_str(), status);
}

TestRunner::TestRunner():
    mCurrent(nullptr),
    mIsResolved(false),
    mIsSetup(false),
    mIsRunning(false),
    mVerbosity(Verbosity::kDefault),
    mCount(0),
    mPassedCount(0),
    mFailedCount(0),
    mSkippedCount(0),
    mTimeout(kTimeoutDefault) {}

void TestRunner::runTest() {
  setupRunner();

  // Print initial header if this is the first run.
  if (!mIsRunning) {
    printStartRunner();
    mIsRunning = true;
  }

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
      // Transfer the verbosity of the TestRunner to the Test.
      (*mCurrent)->enableVerbosity(mVerbosity);

      (*mCurrent)->setup();

      // support assertXxx() statements inside the setup() method
      if ((*mCurrent)->getStatus() == Test::kStatusNew) {
        (*mCurrent)->setStatus(Test::kStatusSetup);
      }
      break;
    case Test::kStatusSetup:
      {
        // Check for timeout. mTimeout == 0 means infinite timeout.
        // NOTE: It feels like this code should go into the Test::loop() method
        // (like the extra bit of code in TestOnce::loop()) because it seems
        // like we could want the timeout to be configurable on a case by case
        // basis. This would cause the testing() code to move down into a new
        // again() virtual method dispatched from Test::loop(), analogous to
        // once(). But let's keep the code here for now.
        unsigned long now = millis();
        if (mTimeout > 0 && now >= mStartTime + 1000L * mTimeout) {
          (*mCurrent)->expire();
        } else {
          (*mCurrent)->loop();

          // If test status is unresolved (i.e. still in kStatusSetup state)
          // after loop(), then this is a continuous testing() test case, so
          // skip to the next test. Otherwise, stay on the current test so that
          // the next iteration of runTest() can resolve the current test.
          if ((*mCurrent)->getStatus() == Test::kStatusSetup) {
            // skip to the next one, but keep current test in the list
            mCurrent = (*mCurrent)->getNext();
          }
        }
      }
      break;
    case Test::kStatusSkipped:
      mSkippedCount++;
      (*mCurrent)->resolve();
      // skip to the next one by taking current test out of the list
      *mCurrent = *(*mCurrent)->getNext();
      break;
    case Test::kStatusPassed:
      mPassedCount++;
      (*mCurrent)->resolve();
      // skip to the next one by taking current test out of the list
      *mCurrent = *(*mCurrent)->getNext();
      break;
    case Test::kStatusFailed:
      mFailedCount++;
      (*mCurrent)->resolve();
      // skip to the next one by taking current test out of the list
      *mCurrent = *(*mCurrent)->getNext();
      break;
    case Test::kStatusExpired:
      mExpiredCount++;
      (*mCurrent)->resolve();
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
    mStartTime = millis();
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

void TestRunner::printStartRunner() {
  if (!isVerbosity(Verbosity::kTestRunSummary)) return;

  Print* printer = Printer::getPrinter();
  printer->print(F("TestRunner started on "));
  printer->print(mCount);
  printer->println(F(" test(s)."));
}

void TestRunner::resolveRun() {
  if (!isVerbosity(Verbosity::kTestRunSummary)) return;

  Print* printer = Printer::getPrinter();
  printer->print(F("TestRunner summary: "));
  printer->print(mPassedCount);
  printer->print(F(" passed, "));
  printer->print(mFailedCount);
  printer->print(F(" failed, "));
  printer->print(mSkippedCount);
  printer->print(F(" skipped, "));
  printer->print(mExpiredCount);
  printer->print(F(" timed out, out of "));
  printer->print(mCount);
  printer->println(F(" test(s)."));

  mIsResolved = true;
}

void TestRunner::listTests() {
  setupRunner();

  Print* printer = Printer::getPrinter();
  printer->print(F("TestRunner test count: "));
  printer->println(mCount);
  for (Test** p = Test::getRoot(); (*p) != nullptr; p = (*p)->getNext()) {
    printer->print(F("Test "));
    Printer::print((*p)->getName());
    printer->print(F("; status: "));
    printer->println((*p)->getStatus());
  }
}

void TestRunner::setRunnerTimeout(TimeoutType timeout) {
  mTimeout = timeout;
}

}
