#include "D2Intercepts.h"
#include "D2Helpers.h"

VOID __declspec(naked) __fastcall ClassicSTUB()
{
	*p_BNCLIENT_ClassicKey = Vars.szClassic;
	__asm {
		lea eax, dword ptr ss : [Vars.szClassic]
			mov dword ptr ds : [0x6FF3E928], eax
			jmp BNCLIENT_DClass;
	}
}

VOID _declspec(naked) _fastcall KeyOwnerSTUB()
{
	*p_BNCLIENT_KeyOwner = Vars.szKeyOwner;
	_asm {
		lea eax, dword ptr ss : [Vars.szKeyOwner]
			mov dword ptr ds : [0x6FF3E934], eax		
			jmp BNCLIENT_DName;
	}
}

VOID __declspec(naked) __fastcall LodSTUB()
{
	*p_BNCLIENT_XPacKey = Vars.szLod;
	__asm {
		lea eax, dword ptr ss : [Vars.szLod]
			mov dword ptr ds : [0x6FF3E930], eax
			jmp BNCLIENT_DLod;
	}
}

void __declspec(naked) GameDraw_Intercept()
{
	__asm
	{
		call GameDraw;

		POP ESI
			POP EBX
			POP ECX
			RETN 4
	}
}

void GameDrawMenu_Intercept(void)
{
	GameDrawMenu();
}

UnitAny* GetSelectedUnit_Intercept(void)
{
	if (Vars.bClickAction)
	{
		if (Vars.dwSelectedUnitId)
		{
			UnitAny* pUnit = D2CLIENT_FindUnit(Vars.dwSelectedUnitId, Vars.dwSelectedUnitType);

			return pUnit;
		}

		return NULL;
	}

	return fpGetSelectedUnit();
}

void __declspec(naked) GameCrashFix_Intercept()
{
	__asm
	{
		CMP ECX, 0
		JE Skip
			MOV DWORD PTR DS : [ECX + 0x10], EDX
			Skip :
		MOV DWORD PTR DS : [EAX + 0xC], 0
			RETN
	}
}
