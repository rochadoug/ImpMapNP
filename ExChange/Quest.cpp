#include "StdAfx.h"
vector<Quest> Quests;
 Quest mQuest;

Quest::Quest()
{
}

Quest::~Quest()
{
}

void Quest::Load() {
	sqlite3* db;
	sqlite3_open("QuestConfig.db", &db);

	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, "SELECT QuestID, Name, Description, Active FROM Quest", -1, &stmt, nullptr);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		Quest q;
		q.QuestID = sqlite3_column_int(stmt, 0);
		q.Name = (const char*)sqlite3_column_text(stmt, 1);
		q.Description = (const char*)sqlite3_column_text(stmt, 2);
		q.Active = sqlite3_column_int(stmt, 3);

		// Carregar condições
		sqlite3_stmt* condStmt;
		sqlite3_prepare_v2(db, "SELECT Id, Description, MonsterCount, MonsterId, MonsterMap FROM QuestCondition WHERE QuestId=?", -1, &condStmt, nullptr);
		sqlite3_bind_int(condStmt, 1, q.QuestID);
		while (sqlite3_step(condStmt) == SQLITE_ROW) {
			QuestCondition cond;
			cond.Id = sqlite3_column_int(condStmt, 0);
			cond.QuestId = q.QuestID;
			cond.Description = (const char*)sqlite3_column_text(condStmt, 1);
			cond.MonsterCount = sqlite3_column_int(condStmt, 2);
			cond.MonsterId = sqlite3_column_int(condStmt, 3);
			cond.MonsterMap = sqlite3_column_int(condStmt, 4);
			q.Conditions.push_back(cond);
		}
		sqlite3_finalize(condStmt);

		// Carregar recompensas
		sqlite3_stmt* rewStmt;
		sqlite3_prepare_v2(db, "SELECT RewardId, ItemGroup, ItemIndex, ItemLevel, ItemQuantity, ItemName FROM QuestReward WHERE QuestId=?", -1, &rewStmt, nullptr);
		sqlite3_bind_int(rewStmt, 1, q.QuestID);
		while (sqlite3_step(rewStmt) == SQLITE_ROW) {
			QuestReward reward;
			reward.RewardId = sqlite3_column_int(rewStmt, 0);
			reward.QuestId = q.QuestID;
			reward.ItemGroup = sqlite3_column_int(rewStmt, 1);
			reward.ItemIndex = sqlite3_column_int(rewStmt, 2);
			reward.ItemLevel = sqlite3_column_int(rewStmt, 3);
			reward.ItemQuantity = sqlite3_column_int(rewStmt, 4);
			reward.ItemName = (const char*)sqlite3_column_text(rewStmt, 5);

			// Carregar moedas ligadas à recompensa
			sqlite3_stmt* coinStmt;
			sqlite3_prepare_v2(db, "SELECT Id, Amount, Name FROM QuestRewardCoin WHERE RewardId=?", -1, &coinStmt, nullptr);
			sqlite3_bind_int(coinStmt, 1, reward.RewardId);
			while (sqlite3_step(coinStmt) == SQLITE_ROW) {
				QuestRewardCoin coin;
				coin.Id = sqlite3_column_int(coinStmt, 0);
				coin.RewardId = reward.RewardId;
				coin.Amount = sqlite3_column_int(coinStmt, 1);
				coin.Name = (const char*)sqlite3_column_text(coinStmt, 2);
				reward.Coins.push_back(coin);
			}
			sqlite3_finalize(coinStmt);

			q.Rewards.push_back(reward);
		}
		sqlite3_finalize(rewStmt);

		Quests.push_back(q);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


