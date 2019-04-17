#include <vcvm/instruction.h>
#include <vcvm/error_codes.h>
#include <vcvm/vm.h>
#include <vcvm/stack_value.h>
#include <vpr/parameters.h>

int load_const_artifact(vm_t* vm, uint32_t a)
{
    if (a > vm->bytecode->artifact_count)
    {
        return VCVM_ERROR_BAD_CONSTANT_COUNT;
    }

    stack_value_t* value = (stack_value_t*)allocate(vm->allocator_options, sizeof(stack_value_t));
    if (value == NULL)
    {
        return VCVM_ERROR_CANT_ALLOCATE;
    }

    uint8_t* uuid = vm->bytecode->artifacts[a];

    stack_value_init(value, vm->allocator_options);
    int result = stack_value_set_uuid(value, uuid);
    if (result != VCVM_STATUS_SUCCESS)
    {
        return result;
    }

    return vm_push(vm, value);
}