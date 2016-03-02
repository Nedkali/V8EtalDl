#include "GameDraw.h"
#include "Main.h"
#include "D2Pointers.h"

using namespace std;
void DrawLogo(void)
{
	static char version[] = "D2Etal " D2ETAL_VERSION;

	static int x = (CalculateTextLen(version, 0).x/2);
	int dx = (GetScreenSize().x/2);

	//drawbackgournd here

	myDrawText(version, dx-x*4, 15, 4, 8);
}

//TODO : Add screen hook