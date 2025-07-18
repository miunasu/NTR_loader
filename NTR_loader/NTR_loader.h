#pragma once
#include <Windows.h>
#include <winnt.h>
#include <stdio.h>

intptr_t ntdll_RtlUserThreadStart;
intptr_t* Ntdll_handle;
intptr_t* Kernel32_handle;
BYTE key;
FARPROC WINAPI GetFunctionAddressByHash(HMODULE hMod, DWORD FuncHash);
__declspec(dllexport) void ExportedFunction();