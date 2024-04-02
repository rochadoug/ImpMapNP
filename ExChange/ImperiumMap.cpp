#include "StdAfx.h"

ImperiumMap impMap;


ImperiumMap::ImperiumMap() {
}
ImperiumMap::~ImperiumMap() {
}

void ImperiumMap::configGeral(){
	sala1.X1 = 151;	sala1.Y1 = 43; sala1.X2 = 190; sala1.Y2 = 71;
	sala2.X1 = 122;	sala2.Y1 = 43; sala2.X2 = 149; sala2.Y2 = 71;
	sala3.X1 = 80;	sala3.Y1 = 43; sala3.X2 = 114; sala3.Y2 = 71;
	sala4.X1 = 103;	sala4.Y1 = 79; sala4.X2 = 160; sala4.Y2 = 97;
	sala5.X1 = 174; sala5.Y1 = 88; sala5.X2 = 213; sala5.Y2 = 140;
	sala6.X1 = 93;	 sala6.Y1 = 106;  sala6.X2 = 148;  sala6.Y2 = 136;
	sala7.X1 = 102;	 sala7.Y1 = 141;  sala7.X2 = 116;  sala7.Y2 = 154;
	sala8.X1 = 81;	 sala8.Y1 = 160;  sala8.X2 = 122;  sala8.Y2 = 200;
	sala9.X1 = 125;	 sala9.Y1 = 159;  sala9.X2 = 176;  sala9.Y2 = 209;
	sala10.X1 = 199; sala10.Y1 = 198; sala10.X2 = 228; sala10.Y2 = 222;

	//Configs da sala 3
	sala3.NpcId = { 222 };  //Strange Crystal
	sala3.RespX = { 110, 116 };
	sala3.RespY = { 58, 58 };
	
	//Configs da sala 4
	sala4.NpcId = { 225, 224, 227 };  //Broken Device //Stone Head//Lion Door
	sala4.RespX = {108, 98, 159, 158};
	sala4.RespY = {88, 88, 103, 97};
	sala4.itemPremio[0] = 0;	//Quantidade
	sala4.itemPremio[1] = 0;	//Grupo
	sala4.itemPremio[2] = 0;	//Id
	sala4.itemPremio[3] = 0;	//Level
	sala4.itemPremio[4] = 0;	//Skill
	sala4.itemPremio[5] = 0;	//Luck
	sala4.itemPremio[6] = 0;	//Opt
	sala4.itemPremio[7] = 0;	//Exc
	sala4.sortQuestValue = 10; // Valor para o rand para q possa consertar o broken device
	
	//Configs da sala 5 (Não possui)

	//Configs da sala 6
	sala6.qReset = 0;
	sala6.NpcId = { 226 };
	sala6.RespX = { 109, 109 };
	sala6.RespY = { 140, 134 };
	sala6.timeToOpen = 5;
	sala6.timeToClose = 3;

	//Configs da sala 7
	sala7.NpcId = {223};
	sala7.secretKey = { 0,1,2,3,4,5 };
	
	sala7.wheelX = {104, 114, 104, 114, 104, 114};
	sala7.wheelY = {143, 143, 148, 148, 153, 153}; 
	
	//Configs da sala 8
	sala8.NpcId = { sala4.NpcId[2] };  // mesmo LionDoor da sala 4

	//Configs da sala 9
	sala9.bossId = 162;
	sala9.bossIndex = -1;
	sala9.bossX = 135;
	sala9.bossY = 182;
	sala9.bossQtd = 1;

}


void ImperiumMap::ChargeConfig()
{
	this->configGeral();

	int Token, Section = -1;
	SMDFile = fopen(IMPMAPTXT, "r");
	if (SMDFile == 0)
	{
		ExchangeMsgBox("[Konvict plugin] Falha ao carregar: %s", IMPMAPTXT);
		::ExitProcess(0);
	}
	//int Cont[10] = {0};
	//char Cnum[10] = { '0' };

	while (true)
	{
		Token = GetToken();
		if (Token == END)
		{
			break;
		}
		if (Token == NUMBER)
			Section = TokenNumber;

		while (true)
		{
			if (Section == 0) //sala 1
			{
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0)
				{
					break;
				}
				this->movereqLevel = TokenNumber; Token = GetToken();	//level necessário pra mover
				this->movereqReset = TokenNumber; Token = GetToken();	//reset necessário pra mover
				this->moveItemGrupo = TokenNumber; Token = GetToken();	//item grupo
				this->moveItemId = TokenNumber; Token = GetToken();		//item id
				this->moveItemLevel = TokenNumber;						//item level
			}

			if (Section == 1) //sala 2
			{
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0)
				{
					break;
				}

				sala1.qReset = TokenNumber; Token = GetToken();
				sala2.qReset = TokenNumber; Token = GetToken();
				sala1.mobQtd = TokenNumber; Token = GetToken();
				sala2.mobQtd = TokenNumber;
			}

			if (Section == 2) //sala 3
			{
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0)
				{
					break;
				}

				sala3.qReset = TokenNumber; Token = GetToken();
				sala3.geralQuest = TokenNumber; Token = GetToken();
				sala3.qtdCreation = TokenNumber; Token = GetToken();
				sala3.mobId = TokenNumber; Token = GetToken();
				sala3.mobQtd = TokenNumber;
			}

			if (Section == 3) //sala 4
			{
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0)
				{
					break;
				}

				sala4.qReset = TokenNumber; Token = GetToken();
				sala4.sortQuestValue = TokenNumber; Token = GetToken();
				sala4.quantQuestValue = TokenNumber; Token = GetToken();
				sala4.itemPremio[0] = TokenNumber; Token = GetToken();
				sala4.itemPremio[1] = TokenNumber; Token = GetToken();
				sala4.itemPremio[2] = TokenNumber; Token = GetToken();
				sala4.itemPremio[3] = TokenNumber; Token = GetToken();
				sala4.itemPremio[4] = TokenNumber; Token = GetToken();
				sala4.itemPremio[5] = TokenNumber; Token = GetToken();
				sala4.itemPremio[6] = TokenNumber; Token = GetToken();
				sala4.itemPremio[7] = TokenNumber;
			}
			if (Section == 4) //Boss sala 5
			{
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0)
				{
					break;
				}	
				sala5.bossId = TokenNumber; Token = GetToken();
				sala5.hpConf = TokenNumber; Token = GetToken();
				sala5.attackDmgConf = TokenNumber; Token = GetToken();
				sala5.attackRateConf = TokenNumber; Token = GetToken();
				sala5.defenseConf = TokenNumber; Token = GetToken();
				sala5.magicDefenseConf = TokenNumber; Token = GetToken();
				sala5.defenseRateConf = TokenNumber; Token = GetToken();
				sala5.regenTimeConf = TokenNumber;
			}
			if (Section == 5)  //Dragon door
			{
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0)
				{
					break;
				}
				sala6.timeToOpen = TokenNumber; Token = GetToken();
				sala6.timeToClose = TokenNumber; 

			}
			if (Section == 6)  //Sala 9
			{
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0)
				{
					break;
				}
				sala9.qReset = TokenNumber; Token = GetToken();
				sala9.countMobConfig = TokenNumber; Token = GetToken();
				sala9.itemReq[0] = TokenNumber; Token = GetToken();
				sala9.itemReq[1] = TokenNumber; Token = GetToken();
				sala9.itemReq[2] = TokenNumber; Token = GetToken();
				sala9.itemReq[3] = TokenNumber;

			}
		}
	}
	fclose(SMDFile);
}

void ImperiumMap::PlayerEnterGame(LPOBJ lpObj, BYTE modo){
	if (lpObj->Type == OBJECT_USER && lpObj->Connected > 2) {
		sPlayer[lpObj->m_Index].mobKills = 0;
		sPlayer[lpObj->m_Index].RoomLevel = 1; //não esquecer de alterar lvl 1

		sala4.isCompleteQuest[lpObj->m_Index] = false;
		sala4.canActivateDisp[lpObj->m_Index] = false;
		sala4.quantMobToSort[lpObj->m_Index] = 0; // Quantidade de monstros mortos na sala para sortear um valor
		
		if (lpObj->Map == 18 && modo == 0) {
			int mX, mY, x1 = 196, y1 = 51, x2 = 204, y2 = 63;

			mX = (x2 - x1) > 0 ? min(x1, x2) + rand() % (max(x1, x2) - min(x1, x2)) : 0;
			mY = (y2 - y1) > 0 ? min(y1, y2) + rand() % (max(y1, y2) - min(y1, y2)) : 0;

			if(gObjCalDistance(lpObj->X,lpObj->Y,mX,mY) > 10)
			gObjTeleport(lpObj->m_Index, 18, mX, mY);
		}
	}
}

bool ImperiumMap::Move(LPOBJ lpObj, BYTE pos, short lvl)
{
	if (pos < 0 || pos > 76 - 1) return false;

	if (lpObj->pInventory[pos].IsItem() == FALSE) return false;
	if (lpObj->pInventory[pos].IsItem() == FALSE) return false;

	if (lpObj->pInventory[pos].m_Number == 0 )
	{
		MsgOutput(lpObj->m_Index, "Não usável");
		return false;
	}
	int Id = lpObj->m_Index;

	if (lpObj->Level < this->movereqLevel) {
		MsgOutput(Id, "Level mínimo necessário %d", this->movereqLevel);
		return false;
	}
	if (pObj[Id].mResets < this->movereqReset) {
		MsgOutput(Id, "Reset mínimo necessário %d", this->movereqReset);
		return false;
	}
	if (lpObj->PkLevel >= 6){
		MsgOutput(Id, "Proibido mover estando PK");
		return false;
	}

	if (lpObj->Map == 17 && cPf.Player[Id].Status != 0) {
		cPf.PlayerOutEvent(lpObj);
	}

	if (lvl == 0) {
		this->PlayerEnterGame(lpObj, 1); //ja zera tudo
		int mX, mY, x1 = 196, y1 = 51, x2 = 204, y2 = 63;
		mX = (x2 - x1) > 0 ? min(x1, x2) + rand() % (max(x1, x2) - min(x1, x2)) : 0;
		mY = (y2 - y1) > 0 ? min(y1, y2) + rand() % (max(y1, y2) - min(y1, y2)) : 0;
		gObjTeleport(Id, 18, mX, mY);
	}
	else if (lvl == 1) {
		sPlayer[Id].RoomLevel = 5;
		impMap.sala5.killedBoss = false;
		impMap.sala5.setBridgeGuardStrong(false);

		int mX, mY, x1 = 162, y1 = 104, x2 = 170, y2 = 105;
		mX = (x2 - x1) > 0 ? min(x1, x2) + rand() % (max(x1, x2) - min(x1, x2)) : 0;
		mY = (y2 - y1) > 0 ? min(y1, y2) + rand() % (max(y1, y2) - min(y1, y2)) : 0;
		gObjTeleport(Id, 18, mX, mY);
	}
	return true;
}

bool ImperiumMap::Move(LPOBJ lpObj, short lvl)
{
	int Id = lpObj->m_Index;
	int mX, mY, x1 = 196, y1 = 51, x2 = 204, y2 = 63;

	switch (lvl)
	{
		case 1: {
			this->PlayerEnterGame(lpObj, 1); //ja zera tudo
			x1 = 196, y1 = 51, x2 = 204, y2 = 63;		
		}break;
		case 2: break;
		case 3: break;
		case 4: break;
		case 5: {
			sPlayer[Id].RoomLevel = 5;
			impMap.sala5.killedBoss = false;
			impMap.sala5.setBridgeGuardStrong(false);
			x1 = 162, y1 = 104, x2 = 170, y2 = 105;
		}break;
		case 6: break;
		case 7: break;
		case 8: {
			sPlayer[Id].RoomLevel = 8;
			x1 = 108, y1 = 159, x2 = 109, y2 = 161;
		} break;
		case 9: {
			sPlayer[Id].RoomLevel = 9;
			x1 = 126, y1 = 173, x2 = 127, y2 = 174;
		} break;
		default: {
			this->PlayerEnterGame(lpObj, 1); //ja zera tudo
			x1 = 196, y1 = 51, x2 = 204, y2 = 63;
		}break;
	}
	
	
	mX = (x2 - x1) > 0 ? min(x1, x2) + rand() % (max(x1, x2) - min(x1, x2)) : 0;
	mY = (y2 - y1) > 0 ? min(y1, y2) + rand() % (max(y1, y2) - min(y1, y2)) : 0;
	gObjTeleport(Id, 18, mX, mY);
	return true;
}

bool ImperiumMap::roomVerify(LPOBJ lpObj)
{
	if (lpObj->Type == OBJECT_USER) {
		if (lpObj->Map == 18) {
			if (lpObj->Authority != 2 && pObj[lpObj->m_Index].mAutority == 0) { //se não for gm
				if (this->GetCurRoom(lpObj) > sPlayer[lpObj->m_Index].RoomLevel) {
					this->Move(lpObj, 0);
					MsgOutput(lpObj->m_Index, "Alcance o nível desta sala");
					return false;
				}
			}
		}
	}
	return true;
}


BOOL ImperiumMap::RoomNPC(LPOBJ lpNpc, LPOBJ lpObj){
	
	if(lpNpc->Class == sala3.NpcId[0])
		return sala3.NPCFunc(lpNpc, lpObj);

	if (lpNpc->Class == sala4.NpcId[0] || lpNpc->Class == sala4.NpcId[1])
		return sala4.NPCFunc(lpNpc, lpObj);

	if (lpNpc->Class == sala6.NpcId[0])
		return sala6.NPCFunc(lpNpc, lpObj);

	if (lpNpc->Class == sala7.NpcId[0])
		return sala7.NPCFunc(lpNpc, lpObj);

	if (lpNpc->Class == sala8.NpcId[0]) {
		if(sPlayer[lpObj->m_Index].RoomLevel == 4)
			return sala4.NPCFunc(lpNpc, lpObj);

		if (sPlayer[lpObj->m_Index].RoomLevel == 8)
			return sala8.NPCFunc(lpNpc, lpObj);
	}
		
	return FALSE;
}

void ImperiumMap::RoomQuestCountKill(LPOBJ lpMob, LPOBJ lpObj){
	int Id = lpObj->m_Index;
	int Room = GetCurRoom(lpObj);
	int MobRoom = GetCurRoom(lpMob);
	
	if (lpObj->Map == 18){
		
		if(Room == 1){
			if (sPlayer[Id].RoomLevel == Room ) {
				if (pObj[Id].mResets > sala1.qReset) {

					if (sala1.mobQtd > 0 && sPlayer[Id].mobKills < sala1.mobQtd) {
						sPlayer[Id].mobKills++;
						MsgOutput(Id, "[Sala %02d] Monstros mortos %lld/%lld", Room, sPlayer[Id].mobKills, sala1.mobQtd);

						if (sPlayer[Id].mobKills == 1) {
							YellowWhispSend("Sala 01", Id, "Missão iniciada automaticamente");
							//YellowWhispSend("Sala 01", Id, "Mate %lld monstros");
						}
					}

					if (sPlayer[Id].mobKills == (sala1.mobQtd - 1)) {
						MsgOutput(Id, "[Sala %d] Você concluiu a missão nesta sala", Room);
						sPlayer[Id].mobKills = 0;
						sPlayer[Id].RoomLevel = 2;
						YellowWhispSend("Sala 01", Id, "Alcançou nível: %02d", sPlayer[Id].RoomLevel);
					}
				}
			}
		}
		if(Room == 2){
			if (sPlayer[Id].RoomLevel == Room) {
				if (pObj[Id].mResets > sala1.qReset) {
					if (sala2.mobQtd > 0 && sPlayer[Id].mobKills < sala2.mobQtd) {
						sPlayer[Id].mobKills++;
						MsgOutput(Id, "[Sala %02d] Monstros mortos %lld/%lld", Room, sPlayer[Id].mobKills, sala2.mobQtd);

						if (sPlayer[Id].mobKills == 1) {
							YellowWhispSend("Sala 02", Id, "Missão iniciada automaticamente");
							//YellowWhispSend("Sala 01", Id, "Mate %lld monstros");
						}
					}

					if (sPlayer[Id].mobKills == (sala2.mobQtd - 1)) {
						MsgOutput(Id, "[Sala %02d] Você concluiu a missão nesta sala", Room);
						sPlayer[Id].mobKills = 0;
						sPlayer[Id].RoomLevel = 3;
						YellowWhispSend("Sala 02", Id, "Alcançou nível: %02d", sPlayer[Id].RoomLevel);
					}
				}
			}
		}
		if(Room == 3){
			if (sPlayer[Id].RoomLevel == Room) {
				if (pObj[Id].mResets > sala1.qReset) {
					if (sala3.mobQtd > 0 && sPlayer[Id].mobKills < sala3.mobQtd && lpMob->Class == sala3.mobId) {
						sPlayer[Id].mobKills++;
						MsgOutput(Id, "[Sala %02d] Monstros mortos %lld/%lld", Room, sPlayer[Id].mobKills, sala3.mobQtd);

						if (sPlayer[Id].mobKills == 1) {
							YellowWhispSend("Sala 03", Id, "Missão iniciada automaticamente");
							//YellowWhispSend("Sala 01", Id, "Mate %lld monstros");
						}
					}

					if (sPlayer[Id].mobKills == (sala3.mobQtd - 1)) {
						MsgOutput(Id, "[Sala %02d] Você concluiu a missão nesta sala", Room);
						sPlayer[Id].mobKills = 0;
						sPlayer[Id].RoomLevel = 4;
						YellowWhispSend("Sala 03", Id, "Alcançou nível: %02d", sPlayer[Id].RoomLevel);
					}
				}
			}
		}
		if(Room == 4){
			if (sPlayer[Id].RoomLevel == Room) {
				sala4.quantMobToSort[Id]++;
				if (sala4.quantMobToSort[Id] >= sala4.quantQuestValue && this->sala4.canActivateDisp[Id] == false) {
					sala4.quantMobToSort[Id] = 0;

					if (sala4.sortQuestValue < 2) //por segurança
						sala4.sortQuestValue = 2;

					int sort = rand() % sala4.sortQuestValue;
					if (sort == sala4.sortQuestValue / 2) {
						this->sala4.canActivateDisp[Id] = true;
						YellowWhispSend("Sala 04", Id, "Parte do dispositivo encontrada");

					}
				}				
			}
		}
		if(Room == 5){
			if (sPlayer[Id].RoomLevel == Room) {
				if (lpMob->Class == this->sala5.bossId) {
					MapAnnounce(18,"[Sala 05] Boss eliminado!");
					this->sala5.killedBoss = true;
					impMap.sPlayer[Id].RoomLevel = 6;
					this->sala5.setBridgeGuardStrong(true);
				}
			}
		}
		if(Room == 6){
			if (sPlayer[Id].RoomLevel == Room) {
			}
		}
		if(Room == 7){
			if (sPlayer[Id].RoomLevel == Room) {
				if (lpMob->Class == 155) {
					if (sala7.playerKey[Id].size() < 6) {
						
						int value = lpMob->X == sala7.wheelX.at(0) && lpMob->Y == sala7.wheelY.at(0) ? 0 : lpMob->X == sala7.wheelX.at(1) && lpMob->Y == sala7.wheelY.at(1) ? 1 : lpMob->X == sala7.wheelX.at(2) && lpMob->Y == sala7.wheelY.at(2) ? 2 : lpMob->X == sala7.wheelX.at(3) && lpMob->Y == sala7.wheelY.at(3) ? 3 : lpMob->X == sala7.wheelX.at(4) && lpMob->Y == sala7.wheelY.at(4) ? 4 : lpMob->X == sala7.wheelX.at(5) && lpMob->Y == sala7.wheelY.at(5) ? 5 : 6;
							
						if (!sala7.playerKey[Id].empty() && find(sala7.playerKey[Id].begin(), sala7.playerKey[Id].end(), value) != sala7.playerKey[Id].end()) {
							//Se já tiver o valor, não faz nada
							MsgOutput(Id, "Você já tem o número %d", value);
						}
						else {
							sala7.playerKey[Id].push_back(value);
							MsgOutput(Id, "%02dº número %d obtido", sala7.playerKey[Id].size(), value);

							if (sala7.playerKey[Id].size() == 6) {
								MsgOutput(Id, "Código completo, boa sorte!");
							}
						}
					}
					else {
						MsgOutput(Id, "Código completo, boa sorte!");
						YellowWhispSend("Sala 07", Id, "%d%d%d%d%d%d", sala7.playerKey[Id].at(0), sala7.playerKey[Id].at(1), sala7.playerKey[Id].at(2), sala7.playerKey[Id].at(3), sala7.playerKey[Id].at(4), sala7.playerKey[Id].at(5));	
					}
				}	
			}
		}
		if(Room == 8){
			if (sPlayer[Id].RoomLevel == Room) {
				
			}
		}
		if(Room == 9){
			if (sPlayer[Id].RoomLevel == Room) {
				sala9.BossRespawn(lpObj, lpMob);
			}
		}
		if(Room == 10){
			if (sPlayer[Id].RoomLevel == Room) {
			}
		}
	}
}

int ImperiumMap::GetCurRoom(LPOBJ lpObj){
	BYTE X = lpObj->X, Y = lpObj->Y;

	if (X >= sala1.X1 && Y >= sala1.Y1 && X <= sala1.X2 && Y <= sala1.Y2) {
		return  1;
	}
	if (X >= sala2.X1 && Y >= sala2.Y1 && X <= sala2.X2 && Y <= sala2.Y2) {
		return  2;
	}
	if (X >= sala3.X1 && Y >= sala3.Y1 && X <= sala3.X2 && Y <= sala3.Y2) {
		return  3;
	}
	if (X >= sala4.X1 && Y >= sala4.Y1 && X <= sala4.X2 && Y <= sala4.Y2) {
		return 4;
	}
	if (X >= sala5.X1 && Y >= sala5.Y1 && X <= sala5.X2 && Y <= sala5.Y2) {
		return 5;
	}
	if (X >= sala6.X1 && Y >= sala6.Y1 && X <= sala6.X2 && Y <= sala6.Y2) {
		return 6;
	}
	if (X >= sala7.X1 && Y >= sala7.Y1 && X <= sala7.X2 && Y <= sala7.Y2) {
		return 7;
	}
	if (X >= sala8.X1 && Y >= sala8.Y1 && X <= sala8.X2 && Y <= sala8.Y2) {
		return 8;
	}
	if (X >= sala9.X1 && Y >= sala9.Y1 && X <= sala9.X2 && Y <= sala9.Y2) {
		return 9;
	}
	if (X >= sala10.X1 && Y >= sala10.Y1 && X <= sala10.X2 && Y <= sala10.Y2) {
		return 10;
	}
	return -1;
}



BOOL ImperiumMap::SalaTres::NPCFunc(LPOBJ lpNpc, LPOBJ lpObj)
{
	int Id = lpObj->m_Index;

	if (lpObj->Map == 18 && lpNpc->Class == impMap.sala3.NpcId[0]) {
		
		if (quest.sPlayer[Id].CurrentQuest < impMap.sala3.geralQuest || quest.sPlayer[Id].CurrentQuest == impMap.sala3.geralQuest && quest.sPlayer[Id].Status <= 1) {
			NpcOutput(lpNpc->m_Index, Id, "Complete a Missão: %s", quest.sQuest[impMap.sala3.geralQuest].qName);
			return TRUE;
		}
		if (pObj[Id].mResets < impMap.sala3.qReset) {
			NpcOutput(lpNpc->m_Index, Id, "Precisa ter %d Resets", impMap.sala3.qReset);
			return TRUE;
		}
		if (CheckInventory(Id, ITEMGET(14, 22), 0) < impMap.sala3.qtdCreation) {
			NpcOutput(lpNpc->m_Index, Id, "Precisa de %d Jewels of Creations", impMap.sala3.qtdCreation);
			return TRUE;
		}
		if (impMap.sPlayer[Id].RoomLevel >= 3) {
			if (lpObj->X > lpNpc->X) {
				gObjTeleport(Id, 18, impMap.sala3.RespX[0], impMap.sala3.RespY[0]);
				DeleteItemInventory(lpObj, ITEMGET(14, 22), 0, impMap.sala3.qtdCreation); // apaga creation
			}
			else
				gObjTeleport(Id, 18, impMap.sala3.RespX[1], impMap.sala3.RespY[1]);
		}
		else {
			NpcOutput(lpNpc->m_Index, Id, "Você ainda não está no nível desta sala");
		}
		return TRUE;
	}
	return FALSE;
}

BOOL ImperiumMap::SalaQuatro::NPCFunc(LPOBJ lpNpc, LPOBJ lpObj)
{
	int Id = lpObj->m_Index;
	bool naoPassa = false;

	if (lpObj->Map == 18) {
		if (lpNpc->Class == impMap.sala4.NpcId.at(0)) { // Stone Head
			if (impMap.sPlayer[Id].RoomLevel < 4 || naoPassa) {
				NpcOutput(lpNpc->m_Index, Id, "Conclua a missão na sala 3 para passar daqui!");
				return TRUE;
			}
			else if (pObj[Id].mResets < impMap.sala3.qReset) {
				NpcOutput(lpNpc->m_Index, Id, "Precisa ter %d Resets", impMap.sala3.qReset);
				return TRUE;
			}
			else if (impMap.sPlayer[Id].RoomLevel >= 4) {
				if (lpObj->X < lpNpc->X) {
					gObjTeleport(Id, 18, impMap.sala4.RespX[0], impMap.sala4.RespY[0]);
					this->isCompleteQuest[Id] = false;
					this->quantMobToSort[Id] = 0;
					//DeleteItemInventory(lpObj, ITEMGET(14, 22), 0, impMap.sala3.qtdCreation); // apaga creation
				}

			}
			return TRUE;
		}
		else if (lpNpc->Class == impMap.sala4.NpcId.at(1)) { // Broken Device
		
			if (impMap.sPlayer[Id].RoomLevel == 4) {
				if (this->canActivateDisp[Id] && this->isCompleteQuest[Id] == false) { //Monstro foi sorteado
					
					if (this->itemPremio[0] > 0) {  //Premia
						int itemrwd = ItemGetNumberMake(this->itemPremio[1], this->itemPremio[2]);
						int SpaceCheck = InventoryFullCheck(Id, itemrwd, this->itemPremio[0]);
						
						if (SpaceCheck < this->itemPremio[0]) {
							NpcOutput(lpNpc->m_Index, Id,"Seu inventário não cabe o prêmio!");
							return TRUE;
						}
						else {
							for (int i = 0; i < this->itemPremio[0]; i++)
							{
								ItemSerialCreateSend(Id, 236, lpObj->X, lpObj->Y, itemrwd, this->itemPremio[3], 0, this->itemPremio[4], this->itemPremio[5], this->itemPremio[6], Id, this->itemPremio[7]);
							}
						}
					}

					NpcOutput(lpNpc->m_Index, Id,"Fui consertado, pode seguir");
					this->isCompleteQuest[Id] = true;  //libera a porta
					this->quantMobToSort[Id] = 0;
					this->canActivateDisp[Id] = false;
				}
				else if (this->canActivateDisp[Id] == false && this->isCompleteQuest[Id] == true) {
					NpcOutput(lpNpc->m_Index, Id, "Você já pode ir");
				}
				else {
					NpcOutput(lpNpc->m_Index, Id, "Falta uma parte para funcionar"); //Peça
					this->isCompleteQuest[Id] = false;
					this->quantMobToSort[Id] = 0;
				}

			}
			if (impMap.sPlayer[Id].RoomLevel > 4) {
				this->isCompleteQuest[Id] = false;
				this->quantMobToSort[Id] = 0;
				this->canActivateDisp[Id] = false;
				impMap.sPlayer[Id].RoomLevel = 4;
				NpcOutput(lpNpc->m_Index, Id, "Falta uma parte para funcionar");
			}
			return TRUE;
		}
		else if (lpNpc->Class == impMap.sala4.NpcId.at(2)) { // Lion Door
			if (impMap.sPlayer[Id].RoomLevel == 4) {
				if (this->isCompleteQuest[Id] == true) {
					impMap.sPlayer[Id].RoomLevel = 5;
					this->isCompleteQuest[Id] = false;
					this->quantMobToSort[Id] = 0;
					this->canActivateDisp[Id] = false;
					gObjTeleport(Id, 18, impMap.sala4.RespX[2], impMap.sala4.RespY[2]);

					impMap.sala5.killedBoss = false;
					impMap.sala5.setBridgeGuardStrong(impMap.sala5.killedBoss);
				}
				else {
					NpcOutput(lpNpc->m_Index, Id, "Ainda há algo a fazer");
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

void ImperiumMap::SalaCinco::setBridgeGuardStrong(bool k) {
	
	for (int m = 0; m < 5800; m++) {
		LPOBJ lpMonster = &gObj[m];

		if (lpMonster->Class == 154 && lpMonster->Map == 18) {

			if (k == false) {
				
				//deixo imortal
				lpMonster->Life = 500000;
				lpMonster->MaxLife = 500000;
				lpMonster->m_AttackDamageMax = 35000;
				lpMonster->m_AttackRating = 40000;
				lpMonster->m_SuccessfulBlocking = 40000;
				lpMonster->m_Defense = 30000;
				lpMonster->m_MagicDefense = 30000;
			}
			else if(k == true) {
				//devolvo o original
				lpMonster->Life = this->hp;
				lpMonster->MaxLife = this->hp;
				lpMonster->m_AttackDamageMax = this->attackDmg;
				lpMonster->m_AttackRating = this->attackRate;
				lpMonster->m_SuccessfulBlocking = this->defenseRate;
				lpMonster->m_Defense = this->defense;
				lpMonster->m_MagicDefense = this->magicDefense;				
			}
		}
	}
}

void ImperiumMap::SalaCinco::setBossStrong(int aIndex, int mobid)
{
	LPOBJ lpMonster = &gObj[aIndex];
	if (mobid == this->bossId && lpMonster->m_RecallMon == -1) {
		if (lpMonster->Map == 18 && impMap.GetCurRoom(lpMonster) == 5) {
			
				//deixo forte
				lpMonster->Life = this->hpConf;
				lpMonster->MaxLife = this->hpConf;
				lpMonster->m_AttackDamageMax = this->attackDmgConf;
				lpMonster->m_AttackRating = this->attackRateConf;
				lpMonster->m_SuccessfulBlocking = this->defenseRateConf;
				lpMonster->m_Defense = this->defenseConf;
				lpMonster->m_MagicDefense = this->magicDefenseConf;
				lpMonster->MaxRegenTime = this->regenTimeConf * 1000;
				MapAnnounce(18,"[Sala 5] O Boss apareceu");
		}
	}

	if (mobid == 154 && lpMonster->Map == 18) {
		//Salvar força original dos guardas da ponte
		this->hp = lpMonster->MaxLife;
		this->attackDmg = lpMonster->m_AttackDamageMax;
		this->attackRate = lpMonster->m_AttackRating;
		this->defenseRate = lpMonster->m_SuccessfulBlocking;
		this->defense = lpMonster->m_Defense;
		this->magicDefense = lpMonster->m_MagicDefense;	
	}

	//Salvar defesa original das arvores na sala 8
	if (mobid == 160 && lpMonster->Map == 18) {	
		impMap.sala8.mobDefenseRate[0] = lpMonster->m_SuccessfulBlocking;
	}
	if (mobid == 161 && lpMonster->Map == 18) {
		impMap.sala8.mobDefenseRate[1] = lpMonster->m_SuccessfulBlocking;
	}
}

void ImperiumMap::SalaSeis::DragonDoorProc()
{
	this->second++;

	if (this->second >= (this->timeToOpen * 60)) {
		this->timeOpend = this->timeToClose * 60; //300seg = 5min
		this->second = 0;
		MapMsgOutput(18, "DragonDoor aberta por %d min", impMap.sala6.timeToClose);
	}

	if (this->timeOpend > 0)
		this->timeOpend--;

	if(this->timeOpend == 1)
		MapMsgOutput(18,"DragonDoor fechada");
}

BOOL ImperiumMap::SalaSeis::NPCFunc(LPOBJ lpNpc, LPOBJ lpObj)
{
	int Id = lpObj->m_Index;

	if (lpObj->Map == 18) {
		if (lpNpc->Class == impMap.sala6.NpcId.at(0)) { // Dragon door
			if (impMap.sPlayer[Id].RoomLevel < 6) {
				NpcOutput(lpNpc->m_Index, Id, "Mate o boss na sala 5 para passar!");
				return TRUE;
			}
			else if (pObj[Id].mResets < impMap.sala6.qReset) {
				NpcOutput(lpNpc->m_Index, Id, "Precisa ter %d Resets", impMap.sala6.qReset);
				return TRUE;
			}
			else if (this->timeOpend == 0) {
				NpcOutput(lpNpc->m_Index, Id, "O portal está fechado agora");
				return TRUE;
			}

			if (lpObj->Y < lpNpc->Y) {
				gObjTeleport(Id, 18, impMap.sala6.RespX[0], impMap.sala6.RespY[0]);
				impMap.sPlayer[Id].RoomLevel = 7;
				impMap.sala7.secretKey.clear();

				vector<int> sortKey[3];
				sortKey[0] = {0, 1, 2, 3, 4, 5};
				sortKey[1] = {5, 4, 3, 2, 1, 0};
				sortKey[2] = {2, 1, 0, 5, 4, 3};

				int mKey = rand() % 3;
				impMap.sala7.secretKey = sortKey[mKey];
				MapMsgOutput(18, "Chave sorteada %d", mKey);  // Não esquecer de tirar
			}
		}
	}
	return 1;
}

BOOL ImperiumMap::SalaSete::NPCFunc(LPOBJ lpNpc, LPOBJ lpObj)
{
	int Id = lpObj->m_Index;

	if (lpObj->Map == 18) {
		if (lpNpc->Class == impMap.sala7.NpcId.at(0)) { //	Secret Npc
			if (impMap.sPlayer[Id].RoomLevel < 7) {
				NpcOutput(lpNpc->m_Index, Id, "Tem quest nas salas anteriores");
				return TRUE;
			}
			else if (pObj[Id].mResets < impMap.sala6.qReset) {
				NpcOutput(lpNpc->m_Index, Id, "Precisa ter %d Resets", impMap.sala6.qReset);
				return TRUE;
			}

			if (lpObj->Y < lpNpc->Y) {
				if (!this->secretKey.empty() && !this->playerKey[Id].empty()) {
					if (this->secretKey == this->playerKey[Id]) {
						impMap.sPlayer[Id].RoomLevel = 8;
						YellowWhispSend("Sala 07", Id, "SEGREDO RESOLVIDO");
						impMap.Move(lpObj, 8); // Go sala 8
						this->playerKey[Id].clear();
					}
					else if (this->playerKey[Id].size() != this->secretKey.size()) {
						NpcOutput(lpNpc->m_Index, Id, "RESOLVA O SEGREDO");
						this->playerKey[Id].clear();
					}
					else {
						YellowWhispSend("Sala 07", Id, "SEQUÊNCIA ERRADA");
						impMap.Move(lpObj,5); //Volta pra sala 5
						this->playerKey[Id].clear();
					}
				}
				else {
					NpcOutput(lpNpc->m_Index, Id, "RESOLVA O SEGREDO");
				}
			}
		}
	}
	return 1;
}

//Programando 11/03/2024 21:50 hr
BOOL ImperiumMap::SalaOito::NPCFunc(LPOBJ lpNpc, LPOBJ lpObj)
{
	int Id = lpObj->m_Index;

	if (lpObj->Map == 18) {
		if (lpNpc->Class == impMap.sala8.NpcId.at(0)) { // Lion door
			if (impMap.sPlayer[Id].RoomLevel < 8) {
				NpcOutput(lpNpc->m_Index, Id, "Não está no level desta sala");
				return TRUE;
			}
			else if (pObj[Id].mResets < impMap.sala8.qReset) {
				NpcOutput(lpNpc->m_Index, Id, "Precisa ter %d Resets", impMap.sala6.qReset);
				return TRUE;
			}

			if (impMap.sPlayer[Id].RoomLevel == 8) {

				if (impMap.sala8.mobDeadTime <= 1) {
					NpcOutput(lpNpc->m_Index, Id, "Acabe com os 3 monstros de uma vez");	
				}
				else {
					impMap.Move(lpObj, 9); // Go sala 9
				}
				return TRUE;
			/*	for (int m = 0; m < 5800; m++) {
					LPOBJ lpMob = &gObj[m];
					if (lpMob->Type == OBJ_MONSTER) {
						if (lpMob->Class == 161 && lpMob->Live != FALSE) {
							NpcOutput(lpNpc->m_Index, Id, "Acabe com os 3 monstros de uma vez");
							break;
							return TRUE;
						}
						else if (lpMob->Class == 161 && lpMob->Live == FALSE){
							impMap.Move(lpObj, 9); // Go sala 9
							break;
							return TRUE;
						}
					}
				} */
				
			}		
		}
	}
	return FALSE;
}

void ImperiumMap::SalaOito::BossDeath(LPOBJ lpMonster, LPOBJ lpObj)
{
	int Id = lpObj->m_Index;

	if (lpObj->Map == 18) {
		if (lpMonster->Class == 161) {
			if (impMap.sala8.mobDeadTime == 0) {
				impMap.sala8.mobDeadTime = 60;
				YellowWhispSend("Sala 8", lpObj->m_Index, "Você tem 1 minuto para ir para sala 9");
			}
		}
	}
}

void ImperiumMap::SalaOito::AttackFunc(LPOBJ lpObj, LPOBJ lpTarget)
{
	for (int m = 0; m < 5800; m++) {
		LPOBJ lpMob = &gObj[m];
		if (lpTarget->Class == 160) {
			if (lpMob->Class == 159 && lpMob->Live != FALSE) {
				lpTarget->m_SuccessfulBlocking = 40000;
			}
			else if (lpMob->Class == 159 && lpMob->Live == FALSE) {
				lpTarget->m_SuccessfulBlocking = mobDefenseRate[0];
			}
		}

		if (lpTarget->Class == 161) {
			if (lpMob->Class == 159 || lpMob->Class == 160 && lpMob->Live != FALSE) {
				lpTarget->m_SuccessfulBlocking = 40000;
			}
			else if (lpMob->Class == 159 || lpMob->Class == 160 && lpMob->Live == FALSE) {
					lpTarget->m_SuccessfulBlocking = mobDefenseRate[1];
			}
		}
	}
}


void ImperiumMap::SalaNove::BossDeath(LPOBJ lpMonster, LPOBJ lpObj)
{

	if (lpMonster->Map == 18) {
		int Id = lpObj->m_Index;

		if (lpMonster->Class == impMap.sala9.bossId) {
			impMap.sala9.bossDelTime = 6;
			impMap.sala9.DeadBoss[Id] = true;
			
			MapAnnounce(18, "[Sala 9] Boss  eliminado!");
			YellowWhispSend("Sala 9", lpObj->m_Index, "Pode ir para a sala 10!");
		}
	}
}

void ImperiumMap::SalaNove::BossDeathClear()
{
	if (impMap.sala9.bossDelTime > 0) {
		impMap.sala9.bossDelTime--;
		if (impMap.sala9.bossDelTime == 1) {

			for (int i = 0; i < 5700; i++) {
				LPOBJ lpObj = &gObj[i];
				if (lpObj->Class == impMap.sala9.bossId && impMap.sala9.bossIndex != -1) {
					gObjDel(lpObj->m_Index);
					impMap.sala9.bossIndex = -1;
				}
			}
		}
	}
}

void ImperiumMap::SalaNove::BossRespawn(LPOBJ lpObj, LPOBJ lpMob)
{
	int Id = lpObj->m_Index;

	if (lpObj->Map == 18) {
		if (lpMob->Class != impMap.sala9.bossId) {
			if (impMap.sala9.bossIndex == -1 && impMap.sala9.DeadBoss[Id] == false) {
				impMap.sala9.countMob[Id]++;

				if (impMap.sala9.countMob[Id] >= impMap.sala9.countMobConfig) {
					impMap.sala9.bossIndex = MonsterAdd(impMap.sala9.bossId, 18, impMap.sala9.bossX, impMap.sala9.bossY); //cria boss
					impMap.sala9.countMob[Id] = 0;
					MapAnnounce(18, "[Sala 9] Boss apareceu!!");
				}
			}
		}	
	}
}

void ImperiumMap::SalaNove::Gate(int aIndex, int gate)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gate == 84) {
		if (impMap.sala9.DeadBoss[aIndex] == false) {
			short x = lpObj->X;
			short y = lpObj->Y;
			BYTE mapNumber = lpObj->Map;
			BYTE dir = lpObj->Dir;
			lpObj->RegenMapNumber = lpObj->Map;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			gObjClearViewport(&gObj[aIndex]);
			GCTeleportSend(&gObj[aIndex], gate, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

			if (lpObj->m_Change >= 0){
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;
			MsgOutput(aIndex, "Você não matou o boss 9 ainda");
		}
		else if (impMap.sala9.itemReq[0] > 0 && CheckInventory(aIndex, ITEMGET(impMap.sala9.itemReq[1], impMap.sala9.itemReq[2]), impMap.sala9.itemReq[3]) < impMap.sala9.itemReq[0]) {
			short x = lpObj->X;
			short y = lpObj->Y;
			BYTE mapNumber = lpObj->Map;
			BYTE dir = lpObj->Dir;
			lpObj->RegenMapNumber = lpObj->Map;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			gObjClearViewport(&gObj[aIndex]);
			GCTeleportSend(&gObj[aIndex], gate, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

			if (lpObj->m_Change >= 0) {
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;

			CItem item;
			item.m_Type = ITEMGET(impMap.sala9.itemReq[1], impMap.sala9.itemReq[2]);
			item.m_Level = impMap.sala9.itemReq[3];

			MsgOutput(aIndex, "Precisa de %d %s item", impMap.sala9.itemReq[0], item.GetName());
		}
		else {
			impMap.sPlayer[aIndex].RoomLevel = 10;
			impMap.sala9.DeadBoss[aIndex] = false;
			impMap.sPlayer[aIndex].rankPoints++;
			int iTeleport = ITEMGET(14, 10);
			ItemSerialCreateSend(aIndex, 18, 213, 204, iTeleport, 12, 0, 0, 0, 0, aIndex, 0);
			AllServerAnnounce("[Mapa Imperium] %s Finalizou todas as quests!", lpObj->Name);
			MySQL.ExecQuery("UPDATE Character SET ImpMapRank = ImpMapRank + 1 where Name = '%s'", lpObj->Name);
			MySQL.Fetch();
		}
	}
}
