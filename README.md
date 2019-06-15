# AUnit

A unit testing framework for Arduino platforms inspired by ArduinoUnit and
Google Test. The unit tests run in the embedded controller, not in a simulator
or emulator. It is almost a drop-in replacement of ArduinoUnit with some
advantages. AUnit supports timeouts and test fixtures. It somtimes consume 50%
less flash memory on the AVR platform, and it has been tested to work on the
AVR, ESP8266, ESP32 and Teensy platforms. The sister AUniter project provides
command line tools to verify, upload and validate the unit tests. The AUniter
tools can be used in a continuous integration system like Jenkins.

Version: 1.3 (2018-06-15)

[![AUniter Jenkins Badge](https://us-central1-xparks2018.cloudfunctions.net/badge?project=AUnit)](https://github.com/bxparks/AUniter)

## Summary

**AUnit** (rhymes with "JUnit") is a unit testing framework inspired
by [ArduinoUnit](https://github.com/mmurdoch/arduinounit)
and [Google Test](https://github.com/google/googletest/).
It is almost a drop-in replacement for the API implemented by ArduinoUnit 2.2.
Just like ArduinoUnit, the unit tests run directly on the microcontrollers
themselves, not on emulators or simulators. The test results are printed on the
`Serial` object by default, but can be redirected to another `Print` object.

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

The various assertion and test status messages can be enabled or disabled using
the `Verbosity` flags on per test basis:
* `enableVerbosity()`
* `disableVerbosity()`

### Missing Features

Here are the features which have *not* been ported over from ArduinoUnit 2.2:

* ArduinoUnit supports multiple `*` wildcards in its `exclude()` and `include()`
  methods. AUnit supports only a single `*` wildcard and it must occur at the
  end if present.

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
* Tested on the following Arduino platforms:
    * AVR (8-bit)
    * Teensy ARM (32-bit)
    * ESP8266 (32-bit)
    * ESP32 (32-bit)

Every feature of AUnit is unit tested using AUnit itself.

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

### Source Code

The source files are organized as follows:
* `src/AUnit.h` - main header file
* `src/AUnitVerbose.h` - verbose version of main header file
* `src/aunit/` - all implementation files
* `tests/` - unit tests written using AUnit itself
* `examples/` - example sketches

### Docs

The [docs/](docs/) directory contains the
[Doxygen docs published on GitHub Pages](https://bxparks.github.io/AUnit/html).

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

Perhaps the best way to see AUnit in action through real life examples. I
currently have 3 Arduino project using AUnit extensively
(look under the `tests/` directory in each project).

* [AceButton](https://github.com/bxparks/AceButton)
    * Originally created using ArduinoUnit 2.2, and I have kept those tests
      backwards compatible. They do not use the new features of AUnit.
* [AceRoutine](https://github.com/bxparks/AceRoutine)
    * Demonstrates the full power of AUnit better.
* [AceSegment](https://github.com/bxparks/AceSegment)
    * Demonstrates the full power of AUnit better.

## Usage

In this section, information about differences between AUnit and ArduinoUnit
will appear in a note marked by ***ArduinoUnit Compatibility***.

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
      TestOnce::teardown();
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

### Binary Assertions

Inside the `test()` and `testing()` macros, the following assertions
are available. These are essentially identical to ArduinoUnit:

* `assertEqual(a, b)`
* `assertNotEqual(a, b)`
* `assertLess(a, b)`
* `assertMore(a, b)`
* `assertLessOrEqual(a, b)`
* `assertMoreOrEqual(a, b)`

#### Supported Parameter Types

The 6 core assert macros (assertEqual, assertNotEqual, assertLess, assertMore,
assertLessOrEqual, assertMoreOrEqual) support the following 18
combinations for their parameter types:

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

As you can see, all 9 combinations of the 3 string types (`char*`, `String`, and
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

#### Parameters Must Match Types

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

### Case Insensitive String Comparisons

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
AUnit offers only the equilvalent of `ASSERT_NEAR()` function:

* `assertNear(a, b, error)`
* `assertNotNear(a, b, error)`

Upon failure, the error messages will look something like:
```
Assertion failed: |(1.00) - (1.10)| > (0.20), file AUnitTest.ino, line 517.
Assertion failed: |(4.00) - (1.10)| <= (0.20), file AUnitTest.ino, line 527.
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

### Boolean Assertions

The following boolean asserts are also available:

* `assertTrue(condition)`
* `assertFalse(condition)`

***ArduinoUnit Compatibility***: _These are identical to ArduinoUnit._

### Test Fixtures

When the unit tests become more complex, using test fixtures will allow you to
place common data objects and methods into a class that can be shared among
multiple test cases. This concept matches very closely to the the test fixtures
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
then `doStuff()` inside `testF()` will execute:

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
      assertEqual(sharedValue, 3);

      // This will not execute if the assertEqual() failed.
      assertLess(...);
    }

    int sharedValue;
};

testF(CustomTestOnce, calculate) {
  assertCustomStuff();

  // This will execute even if assertCustomStuff() failed.
  doStuff();

  // This will immediately exit this method if assertCustomStuff() failed.
  assertTrue(true);

  // This will NOT execute if assertCustomStuff() failed.
  doMoreStuff();
}
```

AUnit tries to mitigate this problem by having every `assertXxx()` macro
perform a check to see if a previous assert statement raise an error condition
for the test. If so, then the assert macro immediately exits. In the code above,
`doMoreStuff()` will not execute, because the `assertNotEqual()` will immidately
exit upon detecting the failure of `assertCustomStuff()`.

Google Test has a
[ASSERT_NO_FATAL_FAILURE( statement)](https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md)
macro that can guard against this possibility. AUnit does not have that macro,
but we get the equivalent effect by doing a `assertTrue(true)` shown above.

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
Assertion passed: Test slow_pass is done, file AUnitTest.ino, line 366.
Assertion passed: Test slow_pass is not failed, file AUnitTest.ino, line 372.
Assertion passed: Test slow_skip is skipped, file AUnitTest.ino, line 448.
Assertion passed: Test slow_skip is not timed out, file AUnitTest.ino, line 451.
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
Assertion passed: (test_slow_skip_instance.state=2) >= (Test::DONE_SKIP=2), file
AUnitTest.ino, line 439.
```

_AUnit has a separate message handler to print a customized message for the
assertTestXxx() meta assertion macros._

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
Status passed, file AUnitTest.ino, line 360.
Status failed, file AUnitTest.ino, line 378.
Status skipped, file AUnitTest.ino, line 380.
Status timed out, file AUnitTest.ino, line 391.
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

### Filtering Test Cases

We can `exclude()` or `include()` test cases using a pattern match:

* `TestRunner::exclude(pattern)`
* `TestRunner::exclude(testClass, pattern)`
* `TestRunner::include(pattern)`
* `TestRunner::include(testClass, pattern)`

These methods are called from the global `setup()` method:

```C++
void setup() {
  TestRunner::exclude("*");
  TestRunner::include("looping*");
  TestRunner::exclude("CustomTestAgain", "*");
  TestRunner::include("CustomTestAgain", "test*");
  ...
}
```

Excluded tests bypass their `setup()` and `teardown()` methods and terminate
immidiately. For the purposes of reporting, however, excluded tests are
counted as "skipped".

The 2-argument versions of `include()` and `exclude()` correspond to the
2 arguments of `testF()` and `testingF()`.

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
* `TEST_VERBOSITY_TESTS_SKIPPED` -> `Verbosity::kTestPassed`
* `TEST_VERBOSITY_TESTS_ALL` -> `Verbosity::kTestAll`
* `TEST_VERBOSITY_ASSERTIONS_FAILED` -> `Verbosity::kAssertionFailed`
* `TEST_VERBOSITY_ASSERTIONS_PASSED` -> `Verbosity::kAssertionPassed`
* `TEST_VERBOSITY_ASSERTIONS_ALL` -> `Verbosity::kAssertionAll`
* `TEST_VERBOSITY_ALL` -> `Verbosity::kAll`
* `TEST_VERBOSITY_NONE` -> `Verbosity::kNone`
* {no equivalent} <- `Verbosity::kDefault`
* {no equivalent} <- `Verbosity::kTestExpired`

### Line Number Mismatch

AUnit suffers from the same compiler/preprocessor bug as ArduinoUnit that causes
the built-in `__LINE__` macro to be off by one. The solution is to add:
```C++
#line 2 {file.ino}
```
as the first line of a unit test sketch.

***ArduinoUnit Compatibility***: _This problem is identical to ArduinoUnit._

### Assertion Message

The various `assertXxx()` macros in AUnit print a message upon pass or fail. For
example, if the assertion was:
```C++
int expected = 3;
int counter = 4;
assertEquals(expected, counter);
```

The error message (if enabled, which is the default) is:
```
Assertion failed: (3) == (4), file AUnitTest.ino, line 134.
```

Asserts with `bool` values produce customized messages, printing "true" or
"false" instead of using the Print class default conversion to `int`:
```C++
assertEquals(true, false);

Assertion failed: (true) == (false), file AUnitTest.ino, line 134.
```

Similarly, the `assertTrue()` and `assertFalse()` macros provide more customized
messages:
```C++
bool ok = false;
assertTrue(ok);

Assertion failed: (false) is true, file AUnitTest.ino, line 134.
```

and
```C++
bool ok = true;
assertFalse(ok);

Assertion failed: (true) is false, file AUnitTest.ino, line 134.
```

***ArduinoUnit Compatibility***:
_ArduinoUnit captures the arguments of the `assertEqual()` macro
and prints:_

```
Assertion failed: (expected=3) == (counter=4), file AUnitTest.ino, line 134.
```

_Each capture of the parameter string consumes flash memory space. If the unit
test has numerous `assertXxx()` statements, the flash memory cost is expensive.
AUnit omits the parameters to reduce flash memory space by about 33%._

_The messages for asserts with bool values are customized for better clarity
(partially to compensate for the lack of capture of the string of the actual
arguments, and are different from ArduinoUnit._

#### Verbose Mode

If you use the verbose header:
```C++
#include <AUnitVerbose.h>
```
the assertion message will contain the string fragments of the arguments
passed into the `assertXxx()` macros, like this:

```
Assertion failed: (expected=3) == (counter=4), file AUnitTest.ino, line 134.
Assertion failed: (ok=false) is true, file AUnitTest.ino, line 134.
```

***ArduinoUnit Compatibility***:
_The verbose mode produces the same messages as ArduinoUnit, at the cost of
increased flash memory usage._

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
TestRunner duration: 0.05 seconds.
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
runs. The default time out is 10 seconds. Currently, the
time out value is global to all test cases, individual test time out values
cannot be set independently. If a test does not finish before that time, then
the test is marked as `timed out` (internally implemented by the
`Test::expire()` method) and a message is printed like this:
```
Test looping_until timed out.
```

The time out value can be changed by calling the static
`TestRunner::setTimeout()` method. Here is an example that sets the timeout to
30 seconds instead:
```C++
void setup() {
  ...
  TestRunner::setTimeout(30);
  ...
}
```

A timeout value of `0` means an infinite timeout, which means that the
`testing()` test case may run forever. To conserve static memory, the value of
the timeout is stored as a single byte `uint8_t`, so the maximum timeout is 255
seconds or 4m15s. (It could be argued that a test taking longer than this is not
really a unit test but an integration test, and should probably use a different
framework, but let me know if you truly need a timeout of greater than 4m15s).

***ArduinoUnit Compatibility***: _Only available in AUnit._

## GoogleTest Adapter

It may be possible to run simple unit tests written using
[Google Test](https://github.com/google/googletest/) API on an Arduino platform
by using the
[aunit/contrib/gtest.h](src/aunit/contrib/gtest.h) adapter. This
adapter layer provides a number of macros Google Test macros which map to
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

## Commandline Tools and Continuous Integration

### AUniter

The command line tools have been moved into the
[AUniter](https://github.com/bxparks/AUniter) project.
The `auniter.sh` script can compile, upload and validate multiple AUnit tests on
multiple Arduino boards. The script can monitor the serial port and determine if
the unit test passed or failed, and it will print out a summary of all unit
tests at the end. Full details are given in the AUniter project, but here are
some quick examples copied from the `AUniter/README.md` file:

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

### Continuous Integration

The AUniter tools have been integrated into the [Jenkins](https://jenkins.io)
continuous integration service. See details in
[Continuous Integration with Jenkins](https://github.com/bxparks/AUniter/tree/develop/jenkins).

## Tips

Collection of useful tidbits.

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

### Class Name Differences

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

### Comparing Pointers

Currently the `assertEqual()` and other `assertXxx()` methods do not support
comparing arbitrary pointers (i.e. `(void*)`. This could change if
[Issue #34](https://github.com/bxparks/AUnit/issues/34) is
resolved. In the meantime, a workaround is to cast the pointer to a `uintptr_t`
integer type from `#include <stdint.h>` and then calling `assertEqual()` on the
integer type.

### Testing Private Helper Methods

There is a school of throught which says that unit tests should test only the
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
you are also using namespaces for your target class:

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

The tricky part is that in `Target.h` you need a forward declaration of the
various auto-generated AUnit test classes, and within the `Target` class itsef,
the `friend` declaration needs to have a global scope `::` specifier before the
name of the test class.

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
---------------------------+---------+-------------+-------------|
```

Not all unit test sketches will experience a savings of 65% of flash memory with
AUnit, but a savings of 30-50% seems to be common.

## Changelog

See [CHANGELOG.md](CHANGELOG.md).

## System Requirements

This library was developed and tested using:
* [Arduino IDE 1.8.5](https://www.arduino.cc/en/Main/Software)
* [Teensyduino 1.41](https://www.pjrc.com/teensy/td_download.html)
* [ESP8266 Arduino Core 2.4.1](https://arduino-esp8266.readthedocs.io/en/2.4.1/)
* [arduino-esp32](https://github.com/espressif/arduino-esp32)

I used MacOS 10.13.3 and Ubuntu 17.10 for most of my development.

The library is tested on the following hardware before each release:

* Arduino Nano clone (16 MHz ATmega328P)
* SparkFun Pro Micro clone (16 MHz ATmega32U4)
* Teensy 3.2 (72 MHz ARM Cortex-M4)
* NodeMCU 1.0 clone (ESP-12E module, 80 MHz ESP8266)
* ESP32 dev board (ESP-WROOM-32 module, 240 MHz dual core Tensilica LX6)

I will occasionally test on the following hardware as a sanity check:

* Arduino UNO R3 clone (16 MHz ATmega328P)
* Arduino Pro Mini clone (16 MHz ATmega328P)
* Teensy LC (48 MHz ARM Cortex-M0+)
* ESP-01 (ESP-01 module, 80 MHz ESP8266)

## License

[MIT License](https://opensource.org/licenses/MIT)

## Feedback and Support

If you have any questions, comments, bug reports, or feature requests, please
file a GitHub ticket instead of emailing me unless the content is sensitive.
(The problem with email is that I cannot reference the email conversation when
other people ask similar questions later.) I'd love to hear about how this
software and its documentation can be improved. I can't promise that I will
incorporate everything, but I will give your ideas serious consideration.

## Authors

* Created by Brian T. Park (brian@xparks.net).
* The Google Test adapter (`gtest.h`) was created by Chris Johnson
  (chrisjohnsonmail@gmail.com).
* The design and syntax of many macros (e.g. `test()`, `assertXxx()`) were
  borrowed from the [ArduinoUnit](https://github.com/mmurdoch/arduinounit)
  project to allow AUnit to be almost a drop-in replacement. Many thanks to
  the ArduinoUnit team for creating such an easy-to-use API.
