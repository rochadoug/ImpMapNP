//###############################################################################################
// Project   :: ACTeaM Classic 0.97d
// GameServer:: 0.96.40				
// Company   :: Advanced CoderZ MU DevelopmenT © 2013
// Revised   :: 17/01/2014
// Coded     :: Mr.Haziel Developer								
//###############################################################################################

#include "StdAfx.h"

//============================================================================================== 
//-- New Fix Max Zen :)
//==============================================================================================
void ReadAutoZen()
{
    BYTE AutoZen = GetPrivateProfileIntA("Common","GetAutomaticZen",1, SISINI);
    if(  AutoZen != 0 )
    {
		HookOffset(0x40A1E7, (DWORD)(&AutoRecvZenMod1), 0xE9);
    memset((int*)(0x40A1EC), 0x90, 49);
	HookOffset(0x40A0FD, (DWORD)(&AutoRecvZenMod2), 0xE9);
    memset((int*)(0x40A102), 0x90, 49);
	HookOffset(0x4096E0, (DWORD)(&AutoRecvZenMod3), 0xE9);
    memset((int*)(0x4096E5), 0x90, 36);
    }
}




void AutoRecvZen(LPOBJ lpObj, int Money)
{
	if (lpObj->Type == OBJECT_MONSTER)
	{
		LPOBJ lpTarget = (LPOBJ)OBJECT_POINTER(lpObj->TargetNumber);

		if (Chat.pegarItem[lpTarget->m_Index].pegar != 0) {


			if ((lpTarget->Money + Money) < 0)
			{
				lpTarget->Money = Money;
				GCMoneySend(lpTarget->m_Index, lpTarget->Money);
				MsgOutput(lpTarget->m_Index, "%s zen desbugado para %d ", lpTarget->Name, Money);
			}
			else
			{
				if ((lpTarget->Money + Money) > 2000000000)
				{
					lpTarget->Money = 2000000000;
					GCMoneySend(lpTarget->m_Index, lpTarget->Money);
					MsgOutput(lpTarget->m_Index, "%s Você está no limimte máximo de zen!", lpTarget->Name);
				}
				else
				{
					lpTarget->Money += Money;
					GCMoneySend(lpTarget->m_Index, lpTarget->Money);
					MsgOutput(lpTarget->m_Index, "%s Você obteve %d de Zen!", lpTarget->Name, Money);
				}
			}
		}
	}
}


__declspec(naked) void AutoRecvZenMod1()
{
	LPOBJ lpObj;

	_asm
	{
		MOV ECX, DWORD PTR SS:[EBP+0x8];
		MOV lpObj, ECX;
	}

	AutoRecvZen(lpObj, lpObj->Money);

	_asm
	{
		MOV EDI, 0x40A21D;
		JMP EDI;
	}
}

__declspec(naked) void AutoRecvZenMod2()
{
	LPOBJ lpObj;
	int Money;

	_asm
	{
		MOV ECX, DWORD PTR SS:[EBP+0x8];
		MOV lpObj, ECX;

		FLD DWORD PTR SS:[EBP-0x98];
		MOV EDI, 0x49D540;
		CALL EDI;

		MOV Money, EAX;
	}

	AutoRecvZen(lpObj, Money);

	_asm
	{
		MOV EDI, 0x40A133;
		JMP EDI;
	}
}

__declspec(naked) void AutoRecvZenMod3()
{
	LPOBJ lpObj;

	_asm
	{
		MOV ECX, DWORD PTR SS:[EBP+0x8];
		MOV lpObj, ECX;
	}

	AutoRecvZen(lpObj, 10000);

	_asm
	{
		MOV EDI, 0x409709;
		JMP EDI;
	}
}