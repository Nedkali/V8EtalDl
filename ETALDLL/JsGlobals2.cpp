
#include "D2Structs.h"
#include "JSGlobals.h"
#include "D2Pointers.h"
#include "v8.h"
#include <Windows.h>
#include "Map\MapHeader.h"
#include <cassert>
#include <cmath>



void Displayresults()
{


}
JS_FUNC(CGetPath)
{

	Isolate* isolate = args.GetIsolate();
	HandleScope handle_scope(isolate);
	if (args.Length() < 6)
		return args.GetReturnValue().Set(Boolean::New(false));

	UINT lvl = 0, x = 0, y = 0, dx = 0, dy = 0, reductionType = 0, radius = 20;

	lvl = args[0]->Uint32Value();
	x = args[1]->Uint32Value();
	y = args[2]->Uint32Value();
	dx = args[3]->Uint32Value();
	dy = args[4]->Uint32Value();
	reductionType = args[5]->BooleanValue();

	Level* level = GetLevel(lvl);
	if (!level)
		return args.GetReturnValue().Set(Boolean::New(false));

	ActMap* map = ActMap::GetMap(level);

	Point start(x, y), end(dx, dy);
	PathReducer* reducer = NULL;

	switch (reductionType)
	{
	case 0: reducer = new WalkPathReducer(map, DiagonalShortcut, radius); MessageBox(0, "walker 0", "GetPath Debug", 0); break;
	case 1: reducer = new TeleportPathReducer(map, DiagonalShortcut, radius); MessageBox(0, "teleport 1", "GetPath Debug", 0); break;
	case 2: reducer = new NoPathReducer(map); MessageBox(0, "no reducer 2", "GetPath Debug", 0); break;
		//case 3: reducer = new JSPathReducer(map, cx, JS_THIS_OBJECT(cx, vp), JS_ARGV(cx, vp)[7], JS_ARGV(cx, vp)[8], JS_ARGV(cx, vp)[9]); break;
	default:
		MessageBox(0, "here default", "GetPath Debug", 0);
		return args.GetReturnValue().Set(Boolean::New(false));
	}


	PointList list;

	AStarPath<> path(map, reducer);

	path.GetPath(start, end, list, true);

	int count = list.size();

	Local<Array> nodes = Array::New(list.size());
	Local<Array> node = Array::New(2);
	for (int i = 0; i < count; i++)
	{
		node->Set(0, Integer::New(list[i].first));
		node->Set(1, Integer::New(list[i].second));
		nodes->Set(i, node);
	}

	//jsval* vec = new jsval[count];
	//for (int i = 0; i < count; i++)
	//{
	//	jsval jx = INT_TO_JSVAL(list[i].first),
	//		jy = INT_TO_JSVAL(list[i].second);
	//
	//	JSObject* point = BuildObject(cx);
	//	JS_SetProperty(cx, point, "x", &jx);
	//	JS_SetProperty(cx, point, "y", &jy);
	//
	//	vec[i] = OBJECT_TO_JSVAL(point);
	//}


	args.GetReturnValue().Set(nodes);
	delete reducer;
	map->CleanUp();
}

//JS_FUNC(CGetPath)
//{
//
//	Isolate* isolate = args.GetIsolate();
//	HandleScope handle_scope(isolate);
//	if (args.Length() < 6)
//		return args.GetReturnValue().Set(Boolean::New(false));
//
//	UINT lvl = 0, x = 0, y = 0, dx = 0, dy = 0, reductionType = 0, radius = 20;	
//
//	lvl = args[0]->Uint32Value();
//	x = args[1]->Uint32Value();
//	y = args[2]->Uint32Value();
//	dx = args[3]->Uint32Value();
//	dy = args[4]->Uint32Value();
//	reductionType = args[5]->BooleanValue();
//
//	Level* level = GetLevel(lvl);
//	if (!level)
//		return args.GetReturnValue().Set(Boolean::New(false));
//
//	ActMap* map = ActMap::GetMap(level);
//
//	Point start(x, y), end(dx, dy);
//	PathReducer* reducer = NULL;
//	
//	switch (reductionType)
//	{
//		case 0: reducer = new WalkPathReducer(map, DiagonalShortcut, radius); break;
//		case 1: reducer = new TeleportPathReducer(map, DiagonalShortcut, radius); MessageBox(0, "here 1", "GetPath Debug", 0); break;
//		case 2: reducer = new NoPathReducer(map); MessageBox(0, "here 2", "GetPath Debug", 0); break;
//		//case 3: reducer = new JSPathReducer(map, cx, JS_THIS_OBJECT(cx, vp), JS_ARGV(cx, vp)[7], JS_ARGV(cx, vp)[8], JS_ARGV(cx, vp)[9]); break;
//		default:
//			MessageBox(0, "here default", "GetPath Debug", 0);
//			return args.GetReturnValue().Set(Boolean::New(false));
//	}
//
//	//MessageBox(0, "Here here", "GetPath Debug", 0);
//
//	PointList list;
//	AStarPath<> path(map, reducer);
//
//	path.GetPath(start, end, list, true);
//
//	path.GetPath(start, end, list, true);
//	map->CleanUp();
//
//	int count = list.size();
//	Local<Integer> vec = Integer::New(count);
//
//	Local<Object> nodes = Object::New();
//	Local<Array> node = Array::New();
//	for (int i = 0; i < count; i++)
//	{
//		Local<Integer> jx = Integer::New(list[i].first),
//			jy = Integer::New(list[i].second);
//		node->Set(String::NewFromUtf8(isolate, "length"), Integer::New(count));
//		node->Set(String::NewFromUtf8(isolate, "x"), Local<Integer>::New(jx));
//		node->Set(String::NewFromUtf8(isolate, "y"), Local<Integer>::New(jy));
//		nodes->Set(count, node);
//	}
//		//jsval* vec = new jsval[count];
//		//for (int i = 0; i < count; i++)
//		//{
//		//	jsval jx = INT_TO_JSVAL(list[i].first),
//		//		jy = INT_TO_JSVAL(list[i].second);
//
//		//	JSObject* point = BuildObject(cx);
//		//	JS_SetProperty(cx, point, "x", &jx);
//		//	JS_SetProperty(cx, point, "y", &jy);
//
//		//	vec[i] = OBJECT_TO_JSVAL(point);
//		//}
//
//		//JSObject* arr = JS_NewArrayObject(cx, count, vec);
//		//JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(arr));
//
//	delete reducer;
//	map->CleanUp();
//	args.GetReturnValue().Set(nodes);
//
//}

