#line 2 "ExternalTests.h"

#ifndef FAILING_TEST_EXTERNAL_TESTS_H
#define FAILING_TEST_EXTERNAL_TESTS_H

#if defined(__AVR__)
  #include <AUnit.h>
#else
  #include <AUnitVerbose.h>
#endif
using namespace aunit;

class CustomAgainFixture: public TestAgain {
  protected:
    virtual void setup() override {
      TestAgain::setup();
      subject = 6;
    }

    virtual void teardown() override {
      TestAgain::teardown();
    }

    void assertCommon(int m) {
      assertLess(m, subject);
    }

    int subject;
};

#endif
