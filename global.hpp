#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <math.h>
#include <tchar.h>
#include <valve-bsp-parser/bsp_parser.hpp>
#include <deque>

using namespace rn;

typedef struct _BOX {
	float x;
	float y;
	float z;
	float Y2;
	float W;
}BOX,*PBOX;
typedef struct _POS {
	float x;
	float y;
	float z;
}POS, * PPOS;
typedef struct _PLAYER {
	DWORD addr;
	DWORD health;
	DWORD team;
	float x;
	float y;
	float z;
	POS p;
	POS head;
}PLAYER, * PPLAYER;

#define AIMPOS_HEAD 6
#define AIMPOS_RANDOM 0

#define xxor(v)			((v ^ 0x65083911))
#define xxoff			0
#define xxbase(v)			((v) - xxoff)

EXTERN_C typedef DWORD(WINAPI* gExpLoadDriver)(const char * key);
EXTERN_C typedef DWORD(WINAPI* gExpGetPidByName)(const char* name, ULONG64 ppid);
EXTERN_C typedef DWORD(WINAPI* gExpGetModuleBase)(ULONG64 pid, const char* name, PULONG64 base);
EXTERN_C typedef DWORD(WINAPI* gExpReadProcMemory)(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len, ULONG64 way);
EXTERN_C typedef DWORD(WINAPI* gExpWriteProcMemory)(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len, ULONG64 way);
EXTERN_C typedef DWORD(WINAPI* gExpHideProcessByPid)(ULONG64 pid, ULONG64 dstpid, ULONG64 needClearObj);
EXTERN_C typedef DWORD(WINAPI* gExpKmInstall)();
EXTERN_C typedef DWORD(WINAPI* gExpKmKeyDown)(ULONG64 kVal);
EXTERN_C typedef DWORD(WINAPI* gExpKmKeyUp)(ULONG64 kVal);
EXTERN_C typedef DWORD(WINAPI* gExpKmMouseLeftDown)();
EXTERN_C typedef DWORD(WINAPI* gExpKmMouseLeftUp)();
EXTERN_C typedef DWORD(WINAPI* gExpKmMouseRightDown)();
EXTERN_C typedef DWORD(WINAPI* gExpKmMouseRightUp)();
EXTERN_C typedef DWORD(WINAPI* gExpKmMouseMoveRelative)(LONG64 dx, LONG64 dy);
EXTERN_C typedef DWORD(WINAPI* gExpKmMouseMoveTo)(LONG64 dx, LONG64 dy);
EXTERN_C typedef DWORD(WINAPI* gExpDwmDrInit)();
EXTERN_C typedef BOOL(WINAPI* gExpDwmDrStartDraw)();
EXTERN_C typedef void(WINAPI* gExpDwmDrEndDraw)();
EXTERN_C typedef void(WINAPI* gExpDwmDrDrawBox)(float X, float Y, float W, float H, float R, float G, float B, float A, int T);

enum ItemDefinitionIndex
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFE_GG,
	WEAPON_KNIFE_CT,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	//WEAPON_MELEE = 74,	// prefab
	WEAPON_AXE = 75,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER
};
enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,   //手枪
	WEAPONTYPE_SUBMACHINEGUN, //冲锋
	WEAPONTYPE_RIFLE,  //步枪
	WEAPONTYPE_SHOTGUN,   //霰弹枪
	WEAPONTYPE_SNIPER_RIFLE,  //狙击
	WEAPONTYPE_MACHINEGUN,  //机枪
	WEAPONTYPE_C4,
	WEAPONTYPE_GRENADE = 9,   //雷
	WEAPONTYPE_STACKABLE = 11,
	WEAPONTYPE_FISTS,   //拳头
	WEAPONTYPE_BREACHCHARGE, //？
	WEAPONTYPE_TABLET,   //药片
	WEAPONTYPE_MELEE,   //？
	WEAPONTYPE_UNKNOWN
};
struct GlowStruct {
	BYTE base[8];
	float red;
	float green;
	float blue;
	float alpha;
	BYTE buffer[16];
	bool redner_occluded;
	bool render_unoccluded;
	bool fullBloom;
	BYTE buffer1[5];
	int glow_style;
};



struct BacktrackRecord
{
	int tick;
	POS pos;
};

enum ConnectionState : int
{
	kNone,
	kChallenge,
	kConnected, //loading screen
	kNew,
	kPrespawn,
	kSpawn,
	kFullyConnected, //fully connected to the game
	kChangeLevel
};


class Commands0X4
{
public:
	int cmd_number_;
	int tick_count_;

	POS view_angles_;
	POS aim_angles_;

	float forward_move_;
	float side_move_;
	float up_move_;
	int buttons_mask_;
};

class Commands
{
	DWORD pad0_; //mysterious hash???

public:
	Commands0X4 cmd_0x4_;

private:
	uint8_t pad1_[46];
};


class VerifiedCommands
{
	Commands commands_;

private:
	DWORD pad0_;
};






class InputCmd
{
public:
	DWORD pCommands_;
	DWORD pVerifiedCommands_;
};

class Input
{
public:

	enum Button : int
	{
		IN_ATTACK = 1 << 0, // Fire weapon
		IN_JUMP = 1 << 1, // Jump
		IN_DUCK = 1 << 2, // Crouch
		IN_FORWARD = 1 << 3, // Walk forward
		IN_BACK = 1 << 4, // Walk backwards
		IN_USE = 1 << 5, // Use (Defuse bomb, etc...)
		IN_CANCEL = 1 << 6, // ??
		IN_LEFT = 1 << 7,// Walk left
		IN_RIGHT = 1 << 8,// Walk right
		IN_MOVELEFT = 1 << 9,// Alias? (not sure)
		IN_MOVERIGHT = 1 << 10,// Alias? (not sure)
		IN_ATTACK2 = 1 << 11,// Secondary fire (Revolver, Glock change fire mode, Famas change fire mode) (not sure)
		IN_RUN = 1 << 12,
		IN_RELOAD = 1 << 13,// Reload weapon
		IN_ALT1 = 1 << 14,
		IN_ALT2 = 1 << 15,
		IN_SCORE = 1 << 16,
		IN_SPEED = 1 << 17,
		IN_WALK = 1 << 18,// Shift
		IN_ZOOM = 1 << 19,// Zoom weapon (not sure)
		IN_WEAPON1 = 1 << 20,
		IN_WEAPON2 = 1 << 21,
		IN_BULLRUSH = 1 << 22
	};

	static constexpr int MULTIPLAYER_BACKUP = 150;


private:
	uint8_t  pad0[0xC];
public:

	bool  bTrackIRAvailable;
	bool  bMouseInitialized;
	bool  bMouseActive;

private:
	uint8_t  pad1[0x9A];
public:

	bool  bCameraInThirdPerson;

private:
	uint8_t  pad2[0x2];
public:

	POS vecCameraOffset;

private:
	uint8_t pad3[0x38];
public:

	InputCmd pCmds;
};



class GlobalVar
{
public:
	float realtime_;
	int simulation_tick_;
	float absolute_frametime_;
	float foot_yaw_;
	float curtime_;
	float frametime_;
	int max_clients_;
	int tick_count_;
	float interval_per_tick_;
	float interpolation_amount_;
};
