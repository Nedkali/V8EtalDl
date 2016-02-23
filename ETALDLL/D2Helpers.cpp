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
			MOV EBP, [D2CLIENT_Say_II]
			PUSH ESI
			PUSH EDI
			JMP D2CLIENT_Say_I
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

	if (*p_D2CLIENT_PlayerUnit)
	{
		wchar_t* wBuffer = AnsiToUnicode(szBuffer);
		memcpy((wchar_t*)p_D2CLIENT_ChatMsg, wBuffer, wcslen(wBuffer) * 2 + 1);
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
	// help button and ! ok msg for disconnected
	/*else if (findControl(CONTROL_BUTTON, 5308, -1, 187, 470, 80, 20) && (!findControl(CONTROL_BUTTON, 5102, -1, 351, 337, 96, 32)))
	{
	memcpy((char*)p_D2MULTI_ChatBoxMsg, szMessage, strlen(szMessage) + 1);
	D2MULTI_DoChat();
	}*/
}
