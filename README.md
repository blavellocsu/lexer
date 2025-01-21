# Lexer and Syntax Analyzer

This project implements the lexical analysis and syntax analysis stages of a compiler, developed in C++. It processes an input source code file, tokenizes the content, and performs syntax analysis to ensure the code adheres to defined grammatical rules.

## Features

- **Lexical Analysis**: Reads the input source code and converts it into a sequence of tokens, identifying elements such as keywords, identifiers, literals, and operators.

- **Syntax Analysis**: Parses the sequence of tokens to verify the syntactical structure of the code, ensuring it conforms to the specified grammar.

## Files

- `lexer.cpp`: Contains the implementation of the lexical and syntax analyzer.

- `SampleInputFile1.txt` & `SampleInputFile2.txt`: Provide example source code files used for testing the lexer and parser.

- `SampleOutputFile1.txt`: Demonstrates the expected output after processing `SampleInputFile1.txt`.

- `Assignment1 Tokens List1.txt`: Lists the tokens identified during the lexical analysis phase.

## Compilation and Execution

### Prerequisites

- **C++ Compiler**: Ensure that a C++ compiler (e.g., g++) is installed on your system.

### Compilation

To compile the program, use the following command:

```bash
g++ -o lexer lexer.cpp
```
## Contributors:

Brandon Lavello
brandonlavello@csu.fullerton.edu

Vincent DeAugustine
vinnied44@csu.fullerton.edu
a
