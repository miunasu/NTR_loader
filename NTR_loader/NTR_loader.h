#pragma once
#include <Windows.h>
#include <winnt.h>
#include <stdio.h>

HANDLE ntdll_RtlUserThreadStart;
HANDLE Ntdll_handle;
HANDLE Kernel32_handle;
BYTE key;
HANDLE exe_handle;
FARPROC WINAPI GetFunctionAddressByHash(HMODULE hMod, DWORD FuncHash);
__declspec(dllexport) void ExportedFunction();
void function(LPTHREAD_START_ROUTINE oep, LPVOID parameter);
