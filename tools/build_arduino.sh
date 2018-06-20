#!/usr/bin/env bash
#
# build_arduino.sh
#
#   A shell wrapper around the arduino(1) commandline program which can verify
#   and upload an Arduino sketch, and validate an AUnit unit test.
#
#   Copyright 2018 (c) Brian T. Park <brian@xparks.net>
#
#   MIT License
#
# Usage:
#
#   $ build_arduino.sh [--help] [--verbose]
#       [--verify | --upload | --test | --monitor]
#       [--board {package}:{arch}:{board}[:parameters]]
#       [--port /dev/ttyUSB0] [--baud baud]
#       [--boards {alias}[:{port}],...] (file.ino | dir) [...]
#
# Flags:
#
#   --verify Verify the compile of the given sketch files.
#   --upload Upload the sketch to the given board at port.
#   --test Upload an AUnit unit test, and verify pass or fail. Automatically
#       invokes the --upload flag.
#   --monitor Use serial_monitor.py to read and echo the serial output.
#   --port /dev/ttyXxx location of the board. Default is /dev/ttyUSB0.
#   --baud baud Speed of the port for serial_montor.py. Default is 115200.
#   --board Fully qualified board name (fqbn) of the target board.
#   --boards {alias[:port}],... Comma-separated list of {alias:port} pairs.
#
#   If the directory is given, then the script looks for a sketch file under
#   the directory with the same name but ending with '.ino'. For example,
#   './build_arduino.sh CommonTest' is equivalent to './build_arduino.sh
#   CommonTest/CommonTest.ino' if CommonTest is a directory.

set -eu

# Can't use $(realpath $(dirname $0)) because realpath doesn't exist on MacOS
DIRNAME=$(dirname $0)

# Determine the location of the config file. Defaults to
# $HOME/.build_arduino_config unless BUILD_ARDUINO_CONFIG is set.
CONFIG_FILE=${BUILD_ARDUINO_CONFIG:-$HOME/.build_arduino_config}

function usage() {
    echo "Usage: build_arduino.sh [--help] [--verbose] \
[--verify | --upload | --test | --monitor] \
[--board {package}:{arch}:{board}[:parameters]] [--port port] [--baud baud] \
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
    if [[ "$matching_line" != '' ]]; then
        echo "$matching_line" | sed -e "s/^$key=\(.*\)/\1/"
    fi
}

function process_boards() {
    local alias_ports=$(echo "$boards" | sed -e 's/,/ /g')
    for alias_port in $alias_ports; do
        # Split {alias}:{port} into two fields.
        local board_alias=$(echo $alias_port \
                | sed -E -e 's/([^:]*):?([^:]*)/\1/')
        local board_port=$(echo $alias_port \
                | sed -E -e 's/([^:]*):?([^:]*)/\2/')

        echo "======== Processing board=$board_alias, port=$board_port"
        local board_value=$(get_config "$board_alias")
        if [[ "$board_value" == '' ]]; then
            echo "FAILED: Unknown board alias '$board_alias'" \
                | tee -a $summary_file
            continue
        fi
        if [[ "$board_port" == '' && "$mode" != 'verify' ]]; then
            echo "FAILED $mode: Unknown port for '$board_alias'" \
                | tee -a $summary_file
            continue
        fi

        board=$board_value
        port=$board_port
        process_files "$@"
    done
}

function process_files() {
    local file
    for file in "$@"; do
        local ino_file=$(get_ino_file $file)
        if [[ ! -f $ino_file ]]; then
            echo "FAILED $mode: file not found: $ino_file" \
                | tee -a $summary_file
            continue
        fi

        process_file $ino_file
    done
}

function process_file() {
    local file=$1
    echo "==== Processing $file"

    if [[ "$board" == '' ]]; then
        echo "FAILED $mode: board '$board' not defined: $file" \
            | tee -a $summary_file
        return
    fi
    if [[ "$port" == '' && "$mode" != 'verify' ]]; then
        echo "FAILED $mode: port for '$board' not defined: $file" \
            | tee -a $summary_file
        return
    fi

    # Determine flags for the arduino(1) commandline
    local board_flag="--board $board"
    local upload_or_verify='--verify'
    if [[ "$mode" == 'upload' || "$mode" == 'test' ]]; then
        upload_or_verify='--upload'
    fi
    local port_flag=''
    if [[ "$port" != '' ]]; then
        port_flag="--port $port"
    fi

    # Execute the arduino(1) command line.
    local cmd="$BUILD_ARDUINO_BINARY $verbose $upload_or_verify $port_flag \
$board_flag $file"
    echo "\$ $cmd"
    local status=0; $cmd || status=$?
    if [[ "$status" != 0 ]]; then
        echo "FAILED $mode: verify or upload failed: $file" \
            | tee -a $summary_file
        return
    fi

    # The verbose mode leaves a dangling line w/o a newline
    if [[ "$verbose" != '' ]]; then
        echo # blank line
    fi

    # Validate the unit test
    if [[ "$mode" == 'test' ]]; then
        validate_test $file
    # Just monitor the serial port
    elif [[ "$mode" == 'monitor' ]]; then
        monitor_serial
    fi
}

# Run the serial monitor in AUnit test validation mode.
function validate_test() {
    local file=$1

    echo # blank line
    local cmd="$DIRNAME/serial_monitor.py --test --port $port --baud $baud"
    echo "\$ $cmd"
    if $cmd; then
        echo "PASSED $mode: $board $port $file" | tee -a $summary_file
    else
        echo "FAILED $mode: $board $port $file" | tee -a $summary_file
    fi
}

# Run the serial monitor in echo mode.
function monitor_serial() {
    echo # blank line
    local cmd="$DIRNAME/serial_monitor.py --port $port --baud $baud"
    echo "\$ $cmd"
    $cmd || true # prevent failure from exiting the entire script
}

function clean_summary_file() {
    if [[ "$summary_file" != '' ]]; then
        rm -f $summary_file
    fi
}

function create_summary_file() {
    summary_file=$(mktemp /tmp/build_arduino.XXXXXX.txt)
    trap "clean_summary_file" EXIT
}

function print_summary_file() {
    echo '======== Summary'
    cat $summary_file
    if ! grep --quiet FAILED $summary_file; then
        echo 'ALL PASSED'
    else
        echo 'FAILURES found'
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

# Parse command line flags
mode='verify'
board=
boards=
port=/dev/ttyUSB0
baud=115200
verbose=
while [[ $# -gt 0 ]]; do
    case $1 in
        --verify) mode='verify' ;;
        --upload) mode='upload' ;;
        --test) mode='test' ;;
        --monitor) mode='monitor' ;;
        --board) shift; board=$1 ;;
        --boards) shift; boards=$1 ;;
        --port) shift; port=$1 ;;
        --baud) shift; baud=$1 ;;
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

check_environment_variables
create_summary_file
if [[ "$boards" != '' ]]; then
    process_boards "$@"
else
    process_files "$@"
fi
print_summary_file
