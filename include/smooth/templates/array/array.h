 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SMOOTHARRAY_ARRAY_
#define _H_SMOOTHARRAY_ARRAY_

#define SArray SMOOTHArray

#include "entry.h"

template <class t> class SMOOTHArray
{
	private:
		long			 nOfEntries;
		long			 greatestIndex;
		bool			 outlinedEntry;
		SMOOTHArray_Entry<t>	*firstEntry;
		SMOOTHArray_Entry<t>	*lastEntry;
		SMOOTHArray_Entry<t>	*prevEntry;
		SMOOTHArray_Entry<t>	*prevDeletedEntry;
		bool			 Cleanup(SMOOTHArray_Entry<t> *);
		bool			 IndexAvailable(long);
	public:
					 SMOOTHArray();
					~SMOOTHArray();
		long			 AddEntry(t);
		bool			 AddEntry(t, long);
		long			 InsertEntryAfter(long, t);
		bool			 InsertEntryAfter(long, t, long);
		long			 InsertEntryAtPos(long, t);
		bool			 InsertEntryAtPos(long, t, long);
		bool			 DeleteEntry(long);
		bool			 DeleteAll();
		t			 GetEntry(long);
		bool			 SetEntry(long, t);
		long			 GetNOfEntries();
		t			 GetFirstEntry();
		t			 GetLastEntry();
		t			 GetNextEntry();
		t			 GetPrevEntry();
		t			 GetNthEntry(int);
};

#endif
