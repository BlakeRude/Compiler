# Compiler
This is my compiler I built in C++, Flex, and Bison, used to compile C-.

C- is a custom language and is a subset of C with some modifications.
The compiler targets Tiny Machine which emulates running on an actual processor.
Changes in the code generation could allow for it to run on actual hardware.


## How to use:
Clone the repository and using a UNIX terminal, locate the src directory in the repo

In a UNIX terminal, input 
```
make
```
Now the binary should be built.

After creating a c- program, you may use the compiler with
```
./c- file.c-
```
Which will create the ASM to feed to the tiny machine.
```
./tm file.tm
```
Your program should now be running!

### Flags
#### -h
help (shows these flags)

#### -d
turns on yydebug
#### -S
turns on symbol table debug
#### -P
prints abstract syntax tree during compile
#### -M
prints memory tree during compile


