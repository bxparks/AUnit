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

#include <stdint.h>
#include "Test.h"

class Print;

namespace aunit {

/**
 * The class that runs the various test cases defined by the test() and
 * testing() macros. It prints the summary of each test as well as the final
 * summary of the entire run at the end. In the future, it may be possible to
 * allow a different TestRunner to be used.
 */
class TestRunner {
  public:
    /** Integer type of the timeout parameter. Seconds. */
    typedef uint8_t TimeoutType;

    /** Run all tests using the current runner. */
    static void run() { getRunner()->runTest(); }

    /** Print out the known tests. For debugging only. */
    static void list() { getRunner()->listTests(); }

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
     * tests.
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
    TestRunner();

    /** Run the current test case and print out the result. */
    void runTest();

    /** Print out the known tests. For debugging only. */
    void listTests();

    /** Print out message at the start of the run. */
    void printStartRunner() const;

    /** Print out the summary of the entire test suite. */
    void resolveRun() const;

    /** Perform any TestRunner initialization. */
    void setupRunner();

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

    /** Set the test runner timeout. */
    void setRunnerTimeout(TimeoutType seconds);

    // The current test case is represented by a pointer to a pointer. This
    // allows treating the root node the same as all the other nodes, and
    // simplifies the code traversing the singly-linked list significantly.
    Test** mCurrent;

    bool mIsResolved;
    bool mIsSetup;
    bool mIsRunning;
    uint8_t mVerbosity;
    uint16_t mCount;
    uint16_t mPassedCount;
    uint16_t mFailedCount;
    uint16_t mSkippedCount;
    uint16_t mExpiredCount;
    uint16_t mStatusErrorCount;
    TimeoutType mTimeout;
    unsigned long mStartTime;
    unsigned long mEndTime;
};

}

#endif
