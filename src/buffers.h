#pragma once

#include <Windows.h>

UINT16 get_uint16(PCHAR Buffer, ULONG Offset);
UINT32 get_uint32(PCHAR Buffer, ULONG Offset);
UINT64 get_uint64(PCHAR Buffer, ULONG Offset);
INT32  get_int32 (PCHAR Buffer, ULONG Offset);
