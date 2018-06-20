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


def monitor(port, baud):
    """Read the serial output and echo the lines to the STDOUT."""
    logging.info('Reading the serial port %s at %s baud' % (port, baud))
    with serial.Serial(port, baud, timeout=10) as ser:
        while True:
            line = ser.readline()
            line = line.decode('ascii')
            if line == '': break
            line = line.rstrip()
            print(line)
    logging.error('No output detected after 10 seconds... exiting.')


# Regular expressions that match the start and end of an AUnit test run.
TEST_START_RE = re.compile(r'TestRunner started')
TEST_END_RE = re.compile(r'TestRunner summary.*(\d+) failed.*(\d+) timed out')

# Constants for the test_mode finite state machine
TEST_MODE_UNKNOWN = 0
TEST_MODE_START_FOUND = 1
TEST_MODE_END_SUMMARY_FOUND = 2


def validate_test(port, baud):
    """Read and verify an AUnit test looking and matching specific lines from
    the TestRunner of AUnit in the serial output.
    """
    logging.info('Reading the AUnit test on serial port %s at %s baud' %
                 (port, baud))
    sumary_line = ''
    test_mode = TEST_MODE_UNKNOWN
    with serial.Serial(port, baud, timeout=10) as ser:
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


# See https://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
def list_ports():
    """List the available serial ports."""
    for comport in serial.tools.list_ports.comports():
        print(comport)


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
        '--list', action='store_true', help='List the available ports (default)')
    parser.add_argument(
        '--test', action='store_true', help='Verify an AUnit test')
    parser.add_argument(
        '--monitor', action='store_true', help='Monitor the serial port')
    args = parser.parse_args()

    # Configure logging.
    logging.basicConfig(
        level=args.log_level, format=LOG_FORMAT, datefmt=DATE_FORMAT)

    try:
        if args.monitor:
            monitor(args.port, args.baud)
        elif args.test:
            validate_test(args.port, args.baud)
        else:
            list_ports()
    except Exception as e:
        logging.error(e)
        exit(1)


if __name__ == '__main__':
    main()
