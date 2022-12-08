# AUnit

[![AUnit Tests](https://github.com/bxparks/AUnit/actions/workflows/aunit_tests.yml/badge.svg)](https://github.com/bxparks/AUnit/actions/workflows/aunit_tests.yml)

A unit testing framework for Arduino platforms inspired by by
[ArduinoUnit](https://github.com/mmurdoch/arduinounit) and [Google
Test](https://github.com/google/googletest/). The unit tests usually run on the
embedded controller which allows detection of architecture-specific problems.
But for faster development, many unit tests can be compiled and executed
natively on Linux or MacOS using the
[EpoxyDuino](https://github.com/bxparks/EpoxyDuino) companion project.

AUnit is almost a drop-in replacement of ArduinoUnit with some advantages. AUnit
supports timeouts and test fixtures. It sometimes consumes 50% less flash memory
on the AVR platform, and it has been tested to work on the AVR, SAMD21, STM32,
ESP8266, ESP32 and Teensy platforms. Another companion project
[AUniter](https://github.com/bxparks/AUniter) project provides command line
tools to verify, upload and validate the unit tests to the microcontroller,
instead of having to go through the Arduino IDE. Both the AUniter and
EpoxyDuino tools can be used in a continuous integration system like Jenkins,
or with [GitHub Actions](https://github.com/features/actions).

**Version**: 1.7.0 (2022-12-08)

**Changelog**: [CHANGELOG.md](CHANGELOG.md)

## Table of Contents

* [Summary](#Summary)
    * [ArduinoUnit Compatible Features](#ArduinoUnitCompatible)
    * [Missing Features](#MissingFeatures)
    * [Added Features](#AddedFeatures)
* [Installation](#Installation)
    * [Source Code](#SourceCode)
* [Documentation](#Documentation)
    * [Examples](#Examples)
* [Usage](#Usage)
    * [Header and Namespace](#HeaderAndNamespace)
    * [Verbose Mode](#VerboseMode)
    * [Defining the Tests](#DefiningTests)
    * [Generated Class and Instance Names](#GeneratedClass)
    * [Binary Assertions](#BinaryAssertions)
        * [Assertion Message Format](#AssertionMessageFormat)
        * [Supported Parameter Types](#SupportedParameterTypes)
        * [Parameter Types Must Match](#ParameterTypesMustMatch)
        * [Pointer Comparisons](#PointerComparisons)
        * [Case Insensitive String Comparisons](#CaseInsensitiveStrings)
    * [Approximate Comparisons](#ApproximateComparisons)
    * [Boolean Assertions](#BooleanAssertions)
    * [Test Fixtures](#TestFixtures)
    * [Early Return and Delayed Assertions](#EarlyReturnDelayedAssertions)
    * [Meta Assertions](#MetaAssertions)
    * [Unconditional Termination](#UnconditionalTermination)
    * [Overridable Methods](#OverridableMethods)
    * [Running the Tests](#RunningTests)
    * [Filtering Test Cases](#FilteringTestCases)
    * [Output Printer](#OutputPrinter)
    * [Controlling Verbosity](#ControllingVerbosity)
    * [Line Number Mismatch](#LineNumberMismatch)
    * [Test Framework Messages](#TestFrameworkMessages)
        * [Assertion Message](#AssertionMessage)
        * [Verbose Mode Message](#VerboseModeMessage)
        * [Test Case Summary](#TestCaseSummary)
        * [Test Runner Summary](#TestRunnerSummary)
    * [Test Timeout](#TestTimeout)
* [GoogleTest Adapter](#GoogleTestAdapter)
* [Command Line Tools](#CommandLineTools)
    * [AUniter](#AUniter)
    * [EpoxyDuino](#EpoxyDuino)
    * [Command Line Flags and Arguments](#CommandLineFlagsAndArguments)
* [Continuous Integration](#ContinuousIntegration)
    * [Arduino IDE/CLI + Cloud](#IdePlusCloud)
    * [Arduino IDE/CLI + Jenkins](#IdePlusJenkins)
    * [EpoxyDuino + Jenkins](#EpoxyDuinoPlusJenkins)
    * [EpoxyDuino + Cloud (Recommended)](#EpoxyDuinoPlusCloud)
* [Tips](#Tips)
    * [Debugging Assertions in Fixtures](#DebuggingFixtures)
    * [Class Hierarchy](#ClassHierarchy)
    * [Testing Private Helper Methods](#PrivateHelperMethods)
* [Benchmarks](#Benchmarks)
* [System Requirements](#SystemRequirements)
    * [Hardware](#Hardware)
    * [Tool Chain](#ToolChains)
    * [Operating System](#ToolChains)
* [License](#License)
* [Feedback and Support](#FeedbackAndSupport)
* [Authors](#Authors)

<a name="Summary"></a>
## Summary

AUnit was created to solve 3 problems with ArduinoUnit 2.2:
* ArduinoUnit consumes too much flash memory on an AVR platform (e.g.
  Arduino UNO, Nano) as explained in
  [ArduinoUnit#70](https://github.com/mmurdoch/arduinounit/issues/70).
* ArduinoUnit does not compile on the ESP8266 platform (see
  [ArduinoUnit#68](https://github.com/mmurdoch/arduinounit/issues/68),
  [ArduinoUnit#55](https://github.com/mmurdoch/arduinounit/issues/55),
  [ArduinoUnit#54](https://github.com/mmurdoch/arduinounit/issues/54)).
* ArduinoUnit does not provide an easy way to create tests using fixtures,
  equivalent to the `TEST_F()` macro in Google Test.

In contrast:
* AUnit consumes as much as 65% *less* flash memory than ArduinoUnit 2.2 on the
  AVR platform. On Teensy-ARM, the savings can be as much as 30%.
* AUnit has been tested on AVR, Teensy-ARM and ESP8266.
* AUnit implements the `testF()` and `testingF()` macros to use fixtures.

The [AUniter](https://github.com/bxparks/AUniter) command line tools can compile
the unit tests, upload them to Arduino boards attached to the serial ports of
the local machine, and validate the output of the AUnit test runner. In
addition, the AUniter script can be integrated into a
[Jenkins](https://jenkins.io) continuous integration service running on the
local machine, and the unit tests can be monitored automatically.

Unit tests written using AUnit can often be compiled and executed natively on
Linux or MacOS using the
[EpoxyDuino](https://github.com/bxparks/EpoxyDuino) library. The output on
the `Serial` object is redirected to the `stdout` of the Unix host. This
provides another avenue for implementing continuous builds or integration.

<a name="ArduinoUnitCompatible"></a>
### ArduinoUnit Compatible Features

For basic unit tests written using ArduinoUnit 2.2, only two changes are
required to convert to AUnit:
* `#include <ArduinoUnit.h>` -> `#include <AUnit.h>`
* `Test::run()` -> `aunit::TestRunner::run()`

Most of the core macros are compatible between ArduinoUnit and AUnit:
* `test()`
* `testing()`
* `assertXxx()`
* Meta Assertions
    * `checkTestXxx()`
    * `assertTestXxx()`
* `externTest()`
* `externTesting()`

AUnit also supports exclude and include filters:
* `TestRunner::exclude()`
* `TestRunner::include()`

Filters can be accessed through
[Command Line Flags](##CommandLineFlagsAndArguments) on desktop machines using
EpoxyDuino

The various assertion and test status messages can be enabled or disabled using
the `Verbosity` flags on per test basis:
* `enableVerbosity()`
* `disableVerbosity()`

<a name="MissingFeatures"></a>
### Missing Features

Here are the features which have *not* been ported over from ArduinoUnit 2.2:

* ArduinoUnit supports multiple `*` wildcards in its `exclude()` and `include()`
  methods. AUnit supports only a single `*` wildcard and it must occur at the
  end if present.

<a name="AddedFeatures"></a>
### Added Features

Here are the features in AUnit which are not available in ArduinoUnit 2.2:

* Configurable timeout parameter to prevent `testing()` test cases from
  running forever:
    * `TestRunner::setTimeout(seconds)`
    * `Test::expire()`
    * `assertTestExpire()`
    * `assertTestNotExpire()`
    * `checkTestExpire()`
    * `checkTestNotExpire()`
* Case-insensitive string comparisons:
    * `assertStringCaseEqual()`
    * `assertStringCaseNotEqual()`
* Approximate comparisons:
    * `assertNear()`
    * `asssertNotNear()`
* Supports 64-bit integer
    * `assertXxx()` support both `long long` and `unsigned long long`
* `test()` and `testing()` macros support both 1 and 2 arguments
    * `test(testName)` and `test(suiteName, testName)`
    * `testing(testName)` and `testing(suiteName, testName)`
* Test fixtures using the "F" variations of existing macros:
    * `testF()`
    * `testingF()`
    * `assertTestXxxF()`
    * `checkTestXxxF()`
    * `externTestF()`
    * `externTestingF()`
* Unconditional termination:
    * `passTestNow()`
    * `failTestNow()`
    * `skipTestNow()`
    * `expireTestNow()`
* `teardown()` method which mirrors the `setup()` method:
    * `teardown()`
* Test filters support 2-arguments, matching `testF()` and `testingF()`:
    * `TestRunner::include(testClass, name)`
    * `TestRunner::exclude(testClass, name)`
* No dynamic allocation of heap memory:
    * The library does not create a `String` object.
    * It reads from a `String` if the calling code provides it.
* Terse and verbose modes:
    * `#include <AUnit.h>` - terse messages use less flash memory
    * `#include <AUnitVerbose.h>` - verbose messages use more flash memory

Every feature of AUnit is unit tested using AUnit itself.

<a name="Installation"></a>
## Installation

The latest stable release is available in the Arduino IDE Library Manager.
Search for "unit test" or "AUnit", select "AUnit", then click the "Install"
button.

The development version can be installed by cloning the
[GitHub repository](https://github.com/bxparks/AUnit), checking out the
`develop` branch, then manually copying over the contents to the `./libraries`
directory used by the Arduino IDE. (The result is a directory named
`./libraries/AUnit`.) See the Preferences menu for the location of the Arduino
sketch directory. The `master` branch contains the stable release.

Using either installation method, you may need to restart the Arduino IDE to
pick up the new library.

<a name="SourceCode"></a>
### Source Code

The source files are organized as follows:
* `src/AUnit.h` - main header file
* `src/AUnitVerbose.h` - verbose version of main header file
* `src/aunit/` - all implementation files
* `tests/` - unit tests written using AUnit itself
* `examples/` - example sketches

<a name="Documentation"></a>
## Documentation

* [README.md](README.md) - this file
* [Doxygen docs](https://bxparks.github.io/AUnit/html) published on GitHub
  Pages can help navigate the classes and header files

<a name="Examples"></a>
### Examples

The `examples/` directory has a number of examples:

* `advanced` - how to subclass `Test` and `TestOnce` manually
* `basic` - using the `test()` macro
* `continuous` - using the `testing()` macro
* `filter` - how to filter tests using `TestRunner::include()` and
  `TestRunner::exclude()`
* `fixture` - how to use the `testF()` macro with test fixtures
* `meta_asserts` - how to use `assertTestXxx()` and `checkTestXxx()`

In the `tests/` directory:

* `AUnitTest` - the unit test for core `AUnit` functions,
* `AUnitMetaTest` - the unit test for meta assertions and `extern*()` macros
* `FilterTest` - manual tests for `include()` and `exclude()` filters
* `SetupAndTeardownTest` - tests to verify that `setup()` and `teardown()` are
  called properly by the finite state machine

Perhaps the best way to see AUnit in action through real life examples. All my
libraries use AUnit for testing and for continuous integration through
EpoxyDuino. Here are some examples:

* [AceButton](https://github.com/bxparks/AceButton)
    * My first Arduino library, which originally used ArduinoUnit 2.2.
    * I kept many of the original ArduinoUnit tests for backwards compatibility
      testing. But over time, I started to use nore AUnit features.
* [AceCRC](https://github.com/bxparks/AceCRC)
* [AceCommon](https://github.com/bxparks/AceCommon)
* [AceRoutine](https://github.com/bxparks/AceRoutine)
* [AceSegment](https://github.com/bxparks/AceSegment)
* [AceSorting](https://github.com/bxparks/AceSorting)
* [AceTimeClock](https://github.com/bxparks/AceTimeClock)
* [AceTime](https://github.com/bxparks/AceTime)

<a name="Usage"></a>
## Usage

In this section, information about differences between AUnit and ArduinoUnit
will appear in a note marked by ***ArduinoUnit Compatibility***.

<a name="HeaderAndNamespace"></a>
### Header and Namespace

To prevent name clashes with other libraries and code, all classes in the AUnit
library are defined in the `aunit` namespace. The user will mostly interact with
the `TestRunner` class. It can be referenced with an explicit namespace
qualifier (i.e. `aunit::TestRunner`), or we can use a `using` directive like
this:
```C++
#include <AUnit.h>
using aunit::TestRunner;
```
or we can import the entire `aunit` namespace:
```C++
#include <AUnit.h>
using namespace aunit;
```

Similar to ArduinoUnit, many of the "functions" in this framework (e.g.
`test()`, `testing()`, `assertXxx()`) are defined as `#define` macros which live
in the global namespace, so it is usually not necessary to import the entire
`aunit` namespace.

<a name="VerboseMode"></a>
### Verbose Mode

By default, AUnit generates terse assertion messages by leaving out
the string arguments of the various `assertXxx()` macros. If you would like
to get the same verbose output as ArduinoUnit, use the following header
instead:

```C++
#include <AUnitVerbose.h>
```

The flash memory consumption on an 8-bit AVR may go up by 20-25% for medium to
large tests. On Teensy ARM, ESP8266 or ESP32, the increased memory size probably
does not matter too much because these microcontrollers have far more flash and
static memory.

<a name="DefiningTests"></a>
### Defining the Tests

The usage of **AUnit** is basically identical to **ArduinoUnit**. The following
macros are used to create a test:

* `test(name) {...}` - creates a subclass of `TestOnce`
* `testing(name) {...}` - creates a subclass of `TestAgain`
* `test(suiteName, name) {...}` - creates a subclass of `TestOnce`
* `testing(suiteName, name) {...}` - creates a subclass of `TestAgain`
* `testF(classname, name) {...}` - creates a subclass of `classname`
* `testingF(classname, name) {...}` - creates a subclass of `classname`

The code in `{ }` following these macros becomes the body of a method in a
subclass derived from the base class indicated above. The `test()` and `testF()`
macros place the code body into the `TestOnce::once()` method. The `testing()`
and `testingF()` macros place the code body into the `TestAgain::again()`
method.

The `test()` and `testing()` macros support 1 or 2 arguments. The one-argument
version is inherited from ArduinoUnit. The two-argument version is
analogous to the `TEST()` macro in GoogleTest, where the `suiteName` can
be used to organize multiple tests into a collection of similar tests. The
grouping is purely in the naming scheme of the generated code, there is no
functional relationship between these tests.

During static initialization, the constructor of the object adds itself to an
internal list. The root of that list is given by `Test::getRoot()`. The
`TestRunner::run()` method traverses the linked list, executing each test case
until it passes, fails or is skipped.

Here is a rough outline of an AUnit unit test sketch:

```C++
#line 2 AUnitTest.ino

#include <AUnit.h>
using namespace aunit;

test(example) {
  ...
  assertXxx(...)
  ...
}

test(ExampleTest, example) {
  ...
}

testing(looping) {
  ...code...
  if (...) {
    pass();
  } else if (...) {
    failTestNow();
  } else {
    skipTestNow();
  }
}

testing(LoopingTest, looping) {
  ...
}

class CustomTestOnce: public TestOnce {
  protected:
    // optional
    void setup() override {
      TestOnce::setup();
      ...setup code...
    }

    // optional
    void teardown() override {
      ...teardown code...
      TestOnce::teardown();
    }

    void assertBigStuff() {
      ...higher level assertions...
    }
};

testF(CustomTestOnce, example_test) {
  ...
  assertBigStuff();
  ...
}

class CustomTestAgain: public TestAgain {
  protected:
    // optional
    void setup() override {
      TestAgain::setup();
      ...setup code...
    }

    // optional
    void teardown() override {
      ...teardown code...
      TestAgain::teardown();
    }

    void assertBigStuff() {
      ...various assertions...
    }
};

testingF(CustomTestAgain, example_test) {
  ...
  assertBigStuff();
  ...
}

void setup() {
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro

  TestRunner::exclude("*");
  TestRunner::include("looping*");
  TestRunner::include("CustomTestAgain", "example*");
}

void loop() {
  TestRunner::run();
}
```

***ArduinoUnit Compatibility***: _The basic structure of the unit test is
identical to ArduinoUnit. AUnit adds the `testF()` and `testingF`() macros,
and the two-argument versions of `test()` and `testing()` which
are not available in ArduinoUnit. The `Test` class in ArduinoUnit has been
replaced with the `TestAgain` class in AUnit._

<a name="GeneratedClass"></a>
### Generated Class and Instance Names

The arguments to the various `test*()` macros are used to generate the name for
the subclasses of `TestOnce` or `TestAgain`, and generate the names of the
instances of those classes. For reference, here are the rules:

* `test(name)`
    * class: `"test_"` + name
    * instance: `"test_"` + name + `"_instance"`
* `testing(name)`
    * class: `"test_"` + name
    * instance: `"test_"` + name + `"_instance"`
* `test(suiteName, name)`
    * class: `suiteName` + `"_"` + name
    * instance: `suiteName` + `"_"` + name + `"_instance"`
* `testing(suiteName, name)`
    * class: `suiteName` + `"_"` + name
    * instance: `suiteName` + `"_"` + name + `"_instance"`
* `testF(className, name)`
    * class: `className` + `"_"` + name
    * instance: `className` + `"_"` + name + `"_instance"`
* `testingF(className, name)`
    * class: `className` + `"_"` + name
    * instance: `className` + `"_"` + name + `"_instance"`

The instance name is available within the test code using the `Test::getName()`
method.

<a name="BinaryAssertions"></a>
### Binary Assertions

Inside the `test()` and `testing()` macros, the following assertions
are available. These are essentially identical to ArduinoUnit:

* `assertEqual(a, b)`
* `assertNotEqual(a, b)`
* `assertLess(a, b)`
* `assertMore(a, b)`
* `assertLessOrEqual(a, b)`
* `assertMoreOrEqual(a, b)`

<a name="AssertionMessageFormat"></a>
#### Assertion Message Format

When the assertion passes, nothing is printed by default. This can be controlled
by the `TestRunner::setVerbosity()` method. See [Controlling
Verbosity](#ControllingVerbosity).

When the assertion fails, an error message of the following format is printed:

```
SampleTest.ino:10: Assertion failed: (2) == (1)
```

The format of the assertion failure messages was changed in v1.7 to the
following:
```
{filName}:{lineNumber}: Assertion failed: {expression}
```

This format is a widely used in many other programs, for example, the C compiler
`gcc`, the C++ compiler `g++`, the Python 3 interpreter `python3`, `grep`, and
the GNU Make program `make`. In particular, the
[quickfix](https://vimhelp.org/quickfix.txt.html) feature in the `vim` text
editor can parse this error format and jump directly to the `fileName` and
`lineNumber` indicated by the error message. See the instructions in
[EpoxyDuino](https://github.com/bxparks/EpoxyDuino) to see how to run unit tests
on a Linux or MacOS machine inside the `vim` editor so that the editor jumps
directly to the files and line numbers where the assertion failure occurred.

<a name="SupportedParameterTypes"></a>
#### Supported Parameter Types

The 6 core assert macros (`assertEqual()`, `assertNotEqual()`, `assertLess()`,
`assertMore()`, `assertLessOrEqual()`, `assertMoreOrEqual()`) support the
following 18 combinations for their parameter types:

* `(bool, bool)`
* `(char, char)`
* `(int, int)`
* `(unsigned int, unsigned int)`
* `(long, long)`
* `(unsigned long, unsigned long)`
* `(long long, long long)`
* `(unsigned long long, unsigned long long)`
* `(double, double)`
* `(const char *, const char *)`
* `(const char *, const String&)`
* `(const char *, const __FlashStringHelper*)`
* `(const String&, const char*)`
* `(const String&, const String&)`
* `(const String&, const __FlashStringHelper*)`
* `(const __FlashStringHelper*, const char*)`
* `(const __FlashStringHelper*, const String&)`
* `(const __FlashStringHelper*, const __FlashStringHelper*)`

The `assertEqual()` and `assertNotEqual()` support arbitrary pointer types
through implicit casts to `const void*`:

* `(const void*, const void*)` (since v1.4)

All 9 combinations of the 3 string types (`char*`, `String`, and
`__FlashStringHelper*`) are supported.

These macros perform deep comparisons for string types instead of just comparing
their pointer values. This is different than the `ASSERT_EQ()` and `ASSERT_NE()`
macros in Google Test which perform only pointer comparisons. In other words,
`assertEqual()` with string types is equivalent to `ASSERT_STREQ()` in Google
Test.

Also for string types, these macros support `nullptr` (unlike the underlying
`strcmp()` function from the C-library). The `nullptr` string is defined to be
"smaller" than any non-null string, including the empty string. Two `nullptr`
strings are considered to be equal however.

Additionally, the usual C++ implicit type conversion and function overloading
matching algorithms apply to support additional argument types.
For example, the following type conversions will occur:

* `signed char` -> `int`
* `unsigned char` -> `int`
* `short` -> `int`
* `unsigned short` -> `int` or `unsigned int` (depending on `sizeof(int)`)
* `char*` -> `const char*`.
* `char[N]` -> `const char*`
* `float` -> `double`
* pointer types -> `const void*`

Note that `char`, `signed char`, and `unsigned char` are 3 distinct types in
C++, so a `(char, char)` will match exactly to one of the `assertXxx()`
methods.

***ArduinoUnit Compatibility***:
_The names of the macros are identical. However, the
type inference logic of two `(a, b)` arguments in the `assertXxx(a, b)` is
slightly different. ArduinoUnit allows the two parameters to be slightly
different types, at the expense of a compiler warning. In AUnit, the
warning becomes a compiler error. See the "Parameters Must Match Types" section
below._

<a name="ParameterTypesMustMatch"></a>
#### Parameter Types Must Match

In ArduinoUnit, the `assertXxx()` macros could be slightly different types, for
example:
```C++
unsigned int uintValue = 5;
assertEqual(5, uintValue);
```

If the compiler warnings are enabled in the Preferences box of the
IDE, a warning from the compiler is printed:

```
../ArduinoUnit/src/ArduinoUnitUtility/Compare.h:17:28: warning:
    comparison between signed and unsigned integer expressions [-Wsign-compare]
    return (!(a<b)) && (!(b<a));
```

In AUnit, the above code produces a compiler error (not a warning) like this:
```
.../AUnit/src/aunit/Assertion.h:29:66: error: call of overloaded
    'assertion(const char [14], int, int, const char[3], <unresolved overloaded
    function type>, unsigned int&)' is ambiguous
    if (!aunit::assertion(__FILE__,__LINE__,(arg1),opName,op,(arg2)))\
...
```
The compiler cannot find an appropriate overloaded version of `assertEqual()`.

The solution is to make the parameters the same type:
```C++
assertEqual(5U, uintValue);
```

On the AVR platform, both a (short) and (int) are 16-bit types, so the following
will produce a compiler error:
```C++
unsigned short ushortValue = 5;
assertEqual(5U, ushortValue);
```
But on Teensy-ARM, ESP8266, and ESP32, a 16-bit (short) can be promoted to a
32-bit (int) without loss of precision, so the above will compile just fine. For
portability, the following should be used on all platforms:
```C++
unsigned short ushortValue = 5;
assertEqual((unsigned short) 5, ushortValue);
```

The integer type promotion rules and function overload matching rules can be
difficult to remember (and sometimes difficult to understand). The best way to
avoid these compiler errors is to make sure that the assertion parameter types
are identical, potentially using explicit casting.

<a name="PointerComparisons"></a>
#### Pointer Comparisons

Version 1.4 adds pointer comparison to `assertEqual()` and `assertNotEqual()`.
Arbritary pointers are implicitly cast to a `const void*` and compared to
each other. If the assertion fails, the pointer is converted to an integer type,
and the hexadecimal value of the pointer is printed. For example,

```C++
test(voidPointer) {
  const int aa[] = {1, 2};
  const long bb[] = {1, 2};

  assertEqual(aa, bb);
}
```

This test will fail with the following error message:
```
AUnitTest.ino:338: Assertion failed: (aa=0x3FFFFF38) == (bb=0x3FFFFF30).
Test voidPointer failed.
```

Comparison against the `nullptr` will work:

```C++
test(nullPointer) {
  const int aa[] = {1, 2};
  assertEqual(aa, nullptr);
}
```

This will print the following:

```
AUnitTest.ino:348: Assertion failed: (aa=0x3FFFFF58) == (nullptr=0x0).
Test nullPointer failed.
```

Comparing a string type (i.e. `const char*`, or `const __FlashStringHelper*`)
to a `nullptr` will cause an error due to ambiguous matches on overloaded
functions. The solution is to explicitly cast the `nullptr` to the corresponding
string type:

```C+++
test(stringPointer) {
  const char aa[] = "abc";

  // assertEqual(aa, nullptr); // Causes errors

  assertEqual(aa, (const char*) nullptr); // Works.
}
```

<a name="CaseInsensitiveStrings"></a>
#### Case Insensitive String Comparisons

Two macros provide case-insensitive string comparisons (analogous to
`ASSERT_STRCASEEQ()` and `ASSERT_STRCASENE()` in Google Test):

* `assertStringCaseEqual(a, b)`
* `assertStringCaseNotEqual(a, b)`

The supported types for `(a, b)` are all 9 combinations of Arduino string types:

* `(const char *, const char *)`
* `(const char *, const String&)`
* `(const char *, const __FlashStringHelper*)`
* `(const String&, const char*)`
* `(const String&, const String&)`
* `(const String&, const __FlashStringHelper*)`
* `(const __FlashStringHelper*, const char*)`
* `(const __FlashStringHelper*, const String&)`
* `(const __FlashStringHelper*, const __FlashStringHelper*)`

***ArduinoUnit Compatibility***: _Not available in ArduinoUnit._

<a name="ApproximateComparisons"></a>
### Approximate Comparisons

Floating point values are difficult to compare because of internal rounding
errors. Google Test provides
[two types of macros to handle floating points](https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#floating-point-comparison):
* `ASSERT_FLOAT_EQ(a, b)`, `ASSERT_DOUBLE_EQ(a, b)` - determine if the
  floating point numbers are within 4
  [Units in the Last Place (ULPs)](https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/)
* `ASSERT_NEAR(a, b, error)` - determine if the absolute distance between
  `a` and `b` is within the given `error`

Since floating point operations are relatively rare in Arduino programming,
AUnit offers only the equivalent of `ASSERT_NEAR()` function:

* `assertNear(a, b, error)`
* `assertNotNear(a, b, error)`

Upon failure, the error messages will look something like:
```
AUnitTest.ino:517: Assertion failed: |(1.00) - (1.10)| > (0.20).
AUnitTest.ino:527: Assertion failed: |(4.00) - (1.10)| <= (0.20).
```

Unlike Google Test where `ASSERT_NEAR()` supports only the `double` type, both
`assertNear()` and `assertNotNear()` support integral types as well. The full
list of supported types is:

* `int`
* `unsigned int`
* `long`
* `unsigned long`
* `double`

Other primitive types (e.g. `char` or `float`) will be automatically converted
to one of these supported types by the compiler. (64-bit integer types
`long long` and `unsigned long long` are not supported here.)

Note that the `abs()` of 2 values of a signed integer type can be larger than
the maximum value that can be represented by the given signed type. Since signed
integer overflow is an
[undefined behavior](https://stackoverflow.com/questions/16188263/is-signed-integer-overflow-still-undefined-behavior-in-c)
in C and C++11, I cannot predict what the compiler will do in that case.
Unsigned types should not have this problem because the distance between two
values of an unsigned type should always fit inside the given unsigned type.
Technically, a similar problem exists for the floating point types (which are
naturally signed), but it is unlikely that you are dealing with floating point
values so close to the maximum values.

<a name="BooleanAssertions"></a>
### Boolean Assertions

The following boolean asserts are also available:

* `assertTrue(condition)`
* `assertFalse(condition)`

***ArduinoUnit Compatibility***: _These are identical to ArduinoUnit._

<a name="TestFixtures"></a>
### Test Fixtures

When the unit tests become more complex, using test fixtures will allow you to
place common data objects and methods into a class that can be shared among
multiple test cases. This concept matches very closely to the test fixtures
in
[Google Test](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md).

To create a test fixture:
1. Derives a new class from either `TestOnce` (if you want to run the test just
   once), or `TestAgain` (if you want to run the test repeatedly).
1. Add any data objects inside the class.
1. Optionally add a `virtual void setup() {...}` method to perform any common
   initialization code. Be sure to call the parent's `setup()` method in the
   *first* line to chain any `setup()` methods defined by the parents. There may
   be multiple parent classes.
1. Optionally add a `virtual void teardown() {...}` method to perform any common
   clean up code. Be sure to call the parent's `teardown()` method in the *last*
   line to chain any `teardown()` methods defined by the parents. There may be
   multiple parent classes.
1. Add any additional shared methods into this new class.

To define your tests, use the `testF()` macro like this:
```C++
class CustomTestOnce: public TestOnce {
  protected:
    // optional
    void setup() override {
      TestOnce::setup();
      ...setup code...
    }

    // optional
    void teardown() override {
      ...teardown code...
      TestOnce::teardown();
    }

    void assertCustomStuff() {
      ...common code...
    }

    int sharedValue;
};

testF(CustomTestOnce, calculate) {
  ...test code here...
}
```
No constructor for `CustomTestOnce` needs to be defined.

Underneath the covers, the `testF()` macro creates a subclass named
`CustomTestOnce_calculate` which inherits from `TestOnce` class. The test code
becomes the body of the `CustomTestOnce_calculate::once()` method. The name of
this test has the class name prepended, so it is `CustomTestOnce_calculate`,
which prevents name collision with other `testF()` tests with the same
name using a different test fixture class.

To define a continuous test, use the `testingF()` macro like this:
```C++
class CustomTestAgain: public TestAgain {
  protected:
    // optional
    void setup() override {
      TestAgain::setup();
      ...setup code...
    }

    // optional
    void teardown() override {
      ...teardown code...
      TestAgain::teardown();
    }

    void assertCustomStuff() {
      ...common code...
    }

    int sharedValue;
};

testingF(CustomTestAgain, calculate) {
  ...test code here...
}
```

Similarly, the `testingF()` macro creates a subclass named
`CustomTestAgain_calculate`, and the test code becomes the body of the
`CustomTestAgain_calculate::again()` method.

See `examples/fixtures/fixtures.ino` to see a working example of the `testF()`
macro.

***ArduinoUnit Compatibility***: _The `testF()` and `testingF()` macros,
and the `teardown()` virtual method are available only in AUnit (and Google
Test), not ArduinoUnit._

<a name="EarlyReturnDelayedAssertions"></a>
### Early Return and Delayed Assertions

AUnit (like ArduinoUnit and Google Test) does not use C++ exceptions. Instead,
the various `assertXxx()` macros perform an early `return` if the condition
evaluates to `false`. That means that the assertions can only bail out of the
current method, not the calling method. If you are using test fixtures, and
create a shared custom assert function, e.g. the `assertCustomStuff()` method
above, any `assertXxx()` statements in shared method will bail out of that
method only. The statement after the `assertCustomStuff()` will continue to
execute.

In other words, in the following example, if the `assertCustomStuff()` fails,
then `assertMoreStuff()` inside `testF()` will execute:

```C++
class CustomTestOnce: public TestOnce {
  protected:
    void assertCustomStuff() {
      assertEqual(sharedValue, 3);

      // This will not execute if the assertEqual() above fails.
      assertLess(...);
    }

    void assertMoreStuff() {
      assertEqual(...);
    }

    int sharedValue;
};

// DON'T DO THIS
testF(CustomTestOnce, dontDoThis) {
  assertCustomStuff();

  // This will execute even if assertCustomStuff() fails.
  assertMoreStuff();
}

// DO THIS INSTEAD
testF(CustomTestOnce, doThis) {
  assertNoFatalFailure(assertCustomStuff());
  assertNoFatalFailure(assertMoreStuff());
}
```

The solution is to use the `assertNoFatalFailure(statement)` macro which checks
whether the inner `statement` returned with a fatal assertion. If so, then it
returns immediately, preventing execution from continuing to the code that
follows. This macro is modeled after the
[ASSERT_NO_FATAL_FAILURE(statement)](https://github.com/google/googletest/blob/master/docs/advanced.md)
macro in Google Test that provides the same functionality.

<a name="MetaAssertions"></a>
### Meta Assertions

The following methods from ArduinoUnit have also been implemented:

* `checkTestDone(name)`
* `checkTestNotDone(name)`
* `checkTestPass(name)`
* `checkTestNotPass(name)`
* `checkTestFail(name)`
* `checkTestNotFail(name)`
* `checkTestSkip(name)`
* `checkTestNotSkip(name)`
* `checkTestExpire(name)` [&ast;]
* `checkTestNotExpire(name)` [&ast;]
* `assertTestDone(name)`
* `assertTestNotDone(name)`
* `assertTestPass(name)`
* `assertTestNotPass(name)`
* `assertTestFail(name)`
* `assertTestNotFail(name)`
* `assertTestSkip(name)`
* `assertTestNotSkip(name)`
* `assertTestExpire(name)` [&ast;]
* `assertTestNotExpire(name)` [&ast;]

The `checkTestXxx()` methods check the status of the test named `name`
and returns a `bool`. The execution continues even if `false`.

The `assertTestXxx()` methods stops the unit test if the status check
returns `false`, and prints assertion messages that look like this:
```
AUnitTest.ino:366: Assertion passed: Test slow_pass is done.
AUnitTest.ino:372: Assertion passed: Test slow_pass is not failed.
AUnitTest.ino:448: Assertion passed: Test slow_skip is skipped.
AUnitTest.ino:451: Assertion passed: Test slow_skip is not timed out.
```
(The human readable version of being `expired` will always be `timed out` or
`not timed out` on the `Serial` output.)

The following macros define `extern` references to test case objects which live
in other `.cpp` files. These are required for the above meta assertions if the
test cases are defined in another file:

* `externTest()`
* `externTesting()`
* `externTestF()`
* `externTestingF()`

***ArduinoUnit Compatibility***: _The methods marked by [&ast;] are only
available in AUnit. Also, the assertion messages are different. ArduinoUnit
reuses the format used by the `assertXxx()` macros, so prints something like
the following:_
```
AUnitTest.ino:439: Assertion passed: (test_slow_skip_instance.state=2) >= (Test::DONE_SKIP=2).
```

_AUnit has a separate message handler to print a customized message for the
assertTestXxx() meta assertion macros._

<a name="UnconditionalTermination"></a>
### Unconditional Termination

The following macros can be used inside the body of `test()` or `testing()`
macro to terminate a test unconditionally. Each macro prints a short message,
and returns immediately from the test, much like an `assertXxx()` macro that
fails.

* `passTestNow()`  [&ast;]
* `failTestNow()` [&ast;]
* `skipTestNow()` [&ast;]
* `expireTestNow()` [&ast;]

The messages look like:
```
AUnitTest.ino:360: Status passed.
AUnitTest.ino:378: Status failed.
AUnitTest.ino:380: Status skipped.
AUnitTest.ino:391: Status timed out.
```

The following methods on the `Test` class also set the `status` of the test, but
these methods do not print any messages (which makes debugging difficult) and
they do *not* terminate the test immediately.

* `pass()` - test passed
* `fail()` - test failed
* `skip()` - test skipped
* `expire()`  - test timed out [&ast;]

In most cases, the `failTestNow()`, `skipTestNow()` and `expireTestNow()` macros
are more useful than the equivalent methods in the `Test` class. However, in a
`testing()` loop test, the `pass()` method is probably better than the
`passTestNow()` macro because we usually don't want to see an error message
from a passing test.

***ArduinoUnit Compatibility***:
_The method(s) marked by [&ast;] are only available in AUnit._

<a name="OverridableMethods"></a>
### Overridable Methods

The following methods are defined at the `Test` base class level:

* `setup()`
* `teardown()`

The `TestOnce` class defines:
* `once()`

The `TestAgain` class defines:
* `again()`

***ArduinoUnit Compatibility***: _These are functionally the same as ArduinoUnit
except with different class names. Instead of `Test` use `TestAgain`. Instead
of `Test::loop` use `TestAgain::again()`. ArduinoUnit does not support a
`teardown()` method._

<a name="RunningTests"></a>
### Running the Tests

We run the test cases in the global `loop()` method by calling
`TestRunner::run()`. The tests are sorted according to the name of the test
given in the argument in the `test()` or `testing()` macro.

Each call to the `run()` method causes one test case to run and be resolved. The
next call to `run()` executes the next test case. This design allows the
`loop()` method to perform a small amount of work and return periodically to
allow the system to perform some actions. On some systems, such as the ESP8266,
an error is generated if `loop()` takes too much CPU time.

```C++
...
void loop() {
  TestRunner::run();
}
```

***ArduinoUnit Compatibility***: _This is equivalent to called `Test::run()` in
ArduinoUnit. AUnit sorts the tests in the same way as ArduinoUnit. In
ArduinoUnit, each call to `Test::run()` will process the entire list of
currently active test cases. In AUnit, each call to `TestRunner::run()` performs
only a single test case, then returns._

<a name="FilteringTestCases"></a>
### Filtering Test Cases

Six filtering methods are available on the `TestRunner` class:
* `TestRunner::include(pattern)` - prefix match
* `TestRunner::include(testClass, pattern)` - prefix match
* `TestRunner::exclude(pattern)` - prefix match
* `TestRunner::exclude(testClass, pattern)` - prefix match
* `TestRunner::includesub(substring)` - substring match (v1.6)
* `TestRunner::excludesub(substring)` - substring match (v1.6)

These methods are called from the global `setup()` method, for example:

```C++
void setup() {
  TestRunner::include("looping*");
  TestRunner::exclude("CustomTestAgain", "*");
  TestRunner::include("CustomTestAgain", "test*");
  TestRunner::include("CustomTestAgain", "test*");
  TestRunner::includesub("net");
  TestRunner::excludesub("net");
  ...
}
```

Excluded tests bypass their `Test::setup()` and `Test::teardown()` methods and
terminate immediately. For the purposes of reporting, excluded tests are counted
as "skipped".

The 2-argument versions of `include()` and `exclude()` correspond to the
2 arguments of `testF()` and `testingF()`.

The filtering methods are also available as command line flags and arguments
(`--include`, `--exclude`, `--includesub` `--excludesub`) if the test
program is compiled using EpoxyDuino under a Unix-like environment. See
the [EpoxyDuino](#EpoxyDuino) section below.

**Implicit Exclude All**: If the *first* filtering request is an "include" (i.e.
`include(pattern)`, `include(testClass, pattern)`, `includesub(substring)`),
all tests are excluded by default initially, instead of being included by
default. Otherwise, the first "include" statement would have no effect.

***ArduinoUnit Compatibility***:
_The equivalent versions in ArduinoUnit are `Test::exclude()` and
`Test::include()` The matching algorithm in AUnit is not as powerful as the one
in ArduinoUnit. AUnit supports only a single wildcard character `*` and that
character can appear only at the end if it is present. For example, the
following are accepted:_

* `TestRunner::exclude("*");`
* `TestRunner::include("f*");`
* `TestRunner::exclude("flash_*");`
* `TestRunner::include("looping*");`
* `TestRunner::include("CustomTestOnce", "flashTest*");`

_AUnit provides 2-argument versions of `include()` and `exclude()`_

<a name="OutputPrinter"></a>
### Output Printer

The default output printer is the `Serial` instance. This can be
changed using the `TestRunner::setPrinter()` method:

```C++
#include <AUnit.h>
using aunit::TestRunner;
...

void setup() {
  Serial1.begin(...);

  TestRunner::setPrinter(&Serial1);
  ...
}

void loop() {
  TestRunner::run();
}
```

***ArduinoUnit Compatibility***:
_This is the equivalent of the `Test::out` static member variable in
ArduinoUnit._

<a name="ControllingVerbosity"></a>
### Controlling the Verbosity

The default verbosity of the test results can be controlled using the
`TestRunner::setVerbosity()` method:
```C++
#include <AUnit.h>
using aunit::TestRunner;
using aunit::Verbosity;
...
void setup() {
  ...
  TestRunner::setVerbosity(Verbosity::kAll);
  ...
}
```

Every test is assigned this default verbosity just before its `Test::setup()`
is called. A unit test can choose to modify the verbosity calling one of the
following methods:

* `void enableVerbosity(uint8_t verbosity);`
    * enables the given verbosity, retaining all the others
* `void disableVerbosity(uint8_t verbosity);`
    * disables the given verbosity, retaining all the others

at the beginning of the test definition, like this:

```C++
test(enable_assertion_passed_messages) {
  enableVerbosity(Verbosity::kAssertionPassed);
  ...
}
```

The values of `verbosity` are defined by the static constants of the `Verbosity`
utility class:

* `Verbosity::kAssertionPassed`
* `Verbosity::kAssertionFailed`
* `Verbosity::kTestPassed`
* `Verbosity::kTestFailed`
* `Verbosity::kTestSkipped`
* `Verbosity::kTestExpired`
* `Verbosity::kTestRunSummary`
* `Verbosity::kAssertionAll` - enables all assert messages
* `Verbosity::kTestAll`
    * same as `(kTestPassed | kTestFailed | kTestSkipped | kTestExpired)`
* `Verbosity::kDefault`
    * same as `(kAssertionFailed | kTestAll | kTestRunSummary )`
* `Verbosity::kAll` - enables all messages
* `Verbosity::kNone` - disables all messages

***ArduinoUnit Compatibility***:
_The following ArduinoUnit variables do not exist:_
* `Test::min_verbosity`
* `Test::max_verbosity`

_The bit field constants have slightly different names:_

* `TEST_VERBOSITY_TESTS_SUMMARY` -> `Verbosity::kTestRunSummary`
* `TEST_VERBOSITY_TESTS_FAILED` -> `Verbosity::kTestFailed`
* `TEST_VERBOSITY_TESTS_PASSED` -> `Verbosity::kTestPassed`
* `TEST_VERBOSITY_TESTS_SKIPPED` -> `Verbosity::kTestSkipped`
* `TEST_VERBOSITY_TESTS_ALL` -> `Verbosity::kTestAll`
* `TEST_VERBOSITY_ASSERTIONS_FAILED` -> `Verbosity::kAssertionFailed`
* `TEST_VERBOSITY_ASSERTIONS_PASSED` -> `Verbosity::kAssertionPassed`
* `TEST_VERBOSITY_ASSERTIONS_ALL` -> `Verbosity::kAssertionAll`
* `TEST_VERBOSITY_ALL` -> `Verbosity::kAll`
* `TEST_VERBOSITY_NONE` -> `Verbosity::kNone`
* {no equivalent} <- `Verbosity::kDefault`
* {no equivalent} <- `Verbosity::kTestExpired`

<a name="LineNumberMismatch"></a>
### Line Number Mismatch

AUnit suffers from the same compiler/preprocessor bug as ArduinoUnit that causes
the built-in `__LINE__` macro to be off by one. The solution is to add:
```C++
#line 2 {file.ino}
```
as the first line of a unit test sketch.

***ArduinoUnit Compatibility***: _This problem is identical to ArduinoUnit._

<a name="TestFrameworkMessages"></a>
### Test Framework Messages

<a name="AssertionMessage"></a>
#### Assertion Message

The various `assertXxx()` macros in AUnit print a message upon pass or fail. For
example, if the assertion was:
```C++
int expected = 3;
int counter = 4;
assertEquals(expected, counter);
```

The error message (if enabled, which is the default) is:
```
AUnitTest.ino:134: Assertion failed: (3) == (4).
```

Asserts with `bool` values produce customized messages, printing "true" or
"false" instead of using the Print class default conversion to `int`:
```C++
assertEquals(true, false);

AUnitTest.ino:134: Assertion failed: (true) == (false).
```

Similarly, the `assertTrue()` and `assertFalse()` macros provide more customized
messages:
```C++
bool ok = false;
assertTrue(ok);

AUnitTest.ino:134: Assertion failed: (false) is true.
```

and
```C++
bool ok = true;
assertFalse(ok);

AUnitTest.ino:134: Assertion failed: (true) is false.
```

***ArduinoUnit Compatibility***:
_ArduinoUnit captures the arguments of the `assertEqual()` macro
and prints:_

```
AUnitTest.ino:134: Assertion failed: (expected=3) == (counter=4).
```

_Each capture of the parameter string consumes flash memory space. If the unit
test has numerous `assertXxx()` statements, the flash memory cost is expensive.
AUnit omits the parameters to reduce flash memory space by about 33%._

_The messages for asserts with bool values are customized for better clarity
(partially to compensate for the lack of capture of the string of the actual
arguments, and are different from ArduinoUnit._

<a name="VerboseModeMessage"></a>
#### Verbose Mode Message

If you use the verbose header:
```C++
#include <AUnitVerbose.h>
```
the assertion message will contain the string fragments of the arguments
passed into the `assertXxx()` macros, like this:

```
AUnitTest.ino:134: Assertion failed: (expected=3) == (counter=4).
AUnitTest.ino:134: Assertion failed: (ok=false) is true.
```

instead of:

```
AUnitTest.ino:134: Assertion failed: (3) == (4).
AUnitTest.ino:134: Assertion failed: (false) is true.
```

***ArduinoUnit Compatibility***:
_As of v1.7, the assertion message format is compatible with the vim editor
and other Linux/MacOS/Unix tools, and no longer compatible with ArduinoUnit

<a name="TestCaseSummary"></a>
#### Test Case Summary

As each test case finishes, the `TestRunner` prints out the summary of the test
case like this:

```
Test bad failed.
Test looping_pass passed.
Test looping_skip skipped.
Test looping_until timed out.
```

***ArduinoUnit Compatibility***: _These are identifcal to ArduinoUnit,
except that the "timed out" status is new to AUnit. See Test Timeout
section below._

<a name="TestRunnerSummary"></a>
#### Test Runner Summary

At the end of the test run, the `TestRunner` prints out the summary
of all test cases, like this:

```
TestRunner duration: 0.05 seconds.
TestRunner summary: 12 passed, 0 failed, 2 skipped, 1 timed out, out of 15 test(s).
```

***ArduinoUnit Compatibility***: _The message format is slightly different than
ArduinoUnit. I changed "Test summary" to "TestRunner summary" because the former
looks identical to the message that could have been printed by a `test(summary)`
test case. AUnit also adds information about tests which timed out. See below._

<a name="TestTimeout"></a>
### Test Timeout

***ArduinoUnit Compatibility***: _Only available in AUnit._

From my experience, it seems incredibly easy to write a `testing()` test case
which accidentally runs forever because the code forgets to call an explicit
`pass()`, `fail()` or `skip()`.

The `TestRunner` in AUnit applies a timeout value to all the test cases that it
runs. The **default timeout is 10 seconds**. A timeout value of `0` means an
**infinite** timeout, which means that the `testing()` test case may run
forever. The value of the timeout is stored as a `uint16_t` type, so the maximum
timeout is 65535 seconds or a bit over 18 hours.

Currently, the timeout value is global to all test cases. Time out values for
individual tests cannot be set independently. If a test does not finish before
that time, then the test is marked as `timed out` (internally implemented by the
`Test::expire()` method) and a message is printed like this:
```
Test looping_until timed out.
```

The timeout value can be changed by calling the static
`TestRunner::setTimeout()` method. Here is an example that sets the timeout to
30 seconds instead:
```C++
void setup() {
  ...
  TestRunner::setTimeout(30);
  ...
}
```

***ArduinoUnit Compatibility***: _Only available in AUnit._

<a name="GoogleTestAdapter"></a>
## GoogleTest Adapter

It may be possible to run simple unit tests written using
[Google Test](https://github.com/google/googletest/) API on an Arduino platform
by using the
[aunit/contrib/gtest.h](src/aunit/contrib/gtest.h) adapter. This
adapter layer provides a number of Google Test macros which map to
their equivalent macros in AUnit:

* `ASSERT_EQ(e, a)` - `assertEqual()`
* `ASSERT_NE(e, a)` - `assertNotEqual()`
* `ASSERT_LT(e, a)` - `assertLess()`
* `ASSERT_GT(e, a)` - `assertMore()`
* `ASSERT_LE(e, a)` - `assertLessOrEqual()`
* `ASSERT_GE(e, a)` - `assertMoreOrEqual()`
* `ASSERT_STREQ(e, a)` - `assertEqual()`
* `ASSERT_STRNE(e, a)` - `assertNotEqual()`
* `ASSERT_STRCASEEQ(e, a)` - `assertStringCaseEqual()`
* `ASSERT_STRCASENE(e, a)` - `assertStringCaseNotEqual()`
* `ASSERT_TRUE(x)` - `assertTrue()`
* `ASSERT_FALSE(x)` - `assertFalse()`
* `ASSERT_NEAR(e, a, error)` - `assertNear()`

To use the `gtest.h` adapter, include the following headers:
```C++
#include <AUnit.h>
#include <aunit/contrib/gtest.h>
```

or

```C++
#include <AUnitVerbose.h>
#include <aunit/contrib/gtest.h>
```

<a name="CommandLineTools"></a>
## Command Line Tools

Each unit test is an independent `*.ino` program. You can run it using your
Arduino IDE. But there are 2 command line tools that can be used to run them.

<a name="AUniter"></a>
### AUniter

The `auniter.sh` script used to be part of this project, but now lives in
its own AUniter (https://github.com/bxparks/AUniter) project. The `auniter.sh`
script is a wrapper around:
* [Arduino IDE in command line mode](https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc), and
* [Arduino CLI tool](https://github.com/arduino/arduino-cli)

Using `auniter.sh`, you can compile, upload and validate multiple AUnit tests on
multiple Arduino boards using a single command.

The script can monitor the serial port and determine if the unit test passed or
failed, and it will print out a summary of all unit tests at the end. Full
details are given in the AUniter project, but here are some quick examples
copied from the `AUniter/README.md` file:

* `$ auniter envs`
    * list the environments configured in the `auniter.ini` config file
* `$ auniter ports`
    * list the available serial ports and devices
* `$ auniter verify nano Blink.ino`
    * verify (compile) `Blink.ino` using the `env:nano` environment
* `$ auniter verify nano,esp8266,esp32 Blink.ino`
    * verify `Blink.ino` on 3 target environments (`env:nano`, `env:esp8266`,
    `env:esp32`)
* `$ auniter upload nano:/dev/ttyUSB0 Blink.ino`
    * upload `Blink.ino` to the `env:nano` target environment connected to
    `/dev/ttyUSB0`
* `$ auniter test nano:USB0 BlinkTest.ino`
    * compile and upload `BlinkTest.ino` using the `env:nano` environment,
      upload it to the board at `/dev/ttyUSB0`, then validate the output of the
      [AUnit](https://github.com/bxparks/AUnit) unit test
* `$ auniter test nano:USB0,esp8266:USB1,esp32:USB2 BlinkTest/ ClockTest/`
    * upload and verify the 2 unit tests (`BlinkTest/BlinkTest.ino`,
      `ClockTest/ClockTest.ino`) on 3 target environments (`env:nano`,
      `env:esp8266`, `env:esp32`) located at the 3 respective ports
      (`/dev/ttyUSB0`, `/dev/ttyUSB1`, `/dev/ttyUSB2`)
* `$ auniter upmon nano:USB0 Blink.ino`
    * upload the `Blink.ino` sketch and monitor the serial port using a
      user-configurable terminal program (e.g. `picocom`) on `/dev/ttyUSB0`

<a name="EpoxyDuino"></a>
### EpoxyDuino

Instead of running the unit tests on the actual microcontrollers themselves, you
can compile and execute AUnit unit tests natively on Linux or MacOS machines
using the EpoxyDuino (https://github.com/bxparks/EpoxyDuino) project.
EpoxyDuino provides a minimal Arduino programming environment that is usually
sufficient to compile and run AUnit test units on the Unix host machine. It
relies on the native C++ compiler, GNU Make, and `Makefile` files for each
`*.ino` unit test like this:

```
APP_NAME := SampleTest
ARDUINO_LIBS := AUnit
include ../../../EpoxyDuino/EpoxyDuino.mk
```

The unit test is compiled into a binary (`SampleTest.out`) using the `make
command, and the binary can be executed like this:
```
$ make
$ ./SampleTest.out
```

Here are a few tips when writing unit tests to run under EpoxyDuino:

**Delay(1000)**

For real Arduino boards, you get more reliable unit tests if you add a
`delay(1000)` at the start of the program. For EpoxyDuino, this is not
necessary, so I recommend calling this only on real Arduino boards, like this:
```C++
void setup() {
#ifdef ARDUINO
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
#endif
  ...
```

**Exit() Status Code**

On real Arduino boards, the unit test (or any program for that matter) never
terminates. The `loop()` function executes forever. On Linux or MacOS using
EpoxyDuino, the test program will terminate at the end through the
`exit()` function. If the tests are successful (i.e. passing or skipped), it
will call `exit(0)`. If there are any failing tests (i.e. failed or timed out),
it will call `exit(1)`.

<a name="CommandLineFlagsAndArguments"></a>
### Command Line Flags and Arguments

(Added in v1.6)

The standard Arduino environment does not provide command line arguments, since
a microcontroller does not normally provide a command line environment. However,
if the AUnit test program is compiled under EpoxyDuino, the standard Unix
command line parameters (`argc` and `argv`)  become available through the
`extern int epoxy_argc` and `extern const char* const* epoxy_argv` global
variables. These allow the `TestRunner` class to provide command line flags and
arguments as follows:

```bash
$ ./test.out --help
Usage: ./test.out [--help] [--include pattern,...] [--exclude pattern,...]
   [--includesub substring,...] [--excludesub substring,...]
   [--] [substring ...]
```

Example, the following runs all tests with substring "net" or "led" in its
name, and skips all others:

```bash
$ ./test.out net led
```

Flags:

* `--include pattern,...`
    * Comma-separated list of patterns to pass to the
      `TestRunner::include(pattern)` method
* `--exclude pattern,...`
    * Comma-separated list of patterns to pass to the
      `TestRunner::exclude(pattern)` method
* `--includesub substring,...`
    * Comma-separated list of substrings to pass to the
      `TestRunner::includesub(substring)` method
* `--excludesub substring,...`
    * Comma-separated list of substrings to pass to the
      `TestRunner::excludesub(substring)` method

Arguments:

* Any **Space**-separated list of words after the optional flags are passed to
  the `TestRunner::includesub(substring)` method.

The command line flags and arguments are processed *after* any hardcoded calls
to `TestRunner::include()` and `TestRunner::exclude()` methods in the global
`setup()` method.

The flags and command line arguments are processed *in order* of appearance
on the command line.

Similar to the hardcoded calls to `TestRunner::include()` and
`TestRunner::exclude()`, if the first command line flag is an `--include` or
`--includesub`, then all tests are *excluded* by default initially. Otherwise,
the first include flag would have no effect.

<a name="ContinuousIntegration"></a>
## Continuous Integration

There are several ways to incorporate AUnit into a continuous integration
system. At the infrastructure level, you can use either a system like
[Jenkins](https://jenkins.io) running on a local machine, or use a cloud-based
continuous integration system like [GitHub
Actions](https://github.com/features/actions). For each of those
infrastructures, you can choose to use the Arduino IDE or CLI build tools, or
you can use EpoxyDuino to compile and run against a Linux or MacOS
environment.

The option matrix looks like this:

```
+----------------+-------------------------+------------------------+
|\ CI environment|                         |                        |
| \____________  | Local (e.g. Jenkins)    | Cloud (e.g. GitHub)    |
|              \ |                         |                        |
| Build tool    \|                         |                        |
+----------------+-------------------------+------------------------+
|                | * can execute tests     | * verify compile-only  |
|                |   on microcontroller    | * cannot actually run  |
| Arduino        | * complex setup and     |   tests on             |
| IDE/CLI        | * maintenance           |   microcontroller      |
|                | * can be slow           | * complex installation |
|                | * (not recommended)     |   and setup            |
|                |                         | * (unverified)         |
+----------------+-------------------------+------------------------+
|                | * verify execution on   | * verify execution on  |
|                |   Unix environment      |   Unix environment     |
| EpoxyDuino     | * faster than IDE/CLI   | * simple setup and     |
|                | * complex setup and     |   maintenance          |
|                |   and maintenance       | * very fast            |
|                | * (unnecessary)         | * (recommended)        |
+----------------+-------------------------+------------------------+
```

<a name="IdePlusCloud"></a>
### Arduino IDE/CLI + Cloud

The big advantage of using the Arduino IDE/CLI to run the AUnit unit tests is
that you can compile them using the precise compiler and tool chain that will be
used against the specific microcontroller that you are interested in verifying.
There can be subtle compiler differences (e.g. size of `int`) or differences in
how the Arduino programming environment was implemented (e.g. availability of
the `FPSTR()` macro). If you use a cloud-based CI infrastructure, then you must
install the complete Arduino IDE/CLI environment into your cloud test runner.
Although I think it's theoretically possible, I have never actually verified
that this can be done.

<a name="IdePlusJenkins"></a>
### Arduino IDE/CLI + Jenkins

This setup is described in [Continuous Integration with
Jenkins](https://github.com/bxparks/AUniter/tree/develop/jenkins), and it worked
reasonably well for small number of unit tests. The problem is that the Arduino
IDE is far too slow when the number of unit tests become non-trivial. And it
also takes too much effort to maintain the local Jenkins infrastructure. The
Jenkins environment seems to be brittle due to its complexity of all of its
moving parts. This is the only environment where you can connect a real Arduino
microcontroller to the local machine and have the unit tests run on the actual
microcontroller. Perhaps for certain situations, running the unit tests on
actual hardware is a requirement. But for most people, I no longer recommend
this environment.

<a name="EpoxyDuinoPlusJenkins"></a>
### EpoxyDuino + Jenkins

Once the Jenkins environment is up and running, I have verified that it is easy
to run the unit tests using EpoxyDuino, since it needs just a C++ compiler
and GNU Make. Things will compile and run a lot faster than using the Arduino
IDE/CLI. However, this combination suffers from the same problem of maintaining
the Jenkins environment. If the unit tests are running in an Unix environment
anyway, it seems far easier to just run them in the cloud. So I don't recommend
using this setup. Just use a cloud CI provider as described below.

<a name="EpoxyDuinoPlusCloud"></a>
### EpoxyDuino + Cloud (Recommended)

A cloud-based continuous integration service like [GitHub
Actions](https://github.com/features/actions) is easy to setup for
EpoxyDuino. Often the C++ compiler and GNU `make` tools are already installed
in the Docker container used by the CI system. The only additional setup is
to install EpoxyDuino, AUnit and other dependent Arduino libraries.

Here are some example YAML files for GitHub Actions:
* https://github.com/bxparks/AceTime/tree/develop/.github/workflows
* https://github.com/bxparks/AceButton/tree/develop/.github/workflows
* https://github.com/bxparks/AceRoutine/tree/develop/.github/workflows
* https://github.com/bxparks/AceCRC/tree/develop/.github/workflows

In the various `aunit_tests.yml` files, the `Setup` step installs the various
dependent libraries using the `git clone` command, for example:
```
git clone https://github.com/bxparks/EpoxyDuino
git clone https://github.com/bxparks/AUnit
```

These commands install the default branch for those repositories, which for most
of my libraries will be the `develop` branch. This makes sense for me because I
want the unit tests to run against the latest commits. However, for many others,
it is probably better to use the `master` branch because it contains the stable
releases:
```
git clone --branch master https://github.com/bxparks/EpoxyDuino
git clone --branch master https://github.com/bxparks/AUnit
```

Using EpoxyDuino with a cloud CI provider (like GitHub Actions) is my
recommended configuration for running AUnit tests because it is easy to setup
and maintain and the tests run fast.

<a name="Tips"></a>
## Tips

Collection of useful tidbits.

<a name="DebuggingFixtures"></a>
### Debugging Assertions in Fixtures

When using test fixtures with the `testF()` and `testingF()` macros, it's often
useful to create helper assertions, such as the `assertCustomStuff()` below.
Debugging such assertion statements can be tricky. I've found that turning on
messages for successful assertions (with a
`enableVerbosity(Verbosity::kAssertionPassed)`) statement can be very helpful:

```C++
class CustomTestOnce: public TestOnce {
  protected:
    // optional
    void setup() override {
      TestOnce::setup();
      ...setup code...
    }

    // optional
    void teardown() override {
      ...teardown code...
      TestOnce::teardown();
    }

    void assertCustomStuff() {
      assertEqual(...);
      ...
      for (...) {
        ...
        assertEqual(...);
        ...
      }
    }

    int sharedValue;
};

testF(CustomTestOnce, calculate) {
  enableVerbosity(Verbosity::kAssertionPassed);

  ...test code here...
  assertCustomStuff();
}
```

<a name="ClassHierarchy"></a>
### Class Hierarchy

To support test fixtures in a more natural way, the class hierarchy
in AUnit is slightly different than ArduinoUnit. In ArduinoUnit we have a
two level hierarchy:
```
  Test ::loop()
    ^
    |
 TestOnce ::once()
 ```

In AUnit, the functionality that supports the `testing()` macro has been
migrated to a separate class called `TestAgain`, like this:

```
       Test ::loop()
         ^
         |
      Assertion
         ^
         |
     MetaAssertion
        ^  ^
       /    \
      /      \
TestAgain  TestOnce
::again()  ::once()
```

Normally, deep inheritance hierarchies like this should be avoided. However,
placing the `Assertion` and `MetaAssertion` classes inside the `Test` hierarchy
allowed those assertion statements to have access to the internal states of the
`Test` instance. This made certain features (like the early return upon delayed
failure) slightly easier to implement. For the most part, the end-users can
ignore the existence of the `Assertion` and `MetaAssertion` classes and think of
this as a simple 2-level inheritance tree.

<a name="PrivateHelperMethods"></a>
### Testing Private Helper Methods

There is a school of thought which says that unit tests should test only the
publically exposed methods of a class or library. I agree mostly with that
sentiment, but not rigidly. I think it is sometimes useful to write unit tests
for `protected` or `private` methods. For example, when creating a chain of
small helper methods, which build up to larger publically exposed methods, it is
extremely useful to write unit tests for the helper methods in isolation.

Normally those helper methods would be `private` because they are used
only within that class, and we don't want to expose them to the public API. One
option is to make them `public` but add a comment in the function to say that it
is exposed only for testing purposes. This does not seem satisfactory because
users will tend to ignore such comments if the helper functions are useful.

I think a better way is to keep the helper functions `private` but make
the unit tests a `friend class` of the target class. The syntax for doing this
can be tricky, it took me a number of attempts to get this right, especially if
when using namespaces for the target class:

```C++
//------------------- Target.h -------------

// Auto-generated test class names.
class Test_helper;
class TargetSuite_helper;
class TargetTest_helper;

namespace mylib {

class Target {
  public:
    void publicMethod() {
      ...
      int a = helper();
      ...
    }

  private:
    // Must have the global scope operator '::'
    friend class ::Test_helper;
    friend class ::TargetSuite_helper;
    friend class ::TargetTest_helper;

    static int helper() {...}
};

}

//------------------- TargetTest.ino -------------

#include <AUnit.h>
#include "Target.h"

using namespace aunit;
using namespace mylib;

test(helper) {
  assertEqual(1, Target::helper(...));
}

test(TargetSuite, helper) {
  assertEqual(1, Target::helper(...));
}

class TargetTest: public TestOnce {
  ...
};

testF(TargetTest, helper) {
  assertEqual(1, Target::helper(...));
}

```

The tricky part is that `Target.h` must have forward declarations of the various
auto-generated AUnit test classes. And within the `Target` class itsef, the
`friend` declarations need to have a global scope `::` specifier before the name
of the test class.

<a name="Benchmarks"></a>
## Benchmarks

AUnit consumes as much as 65% less flash memory than ArduinoUnit 2.2 on an AVR
platform (e.g. Arduino UNO, Nano), and 30% less flash on the Teensy-ARM platform
(e.g. Teensy LC ). (ArduinoUnit 2.3 reduces the flash memory by 30% or so, which
means that AUnit can still consume significantly less flash memory.)

Here are the resource consumption (flash and static) numbers
from
[AceButtonTest](https://github.com/bxparks/AceButton/tree/develop/tests/AceButtonTest)
containing 26 test cases using 331 `assertXxx()`
statements, compiled using AUnit and ArduinoUnit 2.2 on 5 different
microcontrollers:
```
---------------------------+---------+-------------+-------------+
Platform (resource)        |     Max | ArduinoUnit |       AUnit |
---------------------------+---------+-------------+-------------|
Arduino Nano (flash)       |   30720 |       54038 |       18928 |
Arduino Nano (static)      |    2048 |        1061 |         917 |
---------------------------+---------+-------------+-------------|
Teensy LC (flash)          |   63488 |       36196 |       26496 |
Teensy LC (static)         |    8192 |        2980 |        2780 |
---------------------------+---------+-------------+-------------|
Teensy 3.2 (flash)         |  262144 |       51236 |       37920 |
Teensy 3.2 (static)        |   65536 |        5328 |        5236 |
---------------------------+---------+-------------+-------------|
ESP8266 - ESP-12E (flash)  | 1044464 |    does not |      268236 |
ESP8266 - ESP-12E (static) |   81920 |     compile |       33128 |
---------------------------+---------+-------------+-------------|
ESP8266 - ESP-01 (flash)   |  499696 |    does not |      268236 |
ESP8266 - ESP-01 (static)  |   47356 |     compile |       33128 |
---------------------------+---------+-------------+-------------+
```

Not all unit test sketches will experience a savings of 65% of flash memory with
AUnit, but a savings of 30-50% seems to be common.

<a name="SystemRequirements"></a>
## System Requirements

<a name="Hardware"></a>
### Hardware

The library is tested on the following boards:

* Arduino Nano clone (16 MHz ATmega328P)
* SparkFun Pro Micro clone (16 MHz ATmega32U4)
* SAMD21 M0 Mini board (Arduino Zero compatible, 48 MHz ARM Cortex-M0+)
* STM32 Blue Pill (STM32F103C8, 72 MHz ARM Cortex-M3)
* NodeMCU 1.0 (ESP-12E module, 80 MHz ESP8266)
* WeMos D1 Mini (ESP-12E module, 80 MHz ESP8266)
* ESP32 dev board (ESP-WROOM-32 module, 240 MHz dual core Tensilica LX6)
* Teensy 3.2 (96 MHz ARM Cortex-M4)

I will occasionally test on the following hardware as a sanity check:

* Arduino Pro Mini (16 MHz ATmega328P)
* Mini Mega 2560 (Arduino Mega 2560 compatible, 16 MHz ATmega2560)
* Teensy LC (48 MHz ARM Cortex-M0+)

The following boards are **not** supported:

* Any platform using the ArduinoCore-API
  (https://github.com/arduino/ArduinoCore-api), such as:
    * megaAVR (e.g. Nano Every) using ArduinoCore-megaavr
      (https://github.com/arduino/ArduinoCore-megaavr/)
    * SAMD21 boards (e.g. MKRZero) using ArduinoCore-samd
      (https://github.com/arduino/ArduinoCore-samd) starting with
      `arduino:samd` version >= 1.8.10
    * Raspberry Pi Pico (RP2040) using Arduino-Pico
      (https://github.com/earlephilhower/arduino-pico)

<a name="ToolChain"></a>
### Tool Chain

This library was validated using:

* [Arduino IDE 1.8.19](https://www.arduino.cc/en/Main/Software)
* [Arduino CLI 0.19.2](https://arduino.github.io/arduino-cli)
* [Arduino AVR Boards 1.8.4](https://github.com/arduino/ArduinoCore-avr)
* [Arduino SAMD Boards 1.8.9](https://github.com/arduino/ArduinoCore-samd)
* [SparkFun AVR Boards 1.1.13](https://github.com/sparkfun/Arduino_Boards)
* [SparkFun SAMD Boards 1.8.6](https://github.com/sparkfun/Arduino_Boards)
* [STM32duino 2.2.0](https://github.com/stm32duino/Arduino_Core_STM32)
* [ESP8266 Arduino 3.0.2](https://github.com/esp8266/Arduino)
* [ESP32 Arduino 2.0.2](https://github.com/espressif/arduino-esp32)
* [Teensyduino 1.56](https://www.pjrc.com/teensy/td_download.html)

This library is *not* compatible with:

* Any platform using the
  [ArduinoCore-API](https://github.com/arduino/ArduinoCore-api), for example:
    * [Arduino SAMD Boards >=1.8.10](https://github.com/arduino/ArduinoCore-samd)
    * [Arduino megaAVR](https://github.com/arduino/ArduinoCore-megaavr/)
    * [MegaCoreX](https://github.com/MCUdude/MegaCoreX)

(See [Issue #56](https://github.com/bxparks/AUnit/issues/56)
and [Issue #66](https://github.com/bxparks/AUnit/issues/66)).

It should work with [PlatformIO](https://platformio.org/) but I have
not tested it extensively.

The library works under Linux or MacOS (using both g++ and clang++ compilers)
using the EpoxyDuino (https://github.com/bxparks/EpoxyDuino) emulation layer.

<a name="OperatingSystem"></a>
### Operating System

I use Ubuntu 20.04 for the vast majority of my development. I expect that the
library will work fine under MacOS and Windows, but I have not tested them.

<a name="License"></a>
## License

[MIT License](https://opensource.org/licenses/MIT)

<a name="FeedbackAndSupport"></a>
## Feedback and Support

If you have any questions, comments, or feature requests for this library,
please use the [GitHub
Discussions](https://github.com/bxparks/AUnit/discussions) for this project. If
you have bug reports, please file a ticket in [GitHub
Issues](https://github.com/bxparks/AUnit/issues). Feature requests should go
into Discussions first because they often have alternative solutions which are
useful to remain visible, instead of disappearing from the default view of the
Issue tracker after the ticket is closed.

Please refrain from emailing me directly unless the content is sensitive. The
problem with email is that I cannot reference the email conversation when other
people ask similar questions later.

<a name="Authors"></a>
## Authors

* Created by Brian T. Park (brian@xparks.net).
* The Google Test adapter (`gtest.h`) was created by Chris Johnson
  (chrisjohnsonmail@gmail.com).
* @brewmanz increased the maximum allowed value of `TestRunner::setTimeout()`
  from 255 seconds to 65535 seconds (18.2 hours). (See [Issue
  #57](https://github.com/bxparks/AUnit/issues/57)).
* The design and syntax of many macros (e.g. `test()`, `assertXxx()`) were
  borrowed from the [ArduinoUnit](https://github.com/mmurdoch/arduinounit)
  project to allow AUnit to be almost a drop-in replacement. Many thanks to
  the ArduinoUnit team for creating such an easy-to-use API.
