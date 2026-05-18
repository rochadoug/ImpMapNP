#pragma once

struct RoomStruct
{
	bool                 _Invitation;
	bool                 _Live;
	bool                 _Running;
	bool                 _Waiting;
	int                  _Index;
	int                  _Target;
	int                  _Time[3];
};

struct DuelPlayer
{
	bool                 _Duel;
	bool                 _Attack;
	bool                 _Move;
	BYTE                 _State;
	BYTE                 _Room;
	int                  _TargetIndex;
	int                  _Score;
	int                  _Time;
	//para salvar os itens ao dar OK
	CItem*				 Trade;
	BYTE*				 TradeMap;
	//char				 pInventoryCount;
};

class cDuel
{
public:
	cDuel();
	~cDuel();
	bool Load();
	int GetPlayer(std::string Name);
	bool Attack(LPOBJ lpObj, LPOBJ Target);
	bool CheckRoom(int aIndex, int Target);
	void DuelRoom(int aIndex, int Target, int Room);
	void InvitationSend(int aIndex, int Target);
	void InvitationCancel(int aIndex, int Target);
	void InvitationCheck(LPOBJ lpObj);
	void Invitation(int aIndex, char* msg);
	void AcceptSend(int aIndex, int Target);
	void Accept(int aIndex);
	void ClearRoom(int Room);
	void ZerarDuelPlayer(int aIndex, int Target);
	void Killer(LPOBJ lpObj, LPOBJ Target);
	void CheckWinner(int aIndex, int Target);
	void Winner(int aIndex, int Target, int Score1, int Score2);
	static void Run(int Room);
	void Quit(LPOBJ lpObj);

	void MovePlayerFromRoom(LPOBJ lpObj);
	void CheckInvite(LPOBJ lpObj);
	void gObjTradeOkButtonEx(int aIndex);
	void ObjTradeCancelEx(int aIndex);
	void InviteReponse(PMSG_TRADE_RESPONSE *pResp, int aIndex);
	void DuelTradeCommit(int aIndex);
	void DuelTradeClear(int aIndex);
	void DuelTradeFreeMemory();
	bool PutInventoryItem(LPOBJ lpObj);
	bool ReturnInventoryItem(int p1Index, int p2Index);

public:
	bool				 _Active;
	int					 _Level;
	int                  _MapNumber[4];
	int                  _X[4];
	int                  _Y[4];
	int                  _Score[4];
	DWORD                _Time[4];
	char                 _Name[11];
	char				 _Syntax[2][25];

public:
	RoomStruct			    Room[4];

	DuelPlayer               Player[999]; //OBJECT_MAX não precisa alocar 5600 espaços pois só cabe 1000 player conectados 
};

extern cDuel Duel;