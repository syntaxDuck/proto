# Hello World Example

This is a simple example of how to use the Proto editor to create a "Hello, World!" program.

## Creating the File

1. **Start the editor:**
   ```bash
   ./proto hello.c
   ```

2. **Type the program:**
   ```c
   #include <stdio.h>

   int main() {
       printf("Hello, World!\n");
       return 0;
   }
   ```

3. **Save the file:** Press `Ctrl-S`

4. **Quit the editor:** Press `Ctrl-Q`

5. **Compile and run:**
   ```bash
   gcc hello.c -o hello
   ./hello
   ```

## Features Demonstrated

- **Text input:** Normal typing
- **File creation:** New file creation
- **Syntax highlighting:** C syntax is automatically highlighted
- **Saving:** Using `Ctrl-S` to save
- **Navigation:** Arrow keys, Home/End, Page Up/Down