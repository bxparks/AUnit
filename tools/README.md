# Build Script for Arduino Command Line

A shell wrapper around the arduino(1) program with the following
additional features:

1) Supports 3 modes: verify (`--verify`), upload (`--upload`), and test
(`--test`).
2) Multiple `*.ino` files can be given, and the arduino(1) binary will be
executed for each sketch.
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
entire set of `*.ino` files are run through the arduino command line program for
each board/port pair. This is useful for running a set of unit tests across
multiple board types.

## Usage

The script needs to be given 3 pieces of information:
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
$ ./build_arduino.sh --port /dev/ttyUSB0
  --board arduino:avr:nano:cpu=atmega328old --upload Blink.ino
```

### Test

To run the AUnit test and verify pass or fail:
```
$ ./build_arduino.sh --port /dev/ttyUSB0 \
  --board arduino:avr:nano:cpu=atmega328old --test BlinkTest.ino
```

## Board Aliases

The Arduino command line binary wants a fully-qualified specification for the
`--board` flag. It can be quite cumbersome to determine this value. One way is
to set the "Show verbose output during compilation and upload" checkboxes in the
Arduino IDE, then look for the value of the `-fqbn` flag generated in the debug
output. Another way is to track down the `hardware/.../boards.txt` file (there
may be multiple verisons), open it up, and try to reverse engineer the `fqbn` of
a particular Arduino board.

On some boards, the `fqbn` may be quite long. For example, on my ESP32
board, it is
```
espressif:esp32:esp32:PartitionScheme=default,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none`.
```

It is likely that not all the extra parameters are needed, but it is not
easy to figure out which ones can be left out.

Instead of using the `fqbn`, the `build_arduino.sh` script allows
the user to define aliases for the `fqbn` in a file. The format of the file is
a `key=value` pair, like this:
```
uno=arduino:avr:uno
nano=arduino:avr:nano:cpu=atmega328old
leonardo=arduino:avr:leonardo
esp8266=esp8266:esp8266:nodemcuv2
esp32=espressif:esp32:esp32:PartitionScheme=default,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none
```

Create a dotfile (e.g. `$HOME/.build_arduino_config`) in your home directory,
and set the `BUILD_ARDUINO_CONFIG` environment variable in your `.bashrc`
file, like this:
```
export BUILD_ARDUINO_CONFIG="$HOME/.build_arduino
```

The `--boards` flag accepts a comma-separated list of `{alias}:{port}`
pairs, like this:

```
$ ./build_arduino.sh --test \
  --boards nano:/dev/ttyUSB1,leonardo:/dev/ttyACM0 BlinkTest.ino
```

This runs the `BlinkTest.ino` test on 2 boards, an Arduino Nano on
`/dev/ttyUSB1` and an Arduino Leonardo (or a Micro clone) on `/dev/ttyACM0`.

## Environment Variables

There are 2 environment variables that **must** be defined in your `.bashrc`
file:

* `export BUILD_ARDUINO_BINARY={path}` - location of the Arduino command line
  binary
* `export BUILD_ARDUINO_CONFIG={path}` - location of the `.build_arduino_config`
  configuration file

## System Requirements

The `serial_monitor.py` script depends on `pyserial` (tested with 3.4-1).
The `build_arduino.sh` script depends on Arduion IDE being installed
(tested with 1.8.5).

I used Ubuntu 17.10 and Arduino IDE 1.8.5 to develop and test these scripts.

I have tested it somewhat on MacOS. The inclusion of the Teensyduino plugin
causes the Mac version of Arduino to display a security warning dialog box,
which interferes with the command-line usage of this script.
