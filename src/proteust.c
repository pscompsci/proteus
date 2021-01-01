#define PROTEUS_VM_IMPLEMENTATION
#include "vm.h"

Vm vm = {0};

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: ./prott <input.pasm> <outout.pb>");
        fprintf(stderr, "ERROR: expected input and output");
        exit(1);
    }

    const char *in_file_path = argv[1];
    const char *out_file_path = argv[2];

    String_View source = sv_slurp_file(in_file_path);
    vm.program_size = vm_translate_source(source, vm.program, VM_PROGRAM_CAPACITY);
    vm_save_program_to_file(vm.program, vm.program_size, out_file_path);

    return 0;
}
