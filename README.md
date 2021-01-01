# Stack-based VM

## Files

* vm.h  - header only core of the virtual machine
* vmt.c - assembly translator. Outputs translated binary file that can be executed (reads in .vmas and outputs .vmbt)
* vme.c - runtime execution of .vmbt files

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
make ./examples/fib.vmbt
```

To translate a user written source assembly file

```console
./vmt <input file_path>.vmas <output file_path>.vmbt
```

To run a binary file

```console
./vme <file_path>.vmbt
```

## File extensions

### .vmas
VM Assembly language file

### .vmbt
VM Bytecode file
