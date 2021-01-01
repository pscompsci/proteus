#define PROTEUS_VM_IMPLEMENTATION
#include "vm.h"

Vm vm = {0};

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "USAGE: ./prot <input.pb>");
        fprintf(stderr, "ERROR: expected input binary file");
        exit(1);
    }

    const char *in_file_path = argv[1];
    vm_load_program_from_file(&vm, in_file_path);

    for (int i = 0; i < VM_EXECUTION_LIMIT && !vm.halt; i++) {
        Err err = vm_execute_inst(&vm);
        vm_dump_stack(stdout, &vm);
        if (err != ERR_OK) {
            fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
            vm_dump_stack(stderr, &vm);
            exit(1);
        }
    }

    return 0;
}
