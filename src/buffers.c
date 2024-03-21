#include "buffers.h"

UINT16 get_uint16(PCHAR Buffer, ULONG Offset) {
    return (UINT16) (
        ((Buffer[Offset + 0] & 0xFF) << 0 ) |
        ((Buffer[Offset + 1] & 0xFF) << 8 )
    );
}

UINT32 get_uint32(PCHAR Buffer, ULONG Offset) {
    return (UINT32) (
        ((Buffer[Offset + 0] & 0xFF) << 0 ) |
        ((Buffer[Offset + 1] & 0xFF) << 8 ) |
        ((Buffer[Offset + 2] & 0xFF) << 16) |
        ((Buffer[Offset + 3] & 0xFF) << 24)
    );
}

UINT64 get_uint64(PCHAR Buffer, ULONG Offset) {
    return (UINT64) (
        ((Buffer[Offset + 0] & 0xFF) << 0 ) |
        ((Buffer[Offset + 1] & 0xFF) << 8 ) |
        ((Buffer[Offset + 2] & 0xFF) << 16) |
        ((Buffer[Offset + 3] & 0xFF) << 24) |
        ((Buffer[Offset + 4] & 0xFF) << 32) |
        ((Buffer[Offset + 5] & 0xFF) << 40) |
        ((Buffer[Offset + 6] & 0xFF) << 48) |
        ((Buffer[Offset + 7] & 0xFF) << 56)
    );
}

INT32 get_int32(PCHAR Buffer, ULONG Offset) {
    return (INT32) get_uint32(Buffer, Offset);
}
