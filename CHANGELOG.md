# Changelog

* v0.3.0 (2018-03-19)
    * Implement all remaining macros from ArduinoUnit:
      assertTestXxx(), checkTestXxx(), externTest(), externTesting().
* v0.2.0 (2018-03-16)
    * TestRunner can time out long running tests. Default time out is 10
      seconds, but is configurable using TestRunner::setTimeout().
* v0.1.1 (2018-03-15)
    * Fix small bug with Test::setPassOrFail() which caused assertXxx()
      macros which returned true to terminate the testing() test cases.
* v0.1.0 (2018-03-15)
    * First merge into 'master' branch and tagged.
* (2018-03-12)
    * Initial upload to GitHub.
