#pragma once

struct RewardItem {
	
	int	Grupo, Index, Quantia;
	BYTE  Level, Dur, Skill, Luck, Opt, Exc;

	string Name;
	string Account;
	RewardItem() {};
	RewardItem(string acc, int gp, int id, BYTE lv, BYTE dr, BYTE sk, BYTE lk, BYTE op, BYTE ex, int qt, string nome)
		: Account(acc),Grupo(gp), Index(id), Level(lv), Dur(dr), Skill(sk), Luck(lk), Opt(op), Exc(ex),Quantia(qt),Name(nome)  {}
};

class RewardSystem {
public:
	vector<RewardItem> rewardList;
	int NpcId, NpcX, NpcY;


	void Load();
	void GetRewards(LPOBJ lpObj);
	void NpcClick(LPOBJ lpObj);
};

extern RewardSystem rewarSys;