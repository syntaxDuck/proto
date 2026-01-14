# Getting Started with Proto

## Installation

### Prerequisites

- C compiler (GCC or Clang)
- CMake (version 3.15 or higher)
- Unix-like system (Linux, macOS, or WSL)

### Build Instructions

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd proto
   ```

2. **Configure and build:**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the editor:**
   ```bash
   ./proto [filename]
   ```

### Build Options

- **Debug build:** `cmake -DCMAKE_BUILD_TYPE=Debug ..`
- **Release build:** `cmake -DCMAKE_BUILD_TYPE=Release ..`
- **Enable sanitizers:** `cmake -DENABLE_SANITIZERS=ON ..`

## Basic Usage

### Starting the Editor

- **New file:** `./proto`
- **Open existing file:** `./proto filename.txt`

### Key Bindings

| Key Combination | Action |
|-----------------|--------|
| `Ctrl-S` | Save file |
| `Ctrl-Q` | Quit editor |
| `Ctrl-F` | Find/search text |
| `Arrow Keys` | Move cursor |
| `Home/End` | Move to line start/end |
| `Page Up/Down` | Navigate document |
| `Backspace/Delete` | Delete characters |
| Standard typing keys | Text input |

### File Support

Currently supports syntax highlighting for:
- C files (`.c`, `.h`)
- C++ files (`.cpp`)

## Tips

- The status bar shows current file information and cursor position
- Modified files are marked with an asterisk (*)
- Use `Ctrl-F` for quick text search
- All unsaved changes will be prompted when quitting