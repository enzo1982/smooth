/*
 * smooth tutorial 1
 */

#include <smooth/main.h>
#include "t01.h"

Int smooth::Main()
{
	Hello	*app = new Hello();

	Loop();

	Object::DeleteObject(app);

	return 0;
}

Hello::Hello()
{
	mainWnd	= new Window();
	title	= new Titlebar(TB_CLOSEBUTTON);
	hello	= new Button("Hello, World!", NIL, Point(20, 15), Size(160, 40));

	mainWnd->RegisterObject(hello);
	mainWnd->RegisterObject(title);

	RegisterObject(mainWnd);

	mainWnd->SetMetrics(Point(100, 100), Size(200, 100));
}

Hello::~Hello()
{
	mainWnd->UnregisterObject(title);
	mainWnd->UnregisterObject(hello);

	DeleteObject(mainWnd);
	DeleteObject(title);
	DeleteObject(hello);
}
