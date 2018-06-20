#!/usr/bin/env bash
#
# A shell wrapper around the arduino(1) commandline program which can
# verify and upload and arduino sketch, and validate an AUnit unit test.
#
# Usage:
#
#   $ build_arduino.sh [--help] [--verbose] [--verify | --upload | --test ]
#       [--monitor] [--port /dev/ttyUSB0] [--baud baud]
#       [--board {package}:{arch}:{board}[:parameters]]
#       [--boards {alias}[:{port}],...] (file.ino | dir) [...]
#
# Flags:
#
#   --verify Verify the compile of the given sketch files.
#   --upload Upload the sketch to the given board at port.
#   --monitor Use serial_monitor.py to read and echo the serial output.
#   --test Upload an AUnit unit test, and verify pass or fail. Automatically
#       invokes the --upload flag.
#   --port /dev/ttyXxx location of the board. Default is /dev/ttyUSB0.
#   --baud baud Speed of the port for serial_montor.py. Default is 115200.
#   --board Fully qualified board name (fqbn) of the target board.
#   --boards {alias[:port}],... Comma-separated list of {alias:port} pairs.
#
#   If the directory is given, then the script looks for a sketch file under
#   the directory with the same name but ending with '.ino'. For example,
#   './build_arduino.sh CommonTest' is equivalent to './build_arduino.sh
#   CommonTest/CommonTest.ino' if CommonTest is a directory.
#
# Copyright and License:
#
#   Copyright 2018 (c) Brian T. Park <brian@xparks.net>
#   MIT License

set -eu

# Can't use $(realpath $(dirname $0)) because realpath doesn't exist on MacOS
DIRNAME=$(dirname $0)

# Determine the location of the config file. Defaults to
# $HOME/.build_arduino_config unless BUILD_ARDUINO_CONFIG is set.
CONFIG_FILE=${BUILD_ARDUINO_CONFIG:-$HOME/.build_arduino_config}

function usage() {
    echo "Usage: build_arduino.sh [--help] [--verbose] \
[--verify | --upload | --test ] [--monitor] \
[--port port] [--baud baud] [--board {package}:{arch}:{board}[:parameters]] \
[--boards {alias}[:{port}],...] (file.ino | directory) [...]"
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
    # If CONFIG_FILE does not exist then no aliases are defined.
    if [[ ! -f $CONFIG_FILE ]]; then
        return
    fi

    local key=$(echo $1 | sed -e 's/\./\\./g')
    local matching_line=$(grep "^$key=" $CONFIG_FILE)
    if [[ $matching_line != '' ]]; then
        local value=$(echo $matching_line | sed -e "s/^$key=\(.*\)/\1/")
        echo $value
    fi
}

function run_arduino_command_for_boards() {
    local alias_ports=$(echo $boards | sed -e 's/,/ /g')
    for alias_port in $alias_ports; do
        # Split {alias}:{port} into two fields.
        local board_alias=$(echo $alias_port \
                | sed -E -e 's/([^:]*):?([^:]*)/\1/')
        local board_port=$(echo $alias_port \
                | sed -E -e 's/([^:]*):?([^:]*)/\2/')

        echo "======== Processing board=$board_alias, port=$board_port"
        local board_value=$(get_config "$board_alias")
        if [[ "$board_value" == '' ]]; then
            echo "Unknown board alias '$board_alias'"
            if [[ "$mode" == 'test' ]]; then
                echo "FAILED: Unknown board alias '$board_alias'" \
                    >> $test_summary_file
            fi
            continue
        fi
        if [[ "$board_port" == '' && "$mode" != 'verify' ]]; then
            echo "Unknown board port for '$board_alias'"
            if [[ "$mode" == 'test' ]]; then
                echo "FAILED: Unknown board port for '$board_alias'" \
                    >> $test_summary_file
            fi
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

        if ! run_arduino_command $ino_file; then
            if [[ "$mode" == 'test' ]]; then
                echo "FAILED: $board $port $ino_file" >> $test_summary_file
            fi
            continue
        fi

        # Run the serial monitor in test validation mode if --test flag given.
        if [[ "$mode" == 'test' ]]; then
            echo # blank line
            local cmd="$DIRNAME/serial_monitor.py --test \
--port $port --baud $baud"
            echo "\$ $cmd"
            if $cmd; then
                echo "PASSED: $board $port $ino_file" >> $test_summary_file
            else
                echo "FAILED: $board $port $ino_file" >> $test_summary_file
            fi
        # Just run the serial monitor in echo mode.
        elif [[ "$monitor" == 1 ]]; then
            echo # blank line
            local cmd="$DIRNAME/serial_monitor.py --port $port --baud $baud"
            echo "\$ $cmd"
            $cmd || true # prevent failure from exiting the entire script
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
    if [[ "$port" == '' && "$mode" != 'verify' ]]; then
        echo 'Port variable $port not defined, should not happen'
        return 1
    fi

    local board_flag="--board $board"
    local upload_or_verify='--verify'
    if [[ "$mode" == 'upload' || "$mode" == 'test' ]]; then
        upload_or_verify='--upload'
    fi

    local port_flag=''
    if [[ "$port" != '' ]]; then
        port_flag="--port $port"
    fi

    # Execute the arduino(1) command line and handle the error
    local cmd="$BUILD_ARDUINO_BINARY $verbose $upload_or_verify $port_flag \
$board_flag $file"
    echo "\$ $cmd"
    local status=0; $cmd || status=$?
    if [[ "$verbose" != '' && "$mode" == 'test' && "$status" == 0 ]]; then
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
    if ! grep --quiet FAILED $test_summary_file; then
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
