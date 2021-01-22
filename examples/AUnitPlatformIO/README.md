# AUnit on PlatformIO

This is an example unit test that runs on the PlatformIO environment. Created to
track down [AUnit#50](https://github.com/bxparks/AUnit/issues/50) where
`EpoxyDuino/` directory seems to break PlatformIO because it tries to compile
it. Unfortunately, I cannot reproduce it. This seems to work fine on my machine.

## Updating PlatformIO

Here are the various pio commands that I had to run to update
PlatformIO:

```
$ pio upgrade
$ pio update
$ pio lib update
```

## Compile on All Environments

```
$ pio run -t clean
$ pio run
```

## Upload and Monitor

Here is the commnad to compile, upload and monitor the test to an Arduino Nano
on /dev/ttyUSB0:

```
$ pio run -e nano -t upload --upload-port /dev/ttyUSB0 && \
    pio device monitor -b 115200 -p /dev/ttyUSB0
```

Here is the commnad to compile, upload and monitor the test to an ESP32
on /dev/ttyUSB1:

```
$ pio run -e esp32 -t upload --upload-port /dev/ttyUSB1 && \
    pio device monitor -b 115200 -p /dev/ttyUSB1
```

## Compile and Run on Linux or MacOS

```
$ make clean
$ make
$ ./AUnitPlatformIO.out
```
