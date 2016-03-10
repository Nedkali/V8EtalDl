#pragma once
#include "JSGlobals.h"

//Me Methods
JS_METHOD(meSetSkill);
JS_METHOD(meCancel);
JS_METHOD(meRepair);
JS_METHOD(meGetState);
JS_METHOD(meGetQuest);
JS_METHOD(meSwapWeapons);
JS_METHOD(meGetStat);
JS_METHOD(meSelectNPCMenu);
JS_METHOD(meTakeWayPoint);
JS_METHOD(meClickItem);
JS_METHOD(meGetItems);



//Unit Methods
JS_METHOD(unitGetStat);
JS_METHOD(unitGetNext);
JS_METHOD(unitGetState);
JS_METHOD(unitGetItems);


//Room Methods
JS_METHOD(roomGetFirst);
JS_METHOD(roomGetNearby);
JS_METHOD(roomGetNext);
JS_METHOD(roomGetPresetUnits);
JS_METHOD(roomUnitInRoom);