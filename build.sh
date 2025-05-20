#!/bin/sh

# === Configuration ===
SRC_DIR="./src"                         # Source files directory
OUT="./rc"                              # Output binary name
CC="gcc"                                # Compiler
CFLAGS="-Wall -Wextra -Werror -g"       # Compiler flags
ASAN_ENABLED=true                       # Toggle AddressSanitizer

# Append AddressSanitizer flags if enabled
if [ "$ASAN_ENABLED" = true ]; then
    CFLAGS="$CFLAGS -fsanitize=address -fno-omit-frame-pointer"
    echo "AddressSanitizer enabled."
fi

# Collect all .c source files
SRC_FILES=""
for file in "$SRC_DIR"/*.c; do
    SRC_FILES="$SRC_FILES $file"
done

# === Compilation ===
echo "Compiling $OUT from:$SRC_FILES"
$CC $CFLAGS $SRC_FILES -o $OUT

# === Result ===
if [ $? -eq 0 ]; then
    echo "✅ Build successful: $OUT"
else
    echo "❌ Build failed."
fi
