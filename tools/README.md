# Build Script for Arduino Command Line

A shell wrapper around the
[Arduino Commandline Interface](https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc)
which allows programmatic (unattended) workflows:

1) Verifying (compile) multiple `*.ino` files across multiple boards.
2) Uploading multiple `*.ino` files across multiple boards.
3) Testing multiple [AUnit](https://github.com/bxparks/AUnit) unit tests
across multiple boards.
4) Monitoring the serial monitor after uploading the sketch to a board.

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
uploading the sketch using the `serial_monitor.py` helper script.
* If the sketch is a unit test written in AUnit, the `serial_monitor.py`
can parse the Serial output to determine if the unit test passed or failed.
The shell script collects the results of multiple unit tests and prints
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

The `build_arduino.sh` script depends on the
[Arduino IDE](https://arduino.cc/en/Main/Software) being installed
(tested with 1.8.5).

The `serial_monitor.py` script depends on
[pyserial](https://pypi.org/project/pyserial/) (tested with 3.4-1). On
Ubuntu Linux, you may be able to install this using one of:

* `sudo apt install python-serial`, or
* `sudo -H pip install pyserial`

There is one environment variable that **must** be defined in your `.bashrc`
file:

* `export BUILD_ARDUINO_BINARY={path}` - location of the Arduino command line
  binary

A second environment variable is optional and overrides the location of the
board alias config file (see **Board Aliases** section below).. The default is
`$HOME/.build_arduino_config` but can be overriden by the `BUILD_ARDUINO_CONFIG`
variable:

* `export BUILD_ARDUINO_CONFIG={path}` - location of the `.build_arduino_config`
  configuration file

## Usage

Type `build_arduino.sh --help` to get the latest usage:
```
$ ./build_arduino.sh --help
Usage: build_arduino.sh [--help] [--verbose]
    [--verify | --upload | --test | --monitor]
    [--board {package}:{arch}:{board}[:parameters]]
    [--port /dev/ttyUSB0] [--baud baud]
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
$ ./build_arduino.sh \
  --board arduino:avr:nano:cpu=atmega328old --verify Blink.ino
```

### Upload

To upload the sketch to the Arduino board, we need to provide the
`--port` flag:

```
$ ./build_arduino.sh --port /dev/ttyUSB0 \
  --board arduino:avr:nano:cpu=atmega328old --upload Blink.ino
```

### Test

To run the AUnit test and verify pass or fail:
```
$ ./build_arduino.sh --port /dev/ttyUSB0 \
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
$ ./build_arduino.sh --port /dev/ttyUSB0 \
  --board arduino:avr:nano:cpu=atmega328old --monitor BlinkTest.ino
```

The `serial_monitor.py` times out after 10 seconds if the serial monitor is
inactive. If the sketch continues to output something to the serial monitor,
then only one sketch can be monitored.

## Usage of Serial Monitor Script

The `serial_monitor.py` is normally called from the `build_arduino.sh` script.
However it has some capabilities of its own. The most useful is the
ability to list the available tty ports.

### List Ports

The `--list` flag will list the available tty ports. This option is the default
so you can just type:
```
$ ./serial_monitor.py
/dev/ttyS4 - n/a
/dev/ttyS0 - ttyS0
/dev/ttyUSB0 - EzSBC ESP32
/dev/ttyUSB1 - USB2.0-Serial
```

### Monitor the Serial Port

The `--monitor` causes `serial_monitor.py` to read lines from the given `--port`
at the speed of `--baud`, and echo the lines to the STDOUT. If no lines
are detected after 10 seconds, the python script terminates.

### Files and Directory

If the `build_arduino.sh` is given a directory `dir`, it tries to find
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
espressif:esp32:esp32:PartitionScheme=default,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none`.
```

It is likely that not all the extra parameters are needed, but it is not
easy to figure out which ones can be left out.

Instead of using the `fqbn`, the `build_arduino.sh` script allows
the user to define aliases for the `fqbn` in a file. The format of the file is
the [INI file](https://en.wikipedia.org/wiki/INI_file), and the aliases are
in the `[boards]` section:
```
# Board aliases
[boards]
  uno = arduino:avr:uno
  nano = arduino:avr:nano:cpu=atmega328old
  leonardo = arduino:avr:leonardo
  esp8266 = esp8266:esp8266:nodemcuv2
  esp32 = espressif:esp32:esp32:PartitionScheme=default,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none
```

The format of the config key is not precisely defined, but it should probably be
limited to the usual character set for identifiers (`a-z`, `A-Z`, `0-9`,
underscore `_`). It definitely cannot contain an equal sign `=` or space ` `
character.

Save the alias list into the `$HOME/.build_arduino_config` file in your
home directory. (The location of the config file can be
changed using the `BUILD_ARDUINO_CONFIG` environment variable.)

## Multiple Boards

The board aliases can be used in the `--boards` flag, which accepts a
comma-separated list of `{alias}[:{port}]` pairs, like this:

```
$ ./build_arduino.sh --test \
  --boards nano:/dev/ttyUSB1,leonardo:/dev/ttyACM0 BlinkTest.ino
```

This runs the `BlinkTest.ino` test on 2 boards:
* an Arduino Nano on `/dev/ttyUSB1` and,
* an Arduino Leonardo (or a Micro clone) on `/dev/ttyACM0`.

The `port` part of the `alias:port` pair is optional because it is not needed
for the `--verify` mode. You can verify sketches across multiple boards like
this:

```
$ ./build_arduino.sh --verify \
  --boards nano,leonardo,esp8266,esp32 BlinkTest.ino
```

## System Requirements

I used Ubuntu 17.10 and Arduino IDE 1.8.5 to develop and test these scripts.

## Limitations

On MacOS, the [Teensyduino](https://pjrc.com/teensy/teensyduino.html)
plugin to support Teensy boards
causes the Arduino IDE to display
[a security warning dialog box](https://forum.pjrc.com/threads/27197-OSX-pop-up-when-starting-Arduino).
This means that the script is no longer able to run without human-intervention.

On Linux, using Teensyduino, the upload function does work and the
`serial_monitor.py` is not able to monitor the serial output. Teensyduino seems
to be using a different program loader, and I have not spent much time debugging
this.
