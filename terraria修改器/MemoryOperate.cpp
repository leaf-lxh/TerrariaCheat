#include "stdafx.h"
#include "MemoryOperate.h"


/////////////////////////////////////////////////////
//���ط����������ֽ�������ȳ���λ��
//��������...
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
			if (content[CurrentContentPosition] == befound[CurrentBeFoundPosition])//��ֹ���������ǰ��ƥ���ֽ�Ϊ���[0]�������ֽ���ͬ����©�����ֽڵ����
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
//ͨ���ֽ�����������ȡ�����������ڴ����ʼ��ַ ���startΪ0���Զ���ѯ��ʼ��ַ��endΪ0������Ϊ0x7fffffff
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
//ͨ��������������ȡ���̱�ʶ��(���ִ�Сд)
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
//ͨ��PID��ȡĳ���̵���ʼ�ڴ��ַ
/////////////////////////////////////////////////////
DWORD GetBaseAddress(DWORD PID)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(MODULEENTRY32);
	BOOL isExist = Module32First(hProcessSnap, &entry);
	return DWORD(entry.modBaseAddr);
}