 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MESSAGEBOX_
#define _H_OBJSMOOTH_MESSAGEBOX_

#define SMessageBoxApp SMOOTHMessageBoxApp

class SMOOTHMessageBoxApp;
class SMOOTHWindow;
class SMOOTHTitlebar;
class SMOOTHLayer;
class SMOOTHButton;

#include "application.h"

class SMOOTHMessageBoxApp : public SMOOTHApplication
{
	private:
		static int	 nOfMessageBoxes;
		int		 lines;
		SMOOTHString	 line[256];
		wchar_t		*msgicon;
		int		 buttons;
		SMOOTHWindow	*msgbox;
		SMOOTHTitlebar	*titlebar;
		SMOOTHLayer	*lay;
		SMOOTHButton	*okbutton;
		SMOOTHButton	*yesbutton;
		SMOOTHButton	*nobutton;
		SMOOTHButton	*abortbutton;
		SMOOTHButton	*cancelbutton;
		SMOOTHButton	*retrybutton;
		SMOOTHButton	*ignorebutton;

		void		 Create(SMOOTHString, SMOOTHString, int);
	public:
				 SMOOTHMessageBoxApp(SMOOTHString, SMOOTHString, int, wchar_t *);
				~SMOOTHMessageBoxApp();
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

#endif
