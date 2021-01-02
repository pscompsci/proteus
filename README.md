# Stack-based VM

## Components

* vm.h  - Header only core of the virtual machine
* proteus.c - Runtime execution of proteus binary (.pb) files
* proteusc.c - Assembly translator. Takes proteus assembly (.pasm) input and outputs proteus binary

## Getting Started

To compile the required source files:

```console
make all
```

To translate the example assembly source files

```console
make examples
```

To translate a single example source file (eg. fib.vmas)

```console
make ./examples/fib.pb
```

To translate a user written source assembly file

```console
./prott <input file_path>.pasm <output file_path>.pb
```

To run a binary file

```console
./prot <file_path>.pb
```

## File extensions

### .pasm
VM Assembly language file. For examples, see [./examples](./examples)

### .pb
VM Bytecode file
