#ifndef	_H_T02_
#define _H_T02_

#include <smoothx.h>

class HelloWorld : public SMOOTHApplication
{
	private:
		SMOOTHWindow		*wnd;
		SMOOTHTitlebar		*title;
		SMOOTHClient		*client;	// the windows client area
		SMOOTHDivisionbar	*dbar;		// and a divisionbar
	public:
					 HelloWorld();
					~HelloWorld();
};

#endif
