# Development Guide

## Building from Source

### Development Environment Setup

1. **Install dependencies:**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install build-essential cmake clang-format
   
   # macOS
   brew install cmake clang-format
   ```

2. **Clone and configure:**
   ```bash
   git clone <repository-url>
   cd proto
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   ```

3. **Build with development tools:**
   ```bash
   make -j$(nproc)  # Build with multiple cores
   ```

## Code Style

We use `clang-format` for consistent code formatting:

```bash
# Format all files
./scripts/format.sh

# Check formatting (dry run)
find . -name '*.c' -o -name '*.h' | grep -v build | xargs clang-format --dry-run
```

### Style Guidelines

- **Indentation:** 2 spaces, no tabs
- **Line length:** 80 characters max
- **Braces:** Allman style (new line)
- **Naming:** `snake_case` for functions and variables
- **Module prefixes:** Use module prefixes for public functions

## Testing

### Running Tests

```bash
cd build
make test  # Run all tests

# Or run test executable directly
./tests/test_runner
```

### Test Structure

- **Unit tests:** Individual module testing
- **Integration tests:** Cross-module functionality
- **Manual testing:** Interactive editor testing

## Contributing

### Development Workflow

1. **Create feature branch:** `git checkout -b feature-name`
2. **Make changes:** Follow code style guidelines
3. **Add tests:** Ensure new functionality is tested
4. **Run full test suite:** `make test && make format`
5. **Commit:** Use descriptive commit messages
6. **Push:** Create pull request

### Commit Message Format

```
type(scope): brief description

Detailed explanation if needed.

Fixes #issue-number
```

Types:
- `feat`: New feature
- `fix`: Bug fix  
- `docs`: Documentation
- `refactor`: Code refactoring
- `test`: Test additions
- `style`: Code style changes

## Module Organization

```
src/
├── core/           # Core editor logic
├── terminal/       # Terminal handling
├── io/            # Input/output operations
├── file/          # File operations
├── editing/       # Text editing operations
└── synthesis/     # Syntax highlighting
```

### Adding New Modules

1. Create directory under `src/`
2. Add `.c` and `.h` files
3. Update `CMakeLists.txt` to include new files
4. Add tests under `tests/`
5. Update documentation

## Debugging

### Debug Build

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON ..
make
```

### Common Debugging Tools

- **GDB:** `gdb ./proto`
- **Valgrind:** `valgrind --leak-check=full ./proto`
- **AddressSanitizer:** Built-in with sanitizers flag

## Performance Profiling

```bash
# Use perf for performance analysis
perf record --call-graph=dwarf ./proto
perf report
```

## Release Process

1. Update version in `CMakeLists.txt`
2. Update `CHANGELOG.md`
3. Tag release: `git tag -a v0.1.0 -m "Release version 0.1.0"`
4. Push tag: `git push origin v0.1.0`
5. Create GitHub release