#pragma once
#include "global.hpp"


//自定义枚举进程参数
typedef struct
{
	HWND	hwnd;
	DWORD	processId;
}WindowsArg;

class MemTool
{

	HMODULE handle;


public:
	MemTool();
	ULONG64 GetProcModuleBase(DWORD pid, const char* mName);
	LONG64 ReadQWord(DWORD pid, ULONG64 base, ULONG64 offset);
	DWORD ReadDWord(DWORD pid, ULONG64 base, ULONG64 offset);
	float ReadFloat(DWORD pid, ULONG64 base, ULONG64 offset);
	UWORD ReadWWord(DWORD pid, ULONG64 base, ULONG64 offset);
	UCHAR ReadByte(DWORD pid, ULONG64 base, ULONG64 offset);
	void ReadArr(DWORD pid, ULONG64 dst, PVOID arr, ULONG64 size, ULONG64 offset);

	void WriteDword(DWORD pid, ULONG64 base, DWORD data, ULONG64 offset);
	void WriteFloat(DWORD pid, ULONG64 base, float val, ULONG64 offset);
	void WriteByte(DWORD pid, ULONG64 base, UCHAR data, ULONG64 offset);
	void WriteArr(DWORD pid, ULONG64 base, PVOID arr, ULONG64 size, ULONG64 offset);
	BOOLEAN FarCall(DWORD pid, PUCHAR code, ULONG64 size);

	DWORD GetPidByName(const char* name);
	HWND  GetHwndByProcessId(DWORD processId);






	gExpLoadDriver ExpLoadDriver;
	gExpGetPidByName ExpGetPidByName;
	gExpGetModuleBase ExpGetModuleBase;
	gExpReadProcMemory ExpReadProcMemory;
	gExpWriteProcMemory ExpWriteProcMemory;
	gExpHideProcessByPid ExpHideProcessByPid;

	gExpKmInstall ExpKmInstall;
	gExpKmKeyDown ExpKmKeyDown;
	gExpKmKeyUp ExpKmKeyUp;
	gExpKmMouseLeftDown ExpKmMouseLeftDown;
	gExpKmMouseLeftUp ExpKmMouseLeftUp;
	gExpKmMouseRightDown ExpKmMouseRightDown;
	gExpKmMouseRightUp ExpKmMouseRightUp;
	gExpKmMouseMoveRelative ExpKmMouseMoveRelative;
	gExpKmMouseMoveTo ExpKmMouseMoveTo;

	gExpDwmDrInit ExpDwmDrInit;
	gExpDwmDrStartDraw ExpDwmDrStartDraw;
	gExpDwmDrEndDraw ExpDwmDrEndDraw;
	gExpDwmDrDrawBox ExpDwmDrDrawBox;


};

