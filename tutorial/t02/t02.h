/*
 * smooth tutorial 1
 */

#include <smooth.h>

using namespace smooth;

class Hello : public Application
{
	private:
		Window		*mainWnd;
		Titlebar	*title;
		Button		*hello;
	public:
				 Hello();
				~Hello();
	slots:
		Void		 Quit();
};
