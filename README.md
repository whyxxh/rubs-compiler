# Rubs Compiler

this is a recreational programming project where I attempt to make a compiler and see where it can go !

### Quick TODO - Direction

- [x] make an interpreter for arithmetic expressions
- [ ] make an interpreter for a basic programming language
- [ ] make a basic C subset compiler

### more specific steps (first few days)

- [x] Build a lexer to tokenize the code : code -> Tokens
- [x] Parse the tokens into an AST
- [x] make a basic evaluator to test
- [ ] start the language parsing

### progress

**day 0x01:**

File I/O basics

**day 0x02**

Progress on lexer -> functional tokenizer

**day 0x0...idk**

I HAVE A FUCKING INTERPRETER GODDAMN

> [!NOTE]
> use after frees are the devil

**update**

i tried to make the evaluator return floats but i failled miserably

**update**

It now works, I actualy forgot to include the header in the `evaluator.c` and had a prototype of type int that returned 0 always.

