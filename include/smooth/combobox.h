 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_COMBOBOX_
#define _H_OBJSMOOTH_COMBOBOX_

#define SComboBox SMOOTHComboBox

class SMOOTHComboBox;
class SMOOTHLayer;
class SMOOTHToolWindow;
class SMOOTHListBox;

#include "object.h"
#include "list.h"

class SMOOTHAPI SMOOTHComboBox : public SMOOTHObject, public SMOOTHList
{
	private:
		SMOOTHInt		 entryCount;

		SMOOTHToolWindow	*toolWindow;
		SMOOTHLayer		*layer;
		SMOOTHListBox		*listBox;

		SMOOTHBool		 listBoxOpen;
		SMOOTHBool		 closeListBox;

		SMOOTHVoid		 ListBoxProc();
	public:
					 SMOOTHComboBox(SMOOTHPoint, SMOOTHSize, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHComboBox();

		SMOOTHInt		 AddEntry(SMOOTHString, SMOOTHProcParam, SMOOTHVoid *);
		SMOOTHInt		 ModifyEntry(SMOOTHInt, SMOOTHString, SMOOTHProcParam, SMOOTHVoid *);
		SMOOTHInt		 RemoveEntry(SMOOTHInt);
		SMOOTHInt		 SelectEntry(SMOOTHInt);
		SMOOTHVoid		 Cleanup();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_COMBOBOX;

#endif
