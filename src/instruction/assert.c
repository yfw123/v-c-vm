#include <vcvm/instruction.h>
#include <vcvm/error_codes.h>
#include <vcvm/stack_value.h>
#include <vcvm/vm.h>

int vcvm_assert(vcvm_vm_t* vm)
{
    int result;
    vcvm_stack_value_t* value;

    result = vcvm_vm_pop(vm, &value);
    if (result != VCVM_STATUS_SUCCESS)
    {
        return result;
    }

    if (value->type != VCVM_STACK_VALUE_TYPE_INTEGER)
    {
        result = VCVM_ERROR_VM_BAD_TYPES;
        goto done;
    }

    if (value->integer != 1)
    {
        result = VCVM_ERROR_VM_ASSERTION_ERROR;
        goto done;
    }
    result = VCVM_STATUS_SUCCESS;

done:
    dispose((disposable_t*)value);
    release(value->allocator_options, value);
    return result;
}

const vcvm_instruction_t VCVM_ASSERT = {
    .name = "ASSERT",
    .arity = 0,
    .handler = {
        .arity0 = &vcvm_assert,
    }
};