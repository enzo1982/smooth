#ifndef	_H_T01_
#define _H_T01_

#include <smoothx.h>				// include smoothx.h instead of smooth.h in headers

class HelloWorld : public SMOOTHApplication	// every application class derives from SMOOTHApplication
{
	private:
		SMOOTHWindow	*wnd;		// this will be our main window
		SMOOTHTitlebar	*title;		// this will be the windows titlebar
	public:
				 HelloWorld();	// every application needs a constructor
				~HelloWorld();	// and a destructor
};

#endif
