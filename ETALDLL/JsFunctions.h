//////////////////////////////////////////////////////////////////////
// JS Functions
//////////////////////////////////////////////////////////////////////
#include "v8.h"
#include "JsDefines.h"
#include "D2Pointers.h"

using namespace v8;

// **Context
Handle<Context>CreateContext(Isolate* isolate);

JS_FUNC(CLoad);
JS_FUNC(CInclude);
JS_FUNC(CSetTitle);
JS_FUNC(CSendCopyData);
JS_FUNC(CClick);
JS_FUNC(CGetTickCount);
JS_FUNC(CDelay);
JS_FUNC(CCloseD2);
JS_FUNC(CScreenSize);
JS_FUNC(CPrint);
JS_FUNC(CMove);
JS_FUNC(CSetSkill);
JS_FUNC(CExitGame);
JS_FUNC(CGetPlayerUnit);
JS_FUNC(CGetArea);
JS_FUNC(CSay);
JS_FUNC(CRandom);
JS_FUNC(CGetLevel);
JS_FUNC(CClickControl);
JS_FUNC(CGetLocation);
JS_FUNC(CSelectChar);
JS_FUNC(CGetText);
JS_FUNC(CSetText);
JS_FUNC(CClientState);
JS_FUNC(CTESTWAYPOINT);


