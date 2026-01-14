#!/bin/bash

# Format all C source and header files
echo "Formatting C files..."
find . -name "*.c" -o -name "*.h" | grep -v build | xargs clang-format -i

echo "Done formatting!"