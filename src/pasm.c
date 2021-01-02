#define PROTEUS_VM_IMPLEMENTATION
#include "vm.h"

Vm vm = {0};
Label_Table lt = {0};

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: ./pasm <input.s> <outout.o>");
        fprintf(stderr, "ERROR: expected input and output");
        exit(1);
    }

    const char *in_file_path = argv[1];
    const char *out_file_path = argv[2];

    String_View source = sv_slurp_file(in_file_path);
    vm_translate_source(source, &vm, &lt);
    vm_save_program_to_file(&vm, out_file_path);

    return 0;
}
