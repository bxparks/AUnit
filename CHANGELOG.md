# Changelog

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
