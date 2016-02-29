//////////////////////////////////////////////////////////////////////
// JS Functions
//////////////////////////////////////////////////////////////////////
#pragma once

#include "v8.h"
#include "JsDefines.h"
#include "D2Pointers.h"
#include "Unit.h"
using namespace v8;

struct myPresetUnit //for CGetPresetUnits
{
	DWORD dwType;
	DWORD dwRoomX;
	DWORD dwRoomY;
	DWORD dwPosX;
	DWORD dwPosY;
	DWORD dwId;
	DWORD dwLevel;
};
// **Context
Handle<Context>CreateContext(Isolate* isolate);
JS_FUNC(CMe); void init_me();
//Working as intended
JS_FUNC(CLoad);
JS_FUNC(CInclude);
JS_FUNC(CSetTitle);
JS_FUNC(CSendCopyData);
JS_FUNC(CClick);
JS_FUNC(CGetTickCount);
JS_FUNC(CDelay);
JS_FUNC(CCloseD2);
JS_FUNC(CScreenSize);
JS_FUNC(CExitGame);
JS_FUNC(CGetPlayerUnit);
JS_FUNC(CSay);
JS_FUNC(CRandom);
JS_FUNC(CClickControl);
JS_FUNC(CGetLocation);
JS_FUNC(CSelectChar);
JS_FUNC(CGetText);
JS_FUNC(CSetText);
JS_FUNC(CClientState);
JS_FUNC(CTransmute);		//not tested, working in theory
JS_FUNC(CSubmitItem);		//not tested, working in theory
JS_FUNC(CGetUIState);
JS_FUNC(CSetUIState);
JS_FUNC(CGetLocaleString);
JS_FUNC(CUseStatPoint);
JS_FUNC(CUseSkillPoint);
JS_FUNC(CGold);
JS_FUNC(CClickMap); 
JS_FUNC(CGetControl);
JS_FUNC(CSelectRealm);


//currently testing/non functioning/un-finished
JS_FUNC(CPrint);			//needs color code correted
JS_FUNC(CGetPresetUnits);	//subareaid needs corrected
JS_FUNC(CGetArea);			//needs written
JS_FUNC(CGetPath);			//needs written
JS_FUNC(CGetBaseStat);		//needs written
JS_FUNC(CGetUnit);			//needs written
JS_FUNC(CGetDistance);		//needs written

//needs set under me global
JS_FUNC(CSetSkill);

//for testing will be removed
JS_FUNC(CMove);
JS_FUNC(CTESTWAYPOINT);
JS_FUNC(CGetLevel);

//			________________________
//			| TODO LIST OF GLOBALS |
//	________|______Template________|_______
//	|_Function________|________Completion_|
//	|ӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝӝ|

//[-] GLOBALS D2NT
//[+] CheckCollision()
//[+] ClickMap()
//[+] CloseD2()					//added
//[+] Delay()					//added
//[+] ExitGame()				//added
//[+] FileOpen()
//[+] GetArea()					//WIP
//[+] GetBaseStat()				//WIP
//[+] GetControl()
//[+] GetCursorType()
//[+] GetDistance()				//WIP
//[+] GetLocaleString()			//added
//[+] GetPath()					//WIP : Will probably be added last sadly
//[+] GetPlayerFlag()
//[+] GetPlayerUnit()			//added
//[+] GetPresetUnits()			//added
//[+] GetRoom()
//[+] GetScript()
//[+] GetTickCount()
//[+] GetUIState()				//added
//[+] GetUnit()					/WIP
//[+] GetWaypoint()
//[+] Gold()					//added
//[+] Include()					//added
//[+] Load()					//added
//[+] Print()					//added
//[+] RegisterEvent()
//[+] Random()					//added
//[+] RunGC()
//[+] Say()						//added
//[+] SendCopyData()			//added
//[+] SetStatusText()
//[+] SetUIState()				//added
//[+] SubmitItem()				//added
//[+] Transmute()				//added
//[+] UnregisterEvent()

//[-] Me (Global Object)
//[+] Me.Account
//[+] Me.Act
//[+] Me.Areaid
//[+] Me.Charloc
//[+] Me.Charname
//[+] Me.Chickenhp
//[+] Me.Chickenmp
//[+] Me.Classid
//[+] Me.Diff
//[+] Me.Gamename
//[+] Me.Gamepassword
//[+] Me.Gameserverip
//[+] Me.Gametype
//[+] Me.Gatewayid
//[+] Me.Gid
//[+] Me.Hpmax
//[+] Me.Hp
//[+] Me.Ingame
//[+] Me.itemoncursor
//[+] Me.Ladder
//[+] Me.Maxgametime
//[+] Me.Mode
//[+] Me.Mpmax
//[+] Me.Mp
//[+] Me.Name
//[+] Me.Ping
//[+] Me.Playertype
//[+] Me.Playtype
//[+] Me.Quitonhostile
//[+] Me.Realm
//[+] Me.Realmshort
//[+] Me.Revealautomap
//[+] Me.Runwalk
//[+] Me.Showenemyonautomap
//[+] Me.Showmissileonautomap
//[+] Me.Screensize
//[+] Me.Type
//[+] Me.Weaponstab
//[+] Me.X
//[+] Me.Y
//[+] Me.Cancel()
//[+] Me.ClickItem()
//[+] Me.ClickMercItem()
//[+] Me.ClickParty()
//[+] Me.GetCursorItem()
//[+] Me.GetMercCost()
//[+] Me.GetQuest()
//[+] Me.GetSkillStatus()
//[+] Me.Repair()
//[+] Me.SelectNPCMenu()
//[+] Me.SetSkill()
//[+] Me.SwapWeapons()
//[+] Me.TakeWaypoint()
//[+] Me.UseBelt()

//[-] Unit
//[+] Unit.areaid				//added
//[+] Unit.classid				//added
//[+] Unit.code					//added
//[+] Unit.gid
//[+] Unit.hp					//added
//[+] Unit.hpmax				//added
//[+] Unit.itemclass			//added
//[+] Unit.itemdesc		
//[+] Unit.itemflag
//[+] Unit.itemlevel			//added
//[+] Unit.itemloc				//added
//[+] Unit.itemprefix			//added
//[+] Unit.itemsuffix			//added
//[+] Unit.itemtype				//added
//[+] Unit.mode					//added
//[+] Unit.mp					//added
//[+] Unit.mpmax				//added
//[+] Unit.name					//added
//[+] Unit.quality				//added
//[+] Unit.shrinetype
//[+] Unit.spectype
//[+] Unit.subareaid
//[+] Unit.type
//[+] Unit.x					//added
//[+] Unit.xsize
//[+] Unit.y					//added
//[+] Unit.ysize
//[+] Unit.GetItemCost()
//[+] Unit.GetItems()
//[+] Unit.GetMerc()
//[+] Unit.GetNext()
//[+] Unit.GetOptimalAttackPos()
//[+] Unit.GetParent()
//[+] Unit.GetRoom()
//[+] Unit.GetSkill()
//[+] Unit.GetStat()
//[+] Unit.GetState()
//[+] Unit.IsAttackable()

//[-] Script
//[+] Script.name
//[+] Script.gametype
//[+] Script.running
//[+] Script.GetNext()
//[+] Script.Send()
//[+] Script.Stop()

//[-] PlayerUnit
//[+] PlayerUnit.areaid
//[+] PlayerUnit.classid
//[+] PlayerUnit.gid
//[+] PlayerUnit.level
//[+] PlayerUnit.life
//[+] PlayerUnit.name
//[+] PlayerUnit.partyflag
//[+] PlayerUnit.partyid
//[+] PlayerUnit.x
//[+] PlayerUnit.y
//[+] PlayerUnit.GetNext()

//[-] PresetUnit			
//[+] PresetUnit.areaid			//added
//[+] PresetUnit.type			//added
//[+] PresetUnit.id				//added
//[+] PresetUnit.x				//added
//[+] PresetUnit.y				//added
//[+] PresetUnit.roomx			//added
//[+] PresetUnit.roomy			//added
//[+] PresetUnit.subareaid		//needs corrected

//[-] File
//[+] File.eof
//[+] File.pos
//[+] File.size
//[+] File.Close()
//[+] File.ReadLine()
//[+] File.WriteLine()

//[-] Control
//[+] Control.disabled
//[+] Control.pressed
//[+] Control.text
//[+] Control.type
//[+] Control.x
//[+] Control.xsize
//[+] Control.y
//[+] Control.ysize
//[+] Control.Click()
//[+] Control.GetNext()
//[+] Control.GetText()
//[+] Control.SetText()

//[-] Area
//[+] Area.id
//[+] Area.name
//[+] Area.x
//[+] Area.xsize
//[+] Area.y
//[+] Area.ysize

//[-] Room
//[+] Room.areaid
//[+] Room.correcttomb
//[+] Room.x
//[+] Room.xsize
//[+] Room.y
//[+] Room.ysize
//[+] Room.GetFirst()
//[+] Room.GetNearby()
//[+] Room.GetNext()
//[+] Room.GetPresetUnits()
//[+] Room.UnitInRoom()

//[-] Events
//[+] EVENT_GAMEMSG
//[+] EVENT_KEYDOWN
//[+] EVENT_KEYUP
//[+] EVENT_SCRIPTMSG