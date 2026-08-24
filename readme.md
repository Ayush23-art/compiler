# LLVM-Based Compiler for a C-like Language

A compiler for a small C-like programming language implemented in **C++** using the **LLVM** infrastructure. The compiler performs lexical analysis, parsing, semantic analysis, and LLVM IR generation before producing native executables through Clang.

---

## Features

### Frontend

* Lexical analysis (tokenization)
* Recursive descent parser
* Abstract Syntax Tree (AST) construction
* Scoped symbol table
* Semantic analysis
* Static type checking

### Language Features

* Variable declarations
* Arithmetic expressions
* Assignment expressions
* Function declarations
* Function calls
* Recursive functions
* `if` / `else`
* `while`
* Short-circuit logical operators (`&&`, `||`)
* Console input using `scan()`
* Console output using `print()`

### Backend

* LLVM IR generation
* Native executable generation using Clang
* Integration with C standard library (`printf` / `scanf`)

---

## Compiler Pipeline

```
Source Code
     │
     ▼
+---------------------+
| Lexer               |
+---------------------+
     │
     ▼
+---------------------+
| Recursive Descent   |
| Parser              |
+---------------------+
     │
     ▼
+---------------------+
| Abstract Syntax     |
| Tree (AST)          |
+---------------------+
     │
     ▼
+---------------------+
| Semantic Analyzer   |
| • Symbol Tables     |
| • Type Checking     |
| • Scope Resolution  |
+---------------------+
     │
     ▼
+---------------------+
| LLVM IR Generator   |
+---------------------+
     │
     ▼
output.ll
     │
     ▼
clang
     │
     ▼
Native Executable
```

---

## Supported Grammar (Overview)

```c
// Variable declaration
int a = 10;

// Assignment
a = a + 5;

// Function declaration
int add(int a, int b) {
    return a + b;
}

// Function call
int x = add(2, 3);

// Conditional
if (x > 5) {
    print(x);
}
else {
    print(0);
}

// Loop
while (x > 0) {
    x = x - 1;
}

// Input / Output
scan(x);
print(x);
```

---

## Example Program

```c
int factorial(int n) {

    if (n == 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

int main() {

    int x;

    scan(x);

    print(factorial(x));

    return 0;
}
```

### Sample Execution

```
Input:
5

Output:
120
```

---

## Project Structure

```
compiler/
│
├── lexer/
│   ├── lexer.cpp
│   ├── lexer.h
│   └── Token.h
│
├── parser/
│   ├── parser.cpp
│   └── parser.h
│
├── semantic_analyzer/
│   ├── semanticanalyzer.cpp
│   ├── semanticanalyzer.h
│   └── datatype.h
│
├── codegeneration/
│   ├── codegenerator.cpp
│   └── codegenerator.h
│
├── main/
│   └── main.cpp
│
├── build.sh
│
└── README.md
```

---

## Building

### Requirements

* C++17 or later
* LLVM
* Clang

### Build

```bash
./build.sh
```

---

## Running

After building,

```bash
./compiler
```

The compiler:

1. Reads the source program.
2. Performs lexical analysis.
3. Builds the AST.
4. Performs semantic analysis.
5. Generates LLVM IR.
6. Invokes Clang.
7. Produces and executes the final native executable.

---

## Example LLVM IR

```llvm
define i32 @main() {

entry:

    %x = alloca i32

    call i32 (ptr, ...)
        @scanf(ptr @0, ptr %x)

    %0 = load i32, ptr %x

    %1 = call i32 @factorial(i32 %0)

    call i32 (ptr, ...)
        @printf(ptr @1, i32 %1)

    ret i32 0
}
```

---

## Design Highlights

* Recursive descent parser with operator precedence.
* Scoped symbol tables for nested blocks and functions.
* Semantic analysis validates declarations, assignments, function calls, return types, and scope resolution.
* LLVM IR generation follows Static Single Assignment (SSA) form.
* Short-circuit evaluation of logical operators is implemented using LLVM basic blocks and PHI nodes.
* Supports recursive function calls and automatic stack frame generation.

---

## Current Limitations

* Primitive types only (`int`, `char`, `bool`, `void`)
* No arrays
* No structures
* No pointers
* No `for` loops
* No optimization passes

---

## Future Work

* Arrays and indexing
* Structures and user-defined types
* Unary operators
* Constant folding
* Dead code elimination
* Register allocation
* Direct object code generation
* LLVM optimization pipeline integration

---

## Technologies Used

* C++
* LLVM
* Clang
* Recursive Descent Parsing
* Static Semantic Analysis
* LLVM IR
* SSA Form

---

## References

* LLVM Documentation
* Crafting Interpreters — Robert Nystrom
* Engineering a Compiler — Cooper & Torczon
* Modern Compiler Implementation in C
