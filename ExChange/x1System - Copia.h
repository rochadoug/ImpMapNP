#pragma once

class x1System
{
public:
	
	x1System();
	virtual ~x1System();

	char Syntax[10];
	struct Player
	{
		BYTE pStatus; // 0 não lutando, 1 convidou, 2 convidado, 3 aceitou e 4 lutando;
		BYTE pWinLose; // 0 nada; 1 Win; 2 Lose.
		int pTarget;	//Id do inimigo
		short pPoints;	// pontos...
		int pCoinT, pCoinQ;  //moeda e valor da aposta
		short pTimeInvite, pTimeRound; //tempo convite / tempo lutando
	}player[500];

	void X1ComandRequest(LPOBJ lpObj, char* msg);
	void X1ComandReponse(LPOBJ lpObj, char* msg);
	void X1Manager();
	void X1Reset();
	void PlayerReset(LPOBJ lpObj);

	BYTE Active;
	int rLevel, rReset;
};

extern x1System x1[100];