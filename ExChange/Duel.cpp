#include "StdAfx.h"
#include "Duel.h"

cDuel::cDuel()
{

	for (int i = 4800; i < 5600; i++) {
		this->Player[i].Trade = new CItem[32];
		ZerarDuelPlayer(i, -1);
	}
}

cDuel::~cDuel()
{
	this->DuelTradeFreeMemory();
}

bool cDuel::Load()
{
	this->_Active = 1 > 0 ? true : false;
	this->_Level = 1;

	this->_MapNumber[0] = 17;
	this->_MapNumber[1] = 17;
	this->_MapNumber[2] = 17;
	this->_MapNumber[3] = 17;
	this->_X[0] = 103;
	this->_X[1] = 113;
	this->_X[2] = 103;
	this->_X[3] = 113;
	this->_Y[0] = 165;
	this->_Y[1] = 165;
	this->_Y[2] = 154;
	this->_Y[3] = 154;

	this->_Time[0] = 4 * 60;
	this->_Score[0] = 3;
	this->_Score[1] = 3;
	this->_Score[2] = 3;
	this->_Score[3] = 3;

	strcpy_s(this->_Syntax[0], sizeof(this->_Syntax[0]), "/duelo ");
	strcpy_s(this->_Syntax[1], sizeof(this->_Syntax[1]), "/duelar");

	/*Tokenizer          Token;
	TokenizerGroup     Group;
	TokenizerSection   Section;

	Token.ParseFile(std::string("./Data/Duel.txt"), Group);

	if (Group.GetSection(0, Section))
	{
		this->_Active = Section.Rows[0].GetInt(0) > 0 ? true : false;
		this->_Level = Section.Rows[0].GetInt(1);
		this->_Zen = Section.Rows[0].GetInt(2);

		strcpy_s(this->_Column, sizeof(this->_Column), (Section.Rows[0].GetStringPtr(3)));
		strcpy_s(this->_Coin, sizeof(this->_Coin), (Section.Rows[0].GetStringPtr(4)));

		strcpy_s(this->_Syntax[0], sizeof(this->_Syntax[0]), (Section.Rows[0].GetStringPtr(5)));
		strcpy_s(this->_Syntax[1], sizeof(this->_Syntax[1]), (Section.Rows[0].GetStringPtr(6)));

		this->_MapNumber[0] = Section.Rows[1].GetInt(0);
		this->_X[0] = Section.Rows[1].GetInt(1);
		this->_Y[0] = Section.Rows[1].GetInt(2);
		this->_Time[0] = Section.Rows[1].GetInt(3) * 60;
		this->_Score[0] = Section.Rows[1].GetInt(4);

		this->_MapNumber[1] = Section.Rows[2].GetInt(0);
		this->_X[1] = Section.Rows[2].GetInt(1);
		this->_Y[1] = Section.Rows[2].GetInt(2);
		this->_Time[1] = Section.Rows[2].GetInt(3) * 60;
		this->_Score[1] = Section.Rows[2].GetInt(4);

		this->_MapNumber[2] = Section.Rows[3].GetInt(0);
		this->_X[2] = Section.Rows[3].GetInt(1);
		this->_Y[2] = Section.Rows[3].GetInt(2);
		this->_Time[2] = Section.Rows[3].GetInt(3) * 60;
		this->_Score[2] = Section.Rows[3].GetInt(4);

		this->_MapNumber[3] = Section.Rows[4].GetInt(0);
		this->_X[3] = Section.Rows[4].GetInt(1);
		this->_Y[3] = Section.Rows[4].GetInt(2);
		this->_Time[3] = Section.Rows[4].GetInt(3) * 60;
		this->_Score[3] = Section.Rows[4].GetInt(4);
	}*/


	return true;
}

int cDuel::GetPlayer(std::string Name)
{
	for (int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if (gObj[i].Connected == 3)
		{
			if (!strcmp(gObj[i].Name, Name.c_str()))
			{
				return i;
			}
		}
	}

	return -1;
}

bool cDuel::Attack(LPOBJ lpObj, LPOBJ Target)
{
	if (lpObj->Type == OBJTYPE_USER && Target->Type == OBJTYPE_USER)
	{
		if (this->Player[lpObj->m_Index]._Attack == true)
		{
			return false;
		}
	}

	return true;
}

bool cDuel::CheckRoom(int aIndex, int Target)
{
	if (!this->Room[0]._Live && !this->Room[0]._Invitation)
	{
		this->Room[0]._Invitation = true;

		this->Player[aIndex]._Room = 0;

		this->Player[Target]._Room = 0;

		this->Player[aIndex]._State = 1;
		return true;
	}
	else if (!this->Room[1]._Live && !this->Room[1]._Invitation)
	{
		this->Room[1]._Invitation = true;

		this->Player[aIndex]._Room = 1;

		this->Player[Target]._Room = 1;

		this->Player[aIndex]._State = 1;
		return true;
	}
	else if (!this->Room[2]._Live && !this->Room[2]._Invitation)
	{
		this->Room[2]._Invitation = true;

		this->Player[aIndex]._Room = 2;

		this->Player[Target]._Room = 2;

		this->Player[aIndex]._State = 1;
		return true;
	}
	else if (!this->Room[3]._Live && !this->Room[3]._Invitation)
	{
		this->Room[3]._Invitation = true;

		this->Player[aIndex]._Room = 3;

		this->Player[Target]._Room = 3;

		this->Player[aIndex]._State = 1;
		return true;
	}

	return false;
}

void cDuel::DuelRoom(int aIndex, int Target, int room)
{
	this->Room[room]._Invitation = false;

	this->Room[room]._Live = true;

	this->Room[room]._Index = aIndex;

	this->Room[room]._Target = Target;

	PlayerIndexAnnounce(aIndex, "[   DUELO   ]");
	PlayerIndexAnnounce(aIndex, "%s VS %s - Sala : %d", gObj[this->Room[room]._Index].Name, gObj[this->Room[room]._Target].Name, (room + 1));

	PlayerIndexAnnounce(Target, "[   DUELO    ]");
	PlayerIndexAnnounce(Target, "%s VS %s - Sala : %d", gObj[this->Room[room]._Index].Name, gObj[this->Room[room]._Target].Name, (room + 1));

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cDuel::Run, (void*)room, 0, 0);
}

void cDuel::InvitationSend(int aIndex, int Target)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ pTargObj = &gObj[Target];

	this->Player[aIndex]._State = 1;
	this->Player[Target]._State = 2;

	this->Player[aIndex]._Time = 0;
	this->Player[Target]._Time = 0;

	this->Player[aIndex]._TargetIndex = Target;
	this->Player[Target]._TargetIndex = aIndex;

	MsgOutput(aIndex, "Convite de duelo enviado para %s", gObj[Target].Name);
	MsgOutput(Target, "%s lhe convidou para um duelo.", gObj[aIndex].Name);

	/***  Parte do duelo por trade ***/
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 1;
	lpObj->TargetNumber = Target;
	pTargObj->m_IfState.use = 1;
	pTargObj->m_IfState.state = 0;
	pTargObj->m_IfState.type = 1;
	pTargObj->TargetNumber = aIndex;
	lpObj->m_InterfaceTime = GetTickCount();
	pTargObj->m_InterfaceTime = GetTickCount();

	PMSG_TRADE_REQUESTSEND pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x36;
	pMsg.h.size = sizeof(pMsg);
	pMsg.isDuel = 1;
	memcpy(pMsg.szId, lpObj->Name, sizeof(pMsg.szId));

	DataSend(Target, (LPBYTE)&pMsg, pMsg.h.size);
	/** Fim da parte do trade **/
}

void cDuel::InvitationCancel(int aIndex, int Target)
{
	
	this->Player[aIndex]._State = 0;
	this->Player[Target]._State = 0;

	this->Player[aIndex]._Time = 0;
	this->Player[Target]._Time = 0;

	this->Player[aIndex]._TargetIndex = -1;
	this->Player[Target]._TargetIndex = -1;

	this->Room[this->Player[aIndex]._Room]._Live = false;
	this->Room[this->Player[Target]._Room]._Live = false;

	this->Room[this->Player[aIndex]._Room]._Invitation = false;
	this->Room[this->Player[Target]._Room]._Invitation = false;

	this->Player[aIndex]._Room = 0;
	this->Player[Target]._Room = 0;

	MsgOutput(aIndex, "Duelo cancelado.");
	MsgOutput(Target, "Duelo cancelado.");
}

void cDuel::InvitationCheck(LPOBJ lpObj)
{
	int Target = this->Player[lpObj->m_Index]._TargetIndex;

	if (this->Player[lpObj->m_Index]._State == 2)
	{
		this->Player[lpObj->m_Index]._Time++;

		if (this->Player[lpObj->m_Index]._Time > 5)
		{
			this->InvitationCancel(lpObj->m_Index, Target);
		}
	}

	if (this->Player[Target]._State == 1 || this->Player[Target]._State == 2)
	{
		if (gObj[Target].Connected < 3)
		{
			this->InvitationCancel(lpObj->m_Index, Target);
		}
	}
}

void cDuel::Invitation(int aIndex, char* msg)
{
	if (!this->_Active)
	{
		MsgOutput(aIndex, "Comando desabilitado.");
		return;
	}

	memset(this->_Name, '\0', sizeof(this->_Name));

	if (strlen(msg) < 1)
	{
		MsgOutput(aIndex, "Erro de sintaxe!");
		MsgOutput(aIndex, "Digite: %s <Nome>", this->_Syntax[0]);
		MsgOutput(aIndex, "Duelando apostado:", this->_Syntax[0]);
		MsgOutput(aIndex, "Digite: %s <Nome> <Quantidade>", this->_Syntax[0]);
		return;
	}

	sscanf(msg, "%s", &this->_Name);

	int targetIndex = this->GetPlayer(this->_Name);

	if (aIndex == targetIndex)
	{
		MsgOutput(aIndex, "Você não pode duelar contra você mesmo.");
		return;
	}
	else if (targetIndex == -1)
	{
		MsgOutput(aIndex, "%s não está online.", this->_Name);
		return;
	}
	else if (this->Player[aIndex]._State != 0)
	{
		MsgOutput(aIndex, "Você já está em um duelo.");
		return;
	}
	else if (this->Player[targetIndex]._State != 0)
	{
		MsgOutput(aIndex, "Seu oponente já está em um duelo.");
		return;
	}
	else if (gObj[aIndex].m_IfState.use != 0)
	{
		MsgOutput(aIndex, "Você está ocupado no momento.");
		return;
	}
	else if (gObj[targetIndex].m_IfState.use != 0)
	{
		MsgOutput(aIndex, "Seu oponente está ocupado no momento.");
		return;
	}
	else if (gObj[aIndex].Level < this->_Level)
	{
		MsgOutput(aIndex, "É necessário %d de level.", this->_Level);
		return;
	}
	else if (gObj[targetIndex].Level < this->_Level)
	{
		MsgOutput(aIndex, "O oponente não possui level %d", this->_Level);
		return;
	}
	else if (gObj[targetIndex].Map != gObj[aIndex].Map) {
		MsgOutput(aIndex, "Quer duelar a distancia?");
		return;
	}
	else if (gObjCalDistance(&gObj[targetIndex], &gObj[aIndex]) > 2) {
		MsgOutput(aIndex, "Chegue mais perto para duelar");
		return;
	}

	if (this->CheckRoom(aIndex, targetIndex) == false){
			MsgOutput(aIndex, "Não existem salas liberadas no momento.");
			return;
	}
	else {
		this->InvitationSend(aIndex, targetIndex);
	}
}

void cDuel::AcceptSend(int aIndex, int Target)
{
	this->Player[aIndex]._Duel = true;
	this->Player[Target]._Duel = true;

	this->Player[aIndex]._Attack = true;
	this->Player[Target]._Attack = true;

	this->Player[aIndex]._State = 3;
	this->Player[Target]._State = 3;

	this->Player[aIndex]._Time = 0;
	this->Player[Target]._Time = 0;

	MsgOutput(Target, "Adicione os itens para apostar no trade");
	MsgOutput(aIndex, "Adicione os itens para apostar no trade");
}

void cDuel::Accept(int aIndex)
{
	
	if (this->Player[aIndex]._State != 2)
	{
		MsgOutput(aIndex, "Não existe nenhum convite ativo.");
		return;
	}
	else if (gObj[this->Player[aIndex]._TargetIndex].Connected < 3)
	{
		MsgOutput(aIndex, "Não foi possível continuar, seu oponente saiu.");

		this->InvitationCancel(aIndex, this->Player[aIndex]._TargetIndex);
		return;
	}
	
	this->AcceptSend(aIndex, this->Player[aIndex]._TargetIndex);
		
}

void cDuel::ClearRoom(int room)
{
	this->Player[this->Room[room]._Index]._Duel = false;
	this->Player[this->Room[room]._Target]._Duel = false;

	this->Player[this->Room[room]._Index]._Attack = false;
	this->Player[this->Room[room]._Target]._Attack = false;

	this->Player[this->Room[room]._Index]._State = 0;
	this->Player[this->Room[room]._Target]._State = 0;

	this->Player[this->Room[room]._Index]._Score = 0;
	this->Player[this->Room[room]._Target]._Score = 0;

	this->Room[room]._Running = false;

	this->Room[room]._Invitation = false;

	this->Room[room]._Waiting = false;

	this->Room[room]._Live = false;


	this->Room[room]._Index = -1;
	this->Room[room]._Target = -1;

	this->Room[room]._Time[0] = 0;
	this->Room[room]._Time[1] = 0;
	this->Room[room]._Time[2] = 0;
}

void cDuel::ZerarDuelPlayer(int aIndex, int Target)
{
	this->Player[aIndex]._Duel = false;
	this->Player[Target]._Duel = false;

	this->Player[aIndex]._Attack = false;
	this->Player[Target]._Attack = false;

	this->Player[aIndex]._Room = 0;
	this->Player[Target]._Room = 0;

	this->Player[aIndex]._TargetIndex = -1;
	this->Player[Target]._TargetIndex = -1;

	this->Player[aIndex]._State = 0;
	this->Player[Target]._State = 0;

	this->Player[aIndex]._Time = 0;
	this->Player[Target]._Time = 0;

	this->Player[aIndex]._Score = 0;
	this->Player[Target]._Score = 0;

	//this->DuelTradeClear(aIndex);
}

void cDuel::Killer(LPOBJ lpObj, LPOBJ Target)
{
	if (lpObj->Type == OBJECT_USER && Target->Type == OBJECT_USER)
	{
		int Room = this->Player[lpObj->m_Index]._Room;

		if (this->Room[Room]._Live == true && this->Room[Room]._Running == true)
		{
			if (this->Player[lpObj->m_Index]._State == 3 && this->Player[Target->m_Index]._State == 3 && lpObj->Map == this->_MapNumber[Room])
			{
				this->Player[lpObj->m_Index]._Score++;

				PlayerIndexAnnounce(lpObj->m_Index,  "[    PLACAR    ]");

				PlayerIndexAnnounce(lpObj->m_Index, "%s  vs  %s", lpObj->Name, Target->Name);

				PlayerIndexAnnounce(lpObj->m_Index, "%d   x   %d", this->Player[lpObj->m_Index]._Score, this->Player[Target->m_Index]._Score);

				PlayerIndexAnnounce(Target->m_Index, "[    PLACAR    ]");

				PlayerIndexAnnounce(Target->m_Index, "%s  vs  %s", Target->Name, lpObj->Name);

				PlayerIndexAnnounce(Target->m_Index, "%d   x   %d", this->Player[Target->m_Index]._Score, this->Player[lpObj->m_Index]._Score);

				if (this->Player[lpObj->m_Index]._Score == this->_Score[Room])
				{
					this->CheckWinner(lpObj->m_Index, Target->m_Index);
				}
			}
		}
	}
}

void cDuel::CheckWinner(int aIndex, int Target)
{
	int Room = this->Player[aIndex]._Room;

	if (this->Player[aIndex]._Score > this->Player[Target]._Score)
	{
		this->Winner(aIndex, Target, this->Player[aIndex]._Score, this->Player[Target]._Score);
	}
	else if (this->Player[Target]._Score > this->Player[aIndex]._Score)
	{
		this->Winner(aIndex, Target, this->Player[aIndex]._Score, this->Player[Target]._Score);
	}
	else if (this->Player[aIndex]._Score == this->Player[Target]._Score)
	{
		for (int Index = OBJECT_MIN; Index < OBJECT_MAX; Index++)
		{
			if (gObj[Index].Connected == 3)
			{
				PlayerIndexAnnounce(Index, "[   DUELO   ]");
				PlayerIndexAnnounce(Index, "Houve um empate entre %s e %s", gObj[aIndex].Name, gObj[Target].Name);
			}
		}

		this->Player[aIndex]._Move = true;
		this->Player[Target]._Move = true;
		this->ReturnInventoryItem(aIndex, Target);

		this->ClearRoom(Room);

		this->ZerarDuelPlayer(aIndex, Target);

	}
	/*else if (this->Player[Target]._Score == this->Player[aIndex]._Score)  não entendi pq repetir
	{
		for (int Index = OBJECT_MIN; Index < OBJECT_MAX; Index++)
		{
			if (gObj[Index].Connected == 3)
			{
				PlayerIndexAnnounce(Index, "[   DUELO   ]");
				PlayerIndexAnnounce(Index, "Houve um empate entre %s e %s", gObj[Target].Name, gObj[aIndex].Name);
			}
		}

		this->Player[Target]._Move = true;
		this->Player[aIndex]._Move = true;

		this->ClearRoom(Room);

		this->ZerarDuelPlayer(aIndex, Target);
	}*/
}

void cDuel::Winner(int aIndex, int Target, int Score1, int Score2)
{
	int room = this->Player[aIndex]._Room;

	gObj[aIndex].PkLevel = 3;
	GCPkLevelSend(aIndex, 3);

	gObj[Target].PkLevel = 3;
	GCPkLevelSend(Target, 3);

	for (int Index = OBJECT_MIN; Index < OBJECT_MAX; Index++)
	{
		if (gObj[Index].Connected == 3)
		{
			PlayerIndexAnnounce(Index, "[   DUELO   ]");
			PlayerIndexAnnounce(Index, "%s venceu um duelo contra %s por %d x %d", gObj[aIndex].Name, gObj[Target].Name, Score1, Score2);
		}
	}

	GCFireWork(aIndex);
	GCFireWork(Target);

	this->Player[aIndex]._Move = true;
	this->Player[Target]._Move = true;
	if (this->PutInventoryItem(&gObj[aIndex])) {
		MsgOutput(aIndex, "# Itens da aposta recebidos #");
		MsgOutput(Target, "# Itens da aposta entregues #");
	}

	this->ClearRoom(room);

	this->ZerarDuelPlayer(aIndex, Target);
}

void cDuel::Run(int room)
{
	while (true)
	{
		if (Duel.Room[room]._Live == true)
		{
			if (!Duel.Room[room]._Waiting)
			{
				Duel.Room[room]._Time[2]++;

				MsgOutput(Duel.Room[room]._Index,  "Movendo-se para a sala %d em %d Segundo(s)", (room + 1), (6 - Duel.Room[room]._Time[2]));
				MsgOutput(Duel.Room[room]._Target, "Movendo-se para a sala %d em %d Segundo(s)", (room + 1), (6 - Duel.Room[room]._Time[2]));

				if (Duel.Room[room]._Time[2] == 5)
				{
					gObjTeleport(Duel.Room[room]._Index, Duel._MapNumber[room], Duel._X[room], Duel._Y[room]);
					gObjTeleport(Duel.Room[room]._Target, Duel._MapNumber[room], Duel._X[room], Duel._Y[room]);

					gObj[Duel.Room[room]._Index].PkLevel = 6;
					gObj[Duel.Room[room]._Target].PkLevel = 6;

					GCPkLevelSend(Duel.Room[room]._Index, 6);
					GCPkLevelSend(Duel.Room[room]._Target, 6);

					Duel.Room[room]._Waiting = true;
				}

			}
			else if (Duel.Room[room]._Running == true)
			{
				Duel.Room[room]._Time[1]++;

				if (Duel.Room[room]._Time[1] >= Duel._Time[room])
				{
					//Func.MsgOutput(Duel.RoomStruct[Room]._Index, 0, "[   DUELO   ]");
					//Func.MsgOutput(Duel.RoomStruct[Room]._Index, 0, "Infelizmente o tempo foi esgotado.");
					PlayerIndexAnnounce(Duel.Room[room]._Index,  "[   DUELO   ]");
					PlayerIndexAnnounce(Duel.Room[room]._Index, "Infelizmente o tempo foi esgotado.");

					PlayerIndexAnnounce(Duel.Room[room]._Target, "[   DUELO   ]");
					PlayerIndexAnnounce(Duel.Room[room]._Target, "Infelizmente o tempo foi esgotado.");

					Duel.Room[room]._Time[1] = 0;

					Duel.CheckWinner(Duel.Room[room]._Index, Duel.Room[room]._Target);

				}
			}
			else
			{
				Duel.Room[room]._Time[0]++;

				MsgOutput(Duel.Room[room]._Index, "Inicio em %d Segundo(s)", (6 - Duel.Room[room]._Time[0]));

				MsgOutput(Duel.Room[room]._Target, "Inicio em %d Segundo(s)", (6 - Duel.Room[room]._Time[0]));

				if (Duel.Room[room]._Time[0] == 5)
				{
					MsgOutput(Duel.Room[room]._Index,  "O duelo começou! [%d]", (11 - Duel.Room[room]._Time[0]));

					MsgOutput(Duel.Room[room]._Target, "O duelo começou! [%d]", (11 - Duel.Room[room]._Time[0]));

					Duel.Player[Duel.Room[room]._Index]._Attack = false;

					Duel.Player[Duel.Room[room]._Target]._Attack = false;

					Duel.Room[room]._Running = true;

					Duel.Room[room]._Time[0] = 0;
				}
			}
		}
		else
		{
			_endthread();
		}

		Sleep(1300);
	}
}

void cDuel::Quit(LPOBJ lpObj)
{
	if (this->Player[lpObj->m_Index]._Duel == true)
	{
		lpObj->PkLevel = 3;

		//GCPkLevelSend(lpObj->m_Index, 3);

		lpObj->Map = 0; lpObj->X = 125; lpObj->Y = 125;

		int room = this->Player[lpObj->m_Index]._Room;

		int Target = this->Player[lpObj->m_Index]._TargetIndex;

		gObj[Target].PkLevel = 3;

		GCPkLevelSend(Target, 3);

		gObjTeleport(Target, 2, 212, 41);

		for (int Index = OBJECT_MIN; Index < OBJECT_MAX; Index++)
		{
			if (gObj[Index].Connected == 3)
			{	
				MsgOutput(Index,  "[   DUELO   ]");
				MsgOutput(Index, "%s venceu um duelo por W.O.", gObj[Target].Name);
			}
		}

		if(Target != -1)
		this->PutInventoryItem(&gObj[Target]); // Se sair do jogo ou mover os itens vai para o outro
		GCFireWork(Target);

		Duel.ClearRoom(room);

		Duel.ZerarDuelPlayer(Duel.Room[room]._Index, Duel.Room[room]._Target);
	}
}

void cDuel::MovePlayerFromRoom(LPOBJ lpObj)
{
	if (this->Player[lpObj->m_Index]._Move == true)
	{
		if (this->Player[lpObj->m_Index]._Time < 10)
			this->Player[lpObj->m_Index]._Time++;

		if (this->Player[lpObj->m_Index]._Time == 8) {
			if (lpObj->Live == TRUE)
			{
				if (lpObj->Class == 2) //elf
				{
					int X = 171 + rand() % 6;
					int Y = 108 + rand() % 8;

					gObjTeleport(lpObj->m_Index, 3, X, Y);

					lpObj->PkLevel = 3;
					GCPkLevelSend(lpObj->m_Index, 3);

					this->Player[lpObj->m_Index]._Move = false;
					this->Player[lpObj->m_Index]._Time = 0;
				}
				else
				{
					int X = 133 + rand() % 15;
					int Y = 118 + rand() % 15;

					gObjTeleport(lpObj->m_Index, 0, X, Y);

					lpObj->PkLevel = 3;
					GCPkLevelSend(lpObj->m_Index, 3);

					this->Player[lpObj->m_Index]._Move = false;
					this->Player[lpObj->m_Index]._Time = 0;
				}
				lpObj->TradeOk = 0;
				lpObj->pTransaction = 0;
			}
		}
	}
}

void cDuel::CheckInvite(LPOBJ lpObj)
{
	int Target = this->Player[lpObj->m_Index]._TargetIndex;
	
	if (this->Player[lpObj->m_Index]._State == 1 || this->Player[Target]._State == 2) {
		if (lpObj->m_IfState.type == 1) { //trade
			
			this->InvitationCancel(lpObj->m_Index, this->Player[lpObj->m_Index]._TargetIndex);
			
		}
	}
}

void cDuel::gObjTradeOkButtonEx(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int Target = this->Player[aIndex]._TargetIndex;
	BYTE result = 0x01;

	if (this->Player[aIndex]._State < 3) {
		
		gObjTradeOkButton(aIndex);
		return;
	}
	else if (lpObj->TargetNumber != Target) {
		
		gObjTradeOkButton(aIndex);
		return;
	}

	LPOBJ ltObj = &gObj[lpObj->TargetNumber];

	if (lpObj->TradeOk != 1 || ltObj->TradeOk != 1)
	{
		return;
	}

	if (TradeItemInventoryPutTestEx(aIndex) == TRUE) // Testando fix
	{
		if (TradeItemInventoryPutTestEx(Target) == TRUE)
		{
			result = 0x01;
		}
		else result = 0x02;
	}
	else result = 0x02;

	if (result == 0x01) {
		this->DuelTradeCommit(aIndex);
		this->DuelTradeCommit(Target);
		gObjInventoryCommit(aIndex);
		gObjInventoryCommit(Target);

		gObjMakePreviewCharSet(aIndex);
		gObjMakePreviewCharSet(Target);

		GJSetCharacterInfo(&gObj[aIndex], aIndex);
		GJSetCharacterInfo(&gObj[Target], Target);
	}
	else {
		result = 0x02;
		//this->ZerarDuelPlayer(aIndex, Target);  // coloquei aqui
		MsgOutput(aIndex, "[Duelo] Falta espaço para os itens");
		gObjTradeCancelEx(aIndex);
		gObjTradeCancelEx(Target);
		CGTradeResult(aIndex, result);
		CGTradeResult(Target, result);
		return;
	}

	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[Target].TargetNumber = -1;
	gObj[Target].m_IfState.use = 0;
	gObj[aIndex].TradeOk = 0;
	gObj[Target].TradeOk = 0;
	gObj[aIndex].TradeMoney = 0;
	gObj[Target].TradeMoney = 0;
	gObj[aIndex].m_IfState.state = 0;
	gObj[Target].m_IfState.state = 0;

	CGTradeResult(lpObj->m_Index, result);
	CGTradeResult(ltObj->m_Index, result);
	GCItemListSend(aIndex);
	GCItemListSend(Target);

	if (this->Player[lpObj->m_Index]._Duel == true) {
		
		this->DuelRoom(Target, aIndex, this->Player[aIndex]._Room);
	}
}

void cDuel::ObjTradeCancelEx(int aIndex)
{
	int target = this->Player[aIndex]._TargetIndex;
	
	if (this->Player[aIndex]._State != 0) {
		this->InvitationCancel(aIndex, this->Player[aIndex]._TargetIndex);

		gObj[aIndex].m_IfState.use = 1;
		gObj[aIndex].m_IfState.type = 1; // 1 = trade
		gObj[target].m_IfState.use = 1;
		gObj[target].m_IfState.type = 1; // 1 = trade
		ZerarDuelPlayer(aIndex, target);
		gObjTradeCancel(aIndex);
		gObjTradeCancel(target);
	}
	else {
		gObjTradeCancel(aIndex);
		gObjTradeCancel(target);
	}

}

void cDuel::InviteReponse(PMSG_TRADE_RESPONSE * pResp, int aIndex)
{
	if (this->Player[aIndex]._State != 0) {
		int Target = this->Player[aIndex]._TargetIndex;
		if (pResp->Response == TRUE) {

			this->Accept(aIndex);
		}
		else {
			this->InvitationCancel(aIndex, this->Player[aIndex]._TargetIndex);
		}
	}
}

void cDuel::DuelTradeCommit(int aIndex)
{
	//int Target = this->Player[aIndex]._TargetIndex;
	
	
	LPOBJ lpObj = &gObj[aIndex];
	if (this->Player[aIndex]._State != 0) {
		
		for (int slot = 0; slot < 32; slot++) {
				this->Player[aIndex].Trade[slot] = lpObj->Trade[slot];
		}
	}
	//memcpy(this->Player[aIndex].TradeMap, lpObj->TradeMap, 8 * 4);
}


//colocar na entrada do player no jogo se for usar a variavel trade do duelo
void cDuel::DuelTradeClear(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	for (int slot = 0; slot < 32; slot++) {
		
		lpObj->Trade[slot].Clear();
	}
}

void cDuel::DuelTradeFreeMemory()
{
	for (int i = 0; i > 5600; i++) {
		delete this->Player[i].Trade;
	}
}

bool cDuel::PutInventoryItem(LPOBJ lpObj)
{
	int aIndex = lpObj->m_Index;
	int Target = this->Player[aIndex]._TargetIndex;
	
	for (BYTE i = 0; i < 32; i++)
	{
		if (lpObj->Trade[i].IsItem())
		{
			if (gObjInventoryInsertItem2(aIndex, lpObj->Trade[i]) == 255)
			{
				return false;
			}

			lpObj->Trade[i].Clear();
			//this->DuelTradeClear(aIndex);
		}

		if (gObj[Target].Trade[i].IsItem())
		{
			if (gObjInventoryInsertItem2(aIndex, gObj[Target].Trade[i]) == 255)
			{
				return false;
			}
			//this->DuelTradeClear(Target);
			gObj[Target].Trade[i].Clear();
		}
	}
	GCItemListSend(aIndex);
	gObjMakePreviewCharSet(aIndex);
	GJSetCharacterInfo(lpObj, aIndex);
	return true;
}

bool cDuel::ReturnInventoryItem(int p1Index, int p2Index)
{
	LPOBJ  lpObj = &gObj[p1Index];
	LPOBJ  targObj = &gObj[p2Index];
	
	for (BYTE i = 0; i < 32; i++)
	{
		if (lpObj->Trade[i].IsItem())
		{
			if (gObjInventoryInsertItem2(p1Index, lpObj->Trade[i]) == 255)
			{
				return false;
			}

			lpObj->Trade[i].Clear();
			
		}

		if (targObj->Trade[i].IsItem())
		{
			if (gObjInventoryInsertItem2(p2Index, targObj->Trade[i]) == 255)
			{
				return false;
			}
			
			targObj->Trade[i].Clear();
		}
	}
	GCItemListSend(p1Index);
	gObjMakePreviewCharSet(p1Index);
	GJSetCharacterInfo(lpObj, p1Index);
	GCItemListSend(p2Index);
	gObjMakePreviewCharSet(p2Index);
	GJSetCharacterInfo(targObj, p2Index);
	return true;
}

cDuel Duel;