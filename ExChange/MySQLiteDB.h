#pragma once

class MySQLDB {
public:
	MySQLDB();
	~MySQLDB();
	bool OpenDB(string db);
	void Close();

	void createTables();
	void loadQuest(Quest& quest, int questId);
	void insertQuest(const Quest& quest);
	void InsertQuestData();

	 string  dbName; // exemplo: NPC ID 200
private:
	sqlite3* db;
};

extern MySQLDB mSqlite;
