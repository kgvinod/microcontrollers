#!/bin/bash

set -x

PROJECT_DIR=$HOME/work/pictest
OUTPUT_DIR=$PROJECT_DIR/build
CC=/opt/microchip/xc8/v2.41/bin/xc8-cc 

# The paths to the source files
SOURCE_FILES=("$PROJECT_DIR/pictest3.c") # Add more with space in between

# Create the output directory
mkdir -p $OUTPUT_DIR

# The name of the output file
OUTPUT_FILE="pictest"

# Save the current directory
CURRENT_DIR=$(pwd)

# Change to the output directory
cd $OUTPUT_DIR

# Run the compiler
$CC -mcpu=16F877A -o $OUTPUT_FILE "${SOURCE_FILES[@]}"

# Restore the original directory
cd $CURRENT_DIR
