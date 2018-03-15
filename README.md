# AUnit - Unit Testing Framework for Arduino Platforms

## Summary

**AUnit** (rhymes with "JUnit") is a unit testing framework based
on the venerable [ArduinoUnit](https://github.com/mmurdoch/arduinounit)
framework. It is almost a drop-in replacement for the API implemented by
ArduinoUnit version 2.2. Just like ArduinoUnit, the unit tests run directly on
the microcontrollers themselves, not on emulators or simulators.

AUnit was created to solve 2 problems with ArduinoUnit:
* ArduinoUnit consumes too much flash memory on an AVR platform (e.g.
  Arduino UNO, Nano) as explained in
  [ArduinoUnit#70](https://github.com/mmurdoch/arduinounit/issues/70).
* ArduinoUnit does not compile on the ESP8266 platform (see
  [ArduinoUni#68](https://github.com/mmurdoch/arduinounit/issues/68),
  [ArduinoUni#57](https://github.com/mmurdoch/arduinounit/pull/57), and
  [ArduinoUni#55](https://github.com/mmurdoch/arduinounit/issues/55)).


In contrast:
* AUnit consumes as much as 66% *less* flash memory than ArduinoUnit on the
  AVR platform. On Teensy-ARM, the savings can be as much as 30%.
* AUnit has been tested on AVR, Teensy-ARM and ESP8266.

For basic unit tests written using ArduinoUnit, only two changes are required to
convert to AUnit:
* `#include <ArduinoUnit.h>` -> `#include <AUnit.h>`
* `Test::run()` -> `aunit::TestRunner::run()`

Most of the frequently used macros are compatible between ArduinoUnit and AUnit:
* `test()`
* `testing()`
* `assertXxx()`

AUnit supports exclude and include filters:
* `TestRunner::exclude()`
* `TestRunner::include()`

Currently, only a single `*` wildcard is supported and it must occur at the end
if present.

Various "Meta Assertions" from ArduinoUnit have not been implemented yet
(see *Migrating from ArduinoUnit to AUnit* section below).

(**Beta Status**: Although this library has been extensively tested by me, and I
converted my [AceButton](https://github.com/bxparks/AceButton) library to use
it, I consider it currently in "beta stage" until more  users have tested it.)

## Installation

The library will be available in the Arduino IDE Library Manager eventually.

In the mean time, it can be installed by cloning the
[GitHub repository](https://github.com/bxparks/AUnit), then
manually copying over the contents to the `./libraries` directory used
by the Arduino IDE. (The result is a directory named `./libraries/AUnit`.)
See the Preferences menu for the location of the Arduino sketch directory.
You may need to restart the Arduino IDE to let it see the new library.

## Usage

### Header and Namespace

To prevent name clashes with other libraries and code, all classes in the AUnit
library are defined in the `aunit` namespace. The user will normally interact
with only the `TestRunner` class. It can be reference with an explicit namespace
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

### Defining the Tests

The usage of **AUnit** is almost identical to **ArduinoUnit**. A test case is
defined by a fragment of code inside the `{ }` just after the `test()` or
`testing()` macros. The argument to these macros are the name of the test case.
(The name is available within the test code using the `Test::getName()`
method). The `test()` and `testing()` macros use the name to generate a subclass
whose parents are `aunit::TestOnce` and `aunit::Test` respectively.

The macros also generate code to create an instance of the subclass.
The code following after the `test()` and `testing()` macros becomes
the body of the virtual `TestOnce::once()` and `Test::loop` methods
(respectively).

When the instance of the test case is statically initialized, it adds itself to
a linked list. The root of that singly-linked list is given by
`Test::getRoot()`. The `TestRunner::run()` method traverses the linked list,
executing each test case until it passes, fails or is skipped.

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

### Supported Macros

These are identical to ArduinoUnit:

* `test()`
* `testing()`

### Supported Assertions

Inside the `test()` and `testing()` macros, the following assertions
are available. These are essentially identical to ArduinoUnit:

* `assertEqual(a, b)`
* `assertNotEqual(a, b)`
* `assertLess(a, b)`
* `assertMore(a, b)`
* `assertLessOrEqual(a, b)`
* `assertMoreOrEqual(a, b)`

The type inference logic of two `(a, b)` arguments in the `assertXxx(a, b)` is
slightly different than ArduinoUnit. For non-String types, `a` and `b` must have
the same type, after the usual implicit type converisons. For example, the
following implicit conversions will occur:
* `signed char` -> `int`
* `unsigned char` -> `int`
* `short` -> `int`
* `unsigned short` -> `int` or `unsigned int` (depending on `sizeof(int)`)
* `char*` -> `const char*`.
* `char[N]` -> `const char*`
* `float` -> `double`

(Note that `char`, `signed char`, and `unsigned char` are 3 distinct types in
C++.)

Mixing the parameter types will often  produce compiler errors. See comments
and solutions in the *Migrating from ArduinoUnit to AUnit* section below.

For the 3 string types (`char*`, `String`, and `__FlashStringHelper*`),
all 9 combinatorial mixes are supported.

In summary, the following types of `(a, b)` are defined for the various
`assertXxx()` macros:

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

The following boolean asserts are also available, just like ArduinoUnit:

* `assertTrue(condition)`
* `assertFalse(condition)`

### Supported Methods in a Test Case

These are identical to the equivalent methods in ArduinoUnit, and are available
in a `test()` or `testing()` macro:

* `pass()`
* `fail()`
* `skip()`

### Supported Methods on Test Class

These are identical to ArduinoUnit:

* `setup()`
* `loop()`
* `once()`

### Running the Tests

Similar to ArduinioUnit, we run the test cases in the global `loop()` method by
calling `TestRunner::run()`. Each call to the `run()` method causes one test
case to run and be resolved. The next call to `run()` executes the next test
case. This design allows the `loop()` method to perform a small amount of work
and return periodically to allow the system to perform some actions. On some
systems, such as the ESP8266, an error is generated if `loop()` takes too much
CPU time.

```
...
void loop() {
  TestRunner::run();
}
```

### Excluding and Including Test Cases

We can `exclude()` or `include()` test cases using a pattern match,
just like ArduinoUnit. The names are slightly different:

* `TestRunner::exclude()` (equivalent to `Test::exclude()`
* `TestRunner::include()` (equivalent to `Test::include()`

These methods are called from the global `setup()` method:

```
void setup() {
  TestRunner::exclude("*");
  TestRunner::include("looping*");
  ...
}
```

The `TestRunner::exclude()` and `TestRunner::include()` methods in AUnit
are not as powerful as the ones provided by ArduinoUnit. AUnit supports only a
single wildcard character `*` and that character can appear only at the end if
it is present. For example, the following are accepted:

* `TestRunner::exclude("*");`
* `TestRunner::exclude("f*");`
* `TestRunner::include("flash_*");`
* `TestRunner::exclude("looping*");`
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

This is the equivalent of the `Test::out` static member variable in
ArduinoUnit. In AUnit member variables are not exposed, so changes must be
made through the `TestRunner::setPrinter()` method.

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
    * `Verbosity::kAssertionFailed`
    * `Verbosity::kTestFailed`
    * `Verbosity::kTestPassed`
    * `Verbosity::kTestSkipped`
    * `Verbosity::kTestRunSummary`
* `Verbosity::kAll` - enables all messages
* `Verbosity::kNone` - disables all messages

### Line Number Mismatch

AUnit suffers from the same compiler/preprocessor bug as ArduinoUnit that causes
the built-in `__LINE__` macro to be off by one. The solution is to add:
```
#line 2 {file.ino}
```
as the first line of a unit test sketch.

## Migrating from ArduinoUnit to AUnit

### Header

Use
```
#include <AUnit.h>
```
instead of
```
#include <ArduinoUnit.h>
```

### Test Runner

The `Test::run()` method has been moved to a new `TestRunner` class. Use
```
aunit::TestRunner::run();
```
instead of
```
Test::run();
```

### Compile Time Selection

I have found that the following macros are useful during the transition:

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

### Printer

The `Test::out` static variable can be set using the static method on
`TestRunner`. Use

```
TestRunner::setPrinter(&Serial1);
```
instead of
```
Test::out = &Serial1;
```

(The current `Print` object can be accessed through
`aunit::Printer::getPrinter()` but I don't expect this to be needed often.)

### Verbosity

The following ArduinoUnit variables do not exist:
* `Test::verbosity`
* `Test::min_verbosity`
* `Test::max_verbosity`

Verbosity can be set only at the `TestRunner` level. Verbosity cannot be set at
the test case (i.e. `Test` or `TestOnce` class) level individually. There is no
"min" or "max" verbosity level. Each type of message is controlled by a bit
flag. The bit flag can be set using `TestRunner::setVerbosity()`. The bit field
constants have slightly different names:

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

### Missing ArduinoUnit Features

The following methods from ArduinoUnit are not yet implemented:

* `checkTestDone(name)`
* `checkTestNotDone(name)`
* `checkTestPass(name)`
* `checkTestNotPass(name)`
* `checkTestFail(name)`
* `checkTestNotFail(name)`
* `checkTestSkip(name)`
* `checkTestNotSkip(name)`
* `assertTestDone(name)`
* `assertTestNotDone(name)`
* `assertTestPass(name)`
* `assertTestNotPass(name)`
* `assertTestFail(name)`
* `assertTestNotFail(name)`
* `assertTestSkip(name)`
* `assertTestNotSkip(name)`

### Assertion Parameters Omitted in Messages

The various `assertXxx()` macros in AUnit print a slightly shorter
message upon pass or fail. For example, if the assertion was:
```
int expected = 3;
int counter = 4;
assertEquals(expected, counter);
```

ArduinoUnit captures the arguments of the `assertEqual()` macro
and prints:

```
Assertion failed: (expected=3) == (counter=4), file AUnitTest.ino, line 134.
```

AUnit omits the parameters to reduce flash memory space by about 33%:

```
Assertion failed: (3) == (4), file AUnitTest.ino, line 134.
```

### Assertion Parameters Must Match Types

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
Arduino Nano (flash)       |   30720 |       54038 |       18422 |
Arduino Nano (static)      |    2048 |        1061 |         908 |
---------------------------+---------+-------------+-------------|
Teensy LC (flash)          |   63488 |       36196 |       25096 |
Teensy LC (static)         |    8192 |        2980 |        2768 |
---------------------------+---------+-------------+-------------|
Teensy 3.2 (flash)         |  262144 |       51236 |       36136 |
Teensy 3.2 (static)        |   65536 |        5328 |        5224 |
---------------------------+---------+-------------+-------------|
ESP8266 - ESP-12E (flash)  | 1044464 |    does not |      267359 |
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
