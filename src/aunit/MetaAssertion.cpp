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
const char MetaAssertion::kMessageNotPassed[] PROGMEM = "not passed";
const char MetaAssertion::kMessageNotFailed[] PROGMEM = "not failed";
const char MetaAssertion::kMessageSkipped[] PROGMEM = "skipped";
const char MetaAssertion::kMessageNotSkipped[] PROGMEM = "not skipped";
const char MetaAssertion::kMessageExpired[] PROGMEM = "timed out";
const char MetaAssertion::kMessageNotExpired[] PROGMEM = "not timed out";

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
  printer->print(FPSTR(kMessageAssertion));
  printer->print(' ');
  printer->print(FPSTR(ok ? Assertion::kMessagePassed
                          : Assertion::kMessageFailed));
  printer->print(": Test ");
  printer->print(testName);
  printer->print(" is ");
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
