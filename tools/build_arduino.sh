#!/usr/bin/env bash
#
# A shell wrapper around the arduino(1) program with 3 modes: verify, upload,
# and test.
#
# Usage:
#
#   $ build_arduino.sh [--help] [--verbose] [--verify | --upload | --test ]
#       [--monitor] [--port /dev/ttyUSB0] [--baud baud]
#       [--board {package}:{arch}:{board}[:parameters]]
#       [--boards {alias}:{port},...] (file.ino | dir) [...]
#
# Documentation of arduino binary flags:
#
#   https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc
#
# Environment variables:
#
#   BUILD_ARDUINO_BINARY: location of the arduino IDE binary
#   BUILD_ARDUINO_CONFIG: location of the .build_arduino_config file
#
# Example ports (Linux):
#
#   - Arduino Micro: /dev/ttyACM{n}
#   - Arduino Nano: /dev/ttyUSB{n}
#   - ESP8266: /dev/ttyUSB{n}
#
# Flags:
#
#   If the directory is given, then the script looks for a sketch file under
#   the directory with the same name but ending with '.ino'. For example,
#   './build_arduino.sh CommonTest' is equivalent to './build_arduino.sh
#   CommonTest/CommonTest.ino' if CommonTest is a directory.
#
#   The --test flag automatically enables both the --upload and the --monitor
#   flags.
#
#   If none of --verify, --upload nor --test flags are given, then the default
#   is --verify.
#
#   The default value of --port is /dev/ttyUSB0.
#
#   The default value of --baud is 115200.
#
# Copyright and License:
#
#   Copyright 2018 (c) Brian T. Park <brian@xparks.net>
#   MIT License

set -eu

# Can't use $(realpath $(dirname $0)) because realpath doesn't exist on MacOS
DIRNAME=$(dirname $0)

function usage() {
    echo "Usage: build_arduino.sh [--help] [--verbose] \
[--verify | --upload | --test ] [--monitor] \
[--port port] [--baud baud] [--board board] \
[--boards {alias}:{port},...] (file.ino | directory) [...]"
    exit 1
}

# Find the *.ino file, even if only the directory was given, e.g. "CommonTest"
function get_ino_file() {
    local file=$1

    # Ends in '.ino', just return it.
    if [[ "$file" =~ .*\.ino ]]; then
        echo $file
        return
    fi

    # Not a directory, don't know what to do with it, just return it
    if [[ ! -d $file ]]; then
        echo $file
        return
    fi

    # Strip off any trailing '/'
    local dir=$(echo $file | sed -e 's/\/*$//')
    local file=$(basename $dir)
    echo "${dir}/${file}.ino"
}

function get_config() {
    local key=$(echo $1 | sed -e 's/\./\\./g')
    local matching_line=$(grep "^$key=" $BUILD_ARDUINO_CONFIG)
    if [[ $matching_line != '' ]]; then
        local value=$(echo $matching_line | sed -e "s/^$key=\(.*\)/\1/")
        echo $value
    else
        echo ''
    fi
}

function run_arduino_command_for_boards() {
    local board_envs=$(echo $boards | sed -e 's/,/ /')
    for env in $board_envs; do
        # Split {alias}:{port} into two fields.
        local board_alias=$(echo $env | sed -e 's/\([^:]*\):\([^:]*\)/\1/')
        local board_port=$(echo $env | sed -e 's/\([^:]*\):\([^:]*\)/\2/')

        echo "======== Processing board=$board_alias, port=$board_port"
        local board_value=$(get_config $board_alias)
        if [[ "$board_value" == '' ]]; then
            echo "Unknown board alias '$board_alias'"
            continue
        fi

        board=$board_value
        port=$board_port
        run_arduino_command_for_files "$@"
    done
}

function run_arduino_command_for_files() {
    local file
    for file in "$@"; do
        local ino_file=$(get_ino_file $file)
        if [[ ! -f $ino_file ]]; then
            echo "==== Cannot find $ino_file"
            continue
        fi

        set +e; run_arduino_command $ino_file; local status=$?; set -e
        if [[ $status != 0 ]]; then
            continue
        fi

        # Run the serial monitor in test validation mode if --test flag given.
        if [[ "$mode" == 'test' ]]; then
            echo # blank line
            local cmd="$DIRNAME/serial_monitor.py --test \
--port $port --baud $baud"
            echo "\$ $cmd"
            set +e; $cmd; local status=$?; set -e
            if [[ "$status" == 0 ]]; then
                echo "PASSED: $board $port $ino_file" >> $test_summary_file
            else
                echo "FAILED: $board $port $ino_file" >> $test_summary_file
            fi
        # Just run the serial monitor in echo mode.
        elif [[ "$monitor" == 1 ]]; then
            echo # blank line
            local cmd="$DIRNAME/serial_monitor.py --port $port --baud $baud"
            echo "\$ $cmd"
            set +e; $cmd; local status=$?; set -e
        fi
    done
}

function run_arduino_command() {
    local file=$1
    echo "==== Processing $file"

    if [[ "$board" == '' ]]; then
        echo 'Board variable $board not defined, should not happen'
        return 1
    fi
    if [[ "$port" == '' ]]; then
        echo 'Port variable $port not defined, should not happen'
        return 1
    fi

    local board_flag="--board $board"
    local upload_or_verify='--verify'
    if [[ "$mode" == 'upload' || "$mode" == 'test' ]]; then
        upload_or_verify='--upload'
    fi

    # Execute the arduino(1) command line and handle the error
    local cmd="$BUILD_ARDUINO_BINARY $verbose $upload_or_verify --port $port \
$board_flag $file"
    echo "\$ $cmd"
    set +e; $cmd; local status=$?; set -e
    if [[ "$verbose" != '' && "$mode" == 'test' && $status == 0 ]]; then
        echo # blank line
    fi
    if [[ "$status" != 0 ]]; then
        echo "Failed processing: $file"
    fi
    return $status
}

function clean_test_summary_file() {
    if [[ "$test_summary_file" != '' ]]; then
        rm -f $test_summary_file
    fi
}

function create_test_summary_file() {
    test_summary_file=$(mktemp /tmp/build_arduino.XXXXXX.txt)
    trap "clean_test_summary_file" EXIT
}

function print_test_summary_file() {
    echo '======== Test Run Summary'
    cat $test_summary_file
    set +e; grep --quiet FAILED $test_summary_file; local status=$?; set -e
    if [[ "$status" != 0 ]]; then
        echo 'ALL PASSED'
    else
        echo 'FAILURES'
    fi
}

function check_environment_variables() {
    # Check for BUILD_ARDUINO_BINARY
    if [[ -z ${BUILD_ARDUINO_BINARY+x} ]]; then
        echo "BUILD_ARDUINO_BINARY environment variable is not defined"
        exit 1
    fi
    if [[ ! -x $BUILD_ARDUINO_BINARY ]]; then
        echo "BUILD_ARDUINO_BINARY=$BUILD_ARDUINO_BINARY is not an executable"
        exit 1
    fi

    # Check for BUILD_ARDUINO_CONFIG
    if [[ -z ${BUILD_ARDUINO_CONFIG+x} ]]; then
        echo "BUILD_ARDUINO_CONFIG environment variable is not defined"
        exit 1
    fi
    if [[ ! -f $BUILD_ARDUINO_CONFIG ]]; then
        echo "BUILD_ARDUINO_CONFIG=$BUILD_ARDUINO_CONFIG is not readable"
        exit 1
    fi
}

check_environment_variables

# Parse command line flags
port=/dev/ttyUSB0
board=
boards=
mode='--upload'
monitor=0
baud=115200
verbose=
while [[ $# -gt 0 ]]; do
    case $1 in
        --verify) mode='verify' ;;
        --upload) mode='upload' ;;
        --test) mode='test' ;;
        --port) shift; port=$1 ;;
        --baud) shift; baud=$1 ;;
        --board) shift; board=$1 ;;
        --boards) shift; boards=$1 ;;
        --monitor) monitor=1 ;;
        --verbose) verbose='--verbose' ;;
        --help|-h) usage ;;
        -*) echo "Unknown option '$1'"; usage ;;
        *) break ;;
    esac
    shift
done
if [[ "$port" == '' ]]; then
    echo '--port flag must be given'
    usage
fi

# Set up temporary summary file
if [[ "$mode" == 'test' ]]; then
    create_test_summary_file
fi

# Run for various boards and/or files
if [[ "$boards" != '' ]]; then
    run_arduino_command_for_boards "$@"
else
    run_arduino_command_for_files "$@"
fi

# Print test run summary
if [[ "$mode" == 'test' ]]; then
    print_test_summary_file
fi
