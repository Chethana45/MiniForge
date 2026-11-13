# ⚒️ MiniForge
### A Programming Language & Tree-Walk Interpreter Built from Scratch in C++17

MiniForge is an educational interpreted language implemented in C++17. It demonstrates lexical analysis, recursive-descent parsing, AST construction, lexical environments, and runtime evaluation.

> **Release status:** v0.1.0 — working educational interpreter. A bytecode compiler and virtual machine are future extensions, not part of this release.

## Features
- Handwritten lexer and line-aware tokens
- Recursive-descent parser with operator precedence
- AST-based tree-walk interpreter
- Numbers, strings, booleans, null
- Variables, assignment, arithmetic, comparisons, logical operators
- `if` / `else`, `while`
- Functions, recursion, lexical scopes and closures
- Arrays and built-ins: `len`, `push`, `type`
- REPL, token inspection (`--tokens`) and execution tracing (`--trace`)
- Example programs and automated smoke tests

## Build (Linux / WSL)
```bash
sudo apt update
sudo apt install -y build-essential cmake
cmake -S . -B build
cmake --build build -j
./build/miniforge examples/factorial.mf
```

## Build (Windows)
Install Visual Studio Build Tools with the Desktop development with C++ workload and CMake:
```powershell
cmake -S . -B build
cmake --build build --config Release
.uild\Release\miniforge.exe examples\hello.mf
```

## Usage
```bash
./build/miniforge examples/hello.mf
./build/miniforge --tokens examples/factorial.mf
./build/miniforge --trace examples/factorial.mf
./build/miniforge
```
REPL commands: `:quit` exits. Source statements must be complete; use semicolons.

## Example
```javascript
fn factorial(n) {
    if (n <= 1) { return 1; }
    return n * factorial(n - 1);
}
print(factorial(5)); // 120
```

## Project layout
```text
MiniForge/
├── src/main.cpp
├── examples/
├── tests/run_tests.py
├── docs/
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## Tests
After building:
```bash
python3 tests/run_tests.py build/miniforge
```

## Roadmap
- [x] Phase 1 — Lexer and language foundation
- [x] Phase 2 — Recursive-descent parser and AST
- [x] Phase 3 — Interpreter, variables, control flow, functions and recursion
- [x] Phase 4 — Arrays, lexical scopes, closures and runtime diagnostics
- [x] Phase 5 — REPL, CLI tooling, tracing, examples, tests and documentation
- [ ] Future extension — Bytecode compiler and virtual machine
- [ ] Future extension — Garbage collection and optimizer

## License
MIT. See [LICENSE](LICENSE).

**MiniForge — Understand programming languages by building one.** ⚒️
