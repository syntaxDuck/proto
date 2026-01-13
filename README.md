# proto

A minimalist terminal-based text editor written in C.

## Description

proto is a lightweight, fully-functional text editor that runs directly in your terminal. Inspired by classic editors like vi and nano, it provides essential text editing features with a clean, modular architecture. The editor supports syntax highlighting for C/C++ files and requires no external dependencies beyond standard C libraries.

## Features

- **Text Editing**: Full cursor movement and text editing capabilities
- **File Operations**: Open, edit, and save files
- **Syntax Highlighting**: Built-in support for C/C++ files with syntax highlighting
- **Search**: Find text within files using Ctrl+F
- **Status Bar**: Shows current file information, cursor position, and helpful hints
- **Dirty Flag Tracking**: Shows when file has unsaved changes
- **Minimal Dependencies**: Uses only standard C libraries

## Installation

### Prerequisites

- C compiler (GCC or Clang)
- CMake (version 3.10 or higher)
- Unix-like system (Linux, macOS, or WSL)

### Build Instructions

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd proto
   ```

2. Create build directory and configure:
   ```bash
   mkdir -p build
   cd build
   cmake ..
   ```

3. Build the project:
   ```bash
   make
   ```
   Alternatively:
   ```bash
   cmake --build .
   ```

4. Run the editor:
   ```bash
   ./proto [filename]
   ```

   - Run without arguments to start with a new file
   - Pass a filename to open an existing file or create a new one

### Clean Build

```bash
cd build
make clean
```

## Usage

### Key Bindings

- `Ctrl-S`: Save file
- `Ctrl-Q`: Quit editor (with confirmation if unsaved changes)
- `Ctrl-F`: Find/search text
- `Arrow Keys`: Navigate cursor
- `Home/End`: Move to beginning/end of line
- `Page Up/Down`: Navigate through document
- `Backspace/Delete`: Delete characters
- Standard typing keys for text input

### File Support

Currently supports syntax highlighting for:
- C files (`.c`, `.h`)
- C++ files (`.cpp`)

The syntax highlighting system is extensible for additional languages.

## Project Structure

```
proto/
├── src/                    # Source code
│   ├── main.c             # Entry point and main editor loop
│   ├── terminal.c         # Terminal handling and raw mode
│   ├── fileio.c           # File operations (open/save)
│   ├── input.c            # Keyboard input processing
│   ├── output.c           # Screen rendering
│   ├── row.c              # Text row management
│   ├── syntax_highlighting.c # Syntax highlighting
│   ├── find.c             # Search functionality
│   ├── filetypes.c        # File type definitions
│   ├── append.c           # Text append operations
│   └── ops.c              # Editor operations
├── include/                # Header files
├── CMakeLists.txt          # CMake configuration
└── README.md              # This file
```

## Architecture

The editor follows a modular design with clear separation of concerns:

- **Terminal Module**: Handles raw terminal mode and escape sequences
- **File I/O Module**: Manages file operations and content loading
- **Input Module**: Processes keyboard input and special keys
- **Output Module**: Handles screen rendering and cursor positioning
- **Row Module**: Manages text rows and memory allocation
- **Syntax Highlighting**: Provides extensible syntax coloring

## Technical Details

- **Memory Management**: Manual memory allocation for optimal performance
- **Terminal Control**: Uses ANSI escape sequences for screen manipulation
- **Raw Mode**: Disables terminal buffering for immediate key input
- **Configuration**: Constants defined in `state.h` for customization

## Contributing

This project serves as an excellent learning resource for systems programming and text editor internals. Contributions are welcome, especially for:

- Additional syntax highlighting languages
- New features and key bindings
- Performance optimizations
- Code improvements and bug fixes

## License

This project is open source. Please check the LICENSE file for details.

## Version

Current version: 0.0.1