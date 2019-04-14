#ifndef VCVM_INSTRUCTION_H
#define VCVM_INSTRUCTION_H

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

typedef struct vm vm_t;

typedef int (*arity0_t)(vm_t* vm);
typedef int (*arity1_t)(vm_t* vm, uint32_t a);
typedef int (*arity2_t)(vm_t* vm, uint16_t a, uint16_t b);
typedef int (*arity3_t)(vm_t* vm, uint8_t a, uint8_t b, uint8_t c);

typedef struct instruction
{
    const uint8_t arity;
    const union
    {
        arity0_t arity0;
        arity1_t arity1;
        arity2_t arity2;
        arity3_t arity3;
    } handler;
} instruction_t;

int nop(vm_t* vm);
int invalid(vm_t* vm);
int halt(vm_t* vm);
int push_string_constant(vm_t* vm, uint32_t a);

static const instruction_t NOP = {
    .arity = 0,
    .handler = {
        .arity0 = &nop
    }
};

static const instruction_t INVALID = {
    .arity = 0,
    .handler = {
        .arity0 = &invalid
    }
};

static const instruction_t HALT = {
    .arity = 0,
    .handler = {
        .arity0 = &halt
    }
};



#ifdef __cplusplus
}
#endif  //__cplusplus

#endif
