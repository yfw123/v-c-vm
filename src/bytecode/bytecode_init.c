#include <vcvm/bytecode.h>
#include <vpr/parameters.h>
#include <vcvm/error_codes.h>

#include "bytecode_internal.h"

static void bytecode_dispose(void* ctx);

int bytecode_init(bytecode_t* bytecode, allocator_options_t* allocator_options, const uint8_t* raw, size_t size)
{
    bytecode->hdr.dispose = &bytecode_dispose;
    bytecode->allocator_options = allocator_options;

    int result;
    size_t offset = 0;

    result = bytecode_read_uint32(&bytecode->magic, raw, size, &offset);
    if (result != VCVM_STATUS_SUCCESS)
    {
        goto done;
    }

    if (bytecode->magic != 0xDECAF)
    {
        result = VCVM_BYTECODE_BAD_MAGIC;
        goto done;
    }

    result = bytecode_read_integer_constants(bytecode, raw, size, &offset);
    if (result != VCVM_STATUS_SUCCESS)
    {
        goto free_integer_constants;
    }

    result = bytecode_read_string_constants(bytecode, raw, size, &offset);
    if (result != VCVM_STATUS_SUCCESS)
    {
        goto free_string_constants;
    }

    result = bytecode_read_artifact_constants(bytecode, raw, size, &offset);
    if (result != VCVM_STATUS_SUCCESS)
    {
        goto free_artifact_constants;
    }

    goto done;

free_artifact_constants:
    bytecode_dispose_artifact_constants(bytecode);
free_string_constants:
    bytecode_dispose_string_constants(bytecode);
free_integer_constants:
    bytecode_dispose_integer_constants(bytecode);
done:
    return result;
}

void bytecode_dispose(void* ctx)
{
    bytecode_t* bytecode = (bytecode_t*)ctx;
    bytecode_dispose_integer_constants(bytecode);
    bytecode_dispose_string_constants(bytecode);
}