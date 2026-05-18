#pragma once

#define MAX_CASHSHOP 20 //20 cashshop pra cada moeda, inicia a partir do shop 15 pra livrar os originais. 75-15=60

//Packet para mudar shop pelo teclado
struct PMSG_ShopChange
{
public:
	PBMSG_HEAD h;
	BYTE UpDown;
};

#pragma pack(push, 1)
struct PMSG_CashShopSend
{
	PWMSG_HEAD h;
	BYTE count;
};

struct CashShopItem
{
	BYTE slot;
	BYTE TypeCoin;
	int ValueCoin;

	int ItemId;
	int Level;
	int Option;
	int Excellent;
	bool Selected, CanBuy;
};

#pragma pack(pop)

class CashShop {

public:
	CashShop();
	virtual ~CashShop();
	void Init();

	BOOL LoadCashShop(char* Filename, int CType);
	BOOL InsertItemCashShop(int Type, int Index, int Level, int Dur, int Opt1, int Opt2, int Opt3, int ExcOpt, int BuyValue, int BuyType);
	int InventoryMapCheck(int iX, int iY, int Width, int Height);
	void OpenShop(LPOBJ lpObj, int type);
	void ChangeShop(LPOBJ lpObj, BYTE UpDown);
	BYTE gObjShopBuyInventoryInsertItemEx(int aIndex, CItem item);
	void SaveBuyLog(int aIndex, CItem item);

	int Type;
	int Index;
	int Level;
	int Durability;
	int Option[4];
	int CoinValue;
	BYTE ShopInventoryMap[120];
	int ItemCount;
	CItem Item[120];
	CashShopItem cashshop[120];
	BYTE SendItemData[(120 * 5)];
	int SendItemDataLen;
	int ShopCont;
	int Shop_MaxConfig[3];
	int ShopClic;
	int ShopNum;

	int max;

	enum opt { Skill, Luck, Jol, Exc };

};

extern CashShop cash_shop[MAX_CASHSHOP];
extern vector<CashShop> Shopping[MAX_COINS];