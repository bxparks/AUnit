#include "Printer.h"
#include "Verbosity.h"
#include "Compare.h"
#include "MetaAssertion.h"

namespace aunit {

// Print a human readable string fragment from the method name:
// "isDone" -> "is done"
// "isNotDone" ->"is not done"
// ...
// "isExpired" ->"is timed out" (*)
// "isNotExpired" ->"is not timed out" (*)
//
// (*) - The internal "expired" state is consistently known as "timed out"
// externally. Doing a string compare is a bit inefficient in terms of CPU
// cycle, but these are unit tests, we don't need to be super fast.
//
// I can think of 2 alternatives to do this internal to external mapping:
// 1) provide the human-readable mapping in the various assertTestXxx()
// macros,
// 2) maybe use a pointer to member functions and do a switch on that here.
// Either of those seems more complex than this little hack.
void printStatusString(const char* statusName) {
  const char* p;
  if (compareEqual(statusName, F("isExpired"))) {
    p = "isTimedOut"; // can't use F() here without a lot of work
  } else if (compareEqual(statusName, F("isNotExpired"))) {
    p = "isNotTimedOut"; // can't use F() here without a lot of work
  } else {
    p = statusName;
  }

  for (; *p != '\0'; p++) {
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
  // into PROGMEM strings manually and reused them. It's not too bad even with
  // these c-strings, because the compiler will dedupe them.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(": Test ");
  printer->print(testName);
  printer->print(' ');
  printStatusString(statusName);
  printer->print(", file ");
  printer->print(file);
  printer->print(", line ");
  printer->print(line);
  printer->println('.');
}

bool assertionTestStatus(const char* file, uint16_t line, 
    const char* testName, const char* statusName, bool ok) {
  printAssertionTestStatusMessage(ok, file, line, testName, statusName);
  TestRunner::setPassOrFail(ok);
  return ok;
}

}
