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

#include "TestRunner.h" // seems like a circular reference but ok from cpp file
#include "Assertion.h"

namespace aunit {


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

  if (!Assertion::isOutputEnabled(ok)) return;

  // Don't use F() strings here because flash memory strings are not deduped by
  // the compiler, so each template instantiation of this method causes a
  // duplication of all the strings below. See
  // https://github.com/mmurdoch/arduinounit/issues/70
  // for more info.
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

bool Assertion::isOutputEnabled(bool ok) {
  return (ok && TestRunner::isVerbosity(Verbosity::kAssertionPassed)) ||
      (!ok && TestRunner::isVerbosity(Verbosity::kAssertionFailed));
}

bool Assertion::assertion(const char* file, uint16_t line, bool lhs,
    const char* opName, bool (*op)(bool lhs, bool rhs),
    bool rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, char lhs,
    const char* opName, bool (*op)(char lhs, char rhs),
    char rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, int lhs,
    const char* opName, bool (*op)(int lhs, int rhs),
    int rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, unsigned int lhs,
    const char* opName, bool (*op)(unsigned int lhs, unsigned int rhs),
    unsigned int rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, long lhs,
    const char* opName, bool (*op)(long lhs, long rhs),
    long rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, unsigned long lhs,
    const char* opName, bool (*op)(unsigned long lhs, unsigned long rhs),
    unsigned long rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, double lhs,
    const char* opName, bool (*op)(double lhs, double rhs),
    double rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const char* lhs,
    const char* opName, bool (*op)(const char* lhs, const char* rhs),
    const char* rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const char* lhs,
    const char *opName, bool (*op)(const char* lhs, const String& rhs),
    const String& rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const char* lhs,
    const char *opName,
    bool (*op)(const char* lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName, bool (*op)(const String& lhs, const char* rhs),
    const char* rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName, bool (*op)(const String& lhs, const String& rhs),
    const String& rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line, const String& lhs,
    const char *opName,
    bool (*op)(const String& lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const char* rhs),
    const char* rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const String& rhs),
    const String& rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

bool Assertion::assertion(const char* file, uint16_t line,
    const __FlashStringHelper* lhs, const char *opName,
    bool (*op)(const __FlashStringHelper* lhs, const __FlashStringHelper* rhs),
    const __FlashStringHelper* rhs) {
  bool ok = op(lhs, rhs);
  printAssertionMessage(ok, file, line, lhs, opName, rhs);
  TestRunner::setPassOrFail(ok);
  return ok;
}

}
