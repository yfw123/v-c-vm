#include <vcvm/bytecode.h>
#include <vcvm/error_codes.h>
#include <string.h>
#include "bytecode_internal.h"

int bytecode_read_artifact_constants(bytecode_t* bytecode, const uint8_t* raw, size_t size, size_t* offset)
{
    int result;
    result = bytecode_read_uint32(&bytecode->artifact_count, raw, size, offset);
    if (result != VCVM_STATUS_SUCCESS)
    {
        goto done;
    }

    // Read out the integer constants.
    bytecode->artifacts = (uint8_t **)allocate(
        bytecode->allocator_options,
        UUID_SIZE * bytecode->artifact_count);

    if (bytecode->artifacts == NULL)
    {
        result = VCVM_CANT_ALLOCATE;
        goto done;
    }

    // Ensure the space is entirely blanked out
    memset(bytecode->integers, 0, UUID_SIZE * bytecode->artifact_count);

    for (uint32_t i = 0; i < bytecode->string_count; i++)
    {
        uint8_t* uuid = (uint8_t *)allocate(bytecode->allocator_options, UUID_SIZE);
        if (uuid == NULL)
        {
            result = VCVM_CANT_ALLOCATE;
            goto done;
        }

        result = bytecode_read_uuid(uuid, raw, size, offset);
        if (result != VCVM_STATUS_SUCCESS)
        {
            goto done;
        }

        *(bytecode->artifacts + i) = uuid;
    }

    done:
    return result;
}