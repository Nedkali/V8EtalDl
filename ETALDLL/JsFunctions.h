//////////////////////////////////////////////////////////////////////
// JS Functions
//////////////////////////////////////////////////////////////////////
#include "v8.h"
#include "JsDefines.h"
#include "D2Pointers.h"

using namespace v8;

// **Context
Handle<Context>CreateContext(Isolate* isolate);

JS_FUNC(CInclude);
JS_FUNC(CSetTitle);
JS_FUNC(CSendCopyData);
JS_FUNC(CClick);
JS_FUNC(CGetTickCount);
JS_FUNC(CDelay);
JS_FUNC(CCloseD2);
JS_FUNC(CScreenSize);


