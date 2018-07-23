# Unit Tests

It may seem that we have a circular dependency problem by using AUnit to write
unit tests for itself. Fortunately, I built these unit tests incrementally in
layers, first validating the lowest layer of primitive functions (e.g.
`compareString()`), then using that to test the next layer (e.g.
`compareEqual()`), then using that to test the next layer (e.g.
`assertEqual()`), then using that to test meta-assertions (e.g.
`assertTestEqual()`), and so on. In practice, I think these tests are better
than nothing. At the very least, it verifies that the library compiles and runs
on the various supported platforms.

## Deliberately Failing Tests

The following tests contain tests which deliberately fail or time out
because that's what they are testing:

* `FailingTest`
* `SetupAndTeardownTest`

These cannot be run in a continuous integration suite. I need to run these tests
manually and verify as best as I can that the failing tests are failing as
expected.

## Continuous Integration

See [AUniter](https://github.com/bxparks/AUniter) to see how
[Jenkins](https://jenkins.io) is used to run continuous integration
on these tests.
