# Architecture Documentation

## Design Philosophy

Proto follows a modular, layered architecture designed for:

- **Simplicity:** Minimal dependencies, clear interfaces
- **Maintainability:** Logical module separation, consistent naming
- **Extensibility:** Plugin-ready architecture for new features
- **Performance:** Efficient memory usage, fast rendering

## Module Organization

### Core Layer (`src/core/`)

Contains the fundamental editor components:

- **`main.c`**: Entry point, main event loop
- **`state.h/.c`**: Global editor state management
- **`row.h/.c`**: Text row data structures and operations

### Terminal Layer (`src/terminal/`)

Handles low-level terminal interactions:

- **`terminal.h/.c`**: Terminal mode management, window size
- **`ansi.h/.c`**: ANSI escape sequence utilities

### I/O Layer (`src/io/`)

Manages user input and screen output:

- **`input.h/.c`**: Keyboard processing, key bindings
- **`output.h/.c`**: Screen rendering, cursor positioning
- **`append.h/.c`**: Dynamic string buffer utility

### File Layer (`src/file/`)

Handles file operations:

- **`fileio.h/.c`**: File reading/writing operations
- **`filetypes.h/.c`**: File type detection and configuration

### Editing Layer (`src/editing/`)

Core text editing functionality:

- **`ops.h/.c`**: Text insertion, deletion, cursor movement
- **`find.h/.c`**: Search functionality

### Syntax Layer (`src/synthesis/`)

Syntax highlighting and language support:

- **`syntax_highlighting.h/.c`**: Text coloring algorithms

## Data Flow

```
┌─────────────────┐
│   User Input    │
└─────────┬───────┘
          │
    ┌─────▼─────┐
    │ Input Layer│
    └─────┬─────┘
          │
    ┌─────▼─────┐
    │ Editing    │  ←── File Operations
    │ Layer      │
    └─────┬─────┘
          │
    ┌─────▼─────┐
    │ Core State │
    └─────┬─────┘
          │
    ┌─────▼─────┐
    │ Output     │
    │ Layer      │
    └─────┬─────┘
          │
    ┌─────▼─────┐
    │  Terminal  │
    │   Layer    │
    └───────────┘
```

## Memory Management

### Key Principles

1. **Explicit allocation:** All memory manually allocated/freed
2. **Error checking:** NULL checks after every allocation
3. **Cleanup on exit:** Proper resource cleanup in all error paths
4. **Minimal allocations:** Reuse buffers where possible

### Memory Layout

```
struct editorConfig {
    // Cursor state
    int cx, cy, rx;
    
    // View state  
    int rowoff, coloff;
    int screenrows, screencols;
    
    // Document state
    int numrows;
    erow *row;  // Dynamic array of text rows
    
    // UI state
    int dirty;
    char *filename;
    char statusmsg[80];
    time_t statusmsg_time;
    
    // Syntax state
    struct editorSyntax *syntax;
    struct termios orig_termios;
};
```

## Threading Model

Proto is **single-threaded** by design:

- **Simplicity:** No synchronization complexity
- **Performance:** No context switching overhead
- **Portability:** Works across all platforms

## Extensibility Points

### Syntax Highlighting

New language support can be added by:

1. Defining language patterns in `filetypes.c`
2. Adding highlighting rules in `syntax_highlighting.c`
3. Extending `editorSelectSyntaxHighlight()`

### Key Bindings

New key combinations can be added in:

- **`input.c`**: Process key sequences
- **`ops.c`**: Implement editing operations
- **`find.c`**: Add search-related commands

### File Format Support

New file types supported by:

1. Adding file extensions to `C_HL_extensions[]`
2. Defining syntax rules in `C_HL_keywords[]`
3. Implementing file-type specific logic

## Performance Considerations

### Rendering Optimization

- **Incremental updates:** Only redraw changed lines
- **Efficient scrolling:** Viewport-based rendering
- **Minimal syscalls:** Batch terminal operations

### Memory Efficiency

- **String interning:** Reuse common strings
- **Lazy allocation:** Allocate only when needed
- **Buffer reuse:** Reuse append buffers

### Search Performance

- **Linear search:** Optimized for typical file sizes
- **Highlight caching:** Cache syntax highlighting results
- **Incremental search**: Real-time search feedback