/*
 * smooth tutorial 1
 */

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

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
