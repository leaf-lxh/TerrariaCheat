#include "stdafx.h"
#include "MemoryOperate.h"


/////////////////////////////////////////////////////
//返回符合特征的字节组的最先出现位置
//暴力搜索...
/////////////////////////////////////////////////////
int compect(byte content[], int length_c, byte befound[], int length_b)
{
	int CurrentContentPosition = 0;
	int CurrentBeFoundPosition = 0;

	if (length_b > length_c)
	{
		return -1;
	}

	do
	{
		if (CurrentBeFoundPosition == length_b)
		{
			return CurrentContentPosition - CurrentBeFoundPosition;
		}
		if (content[CurrentContentPosition] == befound[CurrentBeFoundPosition])
		{
			CurrentBeFoundPosition++;
		}
		else
		{
			CurrentBeFoundPosition = 0;
			if (content[CurrentContentPosition] == befound[CurrentBeFoundPosition])//防止发生如果当前待匹配字节为与第[0]的特征字节相同，遗漏掉该字节的情况
			{
				CurrentBeFoundPosition++;
			}
		}
		
		CurrentContentPosition++;
		//printf("%d %d %d %d \n", CurrentContentPosition, content[CurrentContentPosition], CurrentBeFoundPosition, befound[CurrentBeFoundPosition]);
	} 
	while (CurrentContentPosition <= length_c);
	return -1;
}

/////////////////////////////////////////////////////
//通过字节组特征来获取符合特征的内存块起始地址 如果start为0则自动查询起始地址，end为0则设置为0x7fffffff
/////////////////////////////////////////////////////
DWORD FindAddress(DWORD PID, byte signature[], int Lengthofbyte,DWORD start,DWORD end)
{
	if (start == NULL)
	{
		start = GetBaseAddress(PID);
	}
	if (end == NULL)
	{
		end = 0x7fffffff;
	}

	DWORD CurrentAddress = start;
	HANDLE hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
	MEMORY_BASIC_INFORMATION info = {};

	while (CurrentAddress < end)
	{
		VirtualQueryEx(hProcessHandle, LPCVOID(CurrentAddress), &info, sizeof(MEMORY_BASIC_INFORMATION));
		if (info.RegionSize == 0)
		{
			break;
		}
		if (info.Protect != PAGE_NOACCESS)
		{
			byte *buf = new byte[info.RegionSize];
			ReadProcessMemory(hProcessHandle, LPCVOID(CurrentAddress), buf, info.RegionSize, 0);
			int FoundPlace = compect(buf, info.RegionSize, signature, Lengthofbyte);
			delete(buf);
			if (FoundPlace != -1)
			{
				return CurrentAddress + FoundPlace;
			}
		}

		CurrentAddress += info.RegionSize;
		
		
	}
	
	return 0;
}

/////////////////////////////////////////////////////
//通过进程名称来获取进程标识符(区分大小写)
/////////////////////////////////////////////////////
DWORD GetPID(wchar_t *ProcessName)
{

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	BOOL isExist = Process32First(hProcessSnap, &entry);

	while (isExist)
	{


		if (_tcscmp(entry.szExeFile, ProcessName) == 0)
		{
			CloseHandle(hProcessSnap);
			return entry.th32ProcessID;
		}
		isExist = Process32Next(hProcessSnap, &entry);

	}
	return 0;
}

/////////////////////////////////////////////////////
//通过PID获取某进程的起始内存地址
/////////////////////////////////////////////////////
DWORD GetBaseAddress(DWORD PID)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(MODULEENTRY32);
	BOOL isExist = Module32First(hProcessSnap, &entry);
	return DWORD(entry.modBaseAddr);
}