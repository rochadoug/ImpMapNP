#pragma once

enum class WarState : BYTE
{
	Closed = 0,
	Register,
	Running
};

struct WarGuildPlayer
{
	int Id;
	BYTE Switch;
	int KillCount;

	WarGuildPlayer()
		: Id(-1), Switch(0), KillCount(0)
	{
	}

	WarGuildPlayer(int id)
		: Id(id), Switch(0), KillCount(0)
	{
	}
};

struct WarGuild
{
	int GuildId;
	int GmId;
	char GuildName[11];

	int Switcher;
	bool Bench;

	int Score;
	bool IsDeviasOwner;

	map<int, WarGuildPlayer> Members;

	WarGuild(int id)
		: GuildId(id),
		GmId(-1),
		Switcher(-1),
		Bench(false),
		Score(0),
		IsDeviasOwner(false)
	{
		strcpy(this->GuildName, "");
	}

	WarGuild(int id, const char* name, int gmId)
		: GuildId(id),
		GmId(gmId),
		Switcher(-1),
		Bench(false),
		Score(0),
		IsDeviasOwner(false)
	{
		strcpy(this->GuildName, name);
	}

	bool HasSwitch() const
	{
		return (Switcher != -1);
	}

	bool IsOnThrone() const
	{
		return Bench;
	}

	bool CanCapture() const
	{
		return HasSwitch() && IsOnThrone();
	}
};

struct WarDeviasTime
{
	BYTE SemanalDay;
	BYTE Min;
	BYTE Hour;

	WarDeviasTime()
	{
	}

	WarDeviasTime(BYTE sd, BYTE h, BYTE m)
		: SemanalDay(sd), Hour(h), Min(m)
	{
	}
};

class WarDevias
{
public:

	WarDevias();
	~WarDevias();

	BYTE isActive;

	WarState State;

	BYTE RegisterNpc;

	BYTE SwitchAreaX1;
	BYTE SwitchAreaX2;
	BYTE SwitchAreaY1;
	BYTE SwitchAreaY2;

	UINT Duration;

	UINT AdTime;
	UINT AdSec;
	UINT AdMin;

	UINT CaptureTime;
	UINT ScoreTime;

	int CurrentOwner;

	vector<WarDeviasTime> Timers;
	map<int, WarGuild> Guilds;

	BYTE CastleAreaX[2];
	BYTE CastleAreaY[2];

	int DeviasOwner;
	char DeviasOwnerName[11];

public:

	void LoadConfig();
	void Manage();

	void UpdateClosed();
	void UpdateRegister();
	void UpdateRunning();
	void UpdateWinner(WarGuild* winner);
	

	void SetSwitch(int playerId, bool OnOff);
	void SetBench(int gmId);

	void Switching(int playerId);

	void RegisterGuild(LPOBJ lpObj);

	bool CheckGuildState();

	void Reset(BYTE Type);

	bool IsInSwitchArea(LPOBJ lpObj);
	bool IsInThroneArea(LPOBJ lpObj);
	bool IsInCastleArea(LPOBJ lpObj);
	bool IsEventMember(LPOBJ lpObj);
	void CheckCastleAccess(LPOBJ lpObj);
	void ClearCastleForCapture(int guildId);
	void KickNonOwnerGuilds();

	void GetGuildDBInfo();

	WarGuild* GetGuild(int guildId);

	WarGuild* FindWinner();
};


extern WarDevias warDevias;