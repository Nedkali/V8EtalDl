//////////////////////////////////////////////////////////////////////
// Input.cpp
//////////////////////////////////////////////////////////////////////
#include "Input.h"


Input::Input(void)
{

}


Input::~Input(void)
{

}

bool Input::Initialize()
{
	bool debug = false;
	int i = 0;
	while (!Vars.bActive)
	{
		if (i >= 300)
		{
			MessageBox(0, "Failed to set hooks, exiting!", "D2Etal", 0);
			return false;
		}

		if (fpGetHwnd() && (MENU::ClientState() == ClientStateMenu || MENU::ClientState() == ClientStateInGame))
		{
			if (!Vars.oldWNDPROC)
				Vars.oldWNDPROC = (WNDPROC)SetWindowLong(fpGetHwnd(), GWL_WNDPROC, (LONG)Input::WndProc);
			if (!Vars.oldWNDPROC)
				continue;

			DWORD _mainThread = GetWindowThreadProcessId(fpGetHwnd(), 0);
			if (_mainThread)
			{
				if (!Vars.hKeybHook)
					Vars.hKeybHook = SetWindowsHookEx(WH_KEYBOARD, Input::KeyPress, NULL, _mainThread);
				if (!Vars.hMouseHook)
					Vars.hMouseHook = SetWindowsHookEx(WH_MOUSE, Input::MouseMove, NULL, _mainThread);
			}
		}
		else
			continue;
		if (Vars.hKeybHook && Vars.hMouseHook)
		{
			Vars.bActive = TRUE;
		}
		if (debug && Vars.oldWNDPROC && Vars.hKeybHook && Vars.hMouseHook) {
			MessageBox(0, "All Hooks Set!", "D2Etal", 0);
		}
		Sleep(50);
		i++;
	}
	return true;
}

void Input::Shutdown()
{
	Vars.bActive = FALSE;
}

LONG WINAPI Input::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* pCopy;
	switch (uMsg)
	{
	case WM_COPYDATA:
		pCopy = (COPYDATASTRUCT*)lParam;

		if (pCopy)
		{
			if (pCopy->dwData == 0x0000)
			{
				//TODO : Add code
			}
		}
		return TRUE;
	}

	return (LONG)CallWindowProcA(Vars.oldWNDPROC, hWnd, uMsg, wParam, lParam);
}

void Input::SendMouseClick(int x, int y, int button)
{
	int HoldX = *vpMouseX;
	int HoldY = *vpMouseY;
	*vpMouseX = x;
	*vpMouseY = y;

	LPARAM lParam = MAKELPARAM(x, y);

	switch (button)
	{
	case 0:
		SendMessage(fpGetHwnd(), WM_LBUTTONDOWN, MK_LBUTTON, lParam);
		Sleep(10);
		SendMessage(fpGetHwnd(), WM_LBUTTONUP, MK_LBUTTON, lParam);
		Sleep(10);
		break;
	case 1:
		SendMessage(fpGetHwnd(), WM_RBUTTONDOWN, MK_RBUTTON, lParam);
		Sleep(25);
		SendMessage(fpGetHwnd(), WM_RBUTTONUP, MK_RBUTTON, lParam);
		Sleep(25);
		break;
	default:
		break;
	}

	*vpMouseX = HoldX;
	*vpMouseY = HoldY;
}

LRESULT CALLBACK Input::KeyPress(int code, WPARAM wParam, LPARAM lParam)
{

	// ? issues causing client to shut down
	if (code >= HC_ACTION)
	{
		WORD repeatCount = LOWORD(lParam);
		bool altState = !!(HIWORD(lParam) & KF_ALTDOWN);
		bool previousState = !!(HIWORD(lParam) & KF_REPEAT);
		bool transitionState = !!(HIWORD(lParam) & KF_UP);
		bool isRepeat = !transitionState && repeatCount != 1;
		bool isDown = !(previousState && transitionState);
		bool isUp = previousState && transitionState;

		switch (wParam)
		{
		case VK_HOME:
			if (isDown && !isRepeat)
			{
				MessageBoxA(NULL, "Key Pressed", "Debug", NULL);
			}
			break;
		case VK_INSERT:

			if (isDown && !isRepeat)
			{
				D2Funcs::Print("ÿc8x: %i" ,fpGetUnitX(fpGetPlayerUnit()));
				D2Funcs::Print("ÿc8y: %i", fpGetUnitY(fpGetPlayerUnit()));
			}
			break;
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}

LRESULT CALLBACK Input::MouseMove(int code, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT* mouse = (MOUSEHOOKSTRUCT*)lParam;
	POINT point = mouse->pt;
	ScreenToClient(mouse->hwnd, &point);

	if (code == HC_ACTION && (point.x < 0 || point.y < 0))
	{
		return CallNextHookEx(NULL, code, wParam, lParam);
	}

	if (code == HC_ACTION)
	{
		bool ScriptPauseForManualControl = false;
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			ScriptPauseForManualControl = true;
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			ScriptPauseForManualControl = false;
			break;
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}