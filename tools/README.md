# Build Script for Arduino Command Line

A shell wrapper around the [Arduino Commandline Interface]
(https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc)
with the following additional features:

1) Supports 3 modes: verify (`--verify`), upload (`--upload`), and test
(`--test`).
2) Multiple `*.ino` files can be given, and the Arduino Commandline binary will
be executed for each sketch.
3) A directory can be given and the script will infer the corresponding
`*.ino` file under that directory.
4) Board aliases can be defined in a user-defined dotfile (e.g.
`$HOME/.build_arduino_config`) which maps a short alias (e.g. "nano") to the
longer board spec used by the arduino binary (e.g.
"arduino:avr:nano:cpu=atmega328old").
5) The script can monitor the serial output of the board immediately after
uploading the sketch using the `serial_monitor.py` helper script.
6) If the sketch is a unit test written in AUnit, the `serial_monitor.py`
can parse the Serial output to determine if the unit test passed or failed.
The shell script collects the results of multiple unit tests and prints
a summary at the end.
7) If multiple board/port pairs are given using the `--boards` flag, then the
entire set of `*.ino` files are run through the Arduino command line program for
each board/port pair. This is useful for running a set of unit tests across
multiple board types.

## Installation

There is one environment variable that **must** be defined in your `.bashrc`
file:

* `export BUILD_ARDUINO_BINARY={path}` - location of the Arduino command line
  binary

A second environment variable is optional and overrides the location of
the board alias config file. The default is `$HOME/.build_arduino_config`
but can be overriden by `BUILD_ARDUINO_CONFIG`:

* `export BUILD_ARDUINO_CONFIG={path}` - location of the `.build_arduino_config`
  configuration file (see **Board Aliases** section below).

The `build_arduino.sh` script depends on the
[Arduino IDE](https://arduino.cc/en/Main/Software) being installed
(tested with 1.8.5).

The `serial_monitor.py` script depends on
[pyserial](https://pypi.org/project/pyserial/) (tested with 3.4-1). On
Linux, you may be able to install this using one of:

* `sudo apt install python-pyserial`
* `sudo -H pip install pyserial`

## Usage

Type `build_arduino.sh --help` to get the latest usage:
```
$ build_arduino.sh [--help] [--verbose] [--verify | --upload | --test ]
    [--monitor] [--port /dev/ttyUSB0] [--baud baud]
    [--board {package}:{arch}:{board}[:parameters]]
    [--boards {alias}:{port},...] (file.ino | dir) [...]
```

At a minimum, the script needs to be given 4 pieces of information:

* `--port port` The tty port where the Arduino board can be found
* `--board board` The identifier for the particular board in the form
    of `{package}:{arch}:{board}[:parameters]`.
* mode (`--verify`, `--upload`, `--test`) The mode determines the actions
    performed. Verify checks for compiler errors. Upload pushes the sketch
    to the board. Test runs the sketch as an AUnit unit test and verifies that
    it passes.
* `file.ino` The Arduino sketch file.

### Verify

The following example verifies that the `Blink.ino` sketch compiles:
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

### List Ports

The `serial_monitor.py` is normally called from the `build_arduino.sh`
script. However, it has the ability to list the available tty ports
using the `--list` flag:
```
$ ./serial_monitor.py --list
```

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
a list of `key=value` pairs, one per line, that look like this:
```
uno=arduino:avr:uno
nano=arduino:avr:nano:cpu=atmega328old
leonardo=arduino:avr:leonardo
esp8266=esp8266:esp8266:nodemcuv2
esp32=espressif:esp32:esp32:PartitionScheme=default,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none
```

(The name of the alias can be composed of the usual characteres, such as `a-z`,
`A-Z`, `0-9`, underscore `_`, or a period `.`,  but it cannot contain
an `=` or space ` ` character.)

Save the alias list into the `$HOME/.build_arduino_config` file in your
home directory. (The location of the config file can be
changed using the `BUILD_ARDUINO_CONFIG` environment variable.)

The board aliases can be used in the `--boards` flag, which accepts a
comma-separated list of `{alias}:{port}` pairs, like this:

```
$ ./build_arduino.sh --test \
  --boards nano:/dev/ttyUSB1,leonardo:/dev/ttyACM0 BlinkTest.ino
```

This runs the `BlinkTest.ino` test on 2 boards:
* an Arduino Nano on `/dev/ttyUSB1` and,
* an Arduino Leonardo (or a Micro clone) on `/dev/ttyACM0`.

## System Requirements

I used Ubuntu 17.10 and Arduino IDE 1.8.5 to develop and test these scripts.

I have tested it somewhat on MacOS. The inclusion of the Teensyduino plugin
causes the Mac version of Arduino to display a security warning dialog box,
which interferes with the command-line usage of this script.
