#include <smooth.h>
#include "t02.h"

SMOOTHVoid SMOOTH::Main()
{
	HelloWorld	*app = new HelloWorld();

	SMOOTH::MessageBox("Hello World!", "Message", MB_OK, IDI_INFORMATION);	// this displays a message box. The fourth parameter can be any valid Windows icon

	SMOOTH::Loop();

	delete app;
}

HelloWorld::HelloWorld()
{
	SetText("Hello World!");						// name our application "Hello World!"

	wnd	= new SMOOTHWindow("Hello World!");
	title	= new SMOOTHTitlebar(true, false, true);
	client	= new SMOOTHClient(NULLPROC);					// a client area without a PaintProc
	dbar	= new SMOOTHDivisionbar(75, OR_VERT | OR_LEFT);			// a vertical divisionbar

	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(client);
	wnd->RegisterObject(dbar);

	wnd->SetMetrics(SMOOTHPoint(80, 80), SMOOTHSize(200, 150));
}

HelloWorld::~HelloWorld()
{
	wnd->UnregisterObject(title);
	wnd->UnregisterObject(client);
	wnd->UnregisterObject(dbar);

	UnregisterObject(wnd);

	SMOOTH::DeleteObject(title);
	SMOOTH::DeleteObject(client);
	SMOOTH::DeleteObject(wnd);
	SMOOTH::DeleteObject(dbar);
}
