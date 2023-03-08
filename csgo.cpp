#pragma once
#include "global.hpp"
#include "Game.h"

;




Game* game = NULL;
/*int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)*/
Game* game = NULL;
int main(
	LPSTR lpCmdLine,
	int nCmdShow)
{
	game = new Game();
	
	LONG ret = game->mem->ExpLoadDriver("skCVXuFtBvzjKw8VdP6oab3fk435U2p7HCQ");
	if (ret != 0x66666666)
	{
		game->tool->Log( "驱动加载失败！", ret);
		return -1;
	}
	//ret = game->mem->ExpKmInstall();
	//if (ret != 0)
	//{
	//	game->tool->Log("键鼠加载失败！", ret);
	//	return -1;
	//}
	HANDLE handleList[100] = { 0 };
	int nowIndex = 0;
	handleList[nowIndex++] = game->KeysWatch();
	handleList[nowIndex++] = game->FindGameProc();
	handleList[nowIndex++] = game->Bhop();
	handleList[nowIndex++] = game->QuickStop();
	handleList[nowIndex++] = game->BackTrack();
	handleList[nowIndex++] = game->UpdateClientInfo();
	handleList[nowIndex++] = game->UpdateMapInfo();
	handleList[nowIndex++] = game->UpdateCmdInfo();
	handleList[nowIndex++] = game->StartDraw(); 
	//handleList[nowIndex++] = game->FakeLag();
	
	game->tool->Log( "加载成功！请登陆游戏，游戏请事先设置无边框模式，否则绘制将失效！", 0);
	ret = game->mem->ExpHideProcessByPid(GetCurrentProcessId(), 0, TRUE);
	while (true)
	{
		DWORD waitRet = WaitForMultipleObjects(nowIndex,&handleList[0], FALSE,5000);
		if (waitRet == WAIT_TIMEOUT) continue;
		char msg[256] = { 0 };
		sprintf(msg, "thread %d crash!", waitRet);
		MessageBoxA(0, msg, "", 0);
	}
	return 0;
}
