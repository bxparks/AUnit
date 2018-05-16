# How to Contribute

Thank you for your interest in this library. I do appreciate any constructive
comments or suggestions about this library. If you would like to contribute
code, please do the following for non-trivial changes:

1. Create an [issue](https://github.com/bxparks/AUnit/issues) or send me
   an email so that I have some advance warning on what you would like to
   change.
1. Please rebase your branch off the 'develop' branch, and preferably squash all
   your changes into a single commit so that it's easier to review.

## Coding Style

I use the following style for this library. If you follow the same
style for consistency, the diffing is much easier.

* formatting
    * 80 column lines
        * _rationale_: I often use vertically split, side-by-side editing on my
          small laptop screen.
    * 2 space indents, no tabs
    * 4 space indents for continuation lines (helps to separate a long
      statement from the next statement indented by 2-spaces)
    * no trailing white spaces
* spacing
    * consistent and generous spaces around operators and symbols
        * e.g. `for (int i = 0; i < 10; i++) {`
        * e.g. `a = (condition) ? 3 : -1;`
        * _rationale_: Helps readability.
    * space after language keywords: e.g. `for`, `while`, `if`, etc
  * no space after function names
* pointer declaration `*` and references `&` attached to the class, not the
  variable
    * e.g. `const char* s`, not `const char *s`
    * e.g. `const String& s`, not `const String &s`
    * _rationale_: I know the latter could be argued to be technically more
      correct under the C/C++ syntax, but I think the former is more intuitive
      for many people.
    * I've personally gone back and forth, and I decided to just pick a style.
* only one variable declaration per line
    * e.g. `int i, j;` not allowed, use 2 lines
    * _rationale_: Helps readability, and avoids the confusion of
     `const char* s, *t;` caused by the previous rule.
* open brace on the same line as the function name (Java style)
    * I realize that putting the opening `{` of a function on the next line
      adds an extra blank space that helps readability.
    * But after you get used to the brace on the same line, you hardly notice
      the extra vertical space anymore, and you pay more attention
      to the indentation level, instead of the curly braces.
* naming conventions
    * macros: UPPER_CASE (usually), lowerCase (AUnit)
        * AUnit violates the UPPER_CASE rule by naming the various
          `test()`, `testing()`, `assertXxx()` macros using lowerCase. This
          came from ArduinoUnit and probably cannot be changed.
        * The problem with macros is that they live in a global namespace
          and naming conflicts can happen.
    * class names: CamelCase
        * e.g. `MyClass`, `YourClass`
    * methods: lowerCamelCase
        * e.g. `doSomething()`, `isCondition()`, etc
        * _rationale_: Seems like the Arduino convention. Helps readability.
    * class static constants: 'k' followed by CamelCase
        * e.g. `kSomeConstant`
        * _rationale_: Prevents conflicts with `#define` macros which use the
          `ALL_CAPS_MACRO` pattern. Since AUnit is a library, I cannot predict
          which other libraries may be used by the end-user. If there is a macro
          conflict, I have no way to fix the problem.
        * In user-land codes, `ALL_CAPS` for constants would be ok because if
          there's a conflict, the user can change it.
    * member variables: 'm' followed by CamelCase
        * e.g. `mSomeVariable`
        * _rationale_: Many symbols beginning with a single or double underscore
          `__` are reserved by the C language, C++ language, or their standard
          libraries. So I avoid them completely.
        * One alternative is to append an underscore *after* the variable name.
          But this makes the `->` and the `.` operators hard to read. The 'm'
          prefix seems consistent with the 'k' prefix for constants, and it's
          relatively easy on the eyes.
    * (exception) member variables of a `struct`: lowerCamelCase
        * If the class is a simple data `struct`, and the member variables
          are meant to be accessed directly, then it seems more readable to omit
          adding the `m` prefix and just use `lowerCamelCase`.
    * class static variables: 's' following by CamelCase
        * e.g. `sRootNode`
        * I realize that this looks awkward, but it *is* consistent with
          the naming convention for other variables.
        * Fortunately, class static variables don't appear too often.
    * global variables
        * There ought to be no global variables in this library.
        * If there were any, the naming convention would be 'gCamelCase'.
* [doxygen](http://www.doxygen.org) comments for basically all public and
  protected methods and constants
    * Comments are recommended for private methods (and non-trivial variables)
      as well, since private methods have a habit of becoming `protected` or 
      `public`.

I guess the final request is: please make the new code consistent with the old
code in the file.

## Unit Tests

The unit tests for AUnit are in the `tests/` directory and are written
in AUnit itself. Any non-trivial change should have a unit test. Even a
seemingly trivial change can often use a unit test to prevent typos.

I had to split the unit tests into multiple `*.ino` files because they became
too big to fit into the 32KB flash memory space of an Arduino UNO or Nano board.

## Authorship and License

Please release your code under the same MIT License as the rest of the library.
Add the MIT License text at the top of the file with the copyright year and your
name.
