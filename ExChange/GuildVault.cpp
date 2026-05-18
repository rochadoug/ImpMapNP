#include "StdAfx.h"

GuildVault guildVault;

GuildVault::GuildVault() {

}

GuildVault::~GuildVault() {

 }

// Abre o bau da guild e salva o atual do player pra usar depois
bool GuildVault::UpdateDBOpenVault(LPOBJ lpObj)
{
	try
	{
		if (lpObj->GuildNumber != 0) {
			if (this->Vaults.at(lpObj->GuildNumber).UsingVault == false && this->OpenTimeCheck(lpObj) == true) {
				this->Vaults.at(lpObj->GuildNumber).UsingVault = true;
				this->Vaults.at(lpObj->GuildNumber).VaultNumber = lpObj->m_Index; //Um valor para bau que não é usado no comando /bau
				MsgOutput(lpObj->m_Index, "UpdateOpen id %d / count %d", lpObj->lpGuild->Index[0], lpObj->lpGuild->Count);
				SQL.ExecQuery("UPDATE Warehouse set Items%02d = Items where AccountId = '%s'", pObj[lpObj->m_Index].mCurrentBau, lpObj->AccountID); //1-atual
				SQL.ExecQuery("UPDATE W SET W.Items = G.G_Warehouse FROM Warehouse W INNER JOIN Guild G ON G.G_Name = '%s' WHERE W.AccountID = '%s'", lpObj->GuildName, lpObj->AccountID);
				//SQL.ExecQuery("UPDATE Warehouse set VaultID = '%d' where AccountId = '%s'", this->Vaults.at(lpObj->GuildNumber).VaultNumber, lpObj->AccountID);

				SQL.Close();
				return true;
			}
			else {
				MsgOutput(lpObj->m_Index, "Bau já está em uso, aguarde");
				return false;
			}
		}
	}
	catch (const std::exception&)
	{

	}
	
	return false;
}

// Quando abrir o bau normal tira o da guild e poe o atual do player
void GuildVault::UpdateDBCloseVault(LPOBJ lpObj)
{
	try
	{
		if (lpObj->GuildNumber != 0) {
			if (this->Vaults.at(lpObj->GuildNumber).UsingVault == true) {
				this->Vaults.at(lpObj->GuildNumber).UsingVault = false;
				this->Vaults.at(lpObj->GuildNumber).VaultNumber = 0;

				// Opção 2
				SQL.ExecQuery("UPDATE Guild SET G_Warehouse = (SELECT Items FROM Warehouse WHERE AccountId = '%s') WHERE G_Name = '%s'", lpObj->AccountID, lpObj->GuildName);
				SQL.ExecQuery("UPDATE Warehouse set Items = Items%02d where AccountId = '%s'", pObj[lpObj->m_Index].mCurrentBau, lpObj->AccountID);
				SQL.ExecQuery("UPDATE Warehouse set VaultID = '%d' where AccountId = '%s'", pObj[lpObj->m_Index].mCurrentBau, lpObj->AccountID);
				SQL.Close();
			}
		}
	}
	catch (const std::exception&)
	{

	}
	
}

void GuildVault::SaveGuildVault(LPOBJ lpObj)
{
	try
	{
		if (lpObj->GuildNumber != 0) {
			if (this->Vaults.at(lpObj->GuildNumber).UsingVault == true) {
				this->Vaults.at(lpObj->GuildNumber).CloseTime[lpObj->m_Index] = 3; // 3 Segundos para o update d DB
			}
		}
	}
	catch (const std::exception&)
	{

	}
	
}

void GuildVault::ManageTime(LPOBJ lpObj)
{
	try
	{
		if (this->Vaults.at(lpObj->GuildNumber).CloseTime[lpObj->m_Index] > 0) {
			this->Vaults.at(lpObj->GuildNumber).CloseTime[lpObj->m_Index]--;

			//MsgOutput(lpObj->m_Index, "Tempo do bau %d", this->Vaults.at(lpObj->GuildNumber).CloseTime[lpObj->m_Index]);
			if (this->Vaults.at(lpObj->GuildNumber).CloseTime[lpObj->m_Index] == 1 && this->Vaults.at(lpObj->GuildNumber).UsingVault == true) {
				this->UpdateDBCloseVault(lpObj);
				//this->Vaults.at(lpObj->GuildNumber).CloseTime[lpObj->m_Index] = 0;
			}
		}
	}
	catch (const std::exception&)
	{

	}
	
}

bool GuildVault::OpenTimeCheck(LPOBJ lpObj)
{
	bool oReturn = true;
	for (int i = 0; i < 35; i++) {
		if (Vaults.at(lpObj->GuildNumber).CloseTime[lpObj->lpGuild->Index[i]] != 0) {
			oReturn = false;
			break;
		}
		
	}
	return oReturn;
}
