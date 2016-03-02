//////////////////////////////////////////////////////////////////////
// MenuControls.cpp
//////////////////////////////////////////////////////////////////////

#pragma once
#include "D2Pointers.h"
#include <Windows.h>
#include <string>
#include <vector>
#include "v8.h"

#define BUTTON_SP findControl(CONTROL_BUTTON, 5106, -1, 264,324,272,35)

enum ClientGameState
{
	ClientStateNull, ClientStateMenu, ClientStateInGame, ClientStateBusy
};

class MENU
{
public:
	MENU(void);
	~MENU(void);

	

public:
	static void locateControl();
	static int GetLocationID();
	static Control* findControl(int Type, int LocaleID, int Disabled, int PosX, int PosY, int SizeX, int SizeY);
	static Control* findControl(int Type, const char* Text, int Disabled, int PosX, int PosY, int SizeX, int SizeY);
	static bool clickControl(Control* pControl);
	static void SetControlText(Control* pControl, const char* Text);
	static ClientGameState ClientState(void);
	static v8::Local<v8::Array> GetText(int x, int a);
};

///////////////////////////////////////////////////
// Control Defines
///////////////////////////////////////////////////
#define CONTROL_EDITBOX			0x01
#define CONTROL_IMAGE			0x02
#define CONTROL_UNUSED			0x03
#define CONTROL_TEXTBOX			0x04
#define CONTROL_SCROLLBAR		0x05
#define CONTROL_BUTTON			0x06
#define CONTROL_LIST			0x07
#define CONTROL_TIMER			0x08
#define CONTROL_SMACK			0x09
#define CONTROL_PROGRESSBAR		0x0a
#define CONTROL_POPUP			0x0b
#define CONTROL_ACCOUNTLIST		0x0c

enum MENU_Location {
	MENU_NONE = 0,
	MENU_LOBBY,
	MENU_INLINE,
	MENU_CHAT,
	MENU_CREATE,
	MENU_JOIN,
	MENU_LADDER,
	MENU_CHANNEL,
	MENU_MAIN_MENU,
	MENU_LOGIN,
	MENU_LOGIN_ERROR,
	MENU_UNABLE_TO_CONNECT,
	MENU_CHAR_SELECT,
	MENU_REALM_DOWN,
	MENU_DISCONNECTED,
	MENU_NEW_CHARACTER,
	MENU_CHARACTER_SELECT_PLEASE_WAIT,
	MENU_LOST_CONNECTION,
	MENU_D2SPLASH,
	MENU_CDKEY_IN_USE,
	MENU_DIFFICULTY,
	MENU_MAIN_MENU_CONNECTING,
	MENU_INVALID_CDKEY,
	MENU_CONNECTING, // not used. MENU_CHARACTER_SELECT_NO_CHARS with a connecting message
	MENU_SERVER_DOWN,
	MENU_PLEASE_WAIT,
	MENU_GAME_EXIST,
	MENU_GATEWAY,
	MENU_GAME_DOES_NOT_EXIST,
	MENU_CHARACTER_CREATE,
	MENU_CHARACTER_CREATE_ALREADY_EXISTS,
	MENU_AGREE_TO_TERMS,
	MENU_NEW_ACCOUNT,
	MENU_PLEASE_READ,
	MENU_REGISTER_EMAIL,
	MENU_CREDITS,
	MENU_CINEMATICS,
	MENU_CHARACTOR_CHANGE_REALM,
	MENU_GAME_IS_FULL,
	MENU_OTHER_MULTIPLAYER,
	MENU_TCP_IP,
	MENU_ENTER_IP_ADDRESS,
	MENU_CHARACTER_SELECT_NO_CHARS,
	MENU_CHARACTER_SELECT_CHANGE_REALM,
};