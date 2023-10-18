#!/bin/bash

# Script should be run from root directory(zsh scipts/run_examples.sh PathToExamples AccountID PrivateKey)
# Keep in mind this is created to correctly run executables which accept arguments [AccountID, PrivateKey]
# Extract command-line arguments
exe_dir="$1"
arg1="$2"
arg2="$3"

# Check if the executable directory exists
if [ ! -d "$exe_dir" ]; then
    echo "Error: The specified directory does not exist."
    exit 1
fi

# Iterate through the executables in the directory
for exe in "$exe_dir"/*; do
    if [ -x "$exe" ]; then
        echo "Running $exe with arguments: $arg1 $arg2"
        "$exe" "$arg1" "$arg2"
        echo "Finished running $exe"
    fi
done