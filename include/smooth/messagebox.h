 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MESSAGEBOX_
#define _H_OBJSMOOTH_MESSAGEBOX_

namespace smooth
{
	class MessageBoxApp;
	class Window;
	class Titlebar;
	class Layer;
	class Button;
};

#include "application.h"

namespace smooth
{
	class MessageBoxApp : public Application
	{
		private:
			static int	 nOfMessageBoxes;
			int		 lines;
			String		 line[256];
			wchar_t		*msgicon;
			int		 buttons;
			Window		*msgbox;
			Titlebar	*titlebar;
			Layer		*lay;
			Button		*okbutton;
			Button		*yesbutton;
			Button		*nobutton;
			Button		*abortbutton;
			Button		*cancelbutton;
			Button		*retrybutton;
			Button		*ignorebutton;

			void		 Create(String, String, int);
		public:
					 MessageBoxApp(String, String, int, wchar_t *);
					~MessageBoxApp();
			void		 MessagePaintProc();
			bool		 MessageKillProc();

			void		 MessageOK();
			void		 MessageCancel();
			void		 MessageYes();
			void		 MessageNo();
			void		 MessageRetry();
			void		 MessageAbort();
			void		 MessageIgnore();
			int		 ShowMessageBox();
	};
};

#endif
