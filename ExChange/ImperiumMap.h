#pragma once

#define IMPMAXROOM		10

class ImperiumMap {
public:
	ImperiumMap();
	~ImperiumMap();

	//enum rooms
	//{
	//	_sala1 = 1,_sala2,_sala3,_sala4,_sala5,_sala6,_sala7,_sala8,_sala9,_sala10
	//};

	struct Player 
	{
		long long mobKills;
		long long bossKills;
		int RoomLevel;
	}sPlayer[1000];

	struct Room
	{	
		BYTE X1, Y1, X2, Y2;
		vector<BYTE> RespX, RespY;
		vector<int> NpcId;
		int qReset;
	};
	//vector<Room> sRoom;

	struct SalaUm : Room
	{		
		long long mobQtd;
	}sala1;

	struct SalaDois : Room
	{
		long long mobQtd;
	}sala2;

	struct SalaTres : Room
	{
		long long mobQtd;
		int mobId;
		int qtdCreation;
		int geralQuest;
		BOOL NPCFunc(LPOBJ lpNpc, LPOBJ lpObj);
	}sala3;

	struct SalaQuatro : Room
	{
		//int EntryGateClass, OutGateClass;
		int sortQuestValue;
		int quantQuestValue;
		int quantMobToSort[1000];
		bool isCompleteQuest[1000];
		bool canActivateDisp[1000];
		int itemPremio[8];
		BOOL NPCFunc(LPOBJ lpNpc, LPOBJ lpObj);

	}sala4;

	struct SalaCinco : Room
	{
		int bossId;
		bool killedBoss;
		int hp, attackDmg, attackRate, defenseRate, defense,magicDefense;
		int hpConf, attackDmgConf, attackRateConf, defenseRateConf, defenseConf, magicDefenseConf, regenTimeConf;
		void setBridgeGuardStrong(bool k);
		void setBossStrong(int aIndex, int mobid);
		enum {enBoss, enGuard};
	}sala5;

	struct SalaSeis : Room
	{
		int timeToOpen,timeToClose, timeOpend, second;
		void DragonDoorProc();
		BOOL NPCFunc(LPOBJ lpNpc, LPOBJ lpObj);
	}sala6;

	struct SalaSete : Room
	{
		vector<int> wheelX, wheelY;
		vector<int> playerKey[1000]; //sequencia gerada ao quebrar estatuas
		vector<int> secretKey;  // sequencia correta
		BOOL NPCFunc(LPOBJ lpNpc, LPOBJ lpObj);
	}sala7;

	struct SalaOito : Room
	{
		int mobDefenseRate[2];
		BOOL NPCFunc(LPOBJ lpNpc, LPOBJ lpObj);
		void AttackFunc(LPOBJ lpObj, LPOBJ lpTarget);
	}sala8;

	struct SalaNove : Room
	{
		int bossId, bossQtd, bossIndex;
		int bossX, bossY;
		int countMob[1000], countMobConfig;
		int itemReq[4];
		int bossDelTime;
		void BossDeath(LPOBJ lpObj);
		void BossDeathClear();
		void BossRespawn(LPOBJ lpObj, LPOBJ lpMob);
	}sala9;

	struct SalaDez : Room
	{

	}sala10;

public:
	int movereqLevel, movereqReset;
	int moveItemGrupo, moveItemId, moveItemLevel;

public:
	BOOL RoomNPC(LPOBJ lpNpc, LPOBJ lpObj);
	void RoomQuestCountKill(LPOBJ lpMob, LPOBJ lpObj);
	int GetCurRoom(LPOBJ lpObj);
	BOOL AttackGerence(LPOBJ lpNpc, LPOBJ lpObj);
	void configGeral();
	void ChargeConfig();
	void PlayerEnterGame(LPOBJ lpObj, BYTE modo);
	bool Move(LPOBJ lpObj, BYTE pos, short lvl);
	bool Move(LPOBJ lpObj, short lvl);
	bool roomVerify(LPOBJ lpObj);
};

extern ImperiumMap impMap;