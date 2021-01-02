#define PROTEUS_VM_IMPLEMENTATION
#include "vm.h"

Vm vm = {0};

static char *shift(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

static void usage(FILE *stream, const char *program)
{
    fprintf(stream, "Usage: %s i <input_filepath>.pb [-l <limit>] [-d]\n", program);
    fprintf(stream, "\t\t i - mandatory path to the binary file to execute\n");
    fprintf(stream, "\t\t-l - limit to the number of instructions executed\n");
    fprintf(stream, "\t\t-d - optional debug\n");
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int main(int argc, char **argv[])
{
    const char *program = shift(&argc, &argv);

    printf("Program: %s\n", program);
    const char *input_file_path = NULL;
    int limit = -1; // infinite iterations by default
    int debug = 0;

    while (argc > 0) {
        const char *flag = shift(&argc, &argv);

        if (strcmp(flag, "i") == 0) {
            if (argc == 0) {
                usage(stderr, program);
                fprintf(stderr, 
                        "ERROR: No argument is provided for the flag %s\n", 
                        flag);
                return 1;
            }
            input_file_path = shift(&argc, &argv);

            char *extension = get_filename_ext(input_file_path);
            if (strcmp(extension, "pb") != 0) {
                usage(stderr, program);
                fprintf(stderr, "ERROR: Incorrect binary file format\n");
                return 1;
            }

        } else if (strcmp(flag, "-l") == 0) {
            if (argc == 0) {
                usage(stderr, program);
                fprintf(stderr, 
                        "ERROR: No argument is provided for the flag %s\n", 
                        flag);
                return 1;
            }
            char *limit_str = shift(&argc, &argv);
            limit = atoi(limit_str);
            if (limit == 0) {
                usage(stderr, program);
                fprintf(stderr, 
                        "ERROR: `%s` converts to 0 iterations\n", 
                        limit_str);
                return 1;
            }
        } else if (strcmp(flag, "-d") == 0) {
            debug = 1;
        } else {
            usage(stderr, program);
            fprintf(stderr, "ERROR: Invalid flag %s\n", flag);
            return 1;
        }
    }

    if (input_file_path == NULL) {
        usage(stderr, program);
        fprintf(stderr, 
                "ERROR: Input file path was not provided. Nothing to execute\n");
        return 1;
    }

    vm_load_program_from_file(&vm, input_file_path);

    if (!debug) {
        Err err = vm_execute_program(&vm, limit);
        vm_dump_stack(stdout, &vm);

        if (err != ERR_OK) {
            fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
            return 1;
        }
    } else {
        while (limit != 0 && !vm.halt) {
            vm_dump_stack(stdout, &vm);
            getchar();
            Err err = vm_execute_inst(&vm);
            if (err != ERR_OK) {
                fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
                return 1;
            }
            if (limit > 0) {
                --limit;
            }
        }
    }

    return 0;
}
