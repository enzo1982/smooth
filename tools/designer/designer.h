 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DESIGNER_
#define _H_OBJSMOOTH_DESIGNER_

class Designer;

#include <smooth.h>
#include "edit_component.h"
#include "status.h"

using namespace smooth;
using namespace smooth::GUI;

class Designer : public Application
{
	private:
		Window				*wnd;
		Titlebar			*title;
		Menubar				*menubar;
		Menubar				*iconbar;
		Statusbar			*statusbar;
		Menu				*menu_file;
		Menu				*menu_dialog;
		Menu				*menu_widgets;
		Menu				*menu_widgets_add;
		Menu				*menu_widgets_add_smooth;

		Timer				*timer1;

		Int				 dlgcounter;
		Designer_Status			 status;
		Array<Designer_EditComponent *>	 dlgs;
		Designer_EditComponent		*active_dlg;

		Void				 NewDialog();
		Bool				 ExitProc();
		Void				 TimerProc();
		Void				 Close();

		Void				 AddButton();
		Void				 AddLayer();
		Void				 AddMenubar();
	public:
						 Designer();
						~Designer();
		Void				 ReportStatus(Designer_Status);
};

#endif
