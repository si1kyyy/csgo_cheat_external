#pragma once
#include "MemTool.h"
#include <tlhelp32.h>



MemTool::MemTool() {
	this->handle = LoadLibraryA("silky.dll");
	this->ExpGetModuleBase = (gExpGetModuleBase)GetProcAddress(this->handle, "ExpGetModuleBase");
	this->ExpLoadDriver = (gExpLoadDriver)GetProcAddress(this->handle, "ExpLoadDriver");
	this->ExpGetPidByName = (gExpGetPidByName)GetProcAddress(this->handle, "ExpGetPidByName");
	this->ExpReadProcMemory = (gExpReadProcMemory)GetProcAddress(this->handle, "ExpReadProcMemory");
	this->ExpWriteProcMemory = (gExpWriteProcMemory)GetProcAddress(this->handle, "ExpWriteProcMemory");

	this->ExpKmInstall = (gExpKmInstall)GetProcAddress(this->handle, "ExpKmInstall");
	this->ExpKmKeyDown = (gExpKmKeyDown)GetProcAddress(this->handle, "ExpKmKeyDown");
	this->ExpKmKeyUp = (gExpKmKeyUp)GetProcAddress(this->handle, "ExpKmKeyUp");
	this->ExpKmMouseLeftDown = (gExpKmMouseLeftDown)GetProcAddress(this->handle, "ExpKmMouseLeftDown");
	this->ExpKmMouseLeftUp = (gExpKmMouseLeftUp)GetProcAddress(this->handle, "ExpKmMouseLeftUp");
	this->ExpKmMouseRightDown = (gExpKmMouseRightDown)GetProcAddress(this->handle, "ExpKmMouseRightDown");
	this->ExpKmMouseRightUp = (gExpKmMouseRightUp)GetProcAddress(this->handle, "ExpKmMouseRightUp");
	this->ExpKmMouseMoveRelative = (gExpKmMouseMoveRelative)GetProcAddress(this->handle, "ExpKmMouseMoveRelative");
	this->ExpKmMouseMoveTo = (gExpKmMouseMoveTo)GetProcAddress(this->handle, "ExpKmMouseMoveTo");

	this->ExpHideProcessByPid = (gExpHideProcessByPid)GetProcAddress(this->handle, "ExpHideProcessByPid");

	//this->ExpDwmDrInit = (gExpDwmDrInit)GetProcAddress(this->handle, "ExpDwmDrInit");
	//this->ExpDwmDrStartDraw = (gExpDwmDrStartDraw)GetProcAddress(this->handle, "ExpDwmDrStartDraw");
	//this->ExpDwmDrEndDraw = (gExpDwmDrEndDraw)GetProcAddress(this->handle, "ExpDwmDrEndDraw");
	//this->ExpDwmDrDrawBox = (gExpDwmDrDrawBox)GetProcAddress(this->handle, "ExpDwmDrDrawBox");
}





ULONG64 MemTool::GetProcModuleBase(DWORD pid, const char* mName) {
	ULONG64 base = 0;
	LONG ret = this->ExpGetModuleBase(pid,(char*)mName,&base);
	return base;
}

LONG64 MemTool::ReadQWord(DWORD pid, ULONG64 base, ULONG64 offset) {
	LONG64 val = 0;
	this->ExpReadProcMemory(pid, base, (ULONG64)&val, 8, offset);
	return val;
}
float MemTool::ReadFloat(DWORD pid, ULONG64 base, ULONG64 offset) {
	DWORD val = 0;
	this->ExpReadProcMemory(pid, base, (ULONG64)&val, 4, offset);
	float t = *(float*)(&val);
	return t;
}
DWORD MemTool::ReadDWord(DWORD pid, ULONG64 base, ULONG64 offset) {
	DWORD val = 0;
	this->ExpReadProcMemory(pid, base, (ULONG64)&val, 4, offset);
	return val;
}
UWORD MemTool::ReadWWord(DWORD pid, ULONG64 base, ULONG64 offset) {
	UWORD val = 0;
	this->ExpReadProcMemory(pid, base, (ULONG64)&val, 2, offset);
	return val;
}
UCHAR MemTool::ReadByte(DWORD pid, ULONG64 base, ULONG64 offset) {
	UCHAR val = 0;
	this->ExpReadProcMemory(pid, base, (ULONG64)&val, 1, offset);
	return val;
}
void MemTool::ReadArr(DWORD pid, ULONG64 dst, PVOID arr, ULONG64 size, ULONG64 offset) {
	this->ExpReadProcMemory(pid, dst, (ULONG64)arr, size, offset);
}



void MemTool::WriteDword(DWORD pid, ULONG64 base, DWORD data, ULONG64 offset) {
	DWORD buf = data;
	LONG ret = this->ExpWriteProcMemory(pid, base, (ULONG64)&buf, 4, offset);
}
void MemTool::WriteFloat(DWORD pid, ULONG64 base, float val, ULONG64 offset) {
	float val1 = val;
	LONG ret = this->ExpWriteProcMemory(pid, base, (ULONG64)&val1, 4, offset);
}

void MemTool::WriteArr(DWORD pid, ULONG64 base, PVOID arr, ULONG64 size, ULONG64 offset) {
	this->ExpWriteProcMemory(pid, base, (ULONG64)arr, size, offset);
}
void MemTool::WriteByte(DWORD pid, ULONG64 base, UCHAR data, ULONG64 offset) {
	UCHAR buf = data;
	LONG ret = this->ExpWriteProcMemory(pid, base, (ULONG64)&buf, 1, offset);
}


DWORD MemTool::GetPidByName(const char* name) {
	ULONG64 pid = 0;
	LONG ret = this->ExpGetPidByName((char*)name,(ULONG64)&pid);
	return pid;
}

