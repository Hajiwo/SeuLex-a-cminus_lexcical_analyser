# SeuLex — C-minus lexer generator

A C++ compiler-design course project. Lex-style rules are standardized and
converted from regular expressions to NFAs, then combined into a DFA and
minimized. The generator writes C++ scanner source for the supplied C-minus sample.

## Build and run

Requires a C++17 compiler and Python 3 for the smoke test.

```sh
make smoke
```

The generator reads `input/c_minus.l`, creates `output/lex.yy.cpp`, and the
compiled scanner reads `cminus.c`, writing tokens to `tokenList.l`.
Run commands from the repository root because paths are working-directory based.
Existing checked-in executables are historical artifacts; build from source.

## Structure and scope

- `Method_RE.cpp`: regular-expression normalization and postfix conversion.
- `Method_Automata.cpp`: NFA/DFA construction and minimization.
- `Read_Lex.cpp`: specification parsing and scanner-source emission.
- `srcFile/FUCTION.l`: scanner runtime template.

The smoke test checks keyword-versus-identifier behavior. This is an educational
ASCII lexer, not a production parser. Unicode, malformed input and long-token
buffer-boundary behavior are not covered by the current validation.
