#include "StdAfx.h"
RewardSystem rewarSys;
void RewardSystem::Load()
{
	this->NpcId = GetPrivateProfileIntA("RewardSystem", "NpcId", 234, NPCINI);
	this->NpcX = GetPrivateProfileIntA("RewardSystem", "NpcX", 234, NPCINI);
	this->NpcY = GetPrivateProfileIntA("RewardSystem", "NpcY", 234, NPCINI);
}

void RewardSystem::GetRewards(LPOBJ lpObj)
{
	rewardList.clear();

	char getNa[20], getAccount[11];
	SQL.ExecQuery("SELECT  [Acc], [ItemGrupo], [ItemId], [ItemLevel], [ItemDur], [ItemSkill], [ItemLuck], [ItemOpt], [ItemExc], [ItemQuantia], [ItemName] FROM RewardItemSystem WHERE Acc = '%s'",lpObj->AccountID);
	while (SQL.Fetch() != SQL_NO_DATA) {

		SQL.GetAsString("ItemName", getNa);
		SQL.GetAsString("Acc", getAccount);
		string name(getNa);
		string acc(getAccount);
		trimInPlace(acc);
		RewardItem item(acc, SQL.GetAsInteger("ItemGrupo"), SQL.GetAsInteger("ItemId"), SQL.GetAsInteger("ItemLevel"), SQL.GetAsInteger("ItemDur"), SQL.GetAsInteger("ItemSkill"), SQL.GetAsInteger("ItemLuck"), SQL.GetAsInteger("ItemOpt"), SQL.GetAsInteger("ItemExc"), SQL.GetAsInteger("ItemQuantia"), name);
		rewardList.push_back(item);
	}
	SQL.Close();
}

void RewardSystem::NpcClick(LPOBJ lpObj)
{
	this->GetRewards(lpObj);
	if (!rewardList.empty()) {
		for (auto it = rewardList.begin(); it != rewardList.end(); ) {

			if (it->Account == lpObj->AccountID) {

				int itemnum = ITEMGET(it->Grupo, it->Index);
				int check = InventoryFullCheck(lpObj->m_Index, itemnum);
				
				if (check > 0) {
					int entregues = min(check, it->Quantia);

					for (int q = 0; q < entregues; q++) {
						ItemSerialCreateSend(lpObj->m_Index, 236, lpObj->X, lpObj->Y,
							itemnum, it->Level, it->Dur, it->Skill, it->Luck, it->Opt,
							lpObj->m_Index, it->Exc);
					}

					it->Quantia -= entregues;

					if (it->Quantia > 0) {
						// Atualiza banco com a nova quantidade
						SQL.ExecQuery("UPDATE RewardItemSystem SET ItemQuantia = %d WHERE ItemName = '%s' AND Acc = '%s'",
							it->Quantia, it->Name.c_str(),lpObj->AccountID);
						SQL.Fetch();
						SQL.Close();
						++it; // mantém na lista
					}
					else {
						// Remove do banco e da lista
						SQL.ExecQuery("DELETE FROM RewardItemSystem WHERE ItemName = '%s' AND Acc = '%s'", it->Name.c_str(), lpObj->AccountID);
						SQL.Fetch();
						SQL.Close();
						it = rewardList.erase(it);
					}

					int pendentes = accumulate(
						rewardList.begin(), rewardList.end(), 0,
						[](int soma, const auto& r) { return soma + r.Quantia; }
					);

					if (pendentes > 0) {
						MsgOutput(lpObj->m_Index, "Você ainda tem %d recompensas.", pendentes);
					}
					else {
						MsgOutput(lpObj->m_Index, "Todas as recompensas foram entregues!");
					}
				}
				else {
					MsgOutput(lpObj->m_Index, "Sem espaço para receber recompensa");
					++it; // não coube nada, deixa para próxima vez
				}
			}
			else {
				
				++it;
			}
		}
	}
	else {
		MsgOutput(lpObj->m_Index, "Sem recompensa a receber");
	}
}
