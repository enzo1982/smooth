/*
 * smooth tutorial 2
 */

#include <smooth.h>
#include <smooth/main.h>

#include "t02.h"

Int smooth::Main()
{
	Hello	*app = new Hello();

	Loop();

	Object::DeleteObject(app);

	return 0;
}

Hello::Hello()
{
	mainWnd	= new Window("Hello world!", Point(100, 100), Size(200, 100));
	title	= new Titlebar(TB_CLOSEBUTTON);
	hello	= new Button("Hello world!", NIL, Point(20, 15), Size(160, 40));

	hello->onAction.Connect(&Hello::Quit, this);

	mainWnd->RegisterObject(hello);
	mainWnd->RegisterObject(title);

	RegisterObject(mainWnd);
}

Hello::~Hello()
{
	mainWnd->UnregisterObject(title);
	mainWnd->UnregisterObject(hello);

	UnregisterObject(mainWnd);

	DeleteObject(mainWnd);
	DeleteObject(title);
	DeleteObject(hello);
}

Void Hello::Quit()
{
	mainWnd->Close();
}
