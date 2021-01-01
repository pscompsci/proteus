# Stack-based VM

## Files

* vm.h  - header only core of the virtual machine
* vmt.c - assembly translator. Outputs translated binary file that can be executed (reads in .vmas and outputs .vmbt)
* vme.c - runtime execution of .vmbt files

## File extensions

### .vmas
VM Assembly language file

### .vmbt
VM Bytecode file
