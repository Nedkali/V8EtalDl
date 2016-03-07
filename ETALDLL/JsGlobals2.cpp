
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
	case 0: reducer = new WalkPathReducer(map, DiagonalShortcut, radius); break;
	case 1: reducer = new TeleportPathReducer(map, DiagonalShortcut, radius); break;
	case 2: reducer = new NoPathReducer(map); break;
	default:
		return args.GetReturnValue().Set(Boolean::New(false));
	}

	PointList list;

	AStarPath<> path(map, reducer);

	path.GetPath(start, end, list, true);

	int count = list.size();

	Local<Array> nodes = Array::New(list.size());
	for (int i = 0; i < count; i++)
	{
		Local<Array> node = Array::New();
		node->Set(0, Integer::New(list[i].first));
		node->Set(1, Integer::New(list[i].second));
		nodes->Set(i, node);
	}
	args.GetReturnValue().Set(nodes);
	delete reducer;
	map->CleanUp();
}

