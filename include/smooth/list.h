 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LIST_
#define _H_OBJSMOOTH_LIST_

#define SList SMOOTHList

class SMOOTHList;

#include "object.h"

class SMOOTHAPI SMOOTHList
{
	public: class SMOOTHAPI Entry
	{
		public:
			SMOOTHInt	 code;
			SMOOTHProcMember;
			SMOOTHVoid	*procParam;
			SMOOTHString	 text;

			SMOOTHBool	 chk;
			SMOOTHBool	 clk;
			SMOOTHInt	 size;
			SMOOTHBool	 sizeset;
			SMOOTHRect	 rect;

					 Entry(SMOOTHInt);
					~Entry();
	};

	protected:
		SMOOTHBool		 entrysizesset;
		SMOOTHInt		 nOfEntries;

		SMOOTHInt		 AddListEntry(SMOOTHInt, SMOOTHString, SMOOTHProcParam, SMOOTHVoid *);
		SMOOTHInt		 ModifyListEntry(SMOOTHInt, SMOOTHString, SMOOTHProcParam, SMOOTHVoid *);
		SMOOTHInt		 RemoveListEntry(SMOOTHInt);

		SMOOTHVoid		 GetSize();
		SMOOTHVoid		 GetListEntriesSize();
	public:
		SMOOTHArray<Entry *>	 entries;

					 SMOOTHList();
					~SMOOTHList();

		SMOOTHVoid		 CleanupList();
		SMOOTHInt		 GetNOfEntries();
		SMOOTHInt		 SelectListEntry(SMOOTHInt);
		SMOOTHInt		 GetSelectedEntry();
		SMOOTHString		 GetSelectedEntryName();
};

#endif
