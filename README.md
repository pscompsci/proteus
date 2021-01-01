# Stack-based VM

## Components

* vm.h  - header only core of the virtual machine
* proteus.c - runtime execution of .vmbt files
* proteusc.c - assembly translator. Outputs translated binary file that can be executed (reads in .pasm and outputs .pb)

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
VM Assembly language file

### .pb
VM Bytecode file
