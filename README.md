# AUnit - Unit Testing Framework for Arduino Platforms

Version: v0.2.0 (2018-03-16)

## Summary

**AUnit** (rhymes with "JUnit") is a unit testing framework based
on the venerable [ArduinoUnit](https://github.com/mmurdoch/arduinounit)
framework. It is almost a drop-in replacement for the API implemented by
ArduinoUnit version 2.2. Just like ArduinoUnit, the unit tests run directly on
the microcontrollers themselves, not on emulators or simulators. The test
results are printed on the `Serial` object by default, but can be redirected to
another `Print` object.

AUnit was created to solve 2 problems with ArduinoUnit:
* ArduinoUnit consumes too much flash memory on an AVR platform (e.g.
  Arduino UNO, Nano) as explained in
  [ArduinoUnit#70](https://github.com/mmurdoch/arduinounit/issues/70).
* ArduinoUnit does not compile on the ESP8266 platform (see
  [ArduinoUni#68](https://github.com/mmurdoch/arduinounit/issues/68),
  [ArduinoUni#57](https://github.com/mmurdoch/arduinounit/pull/57),
  [ArduinoUni#55](https://github.com/mmurdoch/arduinounit/issues/55)), and
  [ArduinoUni#54](https://github.com/mmurdoch/arduinounit/issues/54)).

In contrast:
* AUnit consumes as much as 66% *less* flash memory than ArduinoUnit on the
  AVR platform. On Teensy-ARM, the savings can be as much as 30%.
* AUnit has been tested on AVR, Teensy-ARM and ESP8266.

### Supported or Compatible Features

For basic unit tests written using ArduinoUnit, only two changes are required to
convert to AUnit:
* `#include <ArduinoUnit.h>` -> `#include <AUnit.h>`
* `Test::run()` -> `aunit::TestRunner::run()`

Essentially all of the various macros are compatible between ArduinoUnit and
AUnit:
* `test()`
* `testing()`
* `assertXxx()`
* Meta Assertions
    * `checkTestXxx()`
    * `assertTestXxx()`
* `externTest()`
* `externTesting()`

AUnit supports exclude and include filters:
* `TestRunner::exclude()`
* `TestRunner::include()`

### Missing Features

Here are the features which have not been ported over from ArduinoUnit:

* ArduinoUnit supports multiple `*` wildcards in its `exclude()` and `include()`
  methods. AUnit supports only a single `*` wildcard and it must occur at the
  end if present.

### Added Features

Here are the features in AUnit which are not available in ArduinoUnit:

* The `TestRunner` supports a configurable timeout parameter which
  can prevent `testing()` test cases from running forever. The following
  methods and macros are available in AUnit to support this feature:
    * `Test::expire()`
    * `assertTestExpire()`
    * `assertTestNotExpire()`
    * `checkTestExpire()`
    * `checkTestNotExpire()`
* AUnit works on the ESP8266 platform.

### Beta Status

Although this library has been extensively tested by me, and I converted my
[AceButton](https://github.com/bxparks/AceButton) library to use it, I consider
it currently in "beta stage" until more  users have tested it.

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

## Usage

In this section, information about differences between AUnit and ArduinoUnit
will appear in a note marked by ***ArduinoUnit Compatibility***.

### Header and Namespace

To prevent name clashes with other libraries and code, all classes in the AUnit
library are defined in the `aunit` namespace. The user will mostly interact with
the `TestRunner` class. It can be referenced with an explicit namespace
qualifier (i.e. `aunit::TestRunner`), or we can use a `using` directive like
this:
```
#include <AUnit.h>
using aunit::TestRunner;
```
or we can import the entire `aunit` namespace:
```
#include <AUnit.h>
using namespace aunit;
```

Similar to ArduinoUnit, many of the "functions" in this framework (e.g.
`test()`, `testing()`, `assertXxx()`) are defined as `#define` macros which live
in the global namespace, so it is usually not necessary to import the entire
`aunit` namespace.

***ArduinoUnit Compatibility***: _I have found that the following macros are
useful during the transition:_
```
#define USE_AUNIT 1

#if USE_AUNIT == 1
#include <AUnit.h>
#else
#include <ArduinoUnit.h>
#endif

...

void loop() {
#if USE_AUNIT == 1
  aunit::TestRunner::run();
#else
  Test::run();
#endif
}
```

### Defining the Tests

The usage of **AUnit** is basically identical to **ArduinoUnit**. The following
macros are used to create a subclass of one of the two test classes (`Test` or
`TestOnce`):

* `test()`
* `testing()`

and the code in `{ }` following these macros becomes the body of the `looping()`
or `once()` methods of the two base test classes.

The argument to these macros are the name of the test case, and is used to
generate a name for the subclass. (The name is available within the test code
using the `Test::getName()` method). The macros also generate code to create an
global instance of the subclass, which are static initialized by C++.

During static initialization, the constructor of the object adds itself to an
internal list. The root of that list is given by `Test::getRoot()`. The
`TestRunner::run()` method traverses the linked list, executing each test case
until it passes, fails or is skipped.

Here is a rough outline of an AUnit unit test sketch:

```
#line 2 AUnitTest.ino

#include <AUnit.h>

using aunit::TestRunner;

test(example_test) {
  ... code ...
  assertEqual(a, b);
}

testing(looping_test) {
  ... code ...
  if (...) {
    pass();
  } else if (...) {
    fail();
  } else {
    skip();
  }
}

void setup() {
  Serial.begin(74880); // 74880 is the default for some ESP8266 boards
  while (! Serial); // Wait until Serial is ready - Leonardo

  TestRunner::exclude("*");
  TestRunner::include("looping*");
}

void loop() {
  TestRunner::run();
}
```

***ArduinoUnit Compatibility***: _The basic structure of the
unit test is identical to ArduinoUnit._

### Binary Assertions

Inside the `test()` and `testing()` macros, the following assertions
are available. These are essentially identical to ArduinoUnit:

* `assertEqual(a, b)`
* `assertNotEqual(a, b)`
* `assertLess(a, b)`
* `assertMore(a, b)`
* `assertLessOrEqual(a, b)`
* `assertMoreOrEqual(a, b)`

The following overloaded types for the various `assertXxx()` macros are
defined:

* `(bool, bool)`
* `(char, char)`
* `(int, int)`
* `(unsigned int, unsigned int)`
* `(long, long)`
* `(unsigned long, unsigned long)`
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

As you can see, all 9 combinations of the 3 string types (`char*`, `String`, and
`__FlashStringHelper*`) are supported.

Additionally, the usual C++ implicit type conversion and function overloading
matching algorithms apply. For example, the conversions will occur:

* `signed char` -> `int`
* `unsigned char` -> `int`
* `short` -> `int`
* `unsigned short` -> `int` or `unsigned int` (depending on `sizeof(int)`)
* `char*` -> `const char*`.
* `char[N]` -> `const char*`
* `float` -> `double`

Note that `char`, `signed char`, and `unsigned char` are 3 distinct types in
C++, so a `(char, char)` will match exactly to one of the `assertXxx()`
methods.

***ArduinoUnit Compatibility***:
_The names of the macros are identical. However, the
type inference logic of two `(a, b)` arguments in the `assertXxx(a, b)` is
slightly different. ArduinoUnit allows the two parameters to be slightly
different types, at the expense of a compiler warning. In AUnit, the
warning becomes a compiler error. See below._

#### Assertion Parameters Must Match Types

In ArduinoUnit, the `assertXxx()` macros could be slightly different types, for
example:
```
unsigned int uintValue = 5;
assertEqual(5, uintValue);
```

If the compiler warnings are enabled, a warning from the compiler is printed:

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
```
assertEqual(5U, uintValue);
```

On the AVR platform, both a (short) and (int) are 16-bit types, so the following
will produce a compiler error:
```
unsigned short ushortValue = 5;
assertEqual(5U, ushortValue);
```
But on Teensy-ARM and ESP8266, a 16-bit (short) can be promoted to a 32-bit
(int) without loss of precision, so the above will compile just fine. For
portability, the following should be used on all platforms:
```
unsigned short ushortValue = 5;
assertEqual((unsigned short) 5, ushortValue);
```

The integer type promotion rules and function overload matching rules can be
difficult to remember (and sometimes difficult to understand). The best way to
avoid these compiler errors is to make sure that the assertion parameter types
are identical, potentially using explicit casting.

### Boolean Assertions

The following boolean asserts are also available:

* `assertTrue(condition)`
* `assertFalse(condition)`

***ArduinoUnit Compatibility***: _These are identical to ArduinoUnit._

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

The following macros define `extern` references to test case objects which live
in other `.cpp` files. These are required for the above meta assertions if the
test cases are defined in another file:

* `externTest()`
* `externTesting()`

***ArduinoUnit Compatibility***: _[&ast;] only in AUnit._

### Status Indicator Methods

These methods can be used inside a `test()` or `testing()` macro
to indicate whether the test has passed or failed (or reached some other
status reason).

* `pass()` - test passed
* `fail()` - test failed
* `skip()` - test skipped
* `expire()`  - test timed out [&ast;]

***ArduinoUnit Compatibility***: _[&ast;] only in AUnit._

### Overridable Methods

The following methods are defined at the `Test` base class level:

* `setup()`
* `loop()`
* `once()`

***ArduinoUnit Compatibility***: _These are identifcal to ArduinoUnit._

### Running the Tests

We run the test cases in the global `loop()` method by calling
`TestRunner::run()`. The tests are sorted according to the name of the test
given in the argument in the `test()` or `testing()` macro.

Each call to the `run()` method causes one test case to run and be resolved. The
next call to `run()` executes the next test case. This design allows the
`loop()` method to perform a small amount of work and return periodically to
allow the system to perform some actions. On some systems, such as the ESP8266,
an error is generated if `loop()` takes too much CPU time.

```
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

### Excluding and Including Test Cases

We can `exclude()` or `include()` test cases using a pattern match,
just like ArduinoUnit. The names are slightly different:

* `TestRunner::exclude()`
* `TestRunner::include()`

These methods are called from the global `setup()` method:

```
void setup() {
  TestRunner::exclude("*");
  TestRunner::include("looping*");
  ...
}
```

***ArduinoUnit Compatibility***:
_The equivalent versions in ArduinoUnit are `Test::exclude()` and
`Test::include()` The matching algorithm in AUnit is not as powerful as one in
ArduinoUnit. AUnit supports only a single wildcard character `*` and that
character can appear only at the end if it is present. For example, the
following are accepted:_

* `TestRunner::exclude("*");`
* `TestRunner::include("f*");`
* `TestRunner::exclude("flash_*");`
* `TestRunner::include("looping*");`
* `TestRunner::include("flashTest");`

### Output Printer

The default output printer is the `Serial` instance. This can be
changed using the `TestRunner::setPrinter()` method:

```
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

### Controlling the Verbosity

The verbosity of the test results can be controlled using the the
`setVerbosity()` method:
```
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

The verbosity rules are more primitive (and simpler) than ArduinoUnit. Each
flag below is a bit field that controls whether certain messages are enabled
or disabled. There is no concept of a "minimum" or "maximum" verbosity. Also,
the verbosity of an individual test case cannot be independently controlled, the
`TestRunner` verbosity setting applies to all tests.

The names of the bit field flags are different from ArduinoUnit to avoid name
collisions with other `#define` macros which have global scope. AUnit uses
static constants of the `Verbosity` utility class:

* `Verbosity::kTestRunSummary`
* `Verbosity::kTestFailed`
* `Verbosity::kTestPassed`
* `Verbosity::kTestSkipped`
* `Verbosity::kTestAll`
* `Verbosity::kAssertionFailed`
* `Verbosity::kAssertionPassed`
* `Verbosity::kAssertionAll`
* `Verbosity::kDefault`, equivalent to setting the following
    * `(Verbosity::kAssertionFailed | Verbosity::kTestAll)`
* `Verbosity::kAll` - enables all messages
* `Verbosity::kNone` - disables all messages

***ArduinoUnit Compatibility***:
_The following ArduinoUnit variables do not exist:_`
* `Test::verbosity`
* `Test::min_verbosity`
* `Test::max_verbosity`

_In AUnit, verbosity can be set only at the `TestRunner` level. Verbosity cannot
be set at the test case (i.e. `Test` or `TestOnce` class) level individually.
The bit field constants have slightly different names:_

* `TEST_VERBOSITY_TESTS_SUMMARY` -> `Verbosity::kTestRunSummary`
* `TEST_VERBOSITY_TESTS_FAILED` -> `Verbosity::kTestFailed`
* `TEST_VERBOSITY_TESTS_PASSED` -> `Verbosity::kTestPassed`
* `TEST_VERBOSITY_TESTS_SKIPPED` -> `Verbosity::kTestPassed`
* `TEST_VERBOSITY_TESTS_ALL` -> `Verbosity::kTestAll`
* `TEST_VERBOSITY_ASSERTIONS_FAILED` -> `Verbosity::kAssertionFailed`
* `TEST_VERBOSITY_ASSERTIONS_PASSED` -> `Verbosity::kAssertionPassed`
* `TEST_VERBOSITY_ASSERTIONS_ALL` -> `Verbosity::kAssertionAll`
* `TEST_VERBOSITY_ALL` -> `Verbosity::kAll`
* `TEST_VERBOSITY_NONE` -> `Verbosity::kNone`
* {no equivalent} <- `Verbosity::kDefault`

### Line Number Mismatch

AUnit suffers from the same compiler/preprocessor bug as ArduinoUnit that causes
the built-in `__LINE__` macro to be off by one. The solution is to add:
```
#line 2 {file.ino}
```
as the first line of a unit test sketch.

***ArduinoUnit Compatibility***: _This problem is identical to ArduinoUnit._

### Assertion Message

The various `assertXxx()` macros in AUnit print a message upon pass or fail. For
example, if the assertion was:
```
int expected = 3;
int counter = 4;
assertEquals(expected, counter);
```

The error message (if enabled, which is the default) is:
```
Assertion failed: (3) == (4), file AUnitTest.ino, line 134.
```

***ArduinoUnit Compatibility***:
_ArduinoUnit captures the arguments of the `assertEqual()` macro
and prints:_

```
Assertion failed: (expected=3) == (counter=4), file AUnitTest.ino, line 134.
```

_Each capture of the parameter string consumes flash memory space. If the unit
test has numerous `assertXxx()` statements, the flash memory cost is expensive.
AUnit omits the parameters to reduce flash memory space by about 33%_

### Test Summary

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

### Test Runner Summary

At the end of the test run, the `TestRunner` prints out the summary
of all test cases, like this:

```
TestRunner summary: 12 passed, 0 failed, 2 skipped, 1 timed out, out of 15 test(s).
```

***ArduinoUnit Compatibility***: _The message format is slightly different than
ArduinoUnit. I changed "Test summary" to "TestRunner summary" because the former
looks identical to the message that could have been printed by a `test(summary)`
test case. AUnit also adds information about tests which timed out. See below._

### Test Timeout

***ArduinoUnit Compatibility***: _Only available in AUnit._

From my experience, it seems incredibly easy to write a `testing()` test case
which accidentally runs forever because the code forgets to call an explicit
`pass()`, `fail()` or `skip()`.

The `TestRunner` in AUnit applies a time out value to all the test cases that it
runs. The default time out is 10000 milliseconds (10 seconds). Currently, the
time out value is global to all test cases, individual test time out values
cannot be set independently. If a test does not finish before that time, then
the test is marked as `timed out` (internally implemented by the
`Test::expire()` method) and a message is printed like this:
```
Test looping_until timed out.
```

The time out value can be changed by calling the static
`TestRunner::setTimeout()` method. Here is an example that sets the timeout to 1
second instead:
```
void setup() {
  ...
  TestRunner::setTimeout(1000);
  ...
}
```

A timeout value of `0` means an infinite timeout, which means that the
`testing()` test case may run forever.

***ArduinoUnit Compatibility***: _Only available in AUnit._

## Benchmarks

AUnit consumes as much as 66% less flash memory than ArduinoUnit on an AVR
platform (e.g. Arduino UNO, Nano), and 30% less flash on the Teensy-ARM platform
(e.g. Teensy LC ). Here are the resource consumption (flash and static) numbers
from an actual unit test sketch containing 26 test cases using 331 `assertXxx()`
statements, compiled using AUnit and ArduinoUnit on 4 different
microcontrollers:
```
Platform (resource)        |     Max | ArduinoUnit |       AUnit |
---------------------------+---------+-------------+-------------|
Arduino Nano (flash)       |   30720 |       54038 |       18666 |
Arduino Nano (static)      |    2048 |        1061 |         918 |
---------------------------+---------+-------------+-------------|
Teensy LC (flash)          |   63488 |       36196 |       25228 |
Teensy LC (static)         |    8192 |        2980 |        2780 |
---------------------------+---------+-------------+-------------|
Teensy 3.2 (flash)         |  262144 |       51236 |       36300 |
Teensy 3.2 (static)        |   65536 |        5328 |        5236 |
---------------------------+---------+-------------+-------------|
ESP8266 - ESP-12E (flash)  | 1044464 |    does not |      267479 |
ESP8266 - ESP-12E (static) |   81920 |     compile |       34564 |
---------------------------+---------+-------------+-------------|
ESP8266 - ESP-01 (flash)   |  499696 |    does not |      267359 |
ESP8266 - ESP-01 (static)  |   47356 |     compile |       34564 |
---------------------------+---------+-------------+-------------|
```

Not all unit test sketches will experience a savings of 66% of flash memory with
AUnit, but a savings of 30-50% seems to be common.

## Changelog

See [CHANGELOG.md](CHANGELOG.md).

## System Requirements

This library was developed and tested using:
* [Arduino IDE 1.8.5](https://www.arduino.cc/en/Main/Software)
* [Teensyduino 1.41](https://www.pjrc.com/teensy/td_download.html)
* [ESP8266 Arduino Core 2.4.0](https://arduino-esp8266.readthedocs.io/en/2.4.0-rc2/)

I used MacOS 10.13.3 for most of my development.

The library has been verified to work on the following hardware:

* Arduino Nano clone (16 MHz ATmega328P)
* Arduino UNO R3 clone (16 MHz ATmega328P)
* Teensy LC (48 MHz ARM Cortex-M0+)
* Teensy 3.2 (72 MHz ARM Cortex-M4)
* NodeMCU 1.0 clone (ESP-12E module, 80MHz ESP8266)
* ESP-01 (ESP-01 module, 80MHz ESP8266)

## License

[MIT License](https://opensource.org/licenses/MIT)

## Authors

* Created by Brian T. Park (brian@xparks.net).
* The design and syntax of many macros (e.g. `test()`, `assertXxx()`) were
  borrowed from the [ArduinoUnit](https://github.com/mmurdoch/arduinounit)
  project to allow AUnit to be almost a drop-in replacement. Many thanks to
  the ArduinoUnit team for creating such an easy-to-use API.
