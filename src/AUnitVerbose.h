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

/**
 * @file AUnitVerbose.h
 *
 * Same as AUnit.h except that the verbose versions of the various assertXxx()
 * macros are provided. These capture the strings of the actual arguments in
 * the assert macros and print more verbose and helpful messages in the same
 * format used by ArduinoUnit. The cost is 20-25% increase in flash memory to
 * hold those strings for medium to large unit tests.
 */

#ifndef AUNIT_AUNIT_VERBOSE_H
#define AUNIT_AUNIT_VERBOSE_H

// Blacklist boards using new Arduino API due to incompatibilities.
#if defined(ARDUINO_API_VERSION)
#error Platforms using ArduinoCore-API not supported
#endif

#include "aunit/print64.h"
#include "aunit/Verbosity.h"
#include "aunit/Compare.h"
#include "aunit/Printer.h"
#include "aunit/Test.h"
#include "aunit/Assertion.h"
#include "aunit/MetaAssertion.h"
#include "aunit/TestOnce.h"
#include "aunit/TestAgain.h"
#include "aunit/TestRunner.h"
#include "aunit/AssertVerboseMacros.h" // verbose assertXxx() macros
#include "aunit/MetaAssertMacros.h"
#include "aunit/TestMacros.h"

// Version format: xxyyzz == "xx.yy.zz"
#define AUNIT_VERSION 10700
#define AUNIT_VERSION_STRING "1.7.0"

#endif
