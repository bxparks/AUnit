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

#include <Arduino.h>  // definition of Print
#include "Flash.h"
#include "Printer.h"
#include "Verbosity.h"
#include "Compare.h"
#include "TestRunner.h"
#include "MetaAssertion.h"

namespace aunit {

// Moving these strings into PROGMEM saves 162 bytes of flash memory (from
// elimination of a function) and 130 bytes of static memory,
const char MetaAssertion::kMessageDone[] PROGMEM = "done";
const char MetaAssertion::kMessageNotDone[] PROGMEM = "not done";
const char MetaAssertion::kMessagePassed[] PROGMEM = "passed";
const char MetaAssertion::kMessageNotPassed[] PROGMEM = "not passed";
const char MetaAssertion::kMessageFailed[] PROGMEM = "failed";
const char MetaAssertion::kMessageNotFailed[] PROGMEM = "not failed";
const char MetaAssertion::kMessageSkipped[] PROGMEM = "skipped";
const char MetaAssertion::kMessageNotSkipped[] PROGMEM = "not skipped";
const char MetaAssertion::kMessageExpired[] PROGMEM = "timed out";
const char MetaAssertion::kMessageNotExpired[] PROGMEM = "not timed out";

namespace {

// Print an assertion message describing whether the given 'testName' has passed
// or failed
void printAssertionTestStatusMessage(
    bool ok, const char* file, uint16_t line,
    const char* testName, const __FlashStringHelper* statusMessage) {
  // Many of the following strings are duplicated in Assertion.cpp and
  // the compiler/linker will dedupe them.
  Print* printer = Printer::getPrinter();
  printer->print(file);
  printer->print(':');
  printer->print(line);
  printer->print(": Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(F(": Test "));
  printer->print(testName);
  printer->print(" is ");
  printer->print(statusMessage);
  printer->println('.');
}

}

bool MetaAssertion::assertionTestStatus(const char* file, uint16_t line,
    const char* testName, const __FlashStringHelper* statusMessage, bool ok) {
  if (isDone()) return false;
  if (isOutputEnabled(ok)) {
    printAssertionTestStatusMessage(ok, file, line, testName, statusMessage);
  }
  setPassOrFail(ok);
  return ok;
}

namespace {

// Print message for failNow() macro.
// "{file}:{line}: Status failed."
void printStatusNowMessage(const char* file, uint16_t line,
    const __FlashStringHelper* statusString) {
  // Many of these strings are duplicated in Assertion.cpp and will be deduped
  // by the compiler/linker.
  Print* printer = Printer::getPrinter();
  printer->print(file);
  printer->print(':');
  printer->print(line);
  printer->print(F(": Status "));
  printer->print(statusString);
  printer->println('.');
}

}

bool MetaAssertion::isOutputEnabledForStatus(uint8_t status) const {
  return (status == kStatusFailed && isVerbosity(Verbosity::kTestFailed))
      || (status == kStatusPassed && isVerbosity(Verbosity::kTestPassed))
      || (status == kStatusSkipped && isVerbosity(Verbosity::kTestSkipped))
      || (status == kStatusExpired && isVerbosity(Verbosity::kTestExpired));
}

void MetaAssertion::setStatusNow(const char* file, uint16_t line,
    uint8_t status, const __FlashStringHelper* statusString) {
  if (isDone()) return;
  if (isOutputEnabledForStatus(status)) {
    printStatusNowMessage(file, line, statusString);
  }
  setStatus(status);
}

}
