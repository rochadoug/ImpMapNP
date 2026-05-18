#pragma once


class CMapItem : public CItem
{

public:

	CMapItem();
	virtual ~CMapItem();

	void Init();
	void CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE attributeEx, DWORD number);
	void DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE attributeEx, DWORD number);

//	void CreateItem(int type, int level, int x, int y, float dur, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, DWORD serial);

//	void DropCreateItem(int type, int level, int x, int y, float dur, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, DWORD serial);


public:

	BYTE px;	// BD
	BYTE py;	// BE
	bool live;	// BF
	bool Give;	// C4
	DWORD m_State;	// B8
	DWORD m_Time;	// BC
	int m_UserIndex;	// C0
	DWORD m_LootTime;	// C4
	VIEWPORT_STRUCT VpPlayer[MAX_VIEWPORT];	// C8
	int VPCount;	// 44C
};
