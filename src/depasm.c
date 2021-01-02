#define PROTEUS_VM_IMPLEMENTATION
#include "./vm.h"

Vm vm = {0};

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, 
                "Usage: ./deprott <input_file_path>.pb\n");
        fprintf(stderr, "ERROR: No input binary file provided\n");
        return 1;
    }

    const char *input_file_path = argv[1];
    const char *extension = get_filename_ext(input_file_path);
    
    if (strcmp(extension, "o") != 0) {
        fprintf(stderr, 
                "Usage: ./deprott <input_file_path>.pb <output_file_path>.pasm\n");
        fprintf(stderr, "ERROR: Incorrect binary file format\n");
        return 1;
    }

    vm_load_program_from_file(&vm, input_file_path);

    for(Word i = 0; i < vm.program_size; i++) {
        switch(vm.program[i].type) {
        case NOP:
            printf("nop\n");
            break;
        case PUSH:
            printf("push %ld\n", vm.program[i].operand);
            break;
        case ADD:
            printf("add\n");
            break;
        case SUB:
            printf("sub\n");
            break;
        case MUL:
            printf("mul\n");
            break;
        case DIV:
            printf("div\n");
            break;
        case JMP:
            printf("jmp %ld\n", vm.program[i].operand);
            break;
        case JMP_IF:
            printf("jmp_if %ld\n", vm.program[i].operand);
            break;
        case EQ:
            printf("eq\n");
            break;
        case HALT:
            printf("halt\n");
            break;
        case DUP:
            printf("dup %ld\n", vm.program[i].operand);
            break;
        case PRINT_DEBUG:
            printf("print_debug\n");
            break;
        }
    }

    return 0;
}