 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_DESIGNER
#define H_OBJSMOOTH_DESIGNER

class Designer;

#include <smooth.h>
#include "edit_component.h"
#include "status.h"

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::System;

class Designer : public Application
{
	private:
		GUI::Window			*wnd;
		Titlebar			*title;

		Menubar				*menubar;
		Menubar				*iconbar;
		Statusbar			*statusbar;

		PopupMenu			*menu_file;
		PopupMenu			*menu_dialog;
		PopupMenu			*menu_widgets;
		PopupMenu			*menu_widgets_add;
		PopupMenu			*menu_widgets_add_smooth;

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
