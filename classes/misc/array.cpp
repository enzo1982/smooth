 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/array.h>

S::IndexArray::IndexArray()
{
	nOfEntries	  = 0;
	greatestIndex	  = 0;

	sorted		  = True;

	lastAccessedEntry = 0;

	lockingEnabled	  = False;
	lock		  = NIL;
}

S::IndexArray::~IndexArray()
{
	if (lock != NIL)
	{
		delete lock;

		lock = NIL;
	}
}

S::Bool S::IndexArray::InsertAtPos(Int position, Int index)
{
	if (position > nOfEntries || position < 0) return False;

	if (!IndexAvailable(index)) return False;

	LockForWrite();

	if (greatestIndex < index) greatestIndex = index;

	if (indices.Size() == nOfEntries) indices.Resize(8 > nOfEntries * 1.25 ? 8 : nOfEntries * 1.25);

	memmove(indices + position + 1, indices + position, (nOfEntries - position) * sizeof(Int));

	indices[position] = index;

	nOfEntries++;

	if ((position >=	      1 && indices[position - 1] > index) ||
	    (position <  nOfEntries - 1 && indices[position + 1] < index)) sorted = False;

	Unlock();

	return True;
}

S::Bool S::IndexArray::Move(Int index1, Int index2)
{
	if (index1 > greatestIndex ||
	    index2 > greatestIndex) return False;

	return MoveNth(GetEntryNumberByIndex(index1), GetEntryNumberByIndex(index2));
}

S::Bool S::IndexArray::MoveNth(Int n, Int m)
{
	if (nOfEntries <= n || n < 0 ||
	    nOfEntries <= m || m < 0) return False;

	LockForWrite();

	Int	 backupIndex = indices[n];

	if (m < n) memmove(indices + m + 1, indices + m, (n - m) * sizeof(Int));
	else	   memmove(indices + n, indices + n + 1, (m - n) * sizeof(Int));

	indices[m] = backupIndex;

	sorted = False;

	Unlock();

	return True;
}

S::Bool S::IndexArray::Remove(Int index)
{
	if (index > greatestIndex) return False;

	return RemoveNth(GetEntryNumberByIndex(index));
}

S::Bool S::IndexArray::RemoveNth(Int n)
{
	if (nOfEntries <= n || n < 0) return False;

	if (nOfEntries == 1) return RemoveAll();

	LockForWrite();

	memmove(indices + n, indices + n + 1, (nOfEntries - n - 1) * sizeof(Int));

	nOfEntries--;

	Unlock();

	return True;
}

S::Bool S::IndexArray::RemoveAll()
{
	if (nOfEntries == 0) return True;

	LockForWrite();

	indices.Free();

	nOfEntries	  = 0;
	greatestIndex	  = 0;

	sorted		  = True;

	lastAccessedEntry = 0;

	Unlock();

	return True;
}

S::Int S::IndexArray::GetNthIndex(Int n) const
{
	LockForRead();

	if (nOfEntries > n && n >= 0)
	{
		Int	 index = indices[n];

		Unlock();

		return index;
	}

	Unlock();

	return -1;
}

S::Bool S::IndexArray::EnableLocking() const
{
	lockingEnabled = True;

	if (lock == NIL) lock = new Threads::RWLock();

	return True;
}

S::Bool S::IndexArray::DisableLocking() const
{
	lockingEnabled = False;

	return True;
}

S::Bool S::IndexArray::IndexAvailable(Int index) const
{
	if (index > greatestIndex) return True;

	if (GetEntryNumberByIndex(index) == -1) return True;
	else					return False;
}

S::Int S::IndexArray::GetEntryNumberByIndex(Int index) const
{
	if (nOfEntries == 0) return -1;

	LockForRead();

	Int	 entryNumber  = -1;
	Int	 lastAccessed = lastAccessedEntry;

	/* Check entries around last accessed entry.
	 */
	if	(lastAccessed     <  nOfEntries && indices[lastAccessed    ] == index) entryNumber = lastAccessed;
	else if (lastAccessed     >  0 &&
		 lastAccessed     <= nOfEntries && indices[lastAccessed - 1] == index) entryNumber = lastAccessed - 1;
	else if (lastAccessed + 1 <  nOfEntries && indices[lastAccessed + 1] == index) entryNumber = lastAccessed + 1;

	/* Binary search for sorted lists.
	 */
	else if (sorted)
	{
		Int	 bottom = 0;
		Int	 top	= nOfEntries - 1;

		while (top >= bottom)
		{
			Int	 i = (top + bottom) / 2;
			Int	 e = indices[i];

			if (e == index) { entryNumber = i; break; }

			if (e < index) bottom = i + 1;
			else	       top    = i - 1;
		}
	}

	/* Linear search otherwise.
	 */
	else
	{
		for (Int i = 0; i < nOfEntries; i++)
		{
			if (indices[i] == index) { entryNumber = i; break; }
		}
	}

	if (entryNumber >= 0) lastAccessedEntry = entryNumber;

	Unlock();

	return entryNumber;
}
