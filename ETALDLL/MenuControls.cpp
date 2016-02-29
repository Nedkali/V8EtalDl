//////////////////////////////////////////////////////////////////////
// MenuControls.cpp
//////////////////////////////////////////////////////////////////////
#include "MenuControls.h"
#include "Input.h"

using namespace v8;

MENU::MENU(void)
{

}

MENU::~MENU(void)
{

}

void MENU::locateControl()
{
	Control* pControl = findControl(CONTROL_BUTTON, 5106, -1, 264, 324, 272, 35);
	ControlText* pControlText;

	while (pControl != NULL)
	{

		if (pControl->dwType == CONTROL_BUTTON)
		{

			char* Text2 = wchart_to_char(pControl->wText2);
			int x = pControl->dwPosX;
			int y = pControl->dwPosY;
			int bwidth = pControl->dwSizeX;
			int bheight = pControl->dwSizeY;

			MessageBox(NULL, Text2, "Debug fcontrol", NULL);

		}
		pControl = pControl->pNext;
	}
}

int MENU::GetLocationID()
{
	if (findControl(CONTROL_BUTTON, 5103, -1, 330, 416, 128, 35))
		return MENU_MAIN_MENU_CONNECTING;					//21 Connecting to Battle.net	
	else if (findControl(CONTROL_BUTTON, 5102, -1, 335, 412, 128, 35))
		return MENU_LOGIN_ERROR;								//10 Login Error	
	else if (findControl(CONTROL_BUTTON, 5102, -1, 351, 337, 96, 32))	//5102 =OK
	{
		if (findControl(CONTROL_TEXTBOX, 5351, -1, 268, 320, 264, 120))
			return MENU_LOST_CONNECTION;						//17 lost connection	
		else if (findControl(CONTROL_TEXTBOX, 5347, -1, 268, 320, 264, 120))
			return MENU_DISCONNECTED;						//14  Disconnected
		else
			return MENU_CHARACTER_CREATE_ALREADY_EXISTS;		//30 Character Create - Dupe Name									
	}
	else if (findControl(CONTROL_BUTTON, 5103, -1, 351, 337, 96, 32))		//5103 = CANCEL
	{
		if (findControl(CONTROL_TEXTBOX, 5243, -1, 268, 300, 264, 100))
			return MENU_CHARACTER_SELECT_PLEASE_WAIT;		//16 char select please wait...
		if (findControl(CONTROL_TEXTBOX, (char *)NULL, -1, 268, 320, 264, 120))
			return MENU_PLEASE_WAIT;							//25 "Please Wait..."single player already exists also
	}
	else if (findControl(CONTROL_BUTTON, 5103, -1, 433, 433, 96, 32))
	{
		if (findControl(CONTROL_TEXTBOX, (char *)NULL, -1, 427, 234, 300, 100))
			return MENU_INLINE;								//2 waiting in line	
		else if (findControl(CONTROL_TEXTBOX, 10018, -1, 459, 380, 150, 12))
			return MENU_CREATE;								//4 Create game
		else if (findControl(CONTROL_BUTTON, 5119, -1, 594, 433, 172, 32))
			return MENU_JOIN;								// 5 Join Game
		else if (findControl(CONTROL_BUTTON, 5102, -1, 671, 433, 96, 32))
			return MENU_CHANNEL;								//7 "Channel List"
		else
			return MENU_LADDER;								//6 "Ladder"		
	}
	else if (findControl(CONTROL_BUTTON, 5101, -1, 33, 572, 128, 35))		//5101 = EXIT
	{
		if (findControl(CONTROL_BUTTON, 5288, -1, 264, 484, 272, 35))
			return MENU_LOGIN;								//9 Login
		if (findControl(CONTROL_BUTTON, 5102, -1, 495, 438, 96, 32))
			return MENU_CHARACTER_SELECT_CHANGE_REALM;		//43 char select change realm						
		if (findControl(CONTROL_BUTTON, 5102, -1, 627, 572, 128, 35) && findControl(CONTROL_BUTTON, 10832, -1, 33, 528, 168, 60)) //10832=create new
		{
			if (findControl(CONTROL_BUTTON, 10018, -1, 264, 297, 272, 35)) //NORMAL
				return MENU_DIFFICULTY;						//20 single char Difficulty
			Control* pControl = findControl(CONTROL_TEXTBOX, (char *)NULL, -1, 37, 178, 200, 92);
			if (pControl && pControl->pFirstText && pControl->pFirstText->pNext)
				return MENU_CHAR_SELECT;						//12 char select
			else
			{
				if (findControl(CONTROL_TEXTBOX, 11162, -1, 45, 318, 531, 140) || findControl(CONTROL_TEXTBOX, 11066, -1, 45, 318, 531, 140))
					return MENU_REALM_DOWN;
				else
					return MENU_CHARACTER_SELECT_NO_CHARS;	//42 char info not loaded 
			}
		}
		if (findControl(CONTROL_BUTTON, 5101, -1, 33, 572, 128, 35))		//5101=Exit
		{
			if (findControl(CONTROL_BUTTON, 5102, 0, 627, 572, 128, 35))			//5102=ok
				return MENU_CHARACTER_CREATE;				//29 char create screen with char selected
			else
			{
				if (findControl(CONTROL_TEXTBOX, 5226, -1, 321, 448, 300, 32))
					return MENU_NEW_ACCOUNT;					//32 create new bnet account
				else
					return MENU_NEW_CHARACTER;				//15 char create screen no char selected
			}
		}
	}
	if (findControl(CONTROL_BUTTON, 5102, -1, 335, 450, 128, 35))
	{
		if (findControl(CONTROL_TEXTBOX, 5200, -1, 162, 270, 477, 50))
			return MENU_CDKEY_IN_USE;						//19 CD-KEY in use
		else if (findControl(CONTROL_TEXTBOX, 5190, -1, 162, 420, 477, 100))		 //5190="If using a modem"
			return MENU_UNABLE_TO_CONNECT;					//11 unable to connect
		else
			return MENU_INVALID_CDKEY;						//22 invalid CD-KEY 		
	}
	else if (findControl(CONTROL_TEXTBOX, 5159, -1, 438, 300, 326, 150))
		return MENU_GAME_DOES_NOT_EXIST;						//28 game doesn't exist
	else if (findControl(CONTROL_TEXTBOX, 5161, -1, 438, 300, 326, 150))
		return MENU_GAME_IS_FULL;							//38 Game is full
	else if (findControl(CONTROL_TEXTBOX, 5138, -1, 438, 300, 326, 150))
		return MENU_GAME_EXIST;								//26 Game already exists
	else if (findControl(CONTROL_TEXTBOX, 5139, -1, 438, 300, 326, 150))
		return MENU_SERVER_DOWN;								//24 server down	
	else if (findControl(CONTROL_BUTTON, 5106, -1, 264, 324, 272, 35))				//5106="SINGLE PLAYER"
		return MENU_MAIN_MENU;								//8 Main Menu
	else if (findControl(CONTROL_BUTTON, 11126, -1, 27, 480, 120, 20))					//11126=ENTER CHAT
		return MENU_LOBBY;									//1 base bnet 
	else if (findControl(CONTROL_BUTTON, 5308, -1, 187, 470, 80, 20))					//5308="HELP"
		return MENU_CHAT;									//3 chat bnet 
	else if (findControl(CONTROL_TEXTBOX, 21882, -1, 100, 580, 600, 80))
		return MENU_D2SPLASH;								//18 Spash	
	else if (findControl(CONTROL_BUTTON, 5102, -1, 281, 538, 96, 32))
		return MENU_GATEWAY;									//27 select gateway
	else if (findControl(CONTROL_BUTTON, 5181, -1, 525, 513, 128, 35))
		return MENU_AGREE_TO_TERMS;							//31 agree to terms
	else if (findControl(CONTROL_BUTTON, 5102, -1, 525, 513, 128, 35))
		return MENU_PLEASE_READ;								//33 please read
	else if (findControl(CONTROL_BUTTON, 11097, -1, 265, 527, 272, 35))
		return MENU_REGISTER_EMAIL;							//34 register email
	else if (findControl(CONTROL_BUTTON, 5101, -1, 33, 578, 128, 35))
		return MENU_CREDITS;									//35 Credits
	else if (findControl(CONTROL_BUTTON, 5103, -1, 334, 488, 128, 35))
		return MENU_CINEMATICS;								//36 Cinematics
	else if (findControl(CONTROL_BUTTON, 5116, -1, 264, 350, 272, 35))
		return	MENU_OTHER_MULTIPLAYER;						//39 other multi player
	else if (findControl(CONTROL_BUTTON, 5103, -1, 281, 337, 96, 32))
		return MENU_ENTER_IP_ADDRESS;						//41 enter ip	
	else if (findControl(CONTROL_BUTTON, 5118, -1, 265, 206, 272, 35))
		return	MENU_TCP_IP;									//40 tcp-ip

	return MENU_NONE;
}

Control* MENU::findControl(int Type, int LocaleID, int Disabled, int PosX, int PosY, int SizeX, int SizeY)
{
	char* LocaleString = NULL;
	if (LocaleID >= 0)
	{
		LocaleString = wchart_to_char(fpGetLocaleText((WORD)LocaleID));
		if (!LocaleString)
		{
			return NULL;
		}
		Control* res = findControl(Type, LocaleString, Disabled, PosX, PosY, SizeX, SizeY);
		delete[] LocaleString;
		return res;
	}
	return NULL;
}

Control* MENU::findControl(int Type, const char* Text, int Disabled, int PosX, int PosY, int SizeX, int SizeY)
{
	if (Type == -1 && Text == NULL && Disabled == -1 && PosX == -1 && PosY == -1 && SizeX == -1 && SizeY == -1)
	{
		return *vpFirstControl;
	}

	bool bFound = false;

	for (Control* pControl = *vpFirstControl; pControl; pControl = pControl->pNext)
	{
		// Check type
		if (Type >= 0 && static_cast<int>(pControl->dwType) == Type)
		{
			bFound = true;
		}
		else if (Type >= 0 && static_cast<int>(pControl->dwType) != Type)
		{
			bFound = false;
			continue;
		}

		// Check disabled
		if (Disabled >= 0 && static_cast<int>(pControl->dwDisabled) == Disabled)
		{
			if (pControl->dwType == CONTROL_BUTTON && pControl->dwMaxLength == 1)
			{
				bFound = false;
				continue;
			}
			bFound = true;
		}
		else if (Disabled >= 0 && static_cast<int>(pControl->dwDisabled) != Disabled)
		{
			bFound = false;
			continue;
		}

		// Check PosX
		if (PosX >= 0 && static_cast<int>(pControl->dwPosX) == PosX)
		{
			bFound = true;
		}
		else if (PosX >= 0 && static_cast<int>(pControl->dwPosX) != PosX)
		{
			bFound = false;
			continue;
		}

		// Check PosY
		if (PosY >= 0 && static_cast<int>(pControl->dwPosY) == PosY)
		{
			bFound = true;
		}
		else if (PosY >= 0 && static_cast<int>(pControl->dwPosY) != PosY)
		{
			bFound = false;
			continue;
		}

		// Check SizeX
		if (SizeX >= 0 && static_cast<int>(pControl->dwSizeX) == SizeX)
		{
			bFound = true;
		}
		else if (SizeX >= 0 && static_cast<int>(pControl->dwSizeY) != SizeX)
		{
			bFound = false;
			continue;
		}

		// Check SizeY
		if (SizeY >= 0 && static_cast<int>(pControl->dwSizeY) == SizeY)
		{
			bFound = true;
		}
		else if (SizeY >= 0 && static_cast<int>(pControl->dwSizeY) != SizeY)
		{
			bFound = false;
			continue;
		}

		// Check Text
		if (Text && pControl->dwType == CONTROL_BUTTON)
		{
			char* Text2 = wchart_to_char(pControl->wText2);
			if (!Text2)
			{
				return NULL;
			}

			if (strcmp(Text2, Text) == 0)
			{
				bFound = true;
				delete[] Text2;
			}
			else
			{
				bFound = false;
				delete[] Text2;
				continue;
			}
		}

		if (Text && pControl->dwType == CONTROL_TEXTBOX)
		{
			if (pControl->pFirstText != NULL && pControl->pFirstText->wText != NULL)
			{
				char* Text2 = wchart_to_char(pControl->pFirstText->wText);
				if (!Text2)
				{
					return NULL;
				}
				if (strstr(Text, Text2) != 0)
				{
					bFound = true;
					delete[] Text2;
				}
				else
				{
					bFound = false;
					delete[] Text2;
					continue;
				}
			}
			else
			{
				bFound = false;
				continue;
			}
		}
		if (bFound)
		{
			return pControl;
		}
	}
	return NULL;
}

bool MENU::clickControl(Control* pControl)
{
	if (pControl)
	{
		int x = pControl->dwPosX + (pControl->dwSizeX / 2);
		int y = pControl->dwPosY - (pControl->dwSizeY / 2);
		Input::SendMouseClick(x, y, 0);
		Sleep(50);
		return true;
	}
	return false;
}

void MENU::SetControlText(Control* pControl, const char* Text)
{
	if (pControl && Text)
	{
		wchar_t* wText = char_to_wchart(Text);
		if (!wText)
			return;
		fpSetControlText(pControl, wText);
		delete[] wText;
	}
}

ClientGameState MENU::ClientState(void)
{
	ClientGameState state = ClientStateNull;

	UnitAny* ME = fpGetPlayerUnit();

	if (ME && !(*vpFirstControl))
	{
		if (ME->pInventory)
			state = ClientStateInGame;
		else
			state = ClientStateBusy;
	}
	else if (!ME && *vpFirstControl)
		state = ClientStateMenu;
	else if (!ME && !(*vpFirstControl))
		state = ClientStateNull;

	return state;
}

v8::Local<v8::Array> MENU::GetText(int x, int a)
{
	int count = 0;
	char* Text2 = "";
	char* Line5 = "";
	v8::Local<v8::Array> arguments = v8::Array::New(5);
	Control* pControl = findControl(CONTROL_TEXTBOX, (char *)NULL, -1, 237, 178, 72, 93);
	ControlText* pControlText;

	while (pControl != NULL)
	{
		if (pControl->dwType == CONTROL_TEXTBOX && pControl->pFirstText != NULL && pControl->pFirstText->pNext != NULL)
		{
			if (count == x)
			{
				pControlText = pControl->pFirstText;

				char* Line = wchart_to_char(pControlText->wText);
				char* Line2 = wchart_to_char(pControlText->pNext->wText);
				char* Line3 = wchart_to_char(pControlText->pNext->pNext->wText);
				char* Line4 = wchart_to_char(pControlText->pNext->pNext->pNext->wText);
				if (a > 0)//crashes on single player on next read
				{
					Line5 = wchart_to_char(pControlText->pNext->pNext->pNext->pNext->wText);
				}

				arguments->Set(0, String::New(Line, strlen(Line)));
				arguments->Set(1, String::New(Line2, strlen(Line2)));
				arguments->Set(2, String::New(Line3, strlen(Line3)));
				arguments->Set(3, String::New(Line4, strlen(Line4)));
				arguments->Set(4, String::New(Line5, strlen(Line5)));
				return arguments;
			}
			++count;
		}
		pControl = pControl->pNext;
	}

	return arguments;
}
