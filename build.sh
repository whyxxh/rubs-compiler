#!/bin/sh

# === Configuration ===
SRC_DIR="./src"           # Source files directory
OUT="./rc"                 # Output binary
CC="gcc"                   # Compiler
CFLAGS="-Wall -Wextra" # Flags: warnings + optimization
SRC_FILES=""
for file in "$SRC_DIR"/*.c; do
    SRC_FILES="$SRC_FILES $file"
done

# === Compilation ===
echo "Compiling $OUT from: $SRC_FILES"
$CC $CFLAGS $SRC_FILES -o $OUT

# === Result ===
if [ $? -eq 0 ]; then
    echo "Build successful: $OUT"
else
    echo "Build failed."
fi
