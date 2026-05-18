#include "StdAfx.h"
MySQLDB mSqlite;

MySQLDB::MySQLDB() : db(nullptr){

}

MySQLDB::~MySQLDB(){
	Close();
}

bool MySQLDB::OpenDB(string database){
	
	int rc = sqlite3_open(database.c_str(), &db); // cria ou abre o banco Quest.db

	if (rc) {
		ExchangeMsgBox( "Erro ao abrir banco: %s" , sqlite3_errmsg(db));
		return false;
	}
		
	return true;
}

void MySQLDB::Close() {
	if (db) sqlite3_close(db);
	db = nullptr;
}

void MySQLDB::createTables() {

	this->OpenDB("QuestConfig.db");

	const char* sqlQuestInit =
		"CREATE TABLE IF NOT EXISTS Quest ("
		"QuestID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"Name TEXT,"
		"Description TEXT,"
		"Active INTEGER);"

		"CREATE TABLE IF NOT EXISTS QuestCondition ("
		"Id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"QuestId INTEGER,"
		"Description TEXT,"
		"ResetCount INTEGER,"
		"MonsterCount INTEGER,"
		"MonsterId INTEGER,"
		"MonsterMap INTEGER,"
		"ItemCount INTEGER,"
		"ItemGroup INTEGER,"
		"ItemId INTEGER,"
		"ItemLevel INTEGER,"
		"ItemMap INTEGER,"
		"FOREIGN KEY (QuestId) REFERENCES Quest(QuestID));"

		"CREATE TABLE IF NOT EXISTS QuestReward ("
		"RewardId INTEGER PRIMARY KEY AUTOINCREMENT,"
		"QuestId INTEGER,"
		"ItemGroup INTEGER,"
		"ItemIndex INTEGER,"
		"ItemLevel INTEGER,"
		"ItemSkill INTEGER,"
		"ItemLuck INTEGER,"
		"ItemOption INTEGER,"
		"ItemExc INTEGER,"
		"ItemQuantity INTEGER,"
		"ItemName TEXT,"
		"FOREIGN KEY (QuestId) REFERENCES Quest(QuestID));"

		"CREATE TABLE IF NOT EXISTS QuestRewardCoin ("
		"Id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"RewardId INTEGER,"
		"CoinType INTEGER,"
		"Amount INTEGER,"
		"Name TEXT,"
		"FOREIGN KEY (RewardId) REFERENCES QuestReward(RewardId));";


	char* errMsg = nullptr;
	if (sqlite3_exec(db, sqlQuestInit, nullptr, nullptr, &errMsg) != SQLITE_OK) {
		
		sqlite3_free(errMsg);
	}
}

void MySQLDB::InsertQuestData() {
	char* errMsg = nullptr;

	// 1. Inserir a quest
	const char* sqlQuest = "INSERT INTO Quest (Name, Description, Active) VALUES ('Caçador de iniciante', 'Mate Budge Dragons para receber sua recompensa.', 1);";

	if (sqlite3_exec(db, sqlQuest, nullptr, nullptr, &errMsg) != SQLITE_OK) {
		sqlite3_free(errMsg);
	}

	// 2. Inserir condição (ligada à quest 1)
	const char* sqlCondition = "INSERT INTO QuestCondition (QuestId, Description, ResetCount, MonsterCount, MonsterId, MonsterMap, ItemCount , ItemGroup, ItemId, ItemLevel, ItemMap) VALUES (1, 'Derrote 10 Budge Dragon em Lorencia', 0, 10, 2, 0, 0, 0, 0, 0, 0);";

	if (sqlite3_exec(db, sqlCondition, nullptr, nullptr, &errMsg) != SQLITE_OK) {
		sqlite3_free(errMsg);
	}

	// 3. Inserir recompensa principal (item)
	const char* sqlReward =
		"INSERT INTO QuestReward (QuestId, ItemGroup, ItemIndex, ItemLevel, ItemSkill, ItemLuck, ItemOption, ItemExc ItemQuantity, ItemName) "
		"VALUES (1, 14, 11, 5, 0, 0, 0, 0, 0, 'Silver Medal');";

	if (sqlite3_exec(db, sqlReward, nullptr, nullptr, &errMsg) != SQLITE_OK) {
		sqlite3_free(errMsg);
	}

	// 4. Inserir moedas ligadas à recompensa
	const char* sqlCoin1 =
		"INSERT INTO QuestRewardCoin (RewardId, CoinType, Amount, Name) "
		"VALUES (1, 0, 1, 'Cash');";

	if (sqlite3_exec(db, sqlCoin1, nullptr, nullptr, &errMsg) != SQLITE_OK) {
		sqlite3_free(errMsg);
	}

	const char* sqlCoin2 =
		"INSERT INTO QuestRewardCoin (RewardId, CoinType, Amount, Name) "
		"VALUES (2, 1, 1, 'Gold');";

	if (sqlite3_exec(db, sqlCoin2, nullptr, nullptr, &errMsg) != SQLITE_OK) {
		sqlite3_free(errMsg);
	}
}
