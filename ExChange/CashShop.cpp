#include "StdAfx.h"
CashShop cash_shop[MAX_CASHSHOP];
vector<CashShop> Shopping[MAX_COINS];

CashShop::CashShop()
{
}

CashShop::~CashShop()
{
}

void CashShop::Init()
{
	this->SendItemDataLen = 0;
	this->ItemCount = 0;
	memset(this->ShopInventoryMap, 0, sizeof(this->ShopInventoryMap));
}

BOOL CashShop::LoadCashShop(char* Filename, int CoinType) {
	this->Init();

	int Token;
	SMDFile = fopen(Filename, "r");
	if (SMDFile == false)
	{
		ExchangeMsgBox("[Shop System] Falha ao carregar arquivo: %s", Filename);
		::ExitProcess(0);
	}

	while (true)
	{
		Token = GetToken();

		if (Token == END)
		{
			break;
		}
		if (Token == NUMBER)
		{
			Type = TokenNumber; Token = GetToken();
			Index = TokenNumber; Token = GetToken();
			Level = TokenNumber; Token = GetToken();
			Durability = TokenNumber; Token = GetToken();
			Option[Skill] = TokenNumber; Token = GetToken();
			Option[Luck] = TokenNumber; Token = GetToken();
			Option[Jol] = TokenNumber; Token = GetToken();
			Option[Exc] = TokenNumber; Token = GetToken();
			CoinValue = TokenNumber;


			if (this->InsertItemCashShop(Type, Index, Level, Durability, Option[Skill], Option[Luck], Option[Jol], Option[Exc], CoinValue, CoinType) == false)
			{
				ExchangeMsgBox("Falha ao inserir item no cashshop!",Filename);
				::ExitProcess(0);
			}
		}
	}

	fclose(SMDFile);

	return TRUE;
}

BOOL CashShop::InsertItemCashShop(int Type, int Index, int Level, int Dur, int Opt1, int Opt2, int Opt3, int ExcOpt, int BuyValue, int BuyType)
{
	int Temp = ItemGetNumberMake(Type, Index);

	if (Temp < 0)
	{
		MessageBox(0, "Erro1 no CashShop", "Fire Plugin - Erro", MB_OK);
		return FALSE;
	}

	int Width = 0, Height = 0;

	ItemGetSize(Temp, Width, Height);

	if ((Width < 0) || (Height < 0))
	{
		MessageBox(0, "Erro2 no CashShop", "Fire Plugin - Erro", MB_OK);
		return FALSE;
	}

	int x, y, Blank = (-1);
	for (y = 0; y < 15; y++)
	{
		for (x = 0; x < 8; x++)
		{
			if (this->ShopInventoryMap[(x + (y * 8))] == 0)
			{
				Blank = this->InventoryMapCheck(x, y, Width, Height);

				if (Blank >= 0)
				{
					this->Item[Blank].m_Level = Level;

					if (Dur == 0)
					{
						Dur = ItemGetDurability(ITEMGET(Type, Index), Level, (ExcOpt != 0) ? 1 : 0);
					}

					this->Item[Blank].m_Durability = (float)(Dur);
					this->Item[Blank].Convert(Temp, Opt1, Opt2, Opt3, ExcOpt, 1);

					this->Item[Blank].Value();
					this->Item[Blank].m_BuyMoney = BuyValue;
					this->Item[Blank].m_OldBuyMoney = BuyValue;

					this->cashshop[Blank].ValueCoin = BuyValue;
					this->cashshop[Blank].TypeCoin = BuyType;
					this->cashshop[Blank].ItemId = ITEMGET(Type, Index);
					this->cashshop[Blank].Level = Level;
					this->cashshop[Blank].Option = Opt3;
					this->cashshop[Blank].Excellent = ExcOpt;

					this->SendItemData[this->SendItemDataLen] = Blank;
					this->SendItemDataLen++;

					ItemByteConvert((LPBYTE)(&this->SendItemData[this->SendItemDataLen]), this->Item[Blank]);

					this->SendItemDataLen += 4;
					this->ItemCount++;

					return TRUE;
				}
			}
		}
	}
}

int CashShop::InventoryMapCheck(int iX, int iY, int Width, int Height)
{
	if (((iX + Width) > 8) || ((iY + Height) > 15))
	{
	}
	else
	{
		int Y;
		int X;
		int Blank = 0;

		for (Y = 0; Y < Height; Y++)
		{
			for (X = 0; X < Width; X++)
			{
				if (this->ShopInventoryMap[(((Y + iY) * 8) + (X + iX))])
				{
					Blank++;
					break;
				}
			}
		}

		if (Blank == 0)
		{
			for (Y = 0; Y < Height; Y++)
			{
				for (X = 0; X < Width; X++)
				{
					this->ShopInventoryMap[(((Y + iY) * 8) + (X + iX))] = 1;
				}
			}
			return (iX + (iY * 8));
		}
	}
	return (-1);
}

void CashShop::OpenShop(LPOBJ lpObj, int type)
{
	int aIndex = lpObj->m_Index;
	Coin->GetPlayerCoin(aIndex);
	
	PMSG_TALKRESULT pResult;
	//PMSG_TALKREQUEST* lpMsg;
	PMSG_SHOPITEMCOUNT	pShopItemCount;

	if (gObj[aIndex].Connected != 3)
	{
		return;
	}
	if (gObj[aIndex].CloseType != -1)
	{
		return;
	}
	if (lpObj->m_IfState.use == 1)
	{
		return;
	}
	if (gmEvt.eUser[aIndex].WantGo == true)
	{
		MsgOutput(aIndex, "Não abra shop ao esperar o evento");
		return;
	}

	const int coinPlayer = type == 0 ? 5 : type == 1 ? 3 : type == 2 ? 6 : 0;
	LPSTR coinName = type == 0 ? "Cash" : type == 1 ? "Gold" : type == 2 ? "Point" : "OtherCoin";
	
	if (type == 0)
	{
		ShopNum = 0;
		//MsgOutput(aIndex, "[SHOP CASH] Você possui %d %s", Custom[aIndex].Coin[coinPlayer], coinName);
	}
	else if (type == 1)
	{
		ShopNum = this->max;
		//Func.MsgOutput(aIndex, 1, "[SHOP GOLD] Você possui %d %s", Custom[aIndex].Coin[coinPlayer], coinName);
	}
	else if (type == 2)
	{
		ShopNum = this->max*2;
		//Func.MsgOutput(aIndex, 1, "[SHOP POINT] Você possui %d %s", Custom[aIndex].Coin[coinPlayer], coinName);
	}

	lpObj->ShopNumber = ShopNum;
	lpObj->TargetShopNumber = ShopNum;
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 3;
	lpObj->m_ShopTime = 0;
	pObj[aIndex].CashShopOppend = true;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0;
	pResult.ShopType = (BYTE)(type + 2);

	DataSend(aIndex, (LPBYTE)(&pResult), (DWORD)(pResult.h.size));

	int lOfs = 0;
	BYTE SendByte[1024];
	lOfs += sizeof(pShopItemCount);
	int size = lOfs + cash_shop[ShopNum].SendItemDataLen;

	pShopItemCount.h.c = 0xC2;
	pShopItemCount.h.headcode = 0x31;
	pShopItemCount.h.sizeH = HIBYTE(size);
	pShopItemCount.h.sizeL = LOBYTE(size);
	pShopItemCount.Type = 0;
	pShopItemCount.count = cash_shop[ShopNum].ItemCount;
	memcpy(SendByte, &pShopItemCount, sizeof(pShopItemCount));
	memcpy(SendByte + lOfs, cash_shop[ShopNum].SendItemData, cash_shop[ShopNum].SendItemDataLen);
	DataSend(aIndex, (LPBYTE)SendByte, size);
	MsgOutput(aIndex, "[%s Shop 1] Você possui %d %s", Coin[type].Name, pObj[aIndex].mCoin[type], Coin[type].Name);
	

	//Enviando meus valores
	BYTE send[1024];
	PMSG_CashShopSend* p = (PMSG_CashShopSend*)send;

	p->h.c = 0xC2;
	p->h.headcode = 0xFC;

	int offset = sizeof(PMSG_CashShopSend);
	int dataOfs = 0;

	int count = cash_shop[ShopNum].ItemCount;
	p->count = count;

	for (int i = 0; i < count; i++)
	{
		BYTE slot = cash_shop[ShopNum].SendItemData[dataOfs];
		dataOfs += 1 + 4; // pula slot + item (igual GS faz)

		CashShopItem* item = (CashShopItem*)(send + offset);
		memset(item, 0, sizeof(CashShopItem));

		item->slot = slot;
		item->ValueCoin = cash_shop[ShopNum].cashshop[slot].ValueCoin;
		item->TypeCoin = cash_shop[ShopNum].cashshop[slot].TypeCoin;
		item->ItemId = cash_shop[ShopNum].cashshop[slot].ItemId;
		item->Level = cash_shop[ShopNum].cashshop[slot].Level;
		item->Option = cash_shop[ShopNum].cashshop[slot].Option;
		item->Excellent = cash_shop[ShopNum].cashshop[slot].Excellent;

		offset += sizeof(CashShopItem);
	}

	p->h.sizeH = HIBYTE(offset);
	p->h.sizeL = LOBYTE(offset);

	DataSend(aIndex, send, offset);
}

void CashShop::ChangeShop(LPOBJ lpObj, BYTE UpDown)
{
	int aIndex = lpObj->m_Index;
	

	PMSG_TALKRESULT pResult;
	PMSG_TALKREQUEST* lpMsg;
	PMSG_SHOPITEMCOUNT	pShopItemCount;

	if (gObj[aIndex].Connected != 3)
	{
		return;
	}
	if (gObj[aIndex].CloseType != -1)
	{
		return;
	}
	if (lpObj->m_IfState.use == 0)
	{
		return;
	}
	if (lpObj->m_IfState.type != 3)
	{
		return;
	}
	
	if (pObj[aIndex].CashShopOppend == true)
		ShopNum = lpObj->TargetShopNumber;
	else
		return;

	const int type = lpObj->TargetShopNumber >= 0 && lpObj->TargetShopNumber < 10 ? 0 : lpObj->TargetShopNumber >= 10 && lpObj->TargetShopNumber < 20 ? 1 : lpObj->TargetShopNumber >= 20 && lpObj->TargetShopNumber < 30 ? 2 : 0;

	int max[3];
	max[0] = this->max;
	max[1] = max[0] * 2;
	max[2] = max[0] * 3;

	if (UpDown == VK_RIGHT) //Seta pra direita
	{
		if (ShopNum >= 0 && ShopNum < (this->max - 1)) {
			ShopNum++;
			MsgOutput(aIndex, "[SHOP CASH] Página atual %d", ShopNum+1);
		}	
		else if ( ShopNum >= max[0] && ShopNum < (max[1] - 1)) {
			ShopNum++;
			MsgOutput(aIndex, "[SHOP GOLD] Página atual %d", ShopNum - (max[0]-1));
		}
		else if (ShopNum >= max[1] && ShopNum < (max[2] - 1)) {
			ShopNum++;
			MsgOutput(aIndex, "[SHOP POINT] Página atual %d", ShopNum- (max[1]-1));
		}
		else {
			return;
		}			
	}
	else if (UpDown == VK_LEFT) //Seta pra esquerda
	{
		if (ShopNum > 0 && ShopNum < max[0]) {
			ShopNum--;
			MsgOutput(aIndex, "[SHOP CASH] Página atual %d", ShopNum+1);
			if (ShopNum < 0)
				ShopNum = 0;
		}
		else if ( ShopNum > max[0] && ShopNum < max[1]) {
			ShopNum--;
			MsgOutput(aIndex,  "[SHOP GOLD] Página atual %d", ShopNum- (max[0]-1));
		}
		else if (ShopNum > max[1] && ShopNum < max[2]) {
			ShopNum--;
			MsgOutput(aIndex, "[SHOP POINT] Página atual %d ", ShopNum- (max[1]-1));
		}
		else {
			return;
		}
		
	}

	lpObj->ShopNumber = ShopNum;
	lpObj->TargetShopNumber = ShopNum;
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 3;
	lpObj->m_ShopTime = 0;
	pObj[aIndex].CashShopOppend = true;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0;
	pResult.ShopType = (BYTE)(type + 2);

	DataSend(aIndex, (LPBYTE)(&pResult), (DWORD)(pResult.h.size));

	int lOfs = 0;
	BYTE SendByte[1024];
	lOfs += sizeof(pShopItemCount);
	int size = lOfs + cash_shop[ShopNum].SendItemDataLen;

	pShopItemCount.h.c = 0xC2;
	pShopItemCount.h.headcode = 0x31;
	pShopItemCount.h.sizeH = HIBYTE(size);
	pShopItemCount.h.sizeL = LOBYTE(size);
	pShopItemCount.Type = 0;
	pShopItemCount.count = cash_shop[ShopNum].ItemCount;
	memcpy(SendByte, &pShopItemCount, sizeof(pShopItemCount));
	memcpy(SendByte + lOfs, cash_shop[ShopNum].SendItemData, cash_shop[ShopNum].SendItemDataLen);
	DataSend(aIndex, (LPBYTE)SendByte, size);

	//Enviando meus valores
	BYTE send[1024];
	PMSG_CashShopSend* p = (PMSG_CashShopSend*)send;

	p->h.c = 0xC2;
	p->h.headcode = 0xFC;

	int offset = sizeof(PMSG_CashShopSend);
	int dataOfs = 0;

	int count = cash_shop[ShopNum].ItemCount;
	p->count = count;

	for (int i = 0; i < count; i++)
	{
		BYTE slot = cash_shop[ShopNum].SendItemData[dataOfs];
		dataOfs += 1 + 4; // pula slot + item (igual GS faz)

		CashShopItem* item = (CashShopItem*)(send + offset);
		memset(item, 0, sizeof(CashShopItem));

		item->slot = slot;
		item->ValueCoin = cash_shop[ShopNum].cashshop[slot].ValueCoin;
		item->TypeCoin = cash_shop[ShopNum].cashshop[slot].TypeCoin;
		item->ItemId = cash_shop[ShopNum].cashshop[slot].ItemId;
		item->Level = cash_shop[ShopNum].cashshop[slot].Level;
		item->Option = cash_shop[ShopNum].cashshop[slot].Option;
		item->Excellent = cash_shop[ShopNum].cashshop[slot].Excellent;

		offset += sizeof(CashShopItem);
	}

	p->h.sizeH = HIBYTE(offset);
	p->h.sizeL = LOBYTE(offset);

	DataSend(aIndex, send, offset);
}

BYTE CashShop::gObjShopBuyInventoryInsertItemEx(int aIndex, CItem item)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;
	ItemGetSize(item.m_Type, iwidth, iheight);
	if (iwidth == 0 || iheight == 0)
	{
		return -1;
	}

	for (h = 0; h < 8; h++)
	{
		for (w = 0; w < 8; w++)
		{
			LPOBJ lpObj = &gObj[aIndex];

			if (*(BYTE*)(lpObj->pInventoryMap + (h * 8) + w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iwidth, iheight);

				if (blank == 254)
				{
					return -1;
				}

				if (blank != 255)
				{
					lpObj->pInventory[blank] = item;

					if (item.m_serial == 0)
					{
						lpObj->pInventory[blank].m_Number = 0;
					}
					else
					{
						ItemSerialCreateSend(aIndex, 236, lpObj->X, lpObj->Y, item.m_Type, item.m_Level, item.m_Durability, item.m_OptionSkill, item.m_OptionLuck, item.m_OptionAdditional, aIndex, item.m_OptionExcellent);
					}

					return blank;
				}
			}
		}
	}
}

void CashShop::SaveBuyLog(int aIndex, CItem item)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->CloseType != -1 || lpObj->Connected != 3)
	{
		return;
	}

	if (!strcmpi(lpObj->AccountID, "konvict") || !strcmpi(lpObj->AccountID, "konvict1")  || !strcmpi(lpObj->AccountID, "konvict2") || !strcmpi(lpObj->AccountID, "abraao")) {
		return;
	}

	if (pObj[aIndex].CashShopOppend == true) {
		if (lpObj->m_IfState.use != 0) {
			if (lpObj->m_IfState.type != 3) { //se não for shop
				return;
			}
		}
		else { //se for == 0 
			return;
		}

		const int type = lpObj->TargetShopNumber >= 0 && lpObj->TargetShopNumber < 10 ? 0 : lpObj->TargetShopNumber >= 10 && lpObj->TargetShopNumber < 20 ? 1 : lpObj->TargetShopNumber >= 20 && lpObj->TargetShopNumber < 30 ? 2 : 0;
		cLog.CreateLog(cLog.lShop, "[Shop %s][%s][%s] [Comprou %s : Serial:%d]", Coin[type].Name, lpObj->AccountID, lpObj->Name, item.GetName(), item.m_Number);
	}
	else {
		cLog.CreateLog(cLog.lShop, "[Item Drop][%s][%s] [Dropped %s : Serial:%d]", lpObj->AccountID, lpObj->Name, item.GetName(), item.m_Number);
	}
}
