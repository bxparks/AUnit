#include "Printer.h"
#include "Verbosity.h"
#include "Compare.h"
#include "TestRunner.h"
#include "MetaAssertion.h"

namespace aunit {

// Moving these strings into PROGMEM saves 162 bytes of flash memory (from
// elimination of a function) and 130 bytes of static memory,
//
// TODO: Move these into the Assertion class in a future refactoring.
const char MetaAssertion::kMessageIsDone[] PROGMEM = "is done";
const char MetaAssertion::kMessageIsNotDone[] PROGMEM = "is not done";
const char MetaAssertion::kMessageIsPassed[] PROGMEM = "is passed";
const char MetaAssertion::kMessageIsNotPassed[] PROGMEM = "is not passed";
const char MetaAssertion::kMessageIsFailed[] PROGMEM = "is failed";
const char MetaAssertion::kMessageIsNotFailed[] PROGMEM = "is not failed";
const char MetaAssertion::kMessageIsSkipped[] PROGMEM = "is skipped";
const char MetaAssertion::kMessageIsNotSkipped[] PROGMEM = "is not skipped";
const char MetaAssertion::kMessageIsExpired[] PROGMEM = "is timed out";
const char MetaAssertion::kMessageIsNotExpired[] PROGMEM = "is not timed out";

void MetaAssertion::printAssertionTestStatusMessage(
    bool ok, const char* file, uint16_t line,
    const char* testName, const __FlashStringHelper* statusMessage) {
  bool isOutput =
      (ok && TestRunner::isVerbosity(Verbosity::kAssertionPassed)) ||
      (!ok && TestRunner::isVerbosity(Verbosity::kAssertionFailed));
  if (!isOutput) return;

  // TODO: Merge the common strings between this and printAssertionMesssage()
  // into PROGMEM strings manually and reused them. It's not too bad even with
  // these c-strings, because the compiler will dedupe them.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(": Test ");
  printer->print(testName);
  printer->print(' ');
  printer->print(statusMessage);
  printer->print(", file ");
  printer->print(file);
  printer->print(", line ");
  printer->print(line);
  printer->println('.');
}

bool MetaAssertion::assertionTestStatus(const char* file, uint16_t line,
    const char* testName, const __FlashStringHelper* statusMessage, bool ok) {
  printAssertionTestStatusMessage(ok, file, line, testName, statusMessage);
  TestRunner::setPassOrFail(ok);
  return ok;
}

}
