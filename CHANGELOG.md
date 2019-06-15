# Changelog

* Unreleased
* 1.3 (2019-06-15)
    * Remove direct creation of `String` object, removing last remaining dynamic
      allocation of heap memory from library.
    * Add experimental implementation of `unitduino`, an Anduino emulation
      layer that allows AUnit tests to compile and run on Linux and MacOS.
    * Fix broken compile of print64.cpp on ESP8266 Arduino Core 2.5.1.
    * Use `class` instead of `struct` in the various `test()` macros when
      creating subclasses for each test. Fixes warnings about mismatch between
      'struct' and 'class' in clang++ when forward declaring the tests in code
      that gives access to private members to tests.
* 1.2.1 (2018-12-22)
    * Add support for 64-bit integers (`long long` and `unsigned long long`)
      for various `assertXxx()` statements. Exceptions are: `assertNear()` and
      `assertNotNear()` which do not support 64-bit integers. This is considered
      a minor-point release (1.2.1 instead of 1.3) because the user-visible API
      does not change, just extends to 64-bit integer types.
    * Add `ASSERT_NEAR()` macro to `contrib/gtest.h` (thanks ciband@).
* 1.2 (2018-12-01)
    * Add overloaded 2-argument versions of test() and testing() macros to match
      the TEST() macro in Google Test which takes 2 arguments.
    * Merge 4 new Google Test adapter macros into gtest.h.
    * Add documentation of gtest.h to README.md.
* 1.1.1 (2018-10-18)
    * Work around compiler crash on Arduino Core >= 1.6.22 (IDE >= 1.8.6)
      due to
      [ArduinoCore-avr#Issue39](https://github.com/arduino/ArduinoCore-avr/issues/39).
    * Update Jenkinsfile to be compatible with AUniter 1.7, use new subcommand
      syntax of auniter.sh.
    * Rename tests/auniter.conf to tests/auniter.ini after AUniter 1.6.
    * Clean up various AUniter environments (e.g. env:micro uses SparkFun board
      manager for SparkFun:avr:promicro; env:esp32 uses ESP32 board manager
      for esp32:esp32:esp32 instead of espressif:esp32:esp32).
* 1.1 (2018-07-23)
    * Add assertNear() and assertNotNear() for floating and integral types.
      Fixes #35.
    * Moved deliberately failing tests into FailingTest.ino and add passing
      tests to AUniter/Jenkins.
* 1.0.1 (2018-06-27)
    * Move AUniter tools into separate project and integrate with
      a Jenkins continuous integration service running locally.
* 1.0.0 (2018-06-20)
    * Add auniter.sh commandline script that allows uploading and validation
      of multiple unit tests on multiple Arduino boards. (#31)
    * Include contrib/gtest.h, an adapter header file by
      ciband@, that emulates some of the Google Test API. (#30)
    * Graduate from "beta" status. The library seems stable enough.
* 0.5.3 (2018-05-16)
    * Add assertStringCaseEqual() and assertStringCaseNotEqual(),
      case-insensitive versions of assertEqual() and assertNotEqual() for
      strings. Issue #28.
    * Support nullptr strings in various assertXxx() macros.
* 0.5.2 (2018-05-08)
    * Rename failNow(), passNow(), skipNow(), expireNow() macros to
      failTestNow(), passTestNow(), skipTestNow(), expireTestNow() to reduce
      the chance of name collision.
* 0.5.1 (2018-05-01)
    * Support ESP32.
    * Add failNow(), passNow(), skipNow() and expireNow() macros.
* 0.5.0 (2018-04-25)
    * Support verbose assertion messages using AUnitVerbose.h. Fixes #8.
    * Better assertion messages for assertTrue() and assertFalse(). Fixes #17.
    * Print duration of test runner at the end. Fixes #18.
    * Extract meta assertion tests to tests/AUnitMetaTest, so that core
      tests/AUnitTest can fit inside an Arduino Micro.
* 0.4.2 (2018-04-10)
    * Fix FSM for excluded tests so that they bypass setup() and teardown().
* 0.4.1 (2018-04-06)
    * Add support for `Test::teardown()` for use in test fixtures.
    * Add 2-argument versions of `TestRunner::include()` and
      `TestRunner::exclude()` to match `testF()` and `testingF()`.
* 0.4.0 (2018-03-30)
    * Implement `testF()` and `testingF()` macros for using test fixtures.
    * Added `examples/fixture/` sample code.
    * Removed support for manual `Test` and `TestOnce` test instances. Use
      `testF()` and `testingF()` macros instead.
    * Renamed `Test` and `TestOnce` class hierarchy into `TestAgain` and
      `TestOnce` respectively, and made them siblings not parent/child.
    * Implement early return upon delayed failure (assertions check the test
      status and return early).
    * Add ability to control verbosity on a per-test basis.
* 0.3.3 (2018-03-22)
    * Fix typo in kAssertionFailed flag introduced in 0.3.1.
* 0.3.2 (2018-03-22)
    * Publish doxygen docs using GitHub Pages.
* 0.3.1 (2018-03-20)
    * Add support for Verbosity::kTestExpired.
    * Clean up code in examples/ directory and add documentation in README.md.
* 0.3.0 (2018-03-19)
    * Implement all remaining macros from ArduinoUnit:
      assertTestXxx(), checkTestXxx(), externTest(), externTesting().
* 0.2.0 (2018-03-16)
    * TestRunner can time out long running tests. Default time out is 10
      seconds, but is configurable using TestRunner::setTimeout().
* 0.1.1 (2018-03-15)
    * Fix small bug with Test::setPassOrFail() which caused assertXxx()
      macros which returned true to terminate the testing() test cases.
* 0.1.0 (2018-03-15)
    * First merge into 'master' branch and tagged.
* (2018-03-12)
    * Initial upload to GitHub.
