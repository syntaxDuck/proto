# proto

A **minimal terminal-based text editor written in C**, focused on clarity, low-level control, and learning-oriented systems design.

`proto` implements core editor functionality from scratch using standard C and POSIX APIs, including raw terminal input handling, file I/O, syntax highlighting, and incremental rendering.

---

## ✨ Features

* 📄 Open, edit, and save text files
* 🎨 Syntax highlighting (C / C++)
* 🔍 Incremental search
* ⌨️ Raw terminal input handling (no ncurses)
* 🧠 Manual memory management
* 🧩 Modular architecture (editor core, terminal, file I/O, syntax)

---

## 🧱 Architecture Overview

The codebase is intentionally modular to separate concerns and improve maintainability:

* `editor.c` – Core editor state, rendering, and control flow
* `terminal.c` – Raw mode handling, screen drawing, cursor control
* `fileio.c` – File loading and saving
* `syntax.c` – Language-aware syntax highlighting
* `input.c` – Key handling and editor commands

This structure mirrors real-world systems software and makes it easier to reason about complexity in C.

---

## 🚀 Getting Started

### Build

```bash
make
```

### Run

```bash
./proto <optional_filename>
```

---

## 🛠️ Controls

| Key        | Action      |
| ---------- | ----------- |
| Ctrl-S     | Save file   |
| Ctrl-Q     | Quit        |
| Ctrl-F     | Search      |
| Arrow Keys | Move cursor |

---

## 🎯 Goals of This Project

This project was built to:

* Practice **systems programming in C**
* Gain hands-on experience with **terminal I/O and raw mode**
* Explore **editor internals** (buffer management, rendering loops)
* Emphasize **clarity and correctness over features**

It is inspired by classic minimal editors and educational implementations.

---

## 📸 Demo

> *Add screenshots or a short GIF here showing editing, search, and syntax highlighting.*

---

## 🔮 Future Improvements

* Additional language syntax support
* Undo/redo
* Configurable keybindings
* Basic plugin system
* Automated tests
* CI build via GitHub Actions

---

## 📚 Inspiration

* Kilo text editor by Salvatore Sanfilippo
* Traditional UNIX terminal editors

---

## 📄 License

MIT License

