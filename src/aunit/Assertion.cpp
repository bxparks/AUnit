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
#include "Assertion.h"

namespace aunit {

namespace {

// This can be a template function because it is accessed only through the
// various assertXxx() methods. Those assertXxx() methods are explicitly
// overloaded for the various types that we want to support.
//
// Prints something like the following:
// Assertion failed: (5) == (6), file Test.ino, line 820.
// Assertion passed: (6) == (6), file Test.ino, line 820.
template <typename A, typename B>
void printAssertionMessage(bool ok, const char* file, uint16_t line,
    const A& lhs, const char *opName, const B& rhs) {

  // Don't use F() strings here because flash memory strings are not deduped by
  // the compiler, so each template instantiation of this method causes a
  // duplication of all the strings below. See
  // https://github.com/mmurdoch/arduinounit/issues/70
  // for more info. Normal (const char*) strings will be deduped by the
  // compiler/linker.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(": (");
  printer->print(lhs);
  printer->print(") ");
  printer->print(opName);
  printer->print(" (");
  printer->print(rhs);
  printer->print(')');
  // reuse string in MataAssertion::printAssertionTestStatusMessage()
  printer->print(", file ");
  printer->print(file);
  printer->print(", line ");
  printer->print(line);
  printer->println('.');
}

// Special version of (bool, bool) because Arduino Print.h converts
// bool into int, which prints out "(1) == (0)", which isn't as useful.
// This prints "(true) == (false)".
void printAssertionMessage(bool ok, const char* file, uint16_t line,
    bool lhs, const char *opName, bool rhs) {

  // Don't use F() strings here. Same reason as above.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(": (");
  printer->print(lhs ? "true" : "false");
  printer->print(") ");
  printer->print(opName);
  printer->print(" (");
  printer->print(rhs ? "true" : "false");
  printer->print(')');
  printer->print(", file ");
  printer->print(file);
  printer->print(", line ");
  printer->print(line);
  printer->println('.');
}

// Special version for assertTrue(arg) and assertFalse(arg).
// Prints:
//    "Assertion passed/failed: (arg) is true"
//    "Assertion passed/failed: (arg) is false"
void printAssertionBoolMessage(bool ok, const char* file, uint16_t line,
    bool arg, bool value) {

  // Don't use F() strings here. Same reason as above.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(": (");
  printer->print(arg ? "true" : "false");
  printer->print(") is ");
  printer->print(value ? "true" : "false");
  printer->print(", file ");
  printer->print(file);
  printer->print(", line ");
  printer->print(line);
  printer->println('.');
}

} // namespace

bool Assertion::isOutputEnabled(bool ok) {
  return (ok && isVerbosity(Verbosity::kAssertionPassed)) ||
      (!ok && isVerbosity(Verbosity::kAssertionFailed));
}

bool Assertion::assertionBool(const char* file, uint16_t line, bool arg,
    bool value) {
  if (isDone()) return false;
  bool ok = (arg == value);
  if (isOutputEnabled(ok)) {
    printAssertionBoolMessage(ok, file, line, arg, value);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, bool lhs,
    const char* opName, bool (*op)(bool lhs, bool rhs),
    bool rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, char lhs,
    const char* opName, bool (*op)(char lhs, char rhs),
    char rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, int lhs,
    const char* opName, bool (*op)(int lhs, int rhs),
    int rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, unsigned int lhs,
    const char* opName, bool (*op)(unsigned int lhs, unsigned int rhs),
    unsigned int rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, long lhs,
    const char* opName, bool (*op)(long lhs, long rhs),
    long rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, unsigned long lhs,
    const char* opName, bool (*op)(unsigned long lhs, unsigned long rhs),
    unsigned long rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, double lhs,
    const char* opName, bool (*op)(double lhs, double rhs),
    double rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const char* lhs,
    const char* opName, bool (*op)(const char* lhs, const char* rhs),
    const char* rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const char* lhs,
    const char *opName, bool (*op)(const char* lhs, const String& rhs),
    const String& rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const char* lhs,
    const char *opName,
    bool (*op)(const char* lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName, bool (*op)(const String& lhs, const char* rhs),
    const char* rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName, bool (*op)(const String& lhs, const String& rhs),
    const String& rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName,
    bool (*op)(const String& lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const char* rhs),
    const char* rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const String& rhs),
    const String& rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessage(ok, file, line, lhs, opName, rhs);
  }
  setPassOrFail(ok);
  return ok;
}

namespace {

// Verbose versions of above which accept the string arguments of the
// assertXxx() macros, so that the error messages are more verbose.
//
// Prints something like the following:
// Assertion failed: (x=5) == (y=6), file Test.ino, line 820.
// Assertion passed: (x=6) == (y=6), file Test.ino, line 820.
template <typename A, typename B>
void printAssertionMessageVerbose(bool ok, const char* file,
    uint16_t line, const A& lhs, const __FlashStringHelper* lhsString,
    const char *opName, const B& rhs, const __FlashStringHelper* rhsString) {

  // Don't use F() strings here because flash memory strings are not deduped by
  // the compiler, so each template instantiation of this method causes a
  // duplication of all the strings below. See
  // https://github.com/mmurdoch/arduinounit/issues/70
  // for more info.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(": (");
  printer->print(lhsString);
  printer->print('=');
  printer->print(lhs);
  printer->print(") ");
  printer->print(opName);
  printer->print(" (");
  printer->print(rhsString);
  printer->print('=');
  printer->print(rhs);
  printer->print(')');
  // reuse string in MataAssertion::printAssertionTestStatusMessage()
  printer->print(", file ");
  printer->print(file);
  printer->print(", line ");
  printer->print(line);
  printer->println('.');
}

// Special version of (bool, bool) because Arduino Print.h converts
// bool into int, which prints out "(1) == (0)", which isn't as useful.
// This prints "(x=true) == (y=false)".
void printAssertionMessageVerbose(bool ok, const char* file,
    uint16_t line, bool lhs, const __FlashStringHelper* lhsString,
    const char *opName, bool rhs, const __FlashStringHelper* rhsString) {

  // Don't use F() strings here. Same reason as above.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(": (");
  printer->print(lhsString);
  printer->print('=');
  printer->print(lhs ? "true" : "false");
  printer->print(") ");
  printer->print(opName);
  printer->print(" (");
  printer->print(rhsString);
  printer->print('=');
  printer->print(rhs ? "true" : "false");
  printer->print(')');
  printer->print(", file ");
  printer->print(file);
  printer->print(", line ");
  printer->print(line);
  printer->println('.');
}

// Special version for assertTrue(arg) and assertFalse(arg).
// Prints:
//    "Assertion passed/failed: (x=arg) is true"
//    "Assertion passed/failed: (x=arg) is false"
void printAssertionBoolMessageVerbose(bool ok, const char* file,
    uint16_t line, bool arg, const __FlashStringHelper* argString, bool value) {

  // Don't use F() strings here. Same reason as above.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
  printer->print(": (");
  printer->print(argString);
  printer->print('=');
  printer->print(arg ? "true" : "false");
  printer->print(") is ");
  printer->print(value ? "true" : "false");
  printer->print(", file ");
  printer->print(file);
  printer->print(", line ");
  printer->print(line);
  printer->println('.');
}

} // namespace

bool Assertion::assertionBoolVerbose(const char* file, uint16_t line, bool arg,
    const __FlashStringHelper* argString, bool value) {
  if (isDone()) return false;
  bool ok = (arg == value);
  if (isOutputEnabled(ok)) {
    printAssertionBoolMessageVerbose(ok, file, line, arg, argString, value);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line, bool lhs,
    const __FlashStringHelper* lhsString, const char* opName,
    bool (*op)(bool lhs, bool rhs), bool rhs,
    const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line, char lhs,
    const __FlashStringHelper* lhsString, const char* opName,
    bool (*op)(char lhs, char rhs), char rhs,
    const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line, int lhs,
    const __FlashStringHelper* lhsString, const char* opName,
    bool (*op)(int lhs, int rhs), int rhs,
    const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    unsigned int lhs, const __FlashStringHelper* lhsString, const char* opName,
    bool (*op)(unsigned int lhs, unsigned int rhs),
    unsigned int rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line, long lhs,
    const __FlashStringHelper* lhsString, const char* opName,
    bool (*op)(long lhs, long rhs), long rhs,
    const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    unsigned long lhs, const __FlashStringHelper* lhsString, const char* opName,
    bool (*op)(unsigned long lhs, unsigned long rhs),
    unsigned long rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line, double lhs,
    const __FlashStringHelper* lhsString, const char* opName,
    bool (*op)(double lhs, double rhs), double rhs,
    const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const char* lhs, const __FlashStringHelper* lhsString, const char* opName,
    bool (*op)(const char* lhs, const char* rhs),
    const char* rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const char* lhs, const __FlashStringHelper* lhsString,
    const char *opName, bool (*op)(const char* lhs, const String& rhs),
    const String& rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const char* lhs, const __FlashStringHelper* lhsString, const char *opName,
    bool (*op)(const char* lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const String& lhs, const __FlashStringHelper* lhsString, const char *opName,
    bool (*op)(const String& lhs, const char* rhs),
    const char* rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const String& lhs, const __FlashStringHelper* lhsString, const char *opName,
    bool (*op)(const String& lhs, const String& rhs),
    const String& rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const String& lhs, const __FlashStringHelper* lhsString, const char *opName,
    bool (*op)(const String& lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const __FlashStringHelper* lhsString,
    const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const char* rhs),
    const char* rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const __FlashStringHelper* lhsString,
    const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const String& rhs),
    const String& rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

bool Assertion::assertionVerbose(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const __FlashStringHelper* lhsString,
    const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs, const __FlashStringHelper* rhsString) {
  if (isDone()) return false;
  bool ok = op(lhs, rhs);
  if (isOutputEnabled(ok)) {
    printAssertionMessageVerbose(ok, file, line, lhs, lhsString, opName, rhs,
        rhsString);
  }
  setPassOrFail(ok);
  return ok;
}

}
