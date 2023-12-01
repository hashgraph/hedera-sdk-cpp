#!/bin/bash

# Directory containing executable files
EXECUTABLES_DIRECTORY="../../../build/macos-arm64-debug/sdk/examples/Debug"

if [ -d "$EXECUTABLES_DIRECTORY" ]; then
  echo "$EXECUTABLES_DIRECTORY does exist."
fi

# Iterate over all executables in the directory
for file in "$EXECUTABLES_DIRECTORY"/*; do
    if [ -x "$file" ]; then
        echo "Running $file..."
        
        # Execute the binary executable without additional arguments
        "$file"
        
        echo "Done"
    fi
done
