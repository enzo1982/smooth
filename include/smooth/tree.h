 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TREE_
#define _H_OBJSMOOTH_TREE_

#define STree SMOOTHTree

class SMOOTHTree;

#include "object.h"

const SMOOTHInt ST_ENTRY	= 0;
const SMOOTHInt ST_SUB		= 1;

class SMOOTHAPI SMOOTHTree
{
	public: class SMOOTHAPI Entry
	{
		public:
			SMOOTHInt	 type;
			SMOOTHInt	 code;
			SMOOTHBool	 last;

			SMOOTHProcMember;
			SMOOTHVoid	*procParam;

			SMOOTHString	 text;
			SMOOTHTree	*sub;

			SMOOTHBool	 chk;
			SMOOTHBool	 clk;
			SMOOTHBool	 open;
			SMOOTHInt	 size;
			SMOOTHBool	 sizeset;
			SMOOTHRect	 rect;

					 Entry(SMOOTHInt, SMOOTHInt);
					~Entry();
	};

	protected:
		SMOOTHSize		 subsize;
		SMOOTHBool		 sizeset;
		SMOOTHBool		 entrysizesset;
		SMOOTHInt		 nOfEntries;
		SMOOTHInt		 counter;
		SMOOTHVoid		 GetSize();
		SMOOTHInt		 GetSizeY();
		SMOOTHInt		 GetSizeX();
		SMOOTHVoid		 GetTreeEntriesSize();
		SMOOTHInt		 AddTreeEntry(SMOOTHInt, SMOOTHInt, SMOOTHString, SMOOTHProcParam, SMOOTHVoid *, SMOOTHTree *);
		SMOOTHInt		 RemoveTreeEntry(SMOOTHInt);
	public:
		SMOOTHArray<Entry *>	 entries;

					 SMOOTHTree();
		virtual			~SMOOTHTree();

		virtual SMOOTHInt	 AddEntry(SMOOTHString, SMOOTHProcParam, SMOOTHVoid *);
		virtual SMOOTHInt	 AddEntry(SMOOTHString, SMOOTHTree *);
		virtual SMOOTHInt	 RemoveEntry(SMOOTHInt);
		SMOOTHInt		 GetNOfEntries();
};

#endif
