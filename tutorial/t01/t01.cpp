#include <smooth.h>					// smooth.h provides all SMOOTH objects
#include "t01.h"

SMOOTHVoid SMOOTH::Main()				// Every SMOOTH program starts at SMOOTHMain
{
	HelloWorld	*app = new HelloWorld();	// create an instance of our HelloWorld application

	SMOOTH::Loop();					// enter the main loop

	delete app;					// delete our application object after the program has finished
}

HelloWorld::HelloWorld()				// the constructor creates the application interface
{
	wnd	= new SMOOTHWindow("Hello World!");	// create a SMOOTHWindow named "Hello World!"
	title	= new SMOOTHTitlebar(true, false, true);// create a titlebar for that window

	RegisterObject(wnd);				// register the window at our application

	wnd->RegisterObject(title);			// register the titlebar at our window

	wnd->SetMetrics(SMOOTHPoint(80, 80), SMOOTHSize(200, 150));	// set window metrics
}

HelloWorld::~HelloWorld()				// the destructor deletes all interface objects
{
	wnd->UnregisterObject(title);			// unregister the titlebar

	UnregisterObject(wnd);				// unregister the window

	SMOOTH::DeleteObject(title);			// delete the titlebar object
	SMOOTH::DeleteObject(wnd);			// delete the window object
}
