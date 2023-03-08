#pragma once
#include "global.hpp"
#include "MemTool.h"
#include "Tool.h"


class Game
{
public:
	MemTool* mem;
	Tool* tool;
	bsp_parser* bp;

	DWORD pid = 0;  //ÓÎÏ·½ø³ÌID

	ULONG64 clientBase = 0;
	ULONG64 serverBase = 0;
	ULONG64 mouseEngineBase = 0;

	ULONG64 gbClientState = 0;
	ULONG64 gbSelfAddr = 0;
	ULONG64 gbSelfHealth = 0;
	float gbSlelfPosX = 0.f;
	float gbSlelfPosY = 0.f;
	float gbSlelfPosZ = 0.f;
	short gbSelfWeaponIndex = 0;
	CSWeaponType gbSelfWeaponType = WEAPONTYPE_UNKNOWN;
	ULONG64 gbSelfTeam = 0;
	ULONG64 gbMatrixBase = 0;
	ULONG64 gbLightingBase = 0;

#define SCREEN_WEIGHT 2560
#define SCREEN_HIGHT 1440

	
	LONG weight = SCREEN_WEIGHT;
	LONG hight = SCREEN_HIGHT;
	LONG centerX = SCREEN_WEIGHT / 2;
	LONG centerY = SCREEN_HIGHT / 2;
	

	void Aimbot();
	void ReadMatrix(float* matrixArray);
	void NewAim(PPOS self, PPOS enemy, PPOS ret);
	void GetBonePos(POS* pos, ULONG64 playerBase, int boneIndex);
	void GetSelfEyePos(POS * pos);
	BOOLEAN WorldToScreen(POS* world, POS* screen);
	CSWeaponType GetWeaponType(int itemDefIndex);
	void FlushBackTrackHistory();
	int ChooseBestTick(int max_tick, int preferPlayerIndex, int preferBoneIndex, POS &aimVec3, int &playerIndex, int &boneIndex);
	void RecoilCalc(POS &aimVec);
	void AimTo(POS newP, int p, int b);

public:
	DWORD aimScope;
	DWORD silkyNum;

	InputCmd input_cmd = {};
	int last_outgoing_cmd_num = 0;
	DWORD curr_cmd_address = NULL;
	DWORD curr_verified_cmd_address = NULL;
	GlobalVar server_info;
	int tick_rate = 0;

	Game();
	
	HANDLE FindGameProc();
	HANDLE KeysWatch();
	HANDLE Bhop();
	HANDLE QuickStop();
	HANDLE FakeLag();
	HANDLE BackTrack();
	HANDLE UpdateClientInfo();
	HANDLE UpdateMapInfo();
	HANDLE UpdateCmdInfo();
	HANDLE StartDraw();

	bool canShoot = FALSE;
	char current_map_name[256] = { 0 };
	int current_tick;
	int backtrack_tick;
	std::deque<BacktrackRecord> history_[32][5]{};


};

