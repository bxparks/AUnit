#include "Test.h"

#include <Arduino.h>  // for declaration of 'Serial' on Teensy and others
#include "Compare.h"

namespace aunit {

// Use a static variable inside a function to solve the static initialization
// ordering problem.
Test** Test::getRoot() {
  static Test* root;
  return &root;
}

Test::Test(const char* name):
    mName(name),
    mStatus(kStatusNew),
    mNext(nullptr) {
  insert();
}

Test::Test(const __FlashStringHelper* name):
    mName(name),
    mStatus(kStatusNew),
    mNext(nullptr) {
  insert();
}

void Test::setPassOrFail(bool ok) {
  mStatus = (ok) ? kStatusPassed : kStatusFailed;
}

// Insert the current test case into the singly linked list, sorted by
// getName(). This is an O(N^2) algorithm, but should be good enough for
// small N ~= 100. If N becomes bigger than that, it's probably better to insert
// using an O(N) here, then sort the elements later in TestRunner::run().
// Also, we don't increment a static counter here, because that would introduce
// another static initialization ordering problem.
void Test::insert() {
  // Find the element p whose p->next sorts after the current test
  Test** p = getRoot();
  while (*p != nullptr) {
    if (compareString(getName(), (*p)->getName()) < 0) break;
    p = &(*p)->mNext;
  }
  mNext = *p;
  *p = this;
}

void TestOnce::loop() {
  once();
  if (getStatus() == kStatusSetup) {
    pass();
  }
}

}
