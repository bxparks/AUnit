# UnitDuino

This directory contains a minimal implementation of the Arduino development
framework so that unit tests written in AUnit can be compiled and tested on a
Linux or MacOS machine, instead of running on the embedded microcontroller.

The build process uses [GNU Make](https://www.gnu.org/software/make/manual/).
A `Makefile` needs to be created inside the sketch folder. For example, if the
sketch is `SampleTest/SampleTest.ino`, then the makefile should be
`SampleTest/Makefile`. The unit test is compiled with just a `make` command. It
produces an executable with a `.out` extension, for example, `SampleTest.out`.

Running the AUnit test natively on Linux or MacOS has some advantages, and
these were the reasons why I created this package:

* The development cycle can be lot faster.
* The desktop machine can run unit tests which are too big or consume too
  much memory to fit inside an embedded microcontroller.

The disadvantages are:

* Only a limited set of Arduino functions are supported (see below), so
  not all AUnit tests will compile on the desktop.
* There may be compiler differences between the desktop and the embedded
  environments (e.g. 8-bit integers versus 64-bit integers).

## How to Use

The `Makefile` should have 3 lines:
```
APP_NAME := SampleTest
ARDUINO_LIBS := {list of dependent Arduino libraries}
include {path/to/AUnit}/unitduino/unitduino.mk
```

To build the unit test, just run `make`:
```
$ cd SampleTest
$ make clean
$ make
```

The executable will be created with a `.out` extension. To run it, just type:
```
$ ./SampleTest.out
```

The AUnit output that would normally be printed on the `Serial` on an Arduino
board will be sent to the `STDOUT` of the Linux or MacOS terminal. The output
should be identical to what would be shown on the serial port of the Arduino
controller.

## Supported Arduino Features

The following functions and features of the Arduino framework are implemented:

* `setup()`
* `loop()`
* `delay()`
* `yield()`
* `millis()`
* `Serial`
* `Serial.print()`
* `Serial.println()`
* `String`
* `Print`
* `Stream` (untested)
* `<pgmspace.h>` (limited)
* `PROGMEM`
* `F()` macro
* `EEPROM` (compilation only)

## Additional Arduino Libraries

The dependency to the `AUnit` library is always included by default.
If the unit test depends on additional Arduino libraries, they must be specified
in the `Makefile` using the `ARDUINO_LIBS` parameter. The libraries are referred
to by their base directory name (e.g. `AceButton`, or `AceRoutine`) not the full
path. The `unitduino.mk` file will look for these additional libraries at the
same level as the `AUnit` directory itself. (We assume that the `unitduino`
directory is always located at `AUnit/unitduino/`). This search location can be
changed by the user using the `ARDUINO_LIB_DIR` environment variable. If this is
set, then `make` will use this directory to look for the additional libraries,
instead of the parent directory of `AUnit`.
