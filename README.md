# BrainFuckFuckFuck

An interpreter for Brainfuck and an extended Brainfuck-style language that adds labels, function calls, arguments, return values, explicit termination, cell reset, and comments. The extension keeps the small tape-based execution model while adding control-flow and reuse features to classic Brainfuck.

## Language overview

The original Brainfuck commands remain available for moving the tape pointer, changing cell values, performing loops, and reading or writing bytes. Additional operators are:

| Operator | Meaning |
| --- | --- |
| `^` | Jump to a label. |
| `&` | Call the function beginning at a label. |
| `$` | Copy a completed function's return value into the current cell. |
| `!` | Return from a function, or terminate the top-level program. |
| `#` | Read or write a function argument using an argument index. |
| `*` | Set the current cell to zero. |
| `/` | Begin a comment. |

Labels use otherwise unregistered alphabetic characters. `^` followed by a label character jumps to that label, even when its definition appears later in the source. A label can also mark a callable function region. Function bodies use their own tape and pointer; arguments provide the path for passing values in, and `$` reads the value returned from the cell current at `!`.

For example, `#0` loads the first argument in a function, while `&q` calls the function labelled `q`. A function definition must encounter `!` to return to the caller. At top level, `!` stops execution.

## Build

The implementation is written in the CPL language and the repository's Makefile drives its compiler/build workflow. Use the provided project environment and run:

```bash
make
```

The resulting interpreter executable is written to the output path configured by the Makefile. Use `make clean` to remove generated artifacts if supported by the current build setup.

## Run the examples and checks

The repository contains sample programs and expected output files under `tests/`. Run the full test target with:

```bash
make test
```

The raw Brainfuck cases and extended-language cases can also be run separately:

```bash
make test-raw
make test-bfpp
```

The Python runner supports using an already-built interpreter:

```bash
python3 tests/run_tests.py --no-build --binary ./out-cpl
```

## Source layout

- `main.cpl` is the application entry point.
- `src/` contains tokenization and interpreter logic.
- `include/` contains declarations shared by the implementation.
- `tests/` holds source programs, expected output, and the test runner.
- `Makefile` defines the build and test targets.

The language is experimental; consult the parser/interpreter source when a program depends on subtle label, loop, or argument behavior.

