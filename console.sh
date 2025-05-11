#!/bin/bash

INPUT_FILE="./tests/test.lg"

# Create or clear the input file
> "$INPUT_FILE"

while true; do
    read -rp "> " line
    echo "${line};" > "$INPUT_FILE"
    ./rc "$INPUT_FILE"
done
