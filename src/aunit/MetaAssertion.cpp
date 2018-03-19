#include "Printer.h"
#include "Verbosity.h"
#include "MetaAssertion.h"

namespace aunit {

// Print a human readable string fragment from the method name:
// "isDone" -> "is done"
// "isNotDone" ->"is not done"
//
// If the method names are changed, then this mapping needs to be done in
// another way.
void printStatusString(const char* statusName) {
  for (const char* p = statusName; *p != '\0'; p++) {
    char c = *p;
    if (!isupper(c)) {
      Printer::getPrinter()->print(c);
    } else {
      Printer::getPrinter()->print(' ');
      c = tolower(c);
      Printer::getPrinter()->print(c);
    }
  }
}

void printAssertionTestStatusMessage(bool ok, const char* file, uint16_t line,
    const char* testName, const char* statusName) {
  bool isOutput =
      (ok && TestRunner::isVerbosity(Verbosity::kAssertionPassed)) ||
      (!ok && TestRunner::isVerbosity(Verbosity::kAssertionFailed));
  if (!isOutput) return;

  // TODO: Merge the common strings between this and printAssertionMesssage()
  // into PROGMEM strings manually and reused them.
  Printer::getPrinter()->print("Assertion ");
  Printer::getPrinter()->print(ok ? "passed" : "failed");
  Printer::getPrinter()->print(": Test ");
  Printer::getPrinter()->print(testName);
  Printer::getPrinter()->print(' ');
  printStatusString(statusName);
  Printer::getPrinter()->print(", file ");
  Printer::getPrinter()->print(file);
  Printer::getPrinter()->print(", line ");
  Printer::getPrinter()->print(line);
  Printer::getPrinter()->println('.');
}

bool assertionTestStatus(const char* file, uint16_t line, 
    const char* testName, const char* statusName, bool ok) {
  printAssertionTestStatusMessage(ok, file, line, testName, statusName);
  TestRunner::setPassOrFail(ok);
  return ok;
}

}
