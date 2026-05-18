#include "StdAfx.h"

x1System x1[100];

x1System::x1System()
{
	this->Active = 1;
}

x1System::~x1System()
{
}

void x1System::X1Reset()
{
	for (int i = 4800; i < 5600; i++){

		if (gObj[i].Type == OBJECT_USER && gObj[i].Connected == 3) {
			player[i].pStatus = 0;
			player[i].pWinLose = 0;
			player[i].pCoinQ = 0;
			player[i].pCoinT = 0;
			player[i].pPoints = 0;
			player[i].pTarget = -1;
			player[i].pTimeInvite = 0;
			player[i].pTimeRound = 0;
		}
	}
}

void x1System::PlayerReset(LPOBJ lpObj)
{
	if (lpObj->Type == OBJECT_USER && lpObj->Connected == 3) {
		player[lpObj->m_Index].pStatus = 0;
		player[lpObj->m_Index].pWinLose = 0;
		player[lpObj->m_Index].pCoinQ = 0;
		player[lpObj->m_Index].pCoinT = 0;
		player[lpObj->m_Index].pPoints = 0;
		player[lpObj->m_Index].pTarget = -1;
		player[lpObj->m_Index].pTimeInvite = 0;
		player[lpObj->m_Index].pTimeRound = 0;
	}
}

void x1System::X1ComandRequest(LPOBJ lpObj, char *msg)
{
	char oponente[11], moedaTipo[5], quantia[10];
	oponente[0] = moedaTipo[0] = quantia[0] = '\0';

	if (this->Active != 0) {
		int id = lpObj->m_Index;
		//MsgOutput(id, "Feche as janelas para isso");
		if (msg[0] == '\0' || strlen(msg) < 5)
			return;
		
		if (lpObj->m_IfState.use != 0) {
			MsgOutput(id, "Feche as janelas para isso");
			return;
		}

		if (pObj[id].mResets > this->rReset) {
			
			if (lpObj->Level > this->rLevel) {
				
				this->PlayerReset(lpObj); //zera tudo

				sscanf(msg, "%s %s %s", &oponente, &moedaTipo, &quantia);
				
				this->player[id].pTarget = GetPlayerIndex(oponente);
				if (this->player[id].pTarget == id){
					return;
				}
				if (this->player[id].pTarget == -1) {
					MsgOutput(id, "%s não encontrado", oponente);
					return;
				}
				if (this->player[id].pTarget >= 0 && gObj[this->player[id].pTarget].Connected < 3) {
					MsgOutput(id, "%s está offline", oponente);
					return;
				}
				
				int opId = -1;
				if (this->player[id].pTarget != -1) {
					opId = this->player[id].pTarget;
				}
				
				if (!strcmpi(moedaTipo, "Gold")) {
					this->player[id].pCoinT = 1;   //
					this->player[opId].pCoinT = 1;   //
				}
				else if (!strcmpi(moedaTipo, "Zen")) {
					this->player[id].pCoinT = 2;  //
					this->player[opId].pCoinT = 2;  //
				}

				if (this->player[id].pCoinT == 1) {
					if (!strcmpi(quantia, "1")) {
						this->player[id].pCoinQ = 1;
					}
					else if (!strcmpi(quantia, "5")) {
						this->player[id].pCoinQ = 5;
					}
					else if (!strcmpi(quantia, "10")) {
						this->player[id].pCoinQ = 10;
					}
					else {
						MsgOutput(id, "Quantia deve ser 1 ou 5 ou 10");
						return;
					}

					if (pObj[id].mCoin[1] < this->player[id].pCoinQ) {
						MsgOutput(id, "Você não tem Gold para apostar");
						return;
					}

				}
				else if (this->player[id].pCoinT == 2) {
					if (!strcmpi(quantia, "10kk")) {
						this->player[id].pCoinQ = 10 * 1000000;
					}
					else if (!strcmpi(quantia, "50kk")) {
						this->player[id].pCoinQ = 50 * 1000000;
					}
					else if (!strcmpi(quantia, "100kk")) {
						this->player[id].pCoinQ = 100 * 1000000;
					}
					else {
						MsgOutput(id, "Quantia deve ser 10kk, 50kk ou 100kk");
						return;
					}

					if (lpObj->Money < this->player[id].pCoinQ) {
						MsgOutput(id, "Você não tem Zen para apostar");
						return;
					}
				}
				else {
					MsgOutput(id, "Tipo da aposta deve ser [Zen] ou [Gold]");
					return;
				}


				this->player[id].pStatus = 1;  //convidando
				this->player[id].pTimeInvite = 15;


				if (this->player[opId].pStatus != 0) {
					MsgOutput(id, "%s já está ocupado", oponente);
					this->PlayerReset(lpObj);
					this->PlayerReset(&gObj[opId]);
					return;
				}
				else {
					this->PlayerReset(&gObj[opId]); //zera tudo

					this->player[opId].pStatus = 2; //convidado
					this->player[opId].pTarget = id;
					this->player[opId].pTimeInvite = 15;
					
					MsgOutput(id, "[Convite de x1 enviado %d]",opId);
					MsgOutput(id, "Esperando %s Responder", oponente);
					MsgOutput(opId, "[Convite de x1 recebido]");
					MsgOutput(opId, "%s te chamou para x1", lpObj->Name);
						
					lpObj->m_IfState.use = 1;
					lpObj->m_IfState.state = 0;
					lpObj->m_IfState.type = 1;
					lpObj->TargetNumber = opId;
					gObj[opId].m_IfState.use = 1;
					gObj[opId].m_IfState.state = 0;
					gObj[opId].m_IfState.type = 1;
					gObj[opId].TargetNumber = id;
					lpObj->m_InterfaceTime = GetTickCount();
					gObj[opId].m_InterfaceTime = GetTickCount();

					PMSG_TRADE_REQUESTSEND pMsg;

					pMsg.h.c = 0xC3;
					pMsg.h.headcode = 0x36;
					pMsg.h.size = sizeof(pMsg);
					memcpy(pMsg.szId, gObj[id].Name, sizeof(pMsg.szId));

					DataSend(opId, (LPBYTE)&pMsg, pMsg.h.size);
				}

			}
		}
	}
}

void x1System::X1ComandReponse(LPOBJ lpObj, char * msg)
{
	
}

void x1System::X1Manager()
{
}


