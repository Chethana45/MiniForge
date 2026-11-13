# ⚒️ MiniForge
### Build Your Own Programming Language from Scratch

**MiniForge** is a custom interpreted programming language developed in C++17. It is designed to explore the internal workings of programming languages, including lexical analysis, parsing, Abstract Syntax Trees (AST), semantic analysis, and runtime execution.

Rather than relying on existing language engines, MiniForge aims to implement its own language-processing pipeline from the ground up.

---

## 🎯 Project Goals

- Design a custom programming language and syntax.
- Implement a lexer to tokenize source code.
- Build a recursive descent parser.
- Generate and traverse Abstract Syntax Trees.
- Implement an interpreter and runtime environment.
- Support variables, expressions, control flow, and functions.
- Develop developer tools such as a REPL and execution tracer.

## 🏗️ Architecture

```text
          MiniForge Source Code
                  |
                  v
                Lexer
                  |
                  v
                Tokens
                  |
                  v
                Parser
                  |
                  v
         Abstract Syntax Tree
                  |
                  v
          Semantic Analyzer
                  |
                  v
             Interpreter
                  |
                  v
           Runtime Engine
                  |
             +----+----+
             |         |
             v         v
           Output    Errors
```

## 🛠️ Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Core implementation |
| CMake | Build configuration |
| Recursive Descent | Parsing technique |
| Abstract Syntax Tree | Program representation |
| Tree-Walk Interpreter | Code execution |
| GoogleTest / Custom Tests | Testing |
| Git & GitHub | Version control |

## 🚀 Development Roadmap

### Phase 1 — Language Foundation
- [ ] Define MiniForge language syntax.
- [ ] Design token types and keywords.
- [ ] Implement lexical analysis.
- [ ] Recognize identifiers, numbers, strings, and operators.
- [ ] Build a command-line token inspector.

### Phase 2 — Parser & AST
- [ ] Implement recursive descent parsing.
- [ ] Define AST node structures.
- [ ] Support operator precedence.
- [ ] Parse expressions and statements.
- [ ] Implement syntax error reporting.

### Phase 3 — Interpreter & Runtime
- [ ] Implement variable declarations.
- [ ] Support integer, floating-point, boolean, and string values.
- [ ] Evaluate arithmetic and logical expressions.
- [ ] Implement conditional statements.
- [ ] Support loops.
- [ ] Implement functions and recursion.

### Phase 4 — Advanced Language Features
- [ ] Implement arrays and string operations.
- [ ] Introduce lexical scoping.
- [ ] Support closures.
- [ ] Add built-in functions.
- [ ] Implement semantic analysis.
- [ ] Improve runtime error handling.

### Phase 5 — Developer Tooling
- [ ] Build an interactive REPL.
- [ ] Support `.mf` source files.
- [ ] Implement execution tracing.
- [ ] Add debugging utilities.
- [ ] Explore bytecode compilation.
- [ ] Develop a virtual machine.
- [ ] Complete documentation and automated tests.

## 📂 Project Structure

```text
MiniForge/
│
├── include/
│   ├── lexer.hpp
│   ├── token.hpp
│   ├── parser.hpp
│   ├── ast.hpp
│   ├── interpreter.hpp
│   └── runtime.hpp
│
├── src/
│   ├── main.cpp
│   ├── lexer.cpp
│   ├── parser.cpp
│   ├── interpreter.cpp
│   └── runtime.cpp
│
├── tests/
│
├── examples/
│   ├── hello.mf
│   ├── calculator.mf
│   └── factorial.mf
│
├── docs/
│   ├── grammar.md
│   └── architecture.md
│
├── CMakeLists.txt
├── README.md
└── LICENSE
```

*The structure represents the planned organization; files and modules will be added as development progresses.*

## 💻 Example MiniForge Syntax

The following illustrates the intended language syntax:

```javascript
// Variables
let name = "MiniForge";
let version = 1;

// Functions
fn greet(person) {
    print("Hello, " + person);
}

// Recursion
fn factorial(n) {
    if (n <= 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

greet(name);
print(factorial(5));
```

**Expected output:**

```text
Hello, MiniForge
120
```

*Example of the planned syntax, not a claim that all features are currently executable.*

## 🧠 Core Computer Science Concepts

MiniForge explores:

- Compiler and interpreter design
- Lexical analysis and tokenization
- Context-free grammars
- Recursive descent parsing
- Abstract Syntax Trees
- Expression evaluation
- Symbol tables and scope resolution
- Runtime environments
- Memory and object management
- Bytecode and virtual machines

## 🔮 Future Possibilities

Beyond the initial interpreter, MiniForge aims to explore:

- Bytecode-based execution.
- Stack-based virtual machine architecture.
- Garbage collection.
- Improved error diagnostics.
- Language optimization.
- Standard library development.

## 🎓 Learning Outcomes

Building MiniForge provides practical experience with programming language implementation, systems programming, parsing techniques, runtime design, and low-level software architecture.

## 🤝 Contributions

Suggestions, bug reports, and contributions are welcome as the language evolves.

## 📜 License

This project is intended to be released under the MIT License.

---

**MiniForge — Understanding programming languages by building one.** ⚒️
