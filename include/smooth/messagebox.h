 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MESSAGEBOX_
#define _H_OBJSMOOTH_MESSAGEBOX_

namespace smooth
{
	class MessageBoxApp;

	namespace GUI
	{
		class Window;
		class Titlebar;
		class Layer;
		class Button;
	};
};

#include "application.h"

namespace smooth
{
	class MessageBoxApp : public Application
	{
		private:
			static Int	 nOfMessageBoxes;
			Int		 lines;
			String		 line[256];
			wchar_t		*msgicon;
			Int		 buttons;
			GUI::Window	*msgbox;
			GUI::Titlebar	*titlebar;
			GUI::Layer	*lay;
			GUI::Button	*okbutton;
			GUI::Button	*yesbutton;
			GUI::Button	*nobutton;
			GUI::Button	*abortbutton;
			GUI::Button	*cancelbutton;
			GUI::Button	*retrybutton;
			GUI::Button	*ignorebutton;

			Void		 Create(String, String, Int);
		public:
					 MessageBoxApp(String, String, Int, wchar_t *);
					~MessageBoxApp();
			Void		 MessagePaintProc();
			Bool		 MessageKillProc();

			Void		 MessageOK();
			Void		 MessageCancel();
			Void		 MessageYes();
			Void		 MessageNo();
			Void		 MessageRetry();
			Void		 MessageAbort();
			Void		 MessageIgnore();
			Int		 ShowMessageBox();
	};
};

#endif
