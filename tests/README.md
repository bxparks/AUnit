# Unit Tests

It may seem that we have a circular referencing problem (or a tautology) by
using AUnit to write unit tests for itself. It could be argued that we're can't
really be sure that the code is actually testing anything since the same bug
could be validating itself. But I built `AUnitTest` incrementally in layers,
first validating the first layer of primitive functions (e.g.
`compareString()`), then using that to test the next layer (e.g.
`compareEqual()`), then using that to test the next layer (e.g.
`assertEqual()`), then using that to test meta-assertions (e.g.
`assertTestEqual()`), and so on. In practice, I think these tests are better
than nothing. At the very least, it verifies that the library compiles and runs
on the various supported platforms.
