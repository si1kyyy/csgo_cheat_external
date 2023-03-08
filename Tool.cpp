#pragma once
#include "Tool.h"

CHAR* Tool::GetSelfPhyPath() {

	CHAR* path = (CHAR*)malloc(0x10000);
	memset(path, 0, 0x10000);
	CHAR path1[0x1000];
	HMODULE hm = GetModuleHandle(NULL);
	GetModuleFileNameA(hm, path1, sizeof(path1));
	sprintf(path, "\\??\\%s", path1);
	return path;
}

void Tool::Log(const char* msg, LONG code) {
	if (code != 0 && code != 0xF3000002)
	{
		CHAR txt[1000] = { 0 };
		sprintf(txt, "%s  错误码：%08x",msg,code);
		MessageBoxA(NULL, txt, "踏雪提示：", 0);
	}
	else
	{
		MessageBoxA(NULL, msg, "踏雪提示：", 0);
	}
}
