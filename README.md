# Stack-based VM

## Getting Started

To compile the required source files:

```console
make all
```

To translate the example assembly source files

```console
make examples
```

To translate a single example source file (eg. fib.s)

```console
make ./examples/fib.o
```

To translate a user written source assembly file

```console
./pasm <input file_path>.s <output file_path>.o
```

To run a binary file

```console
./proteus <file_path>.o
```

## Components

### vm.h
Header only core of the virtual machine

### proteus.c
Runtime execution of proteus binary object (.o) files

### pasm.c
Assembly translator/assembler. Takes proteus assembly source (.s) input and outputs proteus binary (.o)

### depasm.c
Disassembles/detranslates proteus binery object files

## File extensions

### .s
VM Assembly language source file. For examples, see [./examples](./examples)

### .o
VM Bytecode object file
