 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DESIGNER_
#define _H_OBJSMOOTH_DESIGNER_

class Designer;

#include <smoothx.h>
#include "edit_component.h"
#include "status.h"

class Designer : public SMOOTHApplication
{
	private:
		SMOOTHWindow				*wnd;
		SMOOTHTitlebar				*title;
		SMOOTHMenubar				*menubar;
		SMOOTHMenubar				*iconbar;
		SMOOTHStatusbar				*statusbar;
		SMOOTHPopupMenu				*menu_file;
		SMOOTHPopupMenu				*menu_dialog;
		SMOOTHPopupMenu				*menu_widgets;
		SMOOTHPopupMenu				*menu_widgets_add;
		SMOOTHPopupMenu				*menu_widgets_add_smooth;

		SMOOTHTimer				*timer1;

		SMOOTHInt				 dlgcounter;
		Designer_Status				 status;
		SMOOTHArray<Designer_EditComponent *>	 dlgs;
		Designer_EditComponent			*active_dlg;

		SMOOTHVoid				 CloseWindow();
		SMOOTHVoid				 NewDialog();
		SMOOTHBool				 KillProc();
		SMOOTHVoid				 TimerProc();

		SMOOTHVoid				 AddButton();
		SMOOTHVoid				 AddLayer();
		SMOOTHVoid				 AddMenubar();
	public:
							 Designer();
							~Designer();
		SMOOTHVoid				 ReportStatus(Designer_Status);
};

#endif
