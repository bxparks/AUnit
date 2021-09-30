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

#if EPOXY_DUINO
#include <stdio.h>
#endif
#include <Arduino.h>  // 'Serial' or SERIAL_PORT_MONITOR
#include <string.h>
#include <stdint.h>
#include "FCString.h"
#include "Compare.h"
#include "Printer.h"
#include "Verbosity.h"
#include "Test.h"
#include "TestRunner.h"
#include "string_util.h"

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

void TestRunner::setLifeCycleMatchingPattern(const char* pattern,
    uint8_t lifeCycle) {
  // Do an implicit excludeAll() if the first filter is an include().
  if (!hasBeenFiltered && lifeCycle == Test::kLifeCycleNew) {
    excludeAll();
  }
  hasBeenFiltered = true;

  size_t length = strlen(pattern);
  if (length > 0 && pattern[length - 1] == '*') {
    // prefix match
    length--;
  } else {
    // exact match
    length++;
  }

  for (Test** p = Test::getRoot(); *p != nullptr; p = (*p)->getNext()) {
    if ((*p)->getName().compareToN(pattern, length) == 0) {
      (*p)->setLifeCycle(lifeCycle);
    }
  }
}

void TestRunner::setLifeCycleMatchingPattern(const char* testClass,
    const char* pattern, uint8_t lifeCycle) {

  // Do an implicit excludeAll() if the first filter is an include().
  if (!hasBeenFiltered && lifeCycle == Test::kLifeCycleNew) {
    excludeAll();
  }
  hasBeenFiltered = true;

  // The effective pattern is the join of testClass and pattern with a '_'
  // delimiter. This must match the algorithm used by testF() and testingF().
  // We use string_join() instead of String so that AUnit can avoid a direct
  // dependency on the String class. AUnit thus avoids allocating any memory on
  // the heap.
  char fullPattern[kMaxPatternLength];
  bool status = internal::string_join(fullPattern, kMaxPatternLength, '_',
      testClass, pattern);
  if (!status) return;

  setLifeCycleMatchingPattern(fullPattern, lifeCycle);
}

void TestRunner::setLifeCycleMatchingSubstring(
    const char* substring, uint8_t lifeCycle) {

  // Do an implicit excludeAll() if the first filter is an include().
  if (!hasBeenFiltered && lifeCycle == Test::kLifeCycleNew) {
    excludeAll();
  }
  hasBeenFiltered = true;

  for (Test** p = Test::getRoot(); *p != nullptr; p = (*p)->getNext()) {
    if ((*p)->getName().hasSubstring(substring)) {
      (*p)->setLifeCycle(lifeCycle);
    }
  }
}

void TestRunner::excludeAll() {
  for (Test** p = Test::getRoot(); *p != nullptr; p = (*p)->getNext()) {
    (*p)->setLifeCycle(Test::kLifeCycleExcluded);
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

namespace {

/**
 * Print the timeMillis as floating point seconds, without using floating point
 * math. This is the equivalent of 'printer->print((float) timeMillis / 1000)',
 * but saves 1400-1600 bytes of flash memory and 12 bytes of static memory.
 */
void printSeconds(Print* printer, unsigned long timeMillis) {
  int s = timeMillis / 1000;
  int ms = timeMillis % 1000;
  printer->print(s);
  printer->print('.');
  if (ms < 100) printer->print('0');
  if (ms < 10) printer->print('0');
  printer->print(ms);
}

}

void TestRunner::printStartRunner() const {
  if (!isVerbosity(Verbosity::kTestRunSummary)) return;

  Print* printer = Printer::getPrinter();
  printer->print(F("TestRunner started on "));
  printer->print(mCount);
  printer->println(F(" test(s)."));
}

void TestRunner::resolveRun() const {
  if (!isVerbosity(Verbosity::kTestRunSummary)) return;
  Print* printer = Printer::getPrinter();

  unsigned long elapsedTime = mEndTime - mStartTime;
  printer->print(F("TestRunner duration: "));
  printSeconds(printer, elapsedTime);
  printer->println(" seconds.");

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
}

void TestRunner::setRunnerTimeout(TimeoutType timeout) {
  mTimeout = timeout;
}

//----------------------------------------------------------------------------
// Command line argument processing on EpoxyDuino
//----------------------------------------------------------------------------

#if EPOXY_DUINO
static void shift(int& argc, const char* const*& argv) {
  argc--;
  argv++;
}

static bool argEquals(const char* s, const char* t) {
  return strcmp(s, t) == 0;
}

static void usageAndExit(int status) {
  fprintf(
    stderr,
    "Usage: %s [--help|-h]\n"
      "   [--include pattern,...] [--exclude pattern,...]\n"
      "   [--includesub substring,...] [--excludesub substring,...]\n"
      "   [--] [substring ...]\n",
    epoxy_argv[0]
  );
  exit(status);
}

void TestRunner::processCommaList(
    const char* const commaList, FilterType filterType) {

  const int kArgumentSize = 64;
  const char* list = commaList;

  // Loop for each comma-separate list of words. Call the filtering command
  // defined by the filterType.
  char argument[kArgumentSize];
  while (*list != '\0') {
    const char* comma = strchr(list, ',');
    int length = (comma) ? (comma - list) : strlen(list);

    // Copy each word of the comma-separated list.
    if (length >= kArgumentSize - 1) {
      length = kArgumentSize - 1;
    }
    memcpy(argument, list, length);
    argument[length] = '\0';

    switch (filterType) {
      case FilterType::kInclude:
        include(argument);
        break;
      case FilterType::kExclude:
        exclude(argument);
        break;
      case FilterType::kIncludeSub:
        includesub(argument);
        break;
      case FilterType::kExcludeSub:
        excludesub(argument);
        break;
    }

    list += (comma) ? length + 1 : length;
  }
}

/**
 * Parse command line flags.
 * Returns the index of the first argument after the flags.
 */
int TestRunner::parseFlags(int argc, const char* const* argv) {
  int argc_original = argc;
  shift(argc, argv);
  while (argc > 0) {
    if (argEquals(argv[0], "--include")) {
      shift(argc, argv);
      if (argc == 0) usageAndExit(1);
      processCommaList(argv[0], FilterType::kInclude);
    } else if (argEquals(argv[0], "--exclude")) {
      shift(argc, argv);
      if (argc == 0) usageAndExit(1);
      processCommaList(argv[0], FilterType::kExclude);
    } else if (argEquals(argv[0], "--includesub")) {
      shift(argc, argv);
      if (argc == 0) usageAndExit(1);
      processCommaList(argv[0], FilterType::kIncludeSub);
    } else if (argEquals(argv[0], "--excludesub")) {
      shift(argc, argv);
      if (argc == 0) usageAndExit(1);
      processCommaList(argv[0], FilterType::kExcludeSub);
    } else if (argEquals(argv[0], "--")) {
      shift(argc, argv);
      break;
    } else if (argEquals(argv[0], "--help") || argEquals(argv[0], "-h")) {
      usageAndExit(0);
      break;
    } else if (argv[0][0] == '-') {
      fprintf(stderr, "Unknonwn flag '%s'\n", argv[0]);
      usageAndExit(1);
    } else {
      break;
    }
    shift(argc, argv);
  }

  return argc_original - argc;
}

void TestRunner::processCommandLine() {
  int args = parseFlags(epoxy_argc, epoxy_argv);

  // Process any remaining *space*-separated arguments using includesub().
  for (int i = args; i < epoxy_argc; i++) {
    includesub(epoxy_argv[i]);
  }
}

#endif

}
