#pragma once

struct QuestPlayer {
	int Id;
	int QuestId;
	int KillCount;
	BYTE Status;
};

struct QuestCondition {
	int Id;
	int QuestId;
	std::string Description;

	int ResetCount;

	int MonsterCount;
	int MonsterId;
	short MonsterMap;

	int ItemCount;
	short ItemConfig[3]; //Grupo, Id e Level
	short ItemMap;
};

struct QuestRewardCoin {
	int Id;
	int RewardId;
	BYTE CoinType;
	short Amount;
	string Name;
};
struct QuestReward {
	int  RewardId;
	int QuestId;

	vector<QuestRewardCoin> Coins;
	

	BYTE ItemGroup;
	BYTE ItemIndex;
	BYTE ItemLevel;
	BYTE ItemSkill;
	BYTE ItemLuck;
	BYTE ItemOption;
	BYTE ItemExc;
	short ItemQuantity;
	std::string ItemName;
};


class Quest {
public:
	Quest();
	~Quest();

	int QuestID;
	std::string Name;
	std::string Description;

	vector<QuestPlayer> Players;
	vector<QuestCondition> Conditions;
	vector<QuestReward> Rewards;

	BYTE Active;
	static const BYTE NpcClass = 249; // exemplo: NPC ID 200

	void Load();
};

extern vector<Quest> Quests;
extern Quest mQuest;
