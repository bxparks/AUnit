# Changelog

* Unreleased
* 1.7.0 (2022-12-08)
    * **Potentially Breaking** Change format of assertion failure message from:
        * "Assertion failed: (expected=3) == (counter=4), file AUnitTest.ino,
          line 134.", to
        * "AUnitTest.ino:134: Assertion failed: (expected=3) == (counter=4)."
        * This format is compatible with various Linux/MacOS/Unix command line
          tools, in particular, the `vim` editor.
        * When used with EpoxyDuino, this message format allows the `vim` editor
          to jump directly to the file and line where the assertion failure
          occurred.
* 1.6.1 (2022-02-02)
    * Upgrade tool chain.
        * Arduino IDE from 1.8.13 to 1.8.19
        * Arduino CLI from 0.14.0 to 0.19.2
        * Arduino AVR Core from 1.8.3 to 1.8.4
        * STM32duino from 2.0.0 to 2.2.0
        * ESP8266 Core from 2.7.4 to 3.0.2
        * ESP32 Core from 1.0.6 to 2.0.2
        * Teensyduino from 1.54 to 1.56
    * Downgrade SAMD21 boards into new "Tier 3: May work but unsupported"
      category.
    * This is a maintenance release. No functional change.
* 1.6.0 (2021-11-02)
    * Add `TestRunner::includesub(word)` and `TestRunner::excludesub(word)`
      which includes or excludes tests based on the substring match on the
      `word`.
    * Call `excludeAll()` if the first filtering request is an `include()` or
      `includesub()`.
        * Otherwise, the first include request does not do anything.
    * Add command line flags and arguments when compiled under EpoxyDuino.
      See [Command Line Flags and
      Arguments](README.md#CommandLineFlagsAndArguments) in the README.md for
      full details. Fixes
      [Issue#76](https://github.com/bxparks/AUnit/issues/76).
        * `--include pattern,...`
            * Calls `TestRunner::include(pattern)` on each `pattern` in the
              comma-separated list.
        * `--exclude pattern,...`
            * Calls `TestRunner::exclude(pattern)` on each `pattern` in the
              comma-separated list.
        * `--includesub substring,...`
            * Calls `TestRunner::includesub(substring)` on each `substring` in
              the comma-separated list.
        * `--excludesub substring,...`
            * Calls `TestRunner::excludesub(substring)` on each `substring` in
              the comma-separated list.
        * `substring ...`
            * Any remaining **space**-separated list of words are processed
              using `TestRunner::includesub(substring)`.
* 1.5.5 (2021-05-03)
    * Prevent accidental inclusion of `HardwareSerial` class (via the `Serial`
      global variable). Remove reference to `SERIAL_PORT_MONITOR` from
      `Printer::sPrinter`. Move reference to `SERIAL_PORT_MONITOR` from
      TestRunner.cpp to TestRunner.h. Saves about 800 bytes on AVR applications
      which don't use `Serial` at all.
    * Activate
      [GitHub Discussions](https://github.com/bxparks/AUnit/discussions)
      for this project.
    * Blacklist all other platforms using ArduinoCore-API (e.g. RP2040) to
      generate a user-friendly error message instead of pages of obscure
      compiler errors.
* 1.5.4 (2021-03-02)
    * Implement `assertNoFatalFailure(statement)` macro to prevent continued
      execution if `statement` contains assertion failures. Fixes
      [Issue #11](https://github.com/bxparks/AUnit/issues/11).
* 1.5.3 (2021-02-23)
    * I botched the 1.5.2 release. Try again as 1.5.3.
* 1.5.2 (2021-02-23)
    * Convert `examples/AUnitPlatformIO/src/AUnitPlatformIO.ino` from
      a symlink to a regular file. The Arduino Library Manager apparently does
      not allow symlinks (see
      https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ). So when I
      created the symlink at v1.3.1 on 2019-07-31, the Library Manager stopped
      updating the library for almost 2 years, until I removed the symlink at
      v1.5.2.
    * No functional change in this release.
* 1.5.1 (2021-01-22)
    * Update UnixHostDuino 0.4 to EpoxyDuino 0.5.
    * No functional change in this release.
* 1.5 (2021-01-18)
    * Add support for STM32duino.
    * Blacklist megaAVR boards due to incompatibility with new Arduino API.
      See Issue #56 (https://github.com/bxparks/AUnit/issues/56).
    * Blacklist SAMD21 boards using `arduino:samd` core version >= 1.8.10
      due to incompatibility with new Arduino API. See Issue #66
      (https://github.com/bxparks/AUnit/issues/66).
    * Enable `F()` for ESP8266 since
      https://github.com/esp8266/Arduino/issues/3369 seems to have been fixed
      some time ago.
* 1.4.1 (2020-11-12)
    * Add Table of Contents to README.md to help navigation.
    * No functional change in this release.
* 1.4 (2020-10-28)
    * Support comparison of 2 arbitrary pointers using
      `assertEqual(const void*, const void*)` and
      `assertNotEqual(const void*, const void*)`. Fixes
      [Issue #34](https://github.com/bxparks/AUnit/issues/34).
    * Add GitHub Workflow Actions continuous integration for examples
      and tests under `examples/` and `tests/`.
* 1.3.3 (2020-09-15)
    * Increase maximum allowed `setTimeout()` from 255 seconds to 65535 seconds
      (18.2 hours). (See [Issue
      #57](https://github.com/bxparks/AUnit/issues/57)).
* 1.3.2 (2020-02-29)
    * Fix typos in README.md. No functional change.
* 1.3.1 (2019-07-31)
    * Add `examples/AUnitPlatformIO` to verify that AUnit works on PlatformIO.
    * Rename unitduino to UnixHostDuino and move to its own repository at
      `https://github.com/bxparks/EpoxyDuino`.
    * Add UnixHostDuino Makefile to all unit tests.
    * Support SAMD21 and ATmega2560 boards.
    * Call `exit(1)` upon failure or timeout, `exit(0)` upon success when using
      UnixHostDuino (#51).
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
