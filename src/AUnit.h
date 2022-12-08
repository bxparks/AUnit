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
 * @mainpage AUnit Library
 *
 * This is the Doxygen documentation for the
 * <a href="https://github.com/bxparks/AUnit">AUnit Library</a>.
 *
 * Click on the "Classes" menu above to see the list of classes.
 *
 * Click on the "Files" menu above to see the list of header files.
 */

/**
 * @file AUnit.h
 *
 * Same as AUnitVerbose.h except that the terse versions of the various
 * assertXxx() macros are provided. These versions print only the values of the
 * parameters given in the assert macros. They do not capture the source text
 * of the assert parameters, which can reduce flash memory by 25-35%.
 */

#ifndef AUNIT_AUNIT_H
#define AUNIT_AUNIT_H

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
#include "aunit/AssertMacros.h" // terse assertXxx() macros
#include "aunit/MetaAssertMacros.h"
#include "aunit/TestMacros.h"

// Version format: xxyyzz == "xx.yy.zz"
#define AUNIT_VERSION 10700
#define AUNIT_VERSION_STRING "1.7.0"

#endif
