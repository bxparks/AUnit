# Build Script for Arduino Command Line

The `auniter.sh` shell is a wrapper around the
[Arduino Commandline Interface](https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc)
that allows programmatic (unattended) workflows:

1) Verifying (compile) multiple `*.ino` files across multiple boards.
2) Uploading multiple `*.ino` files across multiple boards.
3) Testing multiple [AUnit](https://github.com/bxparks/AUnit) unit tests
across multiple boards.
4) Monitoring the serial monitor after uploading the sketch to a board.
5) List the tty ports and the associated Arduino boards (if available).

## Features

* Supports 4 modes: verify (`--verify`), upload (`--upload`), test
(`--test`), and monitor (`--monitor`).
* Multiple `*.ino` files can be given, and the Arduino Commandline binary will
be executed for each sketch in sequence.
* A directory can be given and the script will infer the corresponding
`*.ino` file under that directory.
* User-defined board aliases allow mapping of a short alias (e.g. "nano") to the
fully qualified board name (`fqbn`) used by the arduino binary (e.g.
"arduino:avr:nano:cpu=atmega328old").
* The script can monitor the serial output of the board immediately after
uploading the sketch.
* If the sketch is a unit test written in AUnit, the `serial_monitor.py`
helper script can parse the Serial output to determine if the unit test passed
or failed. The shell script collects the results of multiple unit tests and
prints
a summary at the end.
* If multiple `board:port` pairs are given using the `--boards` flag, then the
entire set of `*.ino` files are run through the Arduino command line program for
each `board:port` pair. This is useful for verifying, uploading or testing
sketches across multiple board types.

## Requirements

These scripts are meant to be used from a Linux environment with the
following core packages installed:
* bash
* python3

The `auniter.sh` script depends on the
[Arduino IDE](https://arduino.cc/en/Main/Software) being installed
(tested with 1.8.5).

The `serial_monitor.py` script depends on
[pyserial](https://pypi.org/project/pyserial/) (tested with 3.4-1). On
Ubuntu Linux, you may be able to install this using one of:

* `sudo apt install python-serial`, or
* `sudo -H pip install pyserial`

There is one environment variable that **must** be defined in your `.bashrc`
file:

* `export AUNITER_ARDUINO_BINARY={path}` - location of the Arduino command line
  binary

I have something like this in my `$HOME/.bashrc` file (which I share across all
my Linux and Mac computers):
```
case $(uname -s) in
  Linux*)
    export AUNITER_ARDUINO_BINARY="$HOME/dev/arduino-1.8.5/arduino"
    ;;
  Darwin*)
    export AUNITER_ARDUINO_BINARY="$HOME/dev/Arduino.app/Contents/MacOS/Arduino"
    ;;
  *)
    export AUNITER_ARDUINO_BINARY=
    ;;
esac
```

A second environment variable is optional and overrides the location of the
board alias config file (see **Board Aliases** section below). The default is
`$HOME/.auniter_config` but can be overriden by the `AUNITER_CONFIG`
variable:

* `export AUNITER_CONFIG={path}` - location of the `.auniter_config`
  configuration file

## Usage

Type `auniter.sh --help` to get the latest usage:
```
$ ./auniter.sh --help
Usage: auniter.sh [--help] [--verbose]
    [--verify | --upload | --test | --monitor | --list_ports]
    [--board {package}:{arch}:{board}[:parameters]] [--port port] [--baud baud]
    [--boards {alias}[:{port}],...] (file.ino | dir) [...]
```

At a minimum, the script needs to be given 3-4 pieces of information:

* mode (`--verify`, `--upload`, `--test`, `--monitor`) The mode determines the
  actions performed. Verify checks for compiler errors. Upload pushes the sketch
  to the board. Test runs the sketch as an AUnit unit test and verifies that it
  passes. Monitor uploads the sketch then echos the Serial output to the STDOUT.
* `--board board` The identifier for the particular board in the form
  of `{package}:{arch}:{board}[:parameters]`.
* `--port port` The tty port where the Arduino board can be found. This is
  optional for the `--verify` mode which does not need to connect to the board.
* `file.ino` The Arduino sketch file.

### Verify

The following example verifies that the `Blink.ino` sketch compiles. The
`--port` flag is not necessary in this case:

```
$ ./auniter.sh \
  --board arduino:avr:nano:cpu=atmega328old --verify Blink.ino
```

### Upload

To upload the sketch to the Arduino board, we need to provide the
`--port` flag:

```
$ ./auniter.sh --port /dev/ttyUSB0 \
  --board arduino:avr:nano:cpu=atmega328old --upload Blink.ino
```

### Test

To run the AUnit test and verify pass or fail:
```
$ ./auniter.sh --port /dev/ttyUSB0 \
  --board arduino:avr:nano:cpu=atmega328old --test BlinkTest.ino
```

A summary of all the test runs are given at the end, like this:

```
[...]
======== Test Run Summary
PASSED test: arduino:avr:nano:cpu=atmega328old /dev/ttyUSB1 AceSegment/tests/CommonTest/CommonTest.ino
PASSED test: arduino:avr:nano:cpu=atmega328old /dev/ttyUSB1 AceSegment/tests/DriverTest/DriverTest.ino
PASSED test: arduino:avr:nano:cpu=atmega328old /dev/ttyUSB1 AceSegment/tests/LedMatrixTest/LedMatrixTest.ino
PASSED test: arduino:avr:nano:cpu=atmega328old /dev/ttyUSB1 AceSegment/tests/RendererTest/RendererTest.ino
PASSED test: arduino:avr:nano:cpu=atmega328old /dev/ttyUSB1 AceSegment/tests/WriterTest/WriterTest.ino
ALL PASSED
```

The `ALL PASSED` indicates that all unit tests passed.

### Monitor (after Uploading)

The `--monitor` mode uploads the given sketch and calls `serial_monitor.py`
to listen to the serial monitor and echo the output to the STDOUT:
```
$ ./auniter.sh --port /dev/ttyUSB0 \
  --board arduino:avr:nano:cpu=atmega328old --monitor BlinkTest.ino
```

The `serial_monitor.py` times out after 10 seconds if the serial monitor is
inactive. If the sketch continues to output something to the serial monitor,
then only one sketch can be monitored.

### List Ports

The `--list_ports` flag will ask `serial_monitor.py` to list the available tty
ports:
```
$ ./auniter.sh --list_ports
/dev/ttyS4 - n/a
/dev/ttyS0 - ttyS0
/dev/ttyUSB0 - EzSBC ESP32
/dev/ttyUSB1 - USB2.0-Serial
```

### Automatic Directory Expansion

If the `auniter.sh` is given a directory `dir`, it tries to find
an ino file located at `dir/dir.ino`, since the ino file must have the
same base name as the parent directory.

Multiple files and directories can be given. The Arduino Commandline will
be executed on each of the ino files in sequence.

## Board Aliases

The Arduino command line binary wants a fully-qualified board name (fqbn)
specification for the `--board` flag. It can be quite cumbersome to determine
this value. One way is to set the "Show verbose output during compilation and
upload" checkboxes in the Arduino IDE, then look for the value of the `-fqbn`
flag generated in the debug output. Another way is to track down the
`hardware/.../boards.txt` file (there may be several verisons), open it up, and
try to reverse engineer the `fqbn` of a particular Arduino board.

On some boards, the `fqbn` may be quite long. For example, on my ESP32 dev
board, it is
```
espressif:esp32:esp32:PartitionScheme=default,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none
```

It is likely that not all the extra parameters are needed, but it is not
easy to figure out which ones can be left out.

Instead of using the `fqbn`, the `auniter.sh` script allows
the user to define aliases for the `fqbn` in a file. The format of the file is
the [INI file](https://en.wikipedia.org/wiki/INI_file), and the aliases are
in the `[boards]` section:
```
# Board aliases
[boards]
  uno = arduino:avr:uno
  nano = arduino:avr:nano:cpu=atmega328old
  leonardo = arduino:avr:leonardo
  esp8266 = esp8266:esp8266:nodemcuv2:CpuFrequency=80,FlashSize=4M1M,LwIPVariant=v2mss536,Debug=Disabled,DebugLevel=None____,FlashErase=none,UploadSpeed=115200
  esp32 = espressif:esp32:esp32:PartitionScheme=default,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none
```

The format of the alias name is not precisely defined, but it should probably be
limited to the usual character set for identifiers (`a-z`, `A-Z`, `0-9`,
underscore `_`). It definitely cannot contain an equal sign `=` or space ` `
character.

Save the alias list into the `$HOME/.auniter_config` file in your
home directory. (The location of the config file can be
changed using the `AUNITER_CONFIG` environment variable.)

## Multiple Boards

The board aliases can be used in the `--boards` flag, which accepts a
comma-separated list of `{alias}[:{port}]` pairs.

The `port` part of the `alias:port` pair is optional because it is not needed
for the `--verify` mode. You can verify sketches across multiple boards like
this:

```
$ ./auniter.sh --verify \
  --boards nano,leonardo,esp8266,esp32 BlinkTest.ino
```

If you want to run the AUnit tests on multiple boards, you must provide the
port of each board, like this:
```
$ ./auniter.sh --test \
  --boards nano:/dev/ttyUSB0,leonardo:/dev/ttyACM0,esp8266:/dev/ttyUSB2,esp32:/dev/ttyUSB1 \
  CommonTest DriverTest LedMatrixTest RendererTest WriterTest
```

This runs the 5 unit tests on 4 boards connected to the ports specified by the
`--boards` flag.

It did not seem worth providing aliases for the ports in the
`$HOME/.auniter_config` file because the specific serial port is assigned by the
OS and can vary depending on the presence of other USB or serial devices.

## Alternatives Considered

### AMake

The [amake](https://github.com/pavelmc/amake) tool is very similar to
`auniter.sh`. It is a shell script that calls out to the Arduino commandline.
Amake does not have the `serial_monitor.py` which allows the AUnit output on the
serial port to be validated, but since `serial_monitor.py` is a separate
Python script, `amake` could be extended to support it.

There are a few features of `amake` that I found problemmatic for my purposes.
* Although `amake` supports the concept of board aliases, the aliases are
hardwared into the `amake` script itself. I felt that it was important to allow
users to define their own board aliases (through the `.auniter_config` dotfile).
* `amake` saves the information about the most recent `*.ino` file and
board type in a cache file named `.amake` in the current directory. This was
designed to make it easy to compile and verify a single INO file repeatedly.
However, `auniter.sh` is designed to make it easy to compile, upload, and
validate multiple INI files, on multiple Arduino boards, on multiple serial
ports.

### Arduino-Makefile

The [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) package
provides a way to create traditional Makefiles and use the traditional `make`
command line program to compile an Arduino sketch. On Ubuntu Linux,
this package can be installed using the normal `apt` program as:
```
$ sudo apt install arduino-mk
```

It installs a dependency called
[arduino-core](https://packages.ubuntu.com/search?keywords=arduino-core).
Unfortunately, the version on Ubuntu is stuck at Arduino version 1.0.5
and the process for upgrading been
[stuck for years](https://github.com/arduino/Arduino/pull/2703).

It is possible to make Arduino-Makefile use the latest Arduino IDE however.

The problem with `Arduino-Makefile` is that it seems to allow only a single
board type target in the Makefile. Changing the target board would mean editting
the `Makefile`. Since I wanted to be able to easily compile, upload and validate
against multiple boards, the `Makefile` solution did not seem to be flexible
enough.

### Arduino Builder

The [Arduino Builde](https://github.com/arduino/arduino-builder) seems to be a
collection of Go-lang programs that provide commandline interface for compiling
Arduino sketches. However, I have not been able to find any documentation that
describes how to actually to use these programs. Maybe eventually I'll be able
to reverse-engineer it, but for now, it was easier for me to write my down shell
script wrapper around the Arduino IDE program.

## System Requirements

I used Ubuntu 17.10 and Arduino IDE 1.8.5 to develop and test these scripts.

## Limitations

On MacOS, the [Teensyduino](https://pjrc.com/teensy/teensyduino.html)
plugin to support Teensy boards
causes the Arduino IDE to display
[a security warning dialog box](https://forum.pjrc.com/threads/27197-OSX-pop-up-when-starting-Arduino).
This means that the script is no longer able to run without human-intervention.
