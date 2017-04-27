#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>

int compect(byte content[], int length_c, byte befound[], int length_b);
DWORD FindAddress(DWORD PID, byte signature[], int Lengthofbyte, DWORD start, DWORD end);
DWORD GetPID(wchar_t *ProcessName);
DWORD GetBaseAddress(DWORD PID);