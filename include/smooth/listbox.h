 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LISTBOX_
#define _H_OBJSMOOTH_LISTBOX_

#define SListBox SMOOTHListBox

class SMOOTHListBox;
class SMOOTHScrollbar;
class SMOOTHComboBox;

#include "object.h"
#include "list.h"

class SMOOTHAPI SMOOTHListBox : public SMOOTHObject, public SMOOTHList
{
	friend class SMOOTHComboBox;
	private:
		SMOOTHInt		 entryCount;

		SMOOTHScrollbar		*scrollbar;
		SMOOTHBool		 needScrollbar;
		SMOOTHInt		 scrollbarPos;
		SMOOTHInt		 lastScrollbarPos;

		SMOOTHBool		 allowReselect;

		SMOOTHVoid		 ScrollbarProc();
	public:
					 SMOOTHListBox(SMOOTHPoint, SMOOTHSize, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHListBox();

		SMOOTHInt		 AddEntry(SMOOTHString, SMOOTHProcParam, SMOOTHVoid *);
		SMOOTHInt		 ModifyEntry(SMOOTHInt, SMOOTHString, SMOOTHProcParam, SMOOTHVoid *);
		SMOOTHInt		 RemoveEntry(SMOOTHInt);
		SMOOTHInt		 SelectEntry(SMOOTHInt);
		SMOOTHVoid		 Cleanup();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_LISTBOX;

#endif
