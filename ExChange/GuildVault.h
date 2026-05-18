#pragma once

struct VaultGuild {
	bool UsingVault; // verficar se tem alguém com bau aberto
	int VaultNumber; //  numero do bau para o sistema do /bau
	short CloseTime[1000];

	VaultGuild() : UsingVault(false), VaultNumber(0) {};
};

class  GuildVault{
	
public:
	GuildVault();
	~GuildVault();
	
	map<int,VaultGuild> Vaults;

	bool UpdateDBOpenVault(LPOBJ lpObj);
	void UpdateDBCloseVault(LPOBJ lpObj);
	void SaveGuildVault(LPOBJ lpObj);
	void ManageTime(LPOBJ lpObj);
	bool OpenTimeCheck(LPOBJ lpObj);
};

extern GuildVault guildVault;