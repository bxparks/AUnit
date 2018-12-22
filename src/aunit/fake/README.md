# Fake Arduino Classes

This directory contains fake versions of some Arduino classes for the purposes
of unit testing. These header files are *not* included automatically by the
`#include <AUnit.h>` preprocessor directive. The various fake classes must be
included manually just after the `<AUnit.h>` is included. The fake classes
live in the `aunit::fake` namespace.

## FakePrint

The `FakePrint` class is an implementation of the
[Print](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Print.h)
class which is the base class of the
[HardwareSerial](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.h)
and other output classes. The `Serial` global object is an instance of the
`HardwareSerial` class. If a user-defined class or method is defined to use an
instance of the `Print` object, instead of hardcoding a dependency to the
`Serial` instance, then an instance of the `FakePrint` class can be substituted
and used to write unit tests for the user-defined class or method.

Let's say the user-defined class is called `Greeter` and a method called
`greet()` prints something out to `Serial`:

```C++
class Greeter {
  public:
    Greeter(Print& printer):
      mPrinter(printer) {}

    void greet(int n) {
      mPrinter.print("hello ");
      mPrinter.print(n);
      mPrinter.println(" world(s)"); 
    }

  private:
    Print& mPrinter;
};
```

If we want to verify that the `Greeter::greet()` method prints what is expected,
we inject an instance of `FakePrint` (instead of `Serial`) in a test called
`GreeterTest.ino`:

```C++
#line 2 "GreeterTest.ino"

#include <AUnit.h>
#include <aunit/fake/FakePrint.h>

using namespace aunit;
using namespace aunit::fake;

test(GreeterTest, greet) {
  FakePrint fakePrint;
  Greeter greeter(fakePrint);

  greeter.greet(1);
  assertEqual("hello 1 world(s)\r\n", fakePrint.getBuffer());
  fakePrint.flush();

  greeter.greet(2);
  assertEqual("hello 2 world(s)\r\n", fakePrint.getBuffer());
  fakePrint.flush();
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
}

void loop() {
  TestRunner::run();
}
```
