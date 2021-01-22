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

## Running the Tests

The best way to run these tests is to use
[EpoxyDuino](https://github.com/bxparks/EpoxyDuino) on a Linux or MacOS
machine.

```
$ make clean
$ make tests

$ make runtests | grep failed
TestRunner summary: 15 passed, 0 failed, 3 skipped, 0 timed out, out of 18 test(s).
TestRunner summary: 4 passed, 0 failed, 0 skipped, 0 timed out, out of 4 test(s).
TestRunner summary: 24 passed, 0 failed, 0 skipped, 0 timed out, out of 24 test(s).
TestRunner summary: 4 passed, 0 failed, 3 skipped, 0 timed out, out of 7 test(s).
TestRunner summary: 5 passed, 0 failed, 0 skipped, 0 timed out, out of 5 test(s).
```

## Deliberately Failing Tests

The following tests contain tests which deliberately fail or time out
because that's what they are testing:

* `FailingTest`
* `SetupAndTeardownTest`

These cannot be run in a continuous integration suite. I need to run these tests
manually and verify as best as I can that the failing tests are failing as
expected.

```
$ make clean
$ make tests

$ make runfailingtests
```

Verify that we get something like the following:
```
TestRunner summary: 6 passed, 5 failed, 1 skipped, 4 timed out, out of 16 test(s).
TestRunner summary: 2 passed, 2 failed, 4 skipped, 2 timed out, out of 10 test(s).
```

## Continuous Integration

Prior to v1.4, I used [AUniter](https://github.com/bxparks/AUniter) with
[Jenkins](https://jenkins.io) to run continuous integration on these tests.
However, the Arduino IDE has shown to be too slow and too flaky to be
used as the basis for continuous integration.

Starting from v1.4, I use
[EpoxyDuino](https://github.com/bxparks/EpoxyDuino) to run the unit tests
on a Linux machine. These are integrated into the GitHub workflow for continuous
integration.
