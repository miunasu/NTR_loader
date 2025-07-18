#include "NTR_loader.h"


__forceinline DWORD ror(DWORD d)
{
	return _rotr(d, 2);
}

__forceinline DWORD hash(char* c)
{
	register DWORD h = 0;
	do
	{
		h = ror(h);
		h += *c ^ key;
	} while (*++c);

	return h;
}


FARPROC WINAPI GetFunctionAddressByHash(HMODULE hMod, DWORD FuncHash)
{
	char* pBaseAddr = (char*)hMod;

	// get pointers to main headers/structures
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pBaseAddr;
	IMAGE_NT_HEADERS* pNTHdr = (IMAGE_NT_HEADERS*)(pBaseAddr + pDosHdr->e_lfanew);
	IMAGE_OPTIONAL_HEADER* pOptionalHdr = &pNTHdr->OptionalHeader;
	IMAGE_DATA_DIRECTORY* pExportDataDir = (IMAGE_DATA_DIRECTORY*)(&pOptionalHdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	IMAGE_EXPORT_DIRECTORY* pExportDirAddr = (IMAGE_EXPORT_DIRECTORY*)(pBaseAddr + pExportDataDir->VirtualAddress);

	// function address we're looking for
	void* pProcAddr;

	// resolve function by ordinal
	pProcAddr = NULL;

	DWORD* pFuncNameTbl = (DWORD*)(pBaseAddr + pExportDirAddr->AddressOfNames);
	WORD* pHintsTbl = (WORD*)(pBaseAddr + pExportDirAddr->AddressOfNameOrdinals);

	DWORD* pEAT = (DWORD*)(pBaseAddr + pExportDirAddr->AddressOfFunctions);

	// parse through table of function names
	DWORD ii = 254;
	for (ii = 0; ii < pExportDirAddr->NumberOfNames; ii++)
	{
		char* sTmpFuncName = (char*)pBaseAddr + (DWORD_PTR)pFuncNameTbl[ii];
		DWORD hash_calc = hash(sTmpFuncName);
		if (hash_calc == FuncHash)
		{
			// found, get the function virtual address = RVA + BaseAddr
			pProcAddr = (FARPROC)(pBaseAddr + (DWORD_PTR)pEAT[pHintsTbl[ii]]);
			break;
		}
	}

	return (FARPROC)pProcAddr;
}