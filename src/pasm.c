#define PROTEUS_VM_IMPLEMENTATION
#include "vm.h"

Vm vm = {0};
Translation_Context tc = {0};

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: ./pasm <input.s> <outout.o>\n");
        fprintf(stderr, "ERROR: expected input and output\n");
        exit(1);
    }

    const char *in_file_path = argv[1];
    const char *out_file_path = argv[2];

    String_View source = sv_slurp_file(in_file_path);
    translator_translate_source(source, &vm, &tc);
    vm_save_program_to_file(&vm, out_file_path);

    return 0;
}
