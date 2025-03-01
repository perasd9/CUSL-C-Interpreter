# CUSL

This is a interpreter for the CUSL programming language, implemented in C from scratch. The project follows the structure based on "Lox", making stack-based virtual machine (VM) that executes CUSL code compiled into bytecode.

## Key components

- **Lexer (Tokenizer)** - Breaks Lox code into tokens
- **Parser** - Constructs an abstract syntax tree (AST)
- **Bytecode Compiler** - Translates AST into a sequence of instructions for the VM
- **Virtual Machine (VM)** - Interprets and executes the bytecode

## Supported Features

- Variables and Scopes (local and global variables, block scopes)
- Control Flow (if, while, for, return)
- Functions and Recursion (first-class functions, closures)
- Object-Oriented Programming (classes, methods, this, super)
- Garbage Collector (mark-and-sweep for automatic memory management)

## Example of CUSL code

Comparing to the other programming languages CUSL is pretty same with the concepts of Python, JS and similar. Nevertheless syntax and keywords are changed a little.
By the example you can recognize similarities so that's the best way to see a differencies without some special explanation for every change individual.

```bash
// function declaration
// you can declare function outside of a sanclass
sanfun helloWithYourName(name) {
    return "Hello " + name;
}

sanclass Person {
    // constructor-like method
    saninit(name, age) {
        this.name = name;
        this.age = age;
    }

    write() {
        print "My name is " + this.name + " and I am " + this.age " year(s) old";
    }

    writeOlderThan20() {
        // conditional branching
        sanif(this.age >= 20) {
            print "My name is " + this.name;
        } else {
            print "Not enough age";
        }
    }
}

// sanclass inheritance
sanclass Student < Person {

}
```
