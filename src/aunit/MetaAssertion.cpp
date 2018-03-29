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

#ifdef ESP8266
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

#include <Arduino.h>  // definition of Print
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

void MetaAssertion::printAssertionTestStatusMessage(
    bool ok, const char* file, uint16_t line,
    const char* testName, const __FlashStringHelper* statusMessage) {
  // Trying to move these strings into PROGMEM actually makes the flash memory
  // consumption bigger. The compile/linker will dedupe these c-strings.
  Print* printer = Printer::getPrinter();
  printer->print("Assertion ");
  printer->print(ok ? "passed" : "failed");
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
  if (isOutputEnabled(ok)) {
    printAssertionTestStatusMessage(ok, file, line, testName, statusMessage);
  }
  TestRunner::setPassOrFail(ok);
  return ok;
}

}
