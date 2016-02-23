//////////////////////////////////////////////////////////////////////
// D2Helpers.cpp
//////////////////////////////////////////////////////////////////////
#include "D2Helpers.h"
#include "D2Pointers.h"



void LoadMPQ(const char* mpq)
{
	D2WIN_InitMPQ("D2Win.DLL", mpq, NULL, 0, 0);
	*p_BNCLIENT_XPacKey = *p_BNCLIENT_ClassicKey = *p_BNCLIENT_KeyOwner = NULL;
	BNCLIENT_DecodeAndLoadKeys();
}

DWORD SAY_I = Pointer::GetDllOffset("D2Client.dll", 0xB27A6);
DWORD SAY_II = Pointer::GetDllOffset("D2Lang.dll", 0x8C60);

void __declspec(naked) __fastcall Say_ASM(DWORD dwPtr)
{
	__asm
	{
		POP EAX
		PUSH ECX
			PUSH EAX
			SUB ESP, 0x110
			PUSH EBX
			PUSH EBP
			MOV EBP, [SAY_II]
			PUSH ESI
			PUSH EDI
			JMP SAY_I
	}
}

void __fastcall Say(const char *szMessage)
{
	using namespace std;

	const char REPLACE_CHAR = (char)(unsigned char)0xFE;

	va_list vaArgs;
	va_start(vaArgs, szMessage);
	int len = _vscprintf(szMessage, vaArgs);
	char* szBuffer = new char[len + 1];
	vsprintf_s(szBuffer, len + 1, szMessage, vaArgs);
	va_end(vaArgs);

	replace(szBuffer, szBuffer + len, REPLACE_CHAR, '%');

	wchar_t* wBuffer = AnsiToUnicode(szBuffer);
	memcpy((wchar_t*)vpChatMsg, wBuffer, wcslen(wBuffer) * 2 + 1);
	delete[] wBuffer;
	wBuffer = NULL;

	MSG* aMsg = new MSG;
	aMsg->hwnd = fpGetHwnd();
	aMsg->message = WM_CHAR;
	aMsg->wParam = VK_RETURN;
	aMsg->lParam = 0x11C0001;
	aMsg->time = NULL;
	aMsg->pt.x = 0x79;
	aMsg->pt.y = 0x1;

	Say_ASM((DWORD)&aMsg);
	delete aMsg;
	aMsg = NULL;
}
