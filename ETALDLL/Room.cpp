#include "Room.h"
//#include "CriticalSections.h"
#include "D2Helpers.h"
#include "D2Pointers.h"

BOOL RevealRoom(Room2* pRoom2, BOOL revealPresets) {
	bool bAdded = false;
	bool bInit = false;

	DWORD dwLevelNo = fpGetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
	//Make sure we have the room.
	if (!pRoom2)
		return false;
	
	//AutoCriticalRoom* cRoom = new AutoCriticalRoom;

	UnitAny* player = fpGetPlayerUnit();
	//Check if we have Room1(Needed in order to reveal)
	if (!(pRoom2 && pRoom2->pLevel && pRoom2->pRoom1)) {
		fpAddRoomData(pRoom2->pLevel->pMisc->pAct, pRoom2->pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, NULL);
		bAdded = true;
	}
	if (!(pRoom2 && pRoom2->pRoom1)){// second check added to see if we DID indeed init the room!
		//delete cRoom;
		return false;
	}

	//If we are somewhere other then the given area, init automap layer to be drawn to.
	if(!(pRoom2 && pRoom2->pLevel && pRoom2->pLevel->dwLevelNo && player->pPath && player->pPath->pRoom1 && player->pPath->pRoom1->pRoom2 && player->pPath->pRoom1->pRoom2->pLevel && player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo == pRoom2->pLevel->dwLevelNo)){
		InitAutomapLayer(pRoom2->pLevel->dwLevelNo);
		bInit = true;
	}

	//Reveal this room!
	fpRevealAutomapRoom(pRoom2->pRoom1, TRUE, (*vpAutomapLayer));

	if(revealPresets)
		DrawPresets(pRoom2);

	//Remove room data if we have added.
	if(bAdded)
		fpRemoveRoomData(pRoom2->pLevel->pMisc->pAct, pRoom2->pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, NULL);
	
	if(bInit)
		InitAutomapLayer(dwLevelNo);

	//delete cRoom;
	return true;
}

/*void DrawPresets(Room2 *pRoom2)
 *	This will find all the shrines, special automap icons, and level names and place on map.
 */
void DrawPresets (Room2 *pRoom2)
{
	//UnitAny* Player = D2CLIENT_GetPlayerUnit();
	//Grabs all the preset units in room.
	for (PresetUnit *pUnit = pRoom2->pPreset; pUnit; pUnit = pUnit->pPresetNext)
	{
		int mCell = -1;
		if (pUnit->dwType == 1)//Special NPCs.
		{
			if (pUnit->dwTxtFileNo == 256)//Izzy
				mCell = 300;
			if (pUnit->dwTxtFileNo == 745)//Hephasto
				mCell = 745;
		} else if (pUnit->dwType == 2) { //Objects on Map
			
			//Add's a special Chest icon over the hidden uberchests in Lower Kurast
			if (pUnit->dwTxtFileNo == 580 && pRoom2->pLevel->dwLevelNo == 79)		
				mCell = 318;

			//Special Units that require special checking:)
			if (pUnit->dwTxtFileNo == 371) 
				mCell = 301; //Countess Chest
			if (pUnit->dwTxtFileNo == 152) 
				mCell = 300; //A2 Orifice
			if (pUnit->dwTxtFileNo == 460) 
				mCell = 1468; //Frozen Anya
			if ((pUnit->dwTxtFileNo == 402) && (pRoom2->pLevel->dwLevelNo == 46))
				mCell = 0; //Canyon/Arcane Waypoint
			if ((pUnit->dwTxtFileNo == 267) && (pRoom2->pLevel->dwLevelNo != 75) && (pRoom2->pLevel->dwLevelNo != 103))
				mCell = 0;
			if ((pUnit->dwTxtFileNo == 376) && (pRoom2->pLevel->dwLevelNo == 107))
				mCell = 376;

			if (pUnit->dwTxtFileNo > 574)
				mCell = 0;

			if (mCell == -1)
			{
				//Get the object cell
				ObjectTxt *obj = fpGetObjectText(pUnit->dwTxtFileNo);

				if (mCell == -1)
				{
					mCell = obj->nAutoMap;//Set the cell number then.
				}
			}
		}

		//Draw the cell if wanted.
		if ((mCell > 0) && (mCell < 1258))
		{
			AutomapCell *pCell = fpNewAutomapCell();
			pCell->nCellNo = (WORD)mCell;
			int pX = (pUnit->dwPosX + (pRoom2->dwPosX * 5));
			int pY = (pUnit->dwPosY + (pRoom2->dwPosY * 5));
			pCell->xPixel = (WORD)((((pX - pY) * 16) / 10) + 1);
			pCell->yPixel = (WORD)((((pY + pX) * 8) / 10) - 3);

			fpAddAutomapCell(pCell, &((*vpAutomapLayer)->pObjects));
		}
	}
}
