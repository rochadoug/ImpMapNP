#include "StdAfx.h"

WarDevias warDevias;

WarDevias::WarDevias(){
	SwitchAreaX1 = 13;
	SwitchAreaX2 = 15;
	SwitchAreaY1 = 43;
	SwitchAreaY2 = 45;

	CastleAreaX[0] = 5;
	CastleAreaX[1] = 80;
	CastleAreaY[0] = 5;
	CastleAreaY[1] = 80;

	LoadConfig();
}

WarDevias::~WarDevias(){
	this->Reset(0);  // Zera Evento
}

/* Chamar no carregamento da dll */
void WarDevias::LoadConfig()
{
	int Token, Section = -1;
	SMDFile = fopen(WARDEVIASTXT, "r");
	if (SMDFile == false)
	{
		ExchangeMsgBox("Falha ao carregar arquivo: %s", WARDEVIASTXT);
		::ExitProcess(0);
	}
	this->Reset(0);
	Timers.clear();
	while (true)
	{
		Token = GetToken();

		if (Token == END){
			break;
		}
		if (Token == NUMBER){
			Section = TokenNumber;
		}

		while (true)
		{
			if (Section == 0){
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0){
					break;
				}

				this->isActive = TokenNumber;	Token = GetToken();
				this->Duration = TokenNumber;
			}
			if (Section == 1) {
				
				Token = GetToken();
				if (strcmpi("end", TokenString) == 0) {
					break;
				}
				BYTE temp1, temp2,temp3;
				temp1 = TokenNumber; Token = GetToken();
				temp2 = TokenNumber; Token = GetToken();
				temp3 = TokenNumber;
				
				if (temp3 < 5)
					temp3 = 5;
				WarDeviasTime  wardeviasTime(temp1, temp2, temp3);
				Timers.push_back(wardeviasTime);

				if (this->isActive > 0 && Timers.size() < 1) {
					ExchangeMsgBox("Obrigatório um tempo para o evento WarDevias");
					ExitProcess(0);
				}
			}
		}
	}
	fclose(SMDFile);

}

WarGuild* WarDevias::GetGuild(int guildId)
{
	auto it = this->Guilds.find(guildId);

	if (it == this->Guilds.end())
	{
		return nullptr;
	}

	return &it->second;
}

bool WarDevias::IsInSwitchArea(LPOBJ lpObj)
{
	return (
		lpObj->Map == 2 &&
		lpObj->X >= SwitchAreaX1 &&
		lpObj->X <= SwitchAreaX2 &&
		lpObj->Y >= SwitchAreaY1 &&
		lpObj->Y <= SwitchAreaY2
		);
}

bool WarDevias::IsInThroneArea(LPOBJ lpObj)
{
	return (
		lpObj->Map == 2 &&
		lpObj->X == 10 &&
		lpObj->Y == 26
		);
}

/*Zerar início e fim do evento*/
void WarDevias::Reset(BYTE Type)
{
	if (Type == 0) {
		this->State = WarState::Closed;

		this->CurrentOwner = 0;

		this->CaptureTime = 0;
		this->ScoreTime = 0;

		this->AdMin = 0;
		this->AdSec = 0;

		this->Guilds.clear();
	}
	if (Type == 1) {
		if (!this->Guilds.empty()) {
			for (auto& g : this->Guilds) {
				if (g.second.Switcher != -1) {
					if (gObj[g.second.Switcher].m_Change == 16) {
						gObj[g.second.Switcher].m_Change = -1;
						gObjViewportListProtocolCreate(&gObj[g.second.Switcher]);
					}
				}
			}
		}
	}
}

/* Rodar na função interfaceCheck ou Thread se quiser*/
void WarDevias::Manage(){
	if (this->isActive != 1)
	{
		AllServerAnnounce("[WarDevias] Parou no inativo!");
		return;
	}

	switch (this->State)
	{
	case WarState::Closed:
		UpdateClosed();
		break;

	case WarState::Register:
		UpdateRegister();
		break;

	case WarState::Running:
		UpdateRunning();
		break;
	}
}

/* Evento em andamento */
void WarDevias::UpdateRunning()
{
	if (this->AdSec < 60)
	{
		this->AdSec++;
	}
	else
	{
		this->AdSec = 0;
		this->AdMin++;
	}

	if (this->AdMin >= this->Duration)
	{
		WarGuild* winner = FindWinner();

		AllServerAnnounce("[WarDevias] Evento finalizado");

		if (winner != nullptr)
		{
			AllServerAnnounce("[WarDevias] Guild vencedora: %s com %d pontos", gObj[winner->GmId].GuildName, winner->Score);
			UpdateWinner(winner);
		}
		else {
			AllServerAnnounce("[WarDevias] Ninguém conseguiu pontuar!");
		}

		this->Reset(1);
		this->Reset(0);

		return;
	}

	CheckGuildState();
}

/* Evento não está em andamento */
void WarDevias::UpdateClosed()
{
	for (WarDeviasTime& t : this->Timers)
	{
		if (sTimer.mySemDay != t.SemanalDay)
		{
			continue;
		}

		if (sTimer.myHour != t.Hour)
		{
			continue;
		}

		int remainMin = t.Min - sTimer.myMin;

		// abre inscrição 5 minutos antes (Coloquei 3 para testar, mas é 5 minutos)
		if (remainMin == 3 && sTimer.mySec == 0)
		{
			this->Guilds.clear();

			this->State = WarState::Register;
			this->DeviasOwner = 0;
			strcpy(this->DeviasOwnerName, "-");

			SQL.ExecQuery("UPDATE Guild SET DeviasOwner = 0 WHERE DeviasOwner = 1");
			SQL.Close();

			AllServerAnnounce("[WarDevias] Inscrições abertas!");

			AllServerAnnounce("[WarDevias] Digite /wardevias para participar");

			return;
		}
	}
}

/* Evento aberto para guilds se cadastrarem (5 minutos para iniciar) */
void WarDevias::UpdateRegister()
{
	for (WarDeviasTime& t : this->Timers)
	{
		if (sTimer.mySemDay != t.SemanalDay)
		{
			continue;
		}

		if (sTimer.myHour != t.Hour)
		{
			continue;
		}

		int remainMin = t.Min - sTimer.myMin;

		// avisos antes do evento
		if (remainMin >= 1 && remainMin <= 4 && sTimer.mySec == 0)
		{
			AllServerAnnounce("[WarDevias] Começando em %d minuto(s)",remainMin);
		}

		// inicia evento
		if (remainMin == 0 && sTimer.mySec == 0) 
		{
			if (this->Guilds.size() < 2) { // Verifica se tem mais de uma guild pra iniciar o evento
				Reset(0);
				return;
			}
			this->State = WarState::Running;

			this->AdMin = 0;
			this->AdSec = 0;

			this->CaptureTime = 0;
			this->ScoreTime = 0;

			this->CurrentOwner = 0;

			AllServerAnnounce("[WarDevias] Inscrições encerradas");
			AllServerAnnounce("[WarDevias] O domínio de Devias começou!");

			return;
		}
	}
}

/* Definir guild vencedora e salvar na DB (Não esquecer de criar as colunas na tabela Guild
	Coluna: DeviasOwner int default 0; e Coluna: DeviasDate Datetime */
void WarDevias::UpdateWinner(WarGuild* winner)
{
	if (winner == nullptr)
	{
		return;
	}

	// remove dono antigo
	SQL.ExecQuery("UPDATE Guild SET DeviasOwner = 0 WHERE DeviasOwner = 1");
	SQL.Close();

	this->DeviasOwner = winner->GuildId;  //Coloca como  dominante na memória
	strcpy(this->DeviasOwnerName, winner->GuildName);
	// define novo dono
	SQL.ExecQuery("UPDATE Guild SET DeviasOwner = 1, DeviasDate = GetDate() WHERE Number = %d",winner->GuildId);
	SQL.Close();

	AllServerAnnounce("[WarDevias] %s dominante do Castelo de Davias!",winner->GuildName);
}

/* Comando /wardevias chama ela pra registrar a guild, se quiser pode chamar por NPC */
void WarDevias::RegisterGuild(LPOBJ lpObj)
{
	int aIndex = lpObj->m_Index;

	if (!this->isActive)
	{
		MsgOutput(aIndex, "War Devias desativado");
		return;
	}

	if (lpObj->GuildNumber <= 0)
	{
		MsgOutput(aIndex, "Você não está em uma Guild");
		return;
	}

	// Guild Master
	if (strcmp(lpObj->Name, lpObj->lpGuild->Names[0]) != 0)
	{
		MsgOutput(aIndex, "Você não é um Guild Master");
		return;
	}

	// período de cadastro
	if (this->State != WarState::Register)
	{
		MsgOutput(aIndex, "Impossível cadastrar agora");
		return;
	}

	// já cadastrada
	if (GetGuild(lpObj->GuildNumber) != nullptr)
	{
		MsgOutput(aIndex, "Sua guild já está cadastrada");
		return;
	}

	WarGuild guild(lpObj->GuildNumber,lpObj->GuildName, aIndex);

	for (int i = 0; i < lpObj->lpGuild->Count; i++)
	{
		int memberId = lpObj->lpGuild->Index[i];

		if (!OBJMAX_RANGE(memberId))
		{
			continue;
		}

		if (gObj[memberId].Connected != 3)
		{
			continue;
		}

		guild.Members.emplace(
			memberId,
			WarGuildPlayer(memberId)
		);

		if (memberId != aIndex)
		{
			MsgOutput(memberId,"[WarDevias] %s entrou no evento",lpObj->GuildName);
		}
	}

	this->Guilds.emplace(
		lpObj->GuildNumber,
		guild
	);

	MsgOutput(aIndex,"[WarDevias] %s cadastrada",lpObj->GuildName);
}

/* Gerencia as mudanças nas guilds durante o evento */
bool WarDevias::CheckGuildState()
{
	if (this->State != WarState::Running)
	{
		return false;
	}

	// procura guild tentando dominar
	for (auto&[id, guild] : this->Guilds)
	{
		if (!guild.CanCapture())
		{
			continue;
		}

		// ainda não existe dono
		if (this->CurrentOwner == 0)
		{
			this->CaptureTime++;

			if (this->CaptureTime == 1) {
				ClearCastleForCapture(guild.GuildId);
			}

			if ((this->CaptureTime % 5) == 0)
			{
				AllServerAnnounce("[WarDevias] %s dominando Devias (%d/30)",guild.GuildName,this->CaptureTime);
			}

			// dominou
			if (this->CaptureTime >= 30)
			{
				this->CurrentOwner = guild.GuildId;

				this->CaptureTime = 0;
				this->ScoreTime = 0;

				guild.IsDeviasOwner = true;

				AllServerAnnounce("[WarDevias] Guild %s dominou Devias!",guild.GuildName);
			}
		}

		// já existe dono
		else if (this->CurrentOwner == guild.GuildId)
		{
			this->ScoreTime++;

			// pontua a cada 5 segundos
			if (this->ScoreTime >= 5)
			{
				this->ScoreTime = 0;

				guild.Score++;

				AllServerAnnounce("[WarDevias] %s marcou %d ponto(s)",guild.GuildName,guild.Score);
			}
		}

		return true;
	}

	// nenhuma guild válida dominando
	if (this->CaptureTime > 0)
	{
		this->CaptureTime = 0;
	}

	if (this->CurrentOwner != 0)
	{
		WarGuild* owner = GetGuild(this->CurrentOwner);

		if (owner != nullptr)
		{
			owner->IsDeviasOwner = false;
		}

		this->CurrentOwner = 0;
		this->ScoreTime = 0;

		AllServerAnnounce(
			"[WarDevias] Devias ficou sem domínio"
		);
	}

	return false;
}

/* Calcula e captura guild vencedora ao final do evento (no tempo final do evento) */
WarGuild* WarDevias::FindWinner()
{
	WarGuild* winner = nullptr;
	if (!this->Guilds.empty()) {
		for (auto&[id, guild] : this->Guilds)
		{
			if (winner == nullptr || guild.Score > winner->Score)
			{
				winner = &guild;
			}
		}
	}
	return winner;
}

/* Gerencia entrar/sair da area do switch*/
void WarDevias::SetSwitch(int playerId, bool OnOff)
{
	if (this->State != WarState::Running)
	{
		return;
	}

	LPOBJ lpPlayer = &gObj[playerId];

	if (lpPlayer->Type != OBJECT_USER)
	{
		return;
	}

	WarGuild* guild = GetGuild(lpPlayer->GuildNumber);

	if (guild == nullptr)
	{
		return;
	}

	// checa se player participa do evento
	if (guild->Members.find(playerId) == guild->Members.end())
	{
		return;
	}

	// PEGAR SWITCH
	if (OnOff)
	{
		// já possui switch
		if (guild->HasSwitch())
		{
			return;
		}

		// outra guild já está usando
		for (auto&[id, g] : this->Guilds)
		{
			if (g.HasSwitch())
			{
				return;
			}
		}

		guild->Switcher = playerId;

		// visual
		if (lpPlayer->m_Change != 16)
		{
			lpPlayer->m_Change = 16;
			gObjViewportListProtocolCreate(lpPlayer);
		}

		AllServerAnnounce(
			"[WarDevias] %s obteve o Switch",
			lpPlayer->GuildName
		);

		return;
	}

	// PERDER SWITCH
	if (guild->Switcher == playerId)
	{
		// remove visual
		if (lpPlayer->m_Change == 16)
		{
			lpPlayer->m_Change = -1;
			gObjViewportListProtocolCreate(lpPlayer);
		}

		guild->Switcher = -1;

		// perde domínio atual
		if (this->CurrentOwner == guild->GuildId)
		{
			this->CurrentOwner = 0;

			this->CaptureTime = 0;
			this->ScoreTime = 0;

			guild->IsDeviasOwner = false;

			AllServerAnnounce(
				"[WarDevias] %s perdeu o domínio de Devias",
				lpPlayer->GuildName
			);
		}

		AllServerAnnounce(
			"[WarDevias] %s perdeu o Switch",
			lpPlayer->GuildName
		);
	}
}

/* Chamar na gObjInterfaceCheck gerencia quem está no switch */
void WarDevias::Switching(int playerId)
{
	LPOBJ lpPlayer = &gObj[playerId];

	if (lpPlayer->Type != OBJECT_USER)
	{
		return;
	}

	if (IsInSwitchArea(lpPlayer))
	{
		SetSwitch(playerId, true);
	}
	else
	{
		SetSwitch(playerId, false);
	}
}

/*Chamar na gObjInterfaceCheck gerencia quem está no trono */
void WarDevias::SetBench(int gmId)
{
	if (this->State != WarState::Running)
	{
		return;
	}

	LPOBJ lpPlayer = &gObj[gmId];

	WarGuild* guild = GetGuild(lpPlayer->GuildNumber);

	if (guild == nullptr)
	{
		return;
	}

	if (gmId != guild->GmId)
	{
		return;
	}

	if (IsInThroneArea(lpPlayer))
	{
		if (!guild->Bench)
		{
			guild->Bench = true;

			AllServerAnnounce(
				"[WarDevias] Guild %s assumiu o Trono!",
				lpPlayer->GuildName
			);
		}
	}
	else
	{
		if (guild->Bench)
		{
			guild->Bench = false;

			this->CurrentOwner = 0;
			this->CaptureTime = 0;

			AllServerAnnounce(
				"[WarDevias] Guild %s perdeu o Trono",
				lpPlayer->GuildName
			);
		}
	}
}

/* Função Helper */
bool WarDevias::IsInCastleArea(LPOBJ lpObj)
{
	return (
		lpObj->Map == 2 &&
		lpObj->X >= CastleAreaX[0] &&
		lpObj->X <= CastleAreaX[1] &&
		lpObj->Y >= CastleAreaY[0] &&
		lpObj->Y <= CastleAreaX[1]
		);
}

/* Função Helper */
bool WarDevias::IsEventMember(LPOBJ lpObj)
{
	WarGuild* guild = GetGuild(lpObj->GuildNumber);

	if (guild == nullptr)
	{
		return false;
	}

	return (
		guild->Members.find(lpObj->m_Index)
		!= guild->Members.end()
		);
}

/* Verifica se está na área do castelo tanto no evento, quanto para território da guild vencedora 
	Chamei ela na MoveProc porém pode ser na InterfaceCheck tbm */
void WarDevias::CheckCastleAccess(LPOBJ lpObj)
{
	if (!IsInCastleArea(lpObj))
	{
		return;
	}
	if (lpObj->Type != OBJTYPE_USER)
	{
		return;
	}

	// evento acontecendo
	if (this->State == WarState::Running)
	{
		if (!IsEventMember(lpObj))
		{
			gObjMoveGate(lpObj->m_Index, 22);

			MsgOutput(
				lpObj->m_Index,
				"[WarDevias] Área inacessível durante evento!"
			);
		}
	}
	else if (this->State == WarState::Closed)
	{
		if (this->DeviasOwner > 0)
		{
			if (lpObj->GuildNumber != this->DeviasOwner)
			{
				MsgOutput(lpObj->m_Index, "Território da %s", this->DeviasOwnerName);
				gObjMoveGate(lpObj->m_Index, 22);
			}
		}
	}
}

/* Função que fiz para remover as guilds ao final do evento, mas nem precisa pois a CheckCastleAccess faz o trabalho dela 
	então essa aqui está sem uso no momento */
void WarDevias::KickNonOwnerGuilds()
{
	if (this->CurrentOwner == 0)
	{
		return;
	}

	for (int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if (gObj[i].Type != OBJTYPE_USER)
		{
			continue;
		}

		if (gObj[i].Connected != 3)
		{
			continue;
		}

		if (!IsInCastleArea(&gObj[i]))
		{
			continue;
		}

		if (gObj[i].GuildNumber == this->CurrentOwner)
		{
			continue;
		}
		gObjMoveGate(i, 22);

	}
}

/* Carrega o estado da guild vencedora vindo do banco pra poder recompensar os membros, e tbm proteger o território do castelo
Essa aqui eu chamei na função de conexão do SQL, pois se chamar ao abrir o GS causa crash, pode chamar ela quando o player entrar no jogo se quiser, opcional*/
void WarDevias::GetGuildDBInfo()
{
	SQL.ExecQuery("SELECT G_Name, Number FROM Guild WHERE DeviasOwner = 1");
	SQL.Fetch();
	SQL.GetAsString("G_Name", this->DeviasOwnerName);
	this->DeviasOwner = SQL.GetAsInteger("Number");
	SQL.Close();

	//LogAddColor(2,"Carregou guild data");
}

/* Limpar castelo quando alguma guild pega trono e switch */
void WarDevias::ClearCastleForCapture(int guildId)
{
	BYTE range[2];
	range[0] = (50 + rand() % 10);
	range[1] = (15 + rand() % 20);

	for (int i = OBJECT_MIN ; i < OBJECT_MAX; i++)
	{
		if (gObj[i].Type != OBJTYPE_USER)
		{
			continue;
		}

		if (gObj[i].Connected != 3)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[i];

		if (!IsInCastleArea(lpObj))
		{
			continue;
		}

		// mantém guild atacante
		if (lpObj->GuildNumber == guildId)
		{
			continue;
		}

		gObjMoveGate(i, 86);
		//gObjTeleport(i,2, range[0], range[1]);
	}
}