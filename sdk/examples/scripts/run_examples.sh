#!/bin/bash

# Script should be executed from project root
# Directory containing executable files
EXECUTABLES_DIRECTORY="build_folder_with_exec_binaries"

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
