# Module CMakeLists.txt for better organization
# This file demonstrates how each module could be built as a library

# Core module
add_library(core STATIC
    src/core/main.c
    src/core/row.c
)

# Terminal module  
add_library(terminal STATIC
    src/terminal/terminal.c
    src/terminal/ansi.c
)

# IO module
add_library(io STATIC
    src/io/input.c
    src/io/output.c
    src/io/append.c
)

# File module
add_library(file STATIC
    src/file/fileio.c
    src/file/filetypes.c
)

# Editing module
add_library(editing STATIC
    src/editing/ops.c
    src/editing/find.c
)

# Syntax module
add_library(syntax STATIC
    src/synthesis/syntax_highlighting.c
)

# Set include directories for each module
target_include_directories(core PUBLIC include)
target_include_directories(terminal PUBLIC include)
target_include_directories(io PUBLIC include)
target_include_directories(file PUBLIC include)
target_include_directories(editing PUBLIC include)
target_include_directories(syntax PUBLIC include)