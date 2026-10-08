# uru-compiler-design

**Note:** This repository is archived and read-only. The GitHub description warns that the code "HAVE SOME BUGS".

Projects from the Compiler Design college course (URU): three independent C++20 CMake projects covering regex/DFA validation and a small expression interpreter.

## Projects

- **`dfa-regex-validator`** — interactive console program that tokenizes an expression and prints the token types matched by validators in `src/lib/validators/` (digits, floats, identifiers, integers, letters, operators, reserved words, words). A prebuilt Windows executable is committed under `bin/`.
- **`general-regex-validator`** — validates words against expressions, printing `VALID: <word>` or `MATCH NOT FOUND: <word>`.
- **`interpreter`** — lexical analyzer, syntax analyzer (parse tree and syntax tree) and a syntax-tree interpreter. It runs `src/data/supercode.ralvarezdev`, which uses assignments, arithmetic (`+ - * / ^`), parentheses, `root(...)` and `print(...)`. Debug output is toggled by constants at the top of `src/main.cpp`.
- **`exceptions`** — shared `expression_exception` library.

## Building

Requires CMake 3.28+ and a C++20 compiler.

```bash
cd interpreter
cmake -S . -B build
cmake --build build
```

The interpreter finds its data file relative to a root folder named `interpreter` (see `move_to_root` and `move_to_data` in `src/main.cpp`), so run it from within the repository layout.

## License

GNU General Public License v3.0 (see `LICENSE`).
