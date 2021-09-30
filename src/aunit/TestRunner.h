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

#ifndef AUNIT_TEST_RUNNER_H
#define AUNIT_TEST_RUNNER_H

#if defined(EPOXY_DUINO)
#include <stdlib.h> // exit()
#endif
#include <stdint.h>
#include <Arduino.h> // SERIAL_PORT_MONITOR, F(), Print
#include "Test.h"

// ESP32 does not defined SERIAL_PORT_MONITOR
#ifndef SERIAL_PORT_MONITOR
#define SERIAL_PORT_MONITOR Serial
#endif

namespace aunit {

/**
 * The class that runs the various test cases defined by the test() and
 * testing() macros. It prints the summary of each test as well as the final
 * summary of the entire run at the end. In the future, it may be possible to
 * allow a different TestRunner to be used.
 */
class TestRunner {
  public:
    /**
     * Integer type of the timeout parameter. Seconds. Default is
     * kTimeoutDefault = 10
     */
    typedef uint16_t TimeoutType;

    /** Run all tests using the current runner. */
    static void run() {
      getRunner()->runTest();
    }

    /** Print out the known tests. For debugging only. */
    static void list() {
      getRunner()->listTests();
    }

    /**
     * Exclude the tests which match the pattern.
     * Currently supports only a trailing '*'. For example, exclude("flash*").
     */
    static void exclude(const char* pattern) {
      getRunner()->setLifeCycleMatchingPattern(
          pattern, Test::kLifeCycleExcluded);
    }

    /**
     * Exclude the tests which match the pattern given by (testClass + "_" +
     * pattern), the same concatenation rule used by the testF() macro.
     * Currently supports only a trailing '*'. For example,
     * exclude("CustomTest", "flash*").
     */
    static void exclude(const char* testClass, const char* pattern) {
      getRunner()->setLifeCycleMatchingPattern(testClass, pattern,
          Test::kLifeCycleExcluded);
    }

    /**
     * Include the tests which match the pattern.
     * Currently supports only a trailing '*'. For example, include("flash*").
     */
    static void include(const char* pattern) {
      getRunner()->setLifeCycleMatchingPattern(pattern, Test::kLifeCycleNew);
    }

    /**
     * Include the tests which match the pattern given by (testClass + "_" +
     * pattern), the same concatenation rule used by the testF() macro.
     * Currently supports only a trailing '*'. For example,
     * include("CustomTest", "flash*").
     */
    static void include(const char* testClass, const char* pattern) {
      getRunner()->setLifeCycleMatchingPattern(testClass, pattern,
          Test::kLifeCycleNew);
    }

    /** Exclude the tests which match the substring. */
    static void excludesub(const char* substring) {
      getRunner()->setLifeCycleMatchingSubstring(
          substring, Test::kLifeCycleExcluded);
    }

    /** Include the tests which match the substring. */
    static void includesub(const char* substring) {
      getRunner()->setLifeCycleMatchingSubstring(
          substring, Test::kLifeCycleNew);
    }

    /** Set the verbosity flag. */
    static void setVerbosity(uint8_t verbosity) {
      getRunner()->setVerbosityFlag(verbosity);
    }

    /** Returns true if ANY of the bit flags of 'verbosity' is set. */
    static bool isVerbosity(uint8_t verbosity) {
      return getRunner()->isVerbosityFlag(verbosity);
    }

    /** Set the output printer. */
    static void setPrinter(Print* printer);

    /**
     * Set test runner timeout across all tests, in seconds. Set to 0 for
     * infinite timeout. Useful for preventing testing() test cases that never
     * end. This is a timeout for the TestRunner itself, not for individual
     * tests. Upper limit is 65535 seconds (just over 18 hours).
     */
    static void setTimeout(TimeoutType seconds) {
      getRunner()->setRunnerTimeout(seconds);
    }

  private:
    /** Default total timeout for the test runner. */
    static const TimeoutType kTimeoutDefault = 10;

    /** Maximum length of the joined matching pattern, including NUL. */
    static const uint8_t kMaxPatternLength = 63 + 1;

    /** Return the singleton TestRunner. */
    static TestRunner* getRunner();

    /** Count the number of tests. */
    static uint16_t countTests();

    // Disable copy-constructor and assignment operator
    TestRunner(const TestRunner&) = delete;
    TestRunner& operator=(const TestRunner&) = delete;

    /** Constructor. */
    TestRunner() {}

    /**
     * Run the current test case and print out the result.
     *
     * The entire implementation code for this method is placed in the header
     * file, instead of the cpp file, to prevent the `HardwareSerial` class from
     * being pulled in unnecessarily, causing an 800 byte increase in flash size
     * on AVR platforms. See the docstring for setupRunner().
     */
    void runTest() {
      setupRunner();

      // Print initial header if this is the first run.
      if (!mIsRunning) {
        printStartRunner();
        mIsRunning = true;
      }

      // If no more test cases, then print out summary of run.
      if (*Test::getRoot() == nullptr) {
        if (!mIsResolved) {
          mEndTime = millis();
          resolveRun();
          mIsResolved = true;
        #if EPOXY_DUINO
          exit((mFailedCount || mExpiredCount) ? 1 : 0);
        #endif
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
      switch ((*mCurrent)->getLifeCycle()) {
        case Test::kLifeCycleNew:
          // Transfer the verbosity of the TestRunner to the Test.
          (*mCurrent)->enableVerbosity(mVerbosity);
          (*mCurrent)->setup();

          // Support assertXxx() statements inside the setup() method by
          // moving to the next lifeCycle state if an assertXxx() did not fail
          // inside the setup().
          if ((*mCurrent)->getLifeCycle() == Test::kLifeCycleNew) {
            (*mCurrent)->setLifeCycle(Test::kLifeCycleSetup);
          }
          break;
        case Test::kLifeCycleExcluded:
          // If a test is excluded, go directly to LifeCycleFinished, without
          // calling setup() or teardown().
          (*mCurrent)->enableVerbosity(mVerbosity);
          (*mCurrent)->setStatus(Test::kStatusSkipped);
          mSkippedCount++;
          (*mCurrent)->setLifeCycle(Test::kLifeCycleFinished);
          break;
        case Test::kLifeCycleSetup:
          {
            // Check for timeout. mTimeout == 0 means infinite timeout. NOTE: It
            // feels like this code should go into the Test::loop() method (like
            // the extra bit of code in TestOnce::loop()) because it seems like
            // we could want the timeout to be configurable on a case by case
            // basis. This would cause the testing() code to move down into a
            // new again() virtual method dispatched from Test::loop(),
            // analogous to once(). But let's keep the code here for now.
            unsigned long now = millis();
            if (mTimeout > 0 && now >= mStartTime + 1000L * mTimeout) {
              (*mCurrent)->expire();
            } else {
              (*mCurrent)->loop();

              // If test status is unresolved (i.e. still in kLifeCycleNew
              // state) after loop(), then this is a continuous testing() test
              // case, so skip to the next test. Otherwise, stay on the current
              // test so that the next iteration of runTest() can resolve the
              // current test.
              if ((*mCurrent)->getLifeCycle() == Test::kLifeCycleSetup) {
                // skip to the next one, but keep current test in the list
                mCurrent = (*mCurrent)->getNext();
              }
            }
          }
          break;
        case Test::kLifeCycleAsserted:
          switch ((*mCurrent)->getStatus()) {
            case Test::kStatusSkipped:
              mSkippedCount++;
              break;
            case Test::kStatusPassed:
              mPassedCount++;
              break;
            case Test::kStatusFailed:
              mFailedCount++;
              break;
            case Test::kStatusExpired:
              mExpiredCount++;
              break;
            default:
              // should never get here
              mStatusErrorCount++;
              break;
          }
          (*mCurrent)->teardown();
          (*mCurrent)->setLifeCycle(Test::kLifeCycleFinished);
          break;
        case Test::kLifeCycleFinished:
          (*mCurrent)->resolve();
          // skip to the next one by taking current test out of the list
          *mCurrent = *(*mCurrent)->getNext();
          break;
      }
    }

    /**
     * Print out the known tests. For debugging only.
     *
     * The entire implementation code for this method is placed in the header
     * file, instead of the cpp file, to prevent the `HardwareSerial` class from
     * being pulled in unnecessarily, causing an 800 byte increase in flash size
     * on AVR platforms. See the docstring for setupRunner().
     */
    void listTests() {
      setupRunner();

      Print* printer = Printer::getPrinter();
      printer->print(F("TestRunner test count: "));
      printer->println(mCount);
      for (Test** p = Test::getRoot(); (*p) != nullptr; p = (*p)->getNext()) {
        printer->print(F("Test "));
        (*p)->getName().print(printer);
        printer->print(F("; lifeCycle: "));
        printer->println((*p)->getLifeCycle());
      }
    }

    /** Print out message at the start of the run. */
    void printStartRunner() const;

    /** Print out the summary of the entire test suite. */
    void resolveRun() const;

    /**
     * Perform TestRunner initialization. The default Printer::getPrinter()
     * is set to `SERIAL_PORT_MONITOR` if it was not already set by something
     * else in the global setup() function.
     *
     * Important flash memory optimization:
     *
     * The `setupRunner()`, `runTest()` and `listTests()` methods are
     * implemented completely in the header file, instead of the cpp file, to
     * prevent accidental and unnecessary consumption of flash memory on certain
     * platforms, particularly the AVR, but also ESP8266 and ESP32.
     *
     * If the compiler sees a reference to setupRunner() in any cpp file, it
     * pulls in the dependency to `SERIAL_PORT_MONITOR` below, which is usually
     * the global `Serial` object, even if the `SERIAL_PORT_MONITOR` is never
     * used in the final binary. Apparently the C++ compiler (or linker) is not
     * smart enough to realize that it is never used. On the other hand, if
     * these methods are fully defined in only the header file, the compiler
     * understands that `SERIAL_PORT_MONITOR` is never used, and will not pull
     * in the code for `HardwareSerial`. The savings is about about 800 bytes of
     * flash for the AVR processors, 3kB of flash for an ESP8266, and about 5kB
     * of flash for an ESP32. Since the AVR processors have the least amount of
     * flash (e.g. 32kB), this optimization is quite important for AVR
     * processors.
     *
     * The most common way for some other library to pull in a dependency to
     * AUnit is by creating helper test classes under a subdirectory in the
     * library. As an example, my libraries usually have a directory structure
     * like the following, where the `src/` directory contains the main code,
     * and the `testing/` subdirectory contains classes which are used only for
     * AUnit tests:
     *
     * @verbatim
     *  FooLibrary/
     *    |-- library.properties
     *    |-- src/
     *    |-- src/FooLibrary.h
     *    |-- src/foo/Bar.cpp
     *    |-- src/foo/Bar.h
     *    |-- src/foo/testing/TestableBar.cpp
     *    |-- src/foo/testing/TestableBar.h
     * @endverbatim
     *
     * Let's suppose the `TestableBar` class is a subclass of the `TestOnce`
     * class. Then `TestableBar.cpp` must include the `AUnit.h` header, which
     * will then pulls in `TestRunner.h`. If `SERIAL_PORT_MONITOR` appears in
     * the `TestRunner.cpp` file, then the compiler seems to pull in the code
     * for `HardwareSerial`, even if the final application code never uses
     * `testing/TestableBar.h`.
     *
     * If the entire implementation code that requires `SERIAL_PORT_MONITOR` is
     * placed in the header file, it seems that the compiler is able to figure
     * out that `HardwareSerial` is not used by the final binary, and does not
     * include the code.
     */
    void setupRunner() {
      if (mIsSetup) return;

      if (! Printer::getPrinter()) {
        Printer::setPrinter(&SERIAL_PORT_MONITOR);
      }

    #if EPOXY_DUINO
      processCommandLine();
    #endif
      mIsSetup = true;
      mCount = countTests();
      mCurrent = Test::getRoot();
      mStartTime = millis();
    }

    /** Enables the given verbosity. */
    void setVerbosityFlag(uint8_t verbosity) { mVerbosity = verbosity; }

    /** Determine the current verbosity. */
    bool isVerbosityFlag(uint8_t verbosity) const {
      return mVerbosity & verbosity;
    }

    /** Set the status of the tests which match the pattern. */
    void setLifeCycleMatchingPattern(const char* pattern, uint8_t lifeCycle);

    /**
     * Set the status of the tests which match the pattern formed by (testClass
     * + "_" + pattern), the same rule used by testF() and testingF()
     */
    void setLifeCycleMatchingPattern(const char* testClass, const char* pattern,
        uint8_t lifeCycle);

    /** Set the status of the tests which match the substring. */
    void setLifeCycleMatchingSubstring(
        const char* substring, uint8_t lifeCycle);

    /** Forcibly exclude all tests. */
    void excludeAll();

    /** Set the test runner timeout. */
    void setRunnerTimeout(TimeoutType seconds);

  #if EPOXY_DUINO
    enum class FilterType : uint8_t {
      kInclude,
      kExclude,
      kIncludeSub,
      kExcludeSub
    };

    /** Process command line arguments on EpoxyDuino. */
    void processCommandLine();

    /** Parse the command line flags. */
    int parseFlags(int argc, const char* const* argv);

    /**
     * Process the comma-separated list of words for --include, --exclude,
     * --includesub and --excludesub flags.
     */
    void processCommaList(const char* commaList, FilterType filterType);
  #endif

  private:
    // The current test case is represented by a pointer to a pointer. This
    // allows treating the root node the same as all the other nodes, and
    // simplifies the code traversing the singly-linked list significantly.
    Test** mCurrent = nullptr;

    bool mIsResolved = false;
    bool mIsSetup = false;
    bool mIsRunning = false;
    uint8_t mVerbosity = Verbosity::kDefault;
    // True if any include(), exclude(), includesub(), excludesub() was invoked.
    bool hasBeenFiltered = false;
    uint16_t mCount = 0;
    uint16_t mPassedCount = 0;
    uint16_t mFailedCount = 0;
    uint16_t mSkippedCount = 0;
    uint16_t mExpiredCount = 0;
    uint16_t mStatusErrorCount = 0;
    TimeoutType mTimeout = kTimeoutDefault;
    unsigned long mStartTime;
    unsigned long mEndTime;
};

}

#endif
