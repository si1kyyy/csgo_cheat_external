#include "Game.h"
#include "offsets.h"



Game::Game() {
	this->mem = new MemTool();
	this->tool = new Tool();
	this->aimScope = 120;
	this->silkyNum = 7;
	this->bp = new bsp_parser();
	this->clientBase = 0;
	this->mouseEngineBase = 0;
	this->serverBase = 0;
	this->canShoot = FALSE;
}

//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================

DWORD Thread_KeyWatch(PVOID param) {
	Game* g = (Game*)param;
	while (true)
	{
		if ((GetAsyncKeyState(VK_ADD) & 1) > 0)
		{
			g->aimScope += 20;
		}
		else if ((GetAsyncKeyState(VK_SUBTRACT) & 1) > 0)
		{
			g->aimScope -= 20;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD1) & 1) > 0)
		{
			g->silkyNum = 1;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD2) & 1) > 0)
		{
			g->silkyNum = 2;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD3) & 1) > 0)
		{
			g->silkyNum = 3;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD4) & 1) > 0)
		{
			g->silkyNum = 4;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD5) & 1) > 0)
		{
			g->silkyNum = 5;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD6) & 1) > 0)
		{
			g->silkyNum = 6;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD7) & 1) > 0)
		{
			g->silkyNum = 7;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD8) & 1) > 0)
		{
			g->silkyNum = 8;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD9) & 1) > 0)
		{
			g->silkyNum = 9;
		}
		else if ((GetAsyncKeyState(VK_F7) & 1) > 0)
		{
			g->canShoot = TRUE;
		}
		else if ((GetAsyncKeyState(VK_F8) & 1) > 0)
		{
			g->canShoot = FALSE;
		}
	}
}

DWORD Thread_Bhop(PVOID param) {
	Game* g = (Game*)param;
	while (true)
	{
		try
		{
			if (GetAsyncKeyState(VK_SPACE))
			{
				int flags = g->mem->ReadDWord(g->pid, xxbase(g->gbSelfAddr + offsets::m_fFlags),xxoff);

				if (flags & (1 << 0)) {
					g->mem->WriteDword(g->pid, xxbase(g->clientBase + offsets::dwForceJump), 6,xxoff);
				}
			}
		}
		catch (...)
		{
			continue;
		}
	}
	return 0;
}
DWORD Thread_NewQuickStop(PVOID param) {
	Game* g = (Game*)param;
	while (true)
	{
		try
		{
			if (g->gbSelfHealth)
			{
				int flags = g->mem->ReadDWord(g->pid, xxbase(g->gbSelfAddr + offsets::m_fFlags),xxoff);
				if ((flags & 1) != 0) {
					float oldxSpeed = g->mem->ReadFloat(g->pid, xxbase(g->gbSelfAddr + offsets::m_vecVelocity),xxoff);
					float oldySpeed = g->mem->ReadFloat(g->pid, xxbase(g->gbSelfAddr + offsets::m_vecVelocity + 4),xxoff);

					float yView = g->mem->ReadFloat(g->pid, xxbase(g->gbClientState + offsets::dwClientState_ViewAngles + 4),xxoff);

					float xSpeed = (oldxSpeed * cos(yView / 180 * 3.1415926f) + oldySpeed * sin(yView / 180 * 3.1415926f));
					float ySpeed = (oldySpeed * cos(yView / 180 * 3.1415926f) - oldxSpeed * sin(yView / 180 * 3.1415926f));
					if (xSpeed != 0.0f && ySpeed != 0.0f)
					{
						if ((GetAsyncKeyState('W') & 0x8000) <= 0 && (GetAsyncKeyState('S') & 0x8000) <= 0)
						{
							if (xSpeed > 15) {
								g->mem->WriteDword(g->pid, xxbase(g->clientBase + offsets::dwForceBackward), 6,xxoff);
							}
							if (xSpeed < -15) {
								g->mem->WriteDword(g->pid, xxbase(g->clientBase + offsets::dwForceForward), 6,xxoff);
							}
						}
						if ((GetAsyncKeyState('A') & 0x8000) <= 0 && (GetAsyncKeyState('D') & 0x8000) <= 0) {

							if (ySpeed > 15) {
								g->mem->WriteDword(g->pid, xxbase(g->clientBase + offsets::dwForceRight), 6,xxoff);
							}
							if (ySpeed < -15) {
								g->mem->WriteDword(g->pid, xxbase(g->clientBase + offsets::dwForceLeft), 6,xxoff);
							}
						}
					}
				}
			}
		}
		catch (...)
		{
			continue;
		}
	}
	return 0;
}








DWORD Thread_FakeLag(PVOID param) {
	Game* g = (Game*)param;
	srand(GetTickCount());
	while (true)
	{
		try
		{
			if (g->gbSelfAddr && g->gbSelfHealth && (GetAsyncKeyState(VK_LBUTTON) & 0x8000) <= 0)
			{
				g->mem->WriteByte(g->pid, xxbase(g->mouseEngineBase + offsets::dwbSendPackets), 0,xxoff);
				std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 70 + 60));
				g->mem->WriteByte(g->pid, xxbase(g->mouseEngineBase + offsets::dwbSendPackets), 1,xxoff);
				Sleep(rand() % 70 + 30);
			}
		}
		catch (...)
		{
			continue;
		}
	}
	return 1;
}
DWORD Thread_UpdateCmdInfo(PVOID param) {
	Game* g = (Game*)param;
	while (true)
	{
		try
		{
			g->mem->ReadArr(g->pid, xxbase(g->clientBase + offsets::dwInput + sizeof(Input) - 8), &g->input_cmd, sizeof(InputCmd),xxoff);
			g->last_outgoing_cmd_num = g->mem->ReadDWord(g->pid, xxbase(g->gbClientState + offsets::clientstate_last_outgoing_command), xxoff);
			g->curr_cmd_address = g->input_cmd.pCommands_ + ((g->last_outgoing_cmd_num + 1) % 150) * sizeof(Commands);
			g->curr_verified_cmd_address = g->input_cmd.pVerifiedCommands_ + ((g->last_outgoing_cmd_num + 1) % 150) * sizeof(VerifiedCommands);
			g->mem->ReadArr(g->pid, xxbase(g->mouseEngineBase + offsets::dwGlobalVars), &g->server_info, sizeof(GlobalVar), xxoff);
			if (g->server_info.interval_per_tick_)
			{
				g->tick_rate = static_cast<int>(round(1.0f / g->server_info.interval_per_tick_));
			}
		}
		catch (...)
		{
			continue;
		}
	}
	return 1;
}
POS pppp = { 0 };
DWORD Thread_BackTrack(PVOID param) {
	Game* g = (Game*)param;
	while (true)
	{
		try
		{
			//if (!GetAsyncKeyState(0x01))
			//{
			//	if (GetAsyncKeyState('W') & 1 << 15 || GetAsyncKeyState('S') & 1 << 15 || GetAsyncKeyState('A') & 1 << 15 || GetAsyncKeyState('D') & 1 << 15)
			//	{
			//		std::this_thread::sleep_for(std::chrono::milliseconds(20));
			//		continue;
			//	}
			//}
			g->mem->WriteByte(g->pid, xxbase(g->mouseEngineBase + offsets::dwbSendPackets), 0,xxoff);
			int incoming_cmd_num = g->last_outgoing_cmd_num + 2;
			int connect_state = g->mem->ReadDWord(g->pid, xxbase(g->gbClientState + offsets::dwClientState_State),xxoff);
			while (connect_state == kFullyConnected)
			{
				DWORD cmd_num = g->mem->ReadDWord(g->pid, xxbase(g->gbClientState + offsets::clientstate_net_channel),xxoff);
				cmd_num = g->mem->ReadDWord(g->pid, xxbase(cmd_num + 0x18),xxoff);
				if (cmd_num >= incoming_cmd_num) break;
			}

			Commands0X4 cmd;
			g->mem->ReadArr(g->pid, xxbase(g->curr_cmd_address + 4), &cmd, sizeof(Commands0X4),xxoff);
			g->current_tick = cmd.tick_count_;

			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) > 0)
			{
				const int chosen_tick = g->backtrack_tick;
				if (chosen_tick > 0)
				{
					//g->mem->WriteDword(g->pid, g->clientBase + offsets::dwForceAttack, 0);
					//apply backtrack
					cmd.buttons_mask_ |= Input::IN_ATTACK;
					if (GetAsyncKeyState(VK_CONTROL) & 1 << 15) cmd.buttons_mask_ |= Input::IN_DUCK;
					cmd.tick_count_ = chosen_tick;
					cmd.view_angles_.x = g->mem->ReadFloat(g->pid, xxbase(g->gbClientState + offsets::dwClientState_ViewAngles),xxoff);
					cmd.view_angles_.y = g->mem->ReadFloat(g->pid, xxbase(g->gbClientState + offsets::dwClientState_ViewAngles + 4),xxoff);
					g->mem->WriteArr(g->pid, xxbase(g->curr_cmd_address + 4), &cmd, sizeof(Commands0X4),xxoff);
					g->mem->WriteArr(g->pid, xxbase(g->curr_verified_cmd_address + 4), &cmd, sizeof(Commands0X4),xxoff);
				}
				else if (g->gbSelfWeaponType != WEAPONTYPE_SNIPER_RIFLE && g->gbSelfWeaponType != WEAPONTYPE_SHOTGUN)
				{
					if (g->canShoot || g->gbSelfWeaponType == WEAPONTYPE_GRENADE || g->gbSelfWeaponType == WEAPONTYPE_KNIFE)
					{
						g->mem->WriteDword(g->pid, xxbase(g->clientBase + offsets::dwForceAttack), 5,xxoff);
					}
				}
			}
			else
			{
				g->mem->WriteDword(g->pid, xxbase(g->clientBase + offsets::dwForceAttack), 4,xxoff);
			}
			g->mem->WriteByte(g->pid, xxbase(g->mouseEngineBase + offsets::dwbSendPackets), 1,xxoff);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		catch (...)
		{
			continue;
		}
	}
	return 1;
}

DWORD Thread_FindGameProc(PVOID param) {
	Game* g = (Game*)param;
	while (true)
	{
		try
		{
			g->pid = g->mem->GetPidByName("csgo.exe");
			Sleep(5000);
		}
		catch (...)
		{
			continue;
		}
	}
}

DWORD Thread_StartDraw(PVOID param) {
	Game* g = (Game*)param;
	while (true)
	{
		try
		{
			g->Aimbot();
		}
		catch (...)
		{
			continue;
		}
	}
}
DWORD Thread_UpdateClientInfo(PVOID param) {
	Game* g = (Game*)param;
	while (true)
	{
		try
		{
			if (g->pid)
			{
				g->clientBase = g->mem->GetProcModuleBase(g->pid, "client.dll");
				g->serverBase = g->mem->GetProcModuleBase(g->pid, "server.dll");
				g->mouseEngineBase = g->mem->GetProcModuleBase(g->pid, "engine.dll");
				g->gbClientState = g->mem->ReadDWord(g->pid, xxbase(g->mouseEngineBase + offsets::dwClientState),xxoff);
				g->gbSelfAddr = g->mem->ReadDWord(g->pid, xxbase(g->clientBase + offsets::dwLocalPlayer),xxoff);
				g->gbSelfHealth = g->mem->ReadDWord(g->pid, xxbase(g->gbSelfAddr + offsets::m_iHealth),xxoff);
				g->gbSlelfPosX = g->mem->ReadFloat(g->pid, xxbase(g->gbSelfAddr + offsets::m_vecOrigin),xxoff);
				g->gbSlelfPosY = g->mem->ReadFloat(g->pid, xxbase(g->gbSelfAddr + offsets::m_vecOrigin + 4),xxoff);
				g->gbSlelfPosZ = g->mem->ReadFloat(g->pid, xxbase(g->gbSelfAddr + offsets::m_vecOrigin + 8),xxoff);
				g->gbSelfTeam = g->mem->ReadDWord(g->pid, xxbase(g->gbSelfAddr + offsets::m_iTeamNum),xxoff);
				g->gbMatrixBase = g->clientBase + offsets::dwViewMatrix;
				int curr_weapon_entity_index = g->mem->ReadDWord(g->pid, xxbase(g->gbSelfAddr + offsets::m_hActiveWeapon),xxoff) & 0xfff;
				DWORD curr_weapon_entity_address = g->mem->ReadDWord(g->pid, xxbase(g->clientBase + offsets::dwEntityList + (curr_weapon_entity_index - 1) * 0x10),xxoff);
				short temp_curr_weapon_def_index = g->mem->ReadWWord(g->pid, xxbase(curr_weapon_entity_address + offsets::m_iItemDefinitionIndex),xxoff);
				g->gbSelfWeaponIndex = (0 <= temp_curr_weapon_def_index && temp_curr_weapon_def_index < 65 ? temp_curr_weapon_def_index : 0);
				g->gbSelfWeaponType = g->GetWeaponType(g->gbSelfWeaponIndex);
			}
			Sleep(5000);
		}
		catch (...)
		{
			continue;
		}
	}
}
DWORD Thread_UpdateMapInfo(PVOID param) {
	Game* g = (Game*)param;
	const char* csgoMapDir = "D:\\steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo\\maps";
	while (true)
	{
		try
		{
			if (g->pid)
			{
				char mapName[40] = { 0 };
				char mapFullName[50] = { 0 };
				g->mem->ReadArr(g->pid, xxbase(g->gbClientState + offsets::dwClientState_Map), mapName, 0x20,xxoff);
				if (strcmp(mapName, g->current_map_name))
				{
					g->bp->unload_map();
					sprintf(mapFullName, "%s.bsp", mapName);
					if (g->bp->load_map(csgoMapDir, mapFullName))
					{
						strcpy(g->current_map_name, mapName);
						//MessageBoxA(0, mapFullName, "success", 0);
					}
				}
			}
			Sleep(5000);
		}
		catch (...)
		{
			continue;
		}
	}
}
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================



HANDLE Game::KeysWatch() {
	return CreateThread(NULL, NULL, Thread_KeyWatch, this, NULL, NULL);//按键监控
}
HANDLE Game::Bhop() {
	return CreateThread(NULL, NULL, Thread_Bhop, this, NULL, NULL);//Bhop监控
}
HANDLE Game::QuickStop() {
	return CreateThread(NULL, NULL, Thread_NewQuickStop, this, NULL, NULL);//急停监控
}
HANDLE Game::FakeLag() {
	return CreateThread(NULL, NULL, Thread_FakeLag, this, NULL, NULL);//假卡
}
HANDLE Game::BackTrack() {
	return CreateThread(NULL, NULL, Thread_BackTrack, this, NULL, NULL);//回溯
}
HANDLE Game::FindGameProc() {
	return CreateThread(NULL, NULL, Thread_FindGameProc, this, NULL, NULL);//获取PID
}
HANDLE Game::StartDraw() {
	return CreateThread(NULL, NULL, Thread_StartDraw, this, NULL, NULL);//自瞄主逻辑
}
HANDLE Game::UpdateClientInfo() {
	return CreateThread(NULL, NULL, Thread_UpdateClientInfo, this, NULL, NULL);//更新Client结构
}
HANDLE Game::UpdateMapInfo() {
	return CreateThread(NULL, NULL, Thread_UpdateMapInfo, this, NULL, NULL);//更新地图信息
}
HANDLE Game::UpdateCmdInfo() {
	return CreateThread(NULL, NULL, Thread_UpdateCmdInfo, this, NULL, NULL);//更新cmd信息
}


//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================

void ViewAngleToScreenRelative(PPOS newP, PPOS nowP, PPOS mouseP) {
	mouseP->y = (newP->y - nowP->y) * 15;
	mouseP->x = (nowP->x - newP->x) * 15;
	//mouseP->y = (newP->y - nowP->y) / 0.1099999994;
	//mouseP->x = (nowP->x - newP->x) / 0.1099999994;
}

int lastP = -1;
int lastB = -1;
void Game::Aimbot() {
	if (!this->pid)
	{
		return;
	}

	this->FlushBackTrackHistory();

	POS bestPos = { 0 };
	int p, b;
	int curr_backtrack_tick = this->ChooseBestTick(0, lastP, lastB, bestPos, p, b);

	this->backtrack_tick = curr_backtrack_tick;

	if (!curr_backtrack_tick) return;

	POS selfBone = { 0 };
	this->GetSelfEyePos(&selfBone);

	POS newP = { 0 };
	this->NewAim(&selfBone, &bestPos, &newP);

	this->RecoilCalc(newP);

	this->AimTo(newP, p, b);
}





void Game::AimTo(POS newP,int p,int b) {
	POS mouseMove = { 0 };
	POS nowAngle = { 0 };
	nowAngle.x = this->mem->ReadFloat(this->pid, xxbase(this->gbClientState + offsets::dwClientState_ViewAngles + 4),xxoff);
	nowAngle.y = this->mem->ReadFloat(this->pid, xxbase(this->gbClientState + offsets::dwClientState_ViewAngles),xxoff);

	
	DWORD mtX;
	ViewAngleToScreenRelative(&newP, &nowAngle, &mouseMove);
	if ((mouseMove.x > 0 ? mouseMove.x : mouseMove.x * -1) > this->weight / 2 || (mouseMove.y > 0 ? mouseMove.y : mouseMove.y * -1) > this->hight / 2)
	{
		return;
	}
	if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) > 0)
	{
		if (p != lastP && lastP != -1)
		{
			Sleep(300);
		}
		lastP = p;
		lastB = b;
		//this->mem->ExpKmMouseMoveRelative(mouseMove.x, mouseMove.y);
		//for (size_t i = 0; i < this->silkyNum * 500000; i++)
		//{
		//	mtX = GetAsyncKeyState(VK_RBUTTON);//无意义代码，防止空循环编译时被优化掉
		//}
		this->mem->WriteFloat(this->pid, xxbase(this->gbClientState + offsets::dwClientState_ViewAngles + 4), (newP.x - nowAngle.x) / 4 / this->silkyNum + nowAngle.x,xxoff);
		this->mem->WriteFloat(this->pid, xxbase(this->gbClientState + offsets::dwClientState_ViewAngles), (newP.y - nowAngle.y) / 4 / this->silkyNum + nowAngle.y, xxoff);
		this->mem->WriteDword(this->pid, 0, mtX,0);
	}
	else
	{
		lastP = -1;
		lastB = -1;
	}
}


void Game::RecoilCalc(POS &aimVec) {
	if (this->gbSelfWeaponType == WEAPONTYPE_SUBMACHINEGUN ||
		this->gbSelfWeaponType == WEAPONTYPE_RIFLE ||
		this->gbSelfWeaponType == WEAPONTYPE_MACHINEGUN)
	{
		float yyyy = this->mem->ReadFloat(this->pid, xxbase(this->gbSelfAddr + offsets::m_aimPunchAngle),xxoff);//后坐力数据
		float xxxx = this->mem->ReadFloat(this->pid, xxbase(this->gbSelfAddr + offsets::m_aimPunchAngle + 4),xxoff);//后坐力数据
		aimVec.x -= xxxx * 2.0099;
		aimVec.y -= yyyy * 2.0099;
	}
}



int Game::ChooseBestTick(int max_tick,int preferPlayerIndex,int preferBoneIndex,POS &out_aimVec3,int &playerIndex,int &boneIndex) {
	int boneArr[] = { 8,6, 5,4, 3 };
	int curr_backtrack_tick = 0;
	int record_to_search = static_cast<int>(0.2f / this->server_info.interval_per_tick_) - 1;//max tick
	float closestPwn = 99999.f;
	POS bestPos = { 0 };

	if (max_tick) record_to_search = max_tick;

	int nowScanPindex = 0;
	int nowScanBindex = 0;
	int tempRetPlayer = -1;
	int tempRetBone = -1;

	for (int i = 0; i < 32; i++)
	{
		bool curPlayerHasChosen = false;
		nowScanPindex = i;
		PLAYER p = { 0 };
		p.addr = this->mem->ReadDWord(this->pid, xxbase(this->clientBase + offsets::dwEntityList + i * 16),xxoff);
		p.health = this->mem->ReadDWord(this->pid, xxbase(p.addr + offsets::m_iHealth),xxoff);
		p.team = this->mem->ReadDWord(this->pid, xxbase(p.addr + offsets::m_iTeamNum),xxoff);
		DWORD objFlush = this->mem->ReadDWord(this->pid, xxbase(p.addr + offsets::m_bDormant),xxoff);
		DWORD boneAddr = this->mem->ReadDWord(this->pid, xxbase(p.addr + offsets::m_dwBoneMatrix),xxoff);
		//
		if (p.team == this->gbSelfTeam ||
			p.health < 1
			|| objFlush != 0)
		{
			for (int j = 0; j < 5; ++j)
			{
				history_[i][j].clear();
			}
			continue;
		}

		for (int j = 0; j < 5; j++)
		{
			nowScanBindex = boneArr[j];
			//remove expired tick
			while (!history_[i][j].empty())
			{
				//删掉超过maxtick以外的tick记录
				if (history_[i][j].back().tick + record_to_search < this->current_tick) history_[i][j].pop_back();
				else break;
			}

			int bone_id = boneArr[j];
			int record_searched = 0;
			for (int k = 0; k < history_[i][j].size(); k++)
			{
				BacktrackRecord br = history_[i][j].at(k);
				POS selfBone = { 0 };
				this->GetSelfEyePos(&selfBone);
				vector3 selfVec = vector3(selfBone.x, selfBone.y, selfBone.z);

				vector3 enemyVec = vector3(br.pos.x, br.pos.y, br.pos.z);
				if (!this->bp->is_visible(selfVec, enemyVec))
				{
					continue;
				}
				POS sc = { 0 };
				this->WorldToScreen(&br.pos, &sc);
				float dx = sc.x > this->centerX ? sc.x - this->centerX : this->centerX - sc.x;
				float dy = sc.y > this->centerY ? sc.y - this->centerY : this->centerY - sc.y;
				if (dx > this->aimScope || dy > this->aimScope)
				{
					continue;
				}

				//float distance = sqrt(dx * dx + dy * dy);
				float distance = dy;
				if (distance < closestPwn)
				{
					curPlayerHasChosen = true;
					closestPwn = distance;
					curr_backtrack_tick = br.tick;
					bestPos.x = br.pos.x;
					bestPos.y = br.pos.y;
					bestPos.z = br.pos.z;
					tempRetPlayer = i;
					tempRetBone = boneArr[j];
				}
				//check searched record, no more than once without backtrack
				if (++record_searched == record_to_search) break;
			}
			if (curPlayerHasChosen && nowScanPindex == preferPlayerIndex && nowScanBindex == preferBoneIndex)
			{
				goto RET;
			}
		}
	}
RET:
	playerIndex = tempRetPlayer;
	boneIndex = tempRetBone;
	out_aimVec3 = bestPos;
	return curr_backtrack_tick;
}



void Game::FlushBackTrackHistory() {
	int boneArr[] = { 8,6, 5,4, 3 };
	for (int i = 0; i < 32; i++)
	{
		PLAYER p = { 0 };
		p.addr = this->mem->ReadDWord(this->pid, xxbase(this->clientBase + offsets::dwEntityList + i * 16),xxoff);
		p.health = this->mem->ReadDWord(this->pid, xxbase(p.addr + offsets::m_iHealth),xxoff);
		p.team = this->mem->ReadDWord(this->pid, xxbase(p.addr + offsets::m_iTeamNum),xxoff);
		DWORD objFlush = this->mem->ReadDWord(this->pid, xxbase(p.addr + offsets::m_bDormant),xxoff);
		DWORD boneAddr = this->mem->ReadDWord(this->pid, xxbase(p.addr + offsets::m_dwBoneMatrix),xxoff);
		if (p.team == this->gbSelfTeam ||
			p.health < 1
			|| objFlush != 0)
		{
			continue;
		}
		for (int j = 0; j < 5; j++)
		{
			POS bone = { 0 };
			this->GetBonePos(&bone, p.addr, boneArr[j]);
			if (history_[i][j].empty() ||
				history_[i][j].front().tick != this->current_tick)
			{
				BacktrackRecord bcr = { this->current_tick, bone };
				history_[i][j].emplace_front(bcr);
			}
			else
			{
				//update the position to avoid duplicated record
				history_[i][j].front().pos = bone;
			}
		}
	}
}












void Game::GetBonePos(POS * pos, ULONG64 playerBase, int boneIndex) {
	DWORD offsetLevel1 = this->mem->ReadDWord(this->pid, xxbase(playerBase + offsets::m_dwBoneMatrix),xxoff) + 12;
	pos->z = this->mem->ReadFloat(this->pid, xxbase(offsetLevel1 + boneIndex * 48 + 32),xxoff);
	pos->y = this->mem->ReadFloat(this->pid, xxbase(offsetLevel1 + boneIndex * 48 + 16),xxoff);
	pos->x = this->mem->ReadFloat(this->pid, xxbase(offsetLevel1 + boneIndex * 48 + 0 ),xxoff);
}

void Game::GetSelfEyePos(POS * pos) {
	POS origin = { 0 };
	this->mem->ReadArr(this->pid, xxbase(this->gbSelfAddr + offsets::m_vecOrigin), &origin, sizeof(POS),xxoff);
	POS offset = { 0 };
	this->mem->ReadArr(this->pid, xxbase(this->gbSelfAddr + offsets::m_vecViewOffset), &offset, sizeof(POS),xxoff);

	pos->x = origin.x + offset.x;
	pos->y = origin.y + offset.y;
	pos->z = origin.z + offset.z;
}

BOOLEAN Game::WorldToScreen(POS * world, POS * screen) {
	float matrixArray[4][4] = { 0.0f };
	this->ReadMatrix(&matrixArray[0][0]);
	screen->z = world->x * matrixArray[3][0] + world->y * matrixArray[3][1] + world->z * matrixArray[3][2] + matrixArray[3][3];
	if (screen->z < 0)
	{
		return FALSE;
	}
	screen->x = (float)(this->weight / 2) + (world->x * matrixArray[0][0] + world->y * matrixArray[0][1] + world->z * matrixArray[0][2] + matrixArray[0][3]) / screen->z * (this->weight / 2);
	screen->y = (float)(this->hight / 2) - (world->x * matrixArray[1][0] + world->y * matrixArray[1][1] + world->z * matrixArray[1][2] + matrixArray[1][3]) / screen->z * (this->hight / 2);
	return TRUE;
}
void Game::ReadMatrix(float* matrixArray) {
	this->mem->ReadArr(this->pid, xxbase(this->gbMatrixBase), matrixArray, 64,xxoff);
}
void Game::NewAim(PPOS self, PPOS enemy, PPOS ret) {
	float aimX = self->x - enemy->x;
	float aimY = self->y - enemy->y;
	float aimZ = self->z - enemy->z + 1;
	double PI = 3.1415926535;
	if (aimX <= 0 && aimY <= 0)
	{
		ret->x = atan(aimY / aimX) / PI * 180;
	}
	else if (aimX >= 0 && aimY <= 0)
	{
		ret->x = atan(aimY / aimX) / PI * 180 + 180;
	}
	else if (aimX >= 0 && aimY >= 0)
	{
		ret->x = atan(aimY / aimX) / PI * 180 - 180;
	}
	else if (aimX <= 0 && aimY >= 0)
	{
		ret->x = atan(aimY / aimX) / PI * 180;
	}
	ret->y = atan(aimZ / sqrt(aimX * aimX + aimY * aimY)) / PI * 180;
}


CSWeaponType Game::GetWeaponType(int itemDefIndex)
{
	switch (itemDefIndex)
	{
	case WEAPON_TASER:
	case WEAPON_KNIFE_GG:
	case WEAPON_KNIFE_CT:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFE_GHOST:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
		return WEAPONTYPE_KNIFE;
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_TEC9:
	case WEAPON_HKP2000:
	case WEAPON_P250:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return WEAPONTYPE_PISTOL;
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_MP5SD:
	case WEAPON_UMP45:
	case WEAPON_BIZON:
	case WEAPON_MP7:
	case WEAPON_MP9:
		return WEAPONTYPE_SUBMACHINEGUN;
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_FAMAS:
	case WEAPON_GALILAR:
	case WEAPON_M4A1:
	case WEAPON_SG556:
	case WEAPON_M4A1_SILENCER:
		return WEAPONTYPE_RIFLE;
	case WEAPON_XM1014:
	case WEAPON_MAG7:
	case WEAPON_SAWEDOFF:
	case WEAPON_NOVA:
		return WEAPONTYPE_SHOTGUN;
	case WEAPON_AWP:
	case WEAPON_G3SG1:
	case WEAPON_SCAR20:
	case WEAPON_SSG08:
		return WEAPONTYPE_SNIPER_RIFLE;
	case WEAPON_M249:
	case WEAPON_NEGEV:
		return WEAPONTYPE_MACHINEGUN;
	case WEAPON_C4:
		return WEAPONTYPE_C4;
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_TAGRENADE:
	case WEAPON_FIREBOMB:
	case WEAPON_DIVERSION:
	case WEAPON_FRAG_GRENADE:
		return WEAPONTYPE_GRENADE;
	case WEAPON_FISTS:
		return WEAPONTYPE_FISTS;
	case WEAPON_BREACHCHARGE:
		return WEAPONTYPE_BREACHCHARGE;
	case WEAPON_TABLET:
		return WEAPONTYPE_TABLET;
	case WEAPON_AXE:
	case WEAPON_HAMMER:
	case WEAPON_SPANNER:
		return WEAPONTYPE_MELEE;
	default:
		return WEAPONTYPE_UNKNOWN;
	}
}