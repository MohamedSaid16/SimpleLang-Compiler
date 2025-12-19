# SimpleLang Compiler

A complete compiler and interpreter for the **SimpleLang** programming language, written in modern **C++17**.  
This project implements all phases of compilation—from lexical analysis to bytecode generation—with an interactive REPL environment for rapid experimentation.

---

## ✨ Features

- **Full Compiler Pipeline**  
  Lexer → Parser → Semantic Analyzer → Interpreter / Compiler

- **Static Typing**
  - Type inference
  - Compile-time type checking

- **Rich Language Features**
  - Variables and assignments
  - Arithmetic operations: `+`, `-`, `*`, `/`, `%`
  - Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
  - Logical operators: `&&`, `||`, `!`
  - Control flow: `if-else`, `while` loops
  - Function declarations and calls
  - Built-in standard library functions

- **Interactive REPL**
  - Read–Eval–Print Loop for testing snippets and experimenting with the language

- **Comprehensive Testing**
  - Unit tests for lexer, parser, interpreter, and other components

- **Modular Architecture**
  - Clean separation of concerns between frontend, analysis, and runtime components

---

## 📁 Project Structure

```text
SimpleLang-Compiler/
├── docs/                    # Documentation
│   ├── language_spec.md     # Language specification
│   ├── compiler_design.md   # Architecture documentation
│   └── examples.md          # Sample programs
├── include/                 # Header files
│   ├── lexer/               # Lexical analysis
│   ├── parser/              # Syntax parsing & AST
│   ├── semantic/            # Semantic analysis and type checking
│   ├── interpreter/         # Interpreter
│   ├── compiler/            # Bytecode compiler / codegen
│   ├── core/                # Core utilities
│   └── runtime/             # Runtime library
├── src/                     # Implementation files
│   ├── lexer/
│   ├── parser/
│   ├── semantic/
│   ├── interpreter/
│   ├── compiler/
│   ├── runtime/
│   ├── core/
│   └── main.cpp             # Entry point (CLI / REPL)
├── examples/                # Example programs
│   ├── hello.sl             # Hello World
│   ├── variables.sl         # Variable examples
│   ├── conditions.sl        # Conditional logic
│   └── loops.sl             # Loop examples
├── tests/                   # Unit tests
│   ├── lexer_tests.cpp
│   ├── parser_tests.cpp
│   └── interpreter_tests.cpp
├── build/                   # Build directory (out-of-source)
├── CMakeLists.txt           # Build configuration
└── README.md                # This file
```

---

## 🚀 Quick Start

### Prerequisites

- **C++17** compatible compiler  
  - GCC 7+, Clang 5+, or MSVC 2017+
- **CMake** 3.10 or higher
- **Make** or **Ninja** build system

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/SimpleLang-Compiler.git
cd SimpleLang-Compiler

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
make -j"$(nproc)"

# Alternatively, use Ninja for faster builds
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
ninja
```

---

## ▶️ Running Examples

From the `build` directory (after building):

```bash
# Run the Hello World example
./simplelang ../examples/hello.sl

# Run other examples
./simplelang ../examples/variables.sl
./simplelang ../examples/conditions.sl
./simplelang ../examples/loops.sl
```

---

## 💬 Using the REPL

```bash
# Start the interactive REPL
./simplelang
```

Example REPL session:

```text
> let x = 10
> let y = 20
> print(x + y)
30
> function add(a, b) { return a + b; }
> print(add(5, 3))
8
> exit
```

---

## 📚 Language Syntax

> Note: Syntax shown here is illustrative; for full details, see [`docs/language_spec.md`](docs/language_spec.md).

### Variables

```python
let x = 10
let name = "John"
let pi = 3.14159
let flag = true
```

### Control Flow

```python
# If-else statement
if x > 10 then
    print("Greater than 10")
else
    print("10 or less")
end

# While loop
let i = 0
while i < 5 do
    print(i)
    let i = i + 1
end
```

### Functions

```python
# Function declaration
function add(a: int, b: int): int {
    return a + b
}

# Function call
let result = add(10, 20)
print(result)
```

### Built-in Functions

```python
print("Hello", "World")       # Print to console
let inputVal = input()        # Read user input
let str = toString(42)        # Convert to string
let num = toInt("123")        # Convert to integer
let len = length("hello")     # Get string length
```

---

## 🧪 Running Tests

From the `build` directory:

```bash
# Run CTest
make test

# Or run a custom test runner if provided
./run_tests            # Runs all tests (if this binary is generated)
```

---

## 🔧 Development

### Adding New Language Features

Typical steps to extend the language:

1. **Update Lexer** (`include/lexer/`, `src/lexer/`)
   - Add new tokens and update tokenization rules.
2. **Update Parser** (`include/parser/`, `src/parser/`)
   - Extend the grammar to handle the new constructs.
3. **Update AST** (`include/parser/AST.h`)
   - Add or modify AST node types.
4. **Update Semantic Analyzer** (`include/semantic/`, `src/semantic/`)
   - Implement type rules and semantic checks.
5. **Update Interpreter / Compiler**
   - Interpreter: `include/interpreter/`, `src/interpreter/`
   - Compiler / code generator: `include/compiler/`, `src/compiler/`
6. **Add Tests** (`tests/`)
   - Add or extend unit tests to cover the new behavior.

### Code Style Guidelines

- Follow **RAII** principles for resource management.
- Prefer **smart pointers** (`std::unique_ptr`, `std::shared_ptr`) over raw owning pointers.
- Maintain **const-correctness** throughout the codebase.
- Use clear, descriptive variable and function names.
- Document complex algorithms and non-obvious design choices.
- Always add or update **unit tests** when changing behavior.

---

## 📊 Performance

The interpreter and runtime are designed with performance in mind:

- Efficient AST walking for expression and statement evaluation.
- Hash-based symbol tables for fast variable lookup.
- Minimal allocations and reuse of internal structures where possible.

For optimal performance builds:

```bash
# Build with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release

# Enable link-time optimization (LTO)
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
```

(Options may vary by compiler and platform.)

---

## 🤝 Contributing


1. Fork the repository.
2. Create a feature branch:  
   ```bash
   git checkout -b feature/amazing-feature
   ```
3. Commit your changes:  
   ```bash
   git commit -m "Add amazing feature"
   ```
4. Push to your fork:  
   ```bash
   git push origin feature/amazing-feature
   ```
5. Open a Pull Request describing your changes.

### Areas for Contribution

- New language features (e.g., arrays, records/structs, classes).
- Improved error reporting and diagnostics.
- Parser and interpreter optimizations.
- Additional sample programs in `examples/`.
- Documentation improvements in `docs/`.
- Bug fixes and refactoring.

---


## 🙏 Acknowledgments

- Inspired by classic compiler texts such as:
  - *Compilers: Principles, Techniques, and Tools* (the “Dragon Book”)
  - *Crafting Interpreters*
- Built with modern C++ best practices.
- Thanks to all current and future contributors helping to improve **SimpleLang**.

---


## 📈 Roadmap

Planned and in-progress work includes:

- [x] Lexer and Parser
- [x] Semantic Analysis
- [x] Interpreter
- [ ] Bytecode Compiler
- [ ] Virtual Machine
- [ ] Garbage Collector
- [ ] Standard Library Expansion
- [ ] Package Manager
- [ ] IDE / Editor Integration (syntax highlighting, LSP, etc.)

---

