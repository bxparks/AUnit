#!/usr/bin/env bash
#
# auniter.sh
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
#   $ auniter.sh [--help] [--verbose]
#       [--verify | --upload | --test | --monitor | --list_ports]
#       [--board {package}:{arch}:{board}[:parameters]]
#       [--port port] [--baud baud]
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
#   './auniter.sh CommonTest' is equivalent to './auniter.sh
#   CommonTest/CommonTest.ino' if CommonTest is a directory.

set -eu

# Can't use $(realpath $(dirname $0)) because realpath doesn't exist on MacOS
DIRNAME=$(dirname $0)

# Determine the location of the config file. Defaults to
# $HOME/.auniter_config unless AUNITER_CONFIG is set.
CONFIG_FILE=${AUNITER_CONFIG:-$HOME/.auniter_config}

function usage() {
    cat <<'END'
Usage: auniter.sh [--help] [--verbose]
    [--verify | --upload | --test | --monitor | --list_ports]
    [--board {package}:{arch}:{board}[:parameters]] [--port port] [--baud baud]
    [--boards {alias}[:{port}],...] (file.ino | directory) [...]
END
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

# Find the given $key in a $section from the $config file.
# Usage: get_config config section key
#
# The config file is expected to be in an INI file format:
#   [section]
#       {key}={value}
#       ...
#   [...]
#       ...
#
function get_config() {
    local config=$1
    local section=$2
    local key=$3

    # If CONFIG_FILE does not exist then no aliases are defined.
    if [[ ! -f $config ]]; then
        return
    fi

    # Use one-liner sed script given in
    # https://stackoverflow.com/questions/6318809, with a bug fix for when the
    # key does not exist in the matching [$section] but exists in a subsequent
    # section.
    sed -n -E \
        "/^\[$section\]/ {
            n;
            :label /^ *$key *=/ { s/[^=]*= *//; p; q; };
            /^\[.*\]/ q;
            n;
            b label;
        }" \
        $config
}

function process_sketches() {
    if [[ "$boards" != '' ]]; then
        process_boards "$@"
    else
        process_files "$@"
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
        local board_value=$(get_config "$CONFIG_FILE" 'boards' "$board_alias")
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
        echo "FAILED $mode: board not defined: $file" \
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
    local cmd="$AUNITER_ARDUINO_BINARY \
$verbose \
$upload_or_verify \
$port_flag \
$board_flag \
$file"
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
    local cmd="$DIRNAME/serial_monitor.py --monitor --port $port --baud $baud"
    echo "\$ $cmd"
    $cmd || true # prevent failure from exiting the entire script
}

function clean_temp_files() {
    if [[ "$summary_file" != '' ]]; then
        rm -f $summary_file
    fi
}

function create_temp_files() {
    summary_file=
    trap "clean_temp_files" EXIT
    summary_file=$(mktemp /tmp/auniter_summary_XXXXXX)
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
    # Check for AUNITER_ARDUINO_BINARY
    if [[ -z ${AUNITER_ARDUINO_BINARY+x} ]]; then
        echo "AUNITER_ARDUINO_BINARY environment variable is not defined"
        exit 1
    fi
    if [[ ! -x $AUNITER_ARDUINO_BINARY ]]; then
        echo "AUNITER_ARDUINO_BINARY=$AUNITER_ARDUINO_BINARY is not executable"
        exit 1
    fi
}

function interrupted() {
    echo 'Interrupted'
    print_summary_file
    exit 1
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
        --list_ports) mode='list_ports' ;;
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
if [[ "$mode" != 'list_ports' && $# -eq 0 ]]; then
    echo 'Must provide file or directory'
    usage
fi

# Must install a trap for Control-C because the script ignores almost all
# interrupts and continues processing.
trap interrupted INT

check_environment_variables
create_temp_files
if [[ "$mode" == 'list_ports' ]]; then
    $DIRNAME/serial_monitor.py --list
else
    process_sketches "$@"
    print_summary_file
fi
