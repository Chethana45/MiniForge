# Architecture
MiniForge 0.1 is a C++17 tree-walk interpreter.

- Lexer: scans source and emits typed tokens with line numbers.
- Parser: recursive-descent parser with precedence layers.
- AST: expression and statement node hierarchy.
- Environment: chained lexical scopes and variable lookup.
- Interpreter: evaluates expressions and executes statements.
- Runtime: values include null, number, boolean, string, array and function.

Implemented: variables, assignment, arithmetic, comparisons, logical operations, print, if/else, while, functions, recursion, lexical closures, arrays, and built-ins `len`, `push`, `type`.

CLI: `--tokens` prints lexical tokens; `--trace` emits statement execution traces.

Limitations: property/index syntax beyond the implemented array forms, full semantic analysis, bytecode compilation, VM, garbage collection, and advanced multiline REPL editing are not implemented in this version.
