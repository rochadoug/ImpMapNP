#include "StdAfx.h"


CMapItem::CMapItem()
{
	this->Init();
}

CMapItem::~CMapItem()
{
	return;

}

void CMapItem::Init()
{
	this->VPCount = 0;
	this->m_State = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		memset(&this->VpPlayer[n], 0, sizeof(VIEWPORT_STRUCT));
	}
}

void CMapItem::CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3,BYTE attributeEx, DWORD number)
{
	this->Init();

	this->m_Level = level;
	this->m_Durability = dur;
	this->Convert(type, Option1, Option2, Option3, attributeEx,  CURRENT_DB_VERSION);
	this->px = x;
	this->py = y;
	this->live = true;
	this->Give = false;
	this->m_State = 1;

	if (this->m_QuestItem != false)
	{
		this->m_Time = GetTickCount() + 60000;
		this->m_LootTime = GetTickCount() + 600000;
		this->m_Number = number;
	}
	else
	{
		this->m_Time = GetTickCount() + 120000;
		this->m_LootTime = GetTickCount() + 3/*lootingtime*/ * 1000;
		this->m_Number = number;
	}

}