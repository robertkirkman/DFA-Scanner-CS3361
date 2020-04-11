# DFA-Scanner-CS3361
#### Link to repository: https://github.com/robertkirkman/DFA-Scanner-CS3361

This program is only compatible with POSIX-compliant operating systems, such as the UNIX-like Linux, MacOS and BSD. If such a system is unavailable, the online IDE [repl.it](https://repl.it/languages/c) includes support for compiling command-line applications like this one using the GNU C compiler on Linux.

## Compliling:

##### **gcc:**
`gcc -o scanner scanner.c automaton.c scan.c -lm`

##### **gcc + make:**
`make`

## Executing:

##### **With included examples:**
`./scanner automaton.txt tokens.txt`

##### **With custom files:**
`./scanner [automaton file] [tokens file]`
