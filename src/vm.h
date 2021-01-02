#ifndef PROTEUS_VM_H_
#define PROTEUS_VM_H_

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))
#define VM_PROGRAM_CAPACITY 1024
#define VM_STACK_CAPACITY   1024

typedef enum {
    ERR_OK = 0,
    ERR_DIV_BY_ZERO,
    ERR_ILLEGAL_INST,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_ILLEGAL_OPERAND,
    ERR_ILLEGAL_INST_ACCESS,
} Err;

typedef int64_t Word;

typedef enum {
    NOP = 0,
    PUSH,
    ADD,
    SUB,
    MUL,
    DIV,
    JMP,
    JMP_IF,
    EQ,
    HALT,
    DUP,
    PRINT_DEBUG,
} Inst_Type;

typedef struct {
    Inst_Type type;
    Word operand;
} Inst;

typedef struct {
    Word stack[VM_STACK_CAPACITY];
    Word sp; // Stack pointer

    Inst program[VM_PROGRAM_CAPACITY];
    Word program_size;

    Word ip; // Instruction pointer
    int halt;
} Vm;

typedef struct {
    size_t count;
    const char *data;
} String_View;

#define INST_NOP()          { .type = NOP }
#define INST_PUSH(value)    { .type = PUSH, .operand=(value) }
#define INST_ADD()          { .type = ADD }
#define INST_SUB()          { .type = SUB }
#define INST_MUL()          { .type = MUL }
#define INST_DIV()          { .type = DIV }
#define INST_JMP(addr)      { .type = JMP, .operand=(addr) }
#define INST_JMP_IF(addr)   { .type = JMP_IF, operand=(addr) }
#define INST_EQ()           { .type = EQ }
#define INST_HALT()         { .type = HALT }
#define INST_DUP(addr)      { .type = DUP, .operand=(addr) }
#define INST_PRINT_DEBUG()  { .type = PRINT_DEBUG }

const char *err_as_cstr(Err err);
const char *inst_type_as_cstr(Inst_Type type);

Err vm_execute_inst(Vm *vm);
Err vm_execute_program(Vm *vm, int limit);

void vm_dump_stack(FILE *stream, const Vm *vm);
void vm_load_program_from_file(Vm *vm, const char *file_path);
void vm_load_program_from_memory(Vm *vm, Inst *program, size_t program_size);
void vm_save_program_to_file(Vm *vm, const char *file_path);

String_View sv_trim(String_View sv);
String_View sv_ltrim(String_View sv);
String_View sv_rtrim(String_View sv);
String_View cstr_as_sv(const char *cstr);
String_View sv_slurp_file(const char *file_path);
String_View sv_chop_by_delim(String_View *sv, char delim);

int sv_to_int(String_View sv);
int sv_eq(String_View a, String_View b);

Inst vm_translate_line(String_View line);

size_t vm_translate_source(String_View source, Inst *program, size_t program_capacity);

#endif  // PROTEUS_VM_H_

#ifdef PROTEUS_VM_IMPLEMENTATION

const char *err_as_cstr(Err err)
{
    switch (err) {
    case ERR_OK:                    return "ERR_OK";
    case ERR_STACK_OVERFLOW:        return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:       return "ERR_STACK_UNDERFLOW";
    case ERR_ILLEGAL_INST:          return "ERR_ILLEGAL_INST";
    case ERR_DIV_BY_ZERO:           return "ERR_DIV_BY_ZERO";
    case ERR_ILLEGAL_INST_ACCESS:   return "ERR_ILLEGAL_INST_ACCESS";
    case ERR_ILLEGAL_OPERAND:       return "ERR_ILLEGAL_OPERAND";
    default:                        assert(0 && "UNREACHABLE");
    }
}

const char *inst_type_as_cstr(Inst_Type type)
{
    switch (type) {
    case NOP:           return "NOP";
    case PUSH:          return "PUSH";
    case ADD:           return "ADD";
    case SUB:           return "SUB";
    case MUL:           return "MUL";
    case DIV:           return "DIV";
    case JMP:           return "JMP";
    case JMP_IF:        return "JMP_IF";
    case EQ:            return "EQ";
    case HALT:          return "HALT";
    case DUP:           return "DUP";
    case PRINT_DEBUG:   return "PRINT_DEBUG";
    default:            assert(0 && "UNREACHABLE");
    }
}

Err vm_execute_program(Vm *vm, int limit)
{
    while (limit != 0 && !vm->halt) {
        Err err = vm_execute_inst(vm);
        if (err != ERR_OK) {
            return err;
        }
        if (limit > 0) {
            limit--;
        }
    }
    return ERR_OK;
}

Err vm_execute_inst(Vm *vm)
{
    if (vm->ip < 0 || vm->ip >= vm->program_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }

    Inst inst = vm->program[vm->ip];

    switch (inst.type) {
    case NOP:
        vm->ip += 1;
        break;
    case PUSH:
        if(vm->sp >= VM_STACK_CAPACITY) {
            return ERR_STACK_OVERFLOW;
        }
        vm->stack[vm->sp++] = inst.operand;
        vm->ip++;
        break;
    case ADD:
        if(vm->sp < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        vm->stack[vm->sp - 2] += vm->stack[vm->sp - 1];
        vm->sp--;
        vm->ip++;
        break;
    case SUB:
        if(vm->sp < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        vm->stack[vm->sp - 2] -= vm->stack[vm->sp - 1];
        vm->sp--;
        vm->ip++;
        break;
    case MUL:
        if(vm->sp < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        vm->stack[vm->sp - 2] *= vm->stack[vm->sp - 1];
        vm->sp--;
        vm->ip++;
        break;
    case DIV:
        if(vm->sp < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        if (vm->stack[vm->sp - 1] == 0) {
            return ERR_DIV_BY_ZERO;
        }
        vm->stack[vm->sp - 2] /= vm->stack[vm->sp - 1];
        vm->sp--;
        vm->ip++;
        break;
    case JMP:
        vm->ip = inst.operand;
        break;
    case JMP_IF:
        if(vm->sp < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        if (vm->stack[vm->sp - 1]) {
            vm->sp--;
            vm->ip = inst.operand;
        } else {
            vm->ip++;
        }
        break;
    case EQ:
        if(vm->sp < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        vm->stack[vm->sp - 2] = vm->stack[vm->sp - 1] == vm->stack[vm->sp - 2];
        vm->sp--;
        vm->ip++;
        break;
    case HALT:
        vm->halt = 1;
        break;
    case DUP:
        if(vm->sp >= VM_STACK_CAPACITY) {
            return ERR_STACK_OVERFLOW;
        }
        if(vm->sp - inst.operand <= 0) {
            return ERR_STACK_UNDERFLOW;
        }
        if (inst.operand < 0) {
            return ERR_ILLEGAL_OPERAND;
        }
        vm->stack[vm->sp] = vm->stack[vm->sp - 1 - inst.operand];
        vm->sp++;
        vm->ip++;
        break;
    case PRINT_DEBUG:
        if(vm->sp < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        printf("%ld\n", vm->stack[vm->sp - 1]);
        vm->sp--;
        vm->ip++;
        break;
    default:
        return ERR_ILLEGAL_INST;
    }

    return ERR_OK;
}

void vm_dump_stack(FILE *stream, const Vm *vm)
{
    fprintf(stream, "Stack:\n");
    if (vm->sp > 0) {
        for (Word i = 0; i < vm->sp; i++) {
            fprintf(stream, "  %ld\n", vm->stack[i]);
        }
    } else {
        fprintf(stream, "  [empty]\n");
    }
}

void vm_load_program_from_memory(Vm *vm, Inst *program, size_t program_size)
{
    assert(program_size < VM_PROGRAM_CAPACITY);
    memcpy(vm->program, program, sizeof(program[0]) * program_size);
    vm->program_size = program_size;
}

void vm_load_program_from_file(Vm *vm, const char *file_path)
{
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, 
                "ERROR: Could not open file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, 
                "ERROR: Could not read file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    long m = ftell(f);
    if (m < 0) {
        fprintf(stderr, 
                "ERROR: Could not read file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    assert(m % sizeof(vm->program[0]) == 0);
    assert((size_t)m <= VM_PROGRAM_CAPACITY * sizeof(vm->program[0]));

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, 
                "ERROR: Could not read file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    vm->program_size = fread(vm->program, 
                             sizeof(vm->program[0]), 
                             m / sizeof(vm->program[0]), f);
    if (ferror(f)) {
        fprintf(stderr, 
                "ERROR: Could not read file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    fclose(f);
}

void vm_save_program_to_file(Vm *vm, 
                             const char *file_path)
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        fprintf(stderr, 
                "ERROR: Could not open file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    fwrite(vm->program, sizeof(vm->program[0]), vm->program_size, f);
    if (ferror(f)) {
        fprintf(stderr, 
                "ERROR: Could not write to file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    fclose(f);
}

String_View cstr_as_sv(const char *cstr)
{
    return (String_View) {
        .count = strlen(cstr),
        .data = cstr,
    };
}

String_View sv_ltrim(String_View sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[i])) {
        i++;
    }

    return (String_View) {
        .count = sv.count - i,
        .data = sv.data + i,    
    };
}

String_View sv_rtrim(String_View sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[sv.count - 1 - i])) {
        i++;
    }

    return (String_View) {
        .count = sv.count - i,
        .data = sv.data
    };
}

String_View sv_trim(String_View sv)
{
    return sv_rtrim(sv_ltrim(sv));
}

String_View sv_chop_by_delim(String_View *sv, char delim)
{
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i++;
    }

    String_View result = {
        .count = i,
        .data = sv->data,
    };

    if (i < sv->count) {
        sv->count -= i + 1;
        sv->data  += i + 1;
    } else {
        sv->count -= i;
        sv->data  += i;
    }

    return result;
}

int sv_eq(String_View a, String_View b)
{
    if (a.count != b.count) {
        return 0;
    } else {
        return memcmp(a.data, b.data, a.count) == 0;
    }
}

int sv_to_int(String_View sv)
{
    int result = 0;
    for (size_t i = 0; i < sv.count && isdigit(sv.data[i]); ++i) {
        result = result * 10 + sv.data[i] - '0';
    }
    return result;
}

Inst vm_translate_line(String_View line)
{
    line = sv_trim(line);
    String_View inst_name = sv_chop_by_delim(&line, ' ');
    String_View operand = sv_trim(sv_chop_by_delim(&line, ';'));
    
    if (sv_eq(inst_name, cstr_as_sv("nop"))) {
        return (Inst) { .type = NOP };
    } else if (sv_eq(inst_name, cstr_as_sv("push"))) {
        return (Inst) { .type = PUSH, .operand = sv_to_int(operand) };
    } else if (sv_eq(inst_name, cstr_as_sv("dup"))) {
        return (Inst) { .type = DUP, .operand = sv_to_int(operand) };
    } else if (sv_eq(inst_name, cstr_as_sv("add"))) {
        return (Inst) { .type = ADD };
    } else if (sv_eq(inst_name, cstr_as_sv("sub"))) {
        return (Inst) { .type = SUB };
    } else if (sv_eq(inst_name, cstr_as_sv("mul"))) {
        return (Inst) { .type = MUL };
    } else if (sv_eq(inst_name, cstr_as_sv("div"))) {
        return (Inst) { .type = DIV };
    } else if (sv_eq(inst_name, cstr_as_sv("eq"))) {
        return (Inst) { .type = EQ };
    } else if (sv_eq(inst_name, cstr_as_sv("jmp"))) {
        return (Inst) { .type = JMP, .operand = sv_to_int(operand) };
    } else if (sv_eq(inst_name, cstr_as_sv("jmp_if"))) {
        return (Inst) { .type = JMP_IF, .operand = sv_to_int(operand)  };
    } else if (sv_eq(inst_name, cstr_as_sv("halt"))) {
        return (Inst) { .type = HALT };
    } else if (sv_eq(inst_name, cstr_as_sv("print_debug"))) {
        return (Inst) { .type = PRINT_DEBUG };
    } else {
        fprintf(stderr, 
                "ERROR: `%.*s` unknown instruction", 
                (int) inst_name.count, 
                inst_name.data);
        exit(1);
    }
}

size_t vm_translate_source(String_View source, 
                           Inst *program, 
                           size_t program_capacity)
{
    size_t program_size = 0;

    while (source.count > 0) {
        assert(program_size <= program_capacity);
        String_View line = sv_trim(sv_chop_by_delim(&source, '\n'));
        if (line.count > 0 && *line.data != ';') {
            program[program_size++] = vm_translate_line(line);
        }
    }

    return program_size;
}

String_View sv_slurp_file(const char *file_path)
{
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, 
                "ERROR: Could not open file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, 
                "ERROR: Could not read file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    long m = ftell(f);
    if (m < 0) {
        fprintf(stderr, 
                "ERROR: Could not read file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    char *buffer = malloc(m);
    if (buffer == NULL) {
        fprintf(stderr, 
                "ERROR: Could not allocate memory for file: %s\n", 
                strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, 
                "ERROR: Could not read file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    size_t n = fread(buffer, 1, m, f);
    if (ferror(f)) {
        fprintf(stderr, 
                "ERROR: Could not read file %s: %s\n", 
                file_path, 
                strerror(errno));
        exit(1);
    }

    fclose(f);

    return (String_View) {
        .count = n,
        .data = buffer
    };
}

#endif // PROTEUS_BM_IMPLEMENTATION
