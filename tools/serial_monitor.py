#!/usr/bin/env python3
#
# Copyright 2018 Brian T. Park <brian@xparks.net>
#
# MIT License
#
"""Monitor the output of the given serial port and echo the output to the
STDOUT. If nothing is seen on the serial output for more than 10 seconds, an
error message is printed.

If the --test flag is given, the output is assumed to come from an AUnit unit
test, and the script validates that the test ran successfully. The script exits
with a status 0 if the test is successful, otherwise exits with a status 1.

Usage:
    serial_monitor.py [--help] [--log_level] [--list | --test | --monitor)
        [--port /dev/ttyPort] [--baud 115200]

Flags:
    --list List the known tty ports. (default)
    --monitor Monitor the serial port and echo the lines to the STDOUT.
    --test Verify an AUnit test suite.
    --port {tty} Set the tty port.
    --baud {baud} Set the baud rate.
    --log_level (INFO|DEBUG|ERROR) Set the logging level.
"""

import argparse
import serial
import serial.tools.list_ports
import logging
import re
from time import sleep

# Logging message format.
LOG_FORMAT = '%(asctime)s %(levelname)s %(name)s: %(message)s'

# Logging date format.
DATE_FORMAT = '%Y-%m-%dT%H:%M:%S%z'

# Time out after this many seconds if the serial port produces no output.
TIMEOUT_ON_IDLE = 10

# Starting point of the number of seconds to wait for the serial port.
# Actual wait time increases using exponential back off.
WAIT_TIME_BASE = 1

# Number attempts to try opening the serial port.
NUM_ATTEMPTS = 4

# Regular expressions that match the start and end of an AUnit test run.
TEST_START_RE = re.compile(r'TestRunner started')
TEST_END_RE = re.compile(r'TestRunner summary.*(\d+) failed.*(\d+) timed out')

# Constants for the test_mode finite state machine
TEST_MODE_UNKNOWN = 0
TEST_MODE_START_FOUND = 1
TEST_MODE_END_SUMMARY_FOUND = 2


def monitor(port, baud):
    """Read the serial output and echo the lines to the STDOUT."""
    logging.info('Reading the serial port %s at %s baud' % (port, baud))
    ser = open_port(port, baud)
    logging.info('Monitoring port %s...' % port)
    try:
        while True:
            line = ser.readline()
            line = line.decode('ascii')
            if line == '': break
            line = line.rstrip()
            print(line)
    finally:
        ser.close()
    logging.error('No output detected after 10 seconds... exiting.')


def validate_test(port, baud):
    """Read and verify an AUnit test looking and matching specific lines from
    the TestRunner of AUnit in the serial output.
    """
    logging.info('Reading the AUnit test on serial port %s at %s baud' %
                 (port, baud))
    ser = open_port(port, baud)
    try:
        summary_line = ''
        test_mode = TEST_MODE_UNKNOWN
        while True:
            line = ser.readline()
            line = line.decode('ascii')
            if line == '': break
            line = line.rstrip()
            print(line)

            if test_mode == TEST_MODE_UNKNOWN:
                match = TEST_START_RE.match(line)
                if match:
                    test_mode = TEST_MODE_START_FOUND
                    continue
                match = TEST_END_RE.match(line)
                if match:
                    logging.error('Found TestRunner result without start')
                    break
            elif test_mode == TEST_MODE_START_FOUND:
                match = TEST_START_RE.match(line)
                if match:
                    logging.error('Unexpected TestRunner start')
                    break
                match = TEST_END_RE.match(line)
                if match:
                    test_mode = TEST_MODE_END_SUMMARY_FOUND
                    summary_line = line
                    break
    finally:
        ser.close()

    if test_mode != TEST_MODE_END_SUMMARY_FOUND:
        raise Excpeption('No output detected after 10 seconds... exiting.')

    if summary_line:
        match = TEST_END_RE.match(line)
        if match:
            num_failed = match.group(1)
            num_expired = match.group(2)
            if num_failed != '0' or num_expired != '0':
                raise Exception('Found %s failed and/or %s timed out' %
                                (num_failed, num_expired))
    else:
        raise Exception('Unexpected TestRunner output')


# See https://stackoverflow.com/questions/12090503
def list_ports():
    """List the available serial ports."""
    for comport in serial.tools.list_ports.comports():
        print(comport)


def open_port(port, baud):
    """Open the given port. Boards like Teensy, Leonardo, and Micro do not
    create a virtual serial port until the Arduino program runs, so we make
    multiple attempts (NUM_ATTEMPTS) to open the port using an exponential back
    off wait time.
    """
    wait_time = WAIT_TIME_BASE
    count = 1
    ser = serial.Serial(port=None, baudrate=baud, timeout=TIMEOUT_ON_IDLE)
    ser.port = port
    while True:
        try:
            logging.info('Opening serial port %s' % port)
            ser.open()
            break
        except:
            if count >= NUM_ATTEMPTS:
                break
            logging.info('Failed... waiting %s seconds to retry...' %
                         wait_time)
            sleep(wait_time)
            count += 1
            wait_time *= 1.5

    if not ser.is_open:
        raise Exception('Unable to open serial port %s after %s attempts' %
                        (port, NUM_ATTEMPTS))
    return ser


def main():
    parser = argparse.ArgumentParser(
        description='Read the given Arduino serial port')
    parser.add_argument(
        '--log_level', action='store', default='DEBUG', help='Logging level')
    parser.add_argument(
        '--port', action='store', default='/dev/ttyUSB0', help='port')
    parser.add_argument(
        '--baud', action='store', default='115200', help='baud')
    parser.add_argument(
        '--list',
        action='store_true',
        help='List the available ports (default)')
    parser.add_argument(
        '--test', action='store_true', help='Verify an AUnit test')
    parser.add_argument(
        '--monitor', action='store_true', help='Monitor the serial port')
    args = parser.parse_args()

    # Configure logging.
    logging.basicConfig(
        level=args.log_level, format=LOG_FORMAT, datefmt=DATE_FORMAT)

    if args.monitor:
        monitor(args.port, args.baud)
    elif args.test:
        validate_test(args.port, args.baud)
    else:
        list_ports()


if __name__ == '__main__':
    main()
