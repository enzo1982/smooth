 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_ARRAY_BACKEND_
#define __OBJSMOOTH_ARRAY_BACKEND_

#include "array.h"
#include "entry.h"

template <class s> s S::Array_Backend<s>::nullValue = ARRAY_NULLVALUE;

template <class s> S::Array_Backend<s>::Array_Backend()
{
	nOfEntries		= 0;
	greatestIndex		= -1;
	outlinedEntry		= False;

	firstEntry		= ARRAY_NULLPOINTER;
	lastEntry		= ARRAY_NULLPOINTER;
	prevAccessedEntry	= ARRAY_NULLPOINTER;

	lastN			= -1024;
	lastNthEntry		= ARRAY_NULLPOINTER;
}

template <class s> S::Array_Backend<s>::~Array_Backend()
{
	RemoveAll();
}

template <class s> S::Bool S::Array_Backend<s>::IndexAvailable(Int index) const
{
	if (index > greatestIndex) return True;

	if (GetArrayEntryByIndex(index) == ARRAY_NULLPOINTER)	return True;
	else							return False;
}

template <class s> S::Array_Entry<s> *S::Array_Backend<s>::GetArrayEntryByIndex(Int index) const
{
	if (nOfEntries == 0) return ARRAY_NULLPOINTER;

	if (prevAccessedEntry != ARRAY_NULLPOINTER)
	{
		if (prevAccessedEntry->GetNext() != ARRAY_NULLPOINTER)
		{
			if (prevAccessedEntry->GetNext()->GetIndex() == index)
			{
				prevAccessedEntry = prevAccessedEntry->GetNext();

				return prevAccessedEntry;
			}
		}

		if (prevAccessedEntry->GetPrev() != ARRAY_NULLPOINTER)
		{
			if (prevAccessedEntry->GetPrev()->GetIndex() == index)
			{
				prevAccessedEntry = prevAccessedEntry->GetPrev();

				return prevAccessedEntry;
			}
		}
	}

	Array_Entry<s>	*entry = firstEntry;

	for (Int i = 0; i < nOfEntries; i++)
	{
		if (entry->GetIndex() == index)
		{
			prevAccessedEntry = entry;

			return entry;
		}

		entry = entry->GetNext();
	}

	return ARRAY_NULLPOINTER;
}

template <class s> S::Array_Entry<s> *S::Array_Backend<s>::GetNthArrayEntry(Int n) const
{
	if (n >= nOfEntries) return ARRAY_NULLPOINTER;

	Array_Entry<s>	*entry = ARRAY_NULLPOINTER;

	if (lastNthEntry == prevAccessedEntry && prevAccessedEntry != ARRAY_NULLPOINTER)
	{
		if (n == lastN)			entry = prevAccessedEntry;
		else if (n == lastN + 1)	entry = prevAccessedEntry->GetNext();
		else if (n == lastN - 1)	entry = prevAccessedEntry->GetPrev();
	}

	if (entry == ARRAY_NULLPOINTER)
	{
		if (n < nOfEntries - n - 1)
		{
			entry = firstEntry;

			for (Int i = 0; i < n; i++) entry = entry->GetNext();
		}
		else
		{
			entry = lastEntry;

			for (Int i = 0; i < nOfEntries - n - 1; i++) entry = entry->GetPrev();
		}
	}

	lastN			= n;
	lastNthEntry		= entry;
	prevAccessedEntry	= entry;

	return entry;
}

template <class s> S::Int S::Array_Backend<s>::AddEntry(const s &value)
{
	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	if (nOfEntries == 0)
	{
		outlinedEntry = False;

		firstEntry = new Array_Entry<s>(value);
		firstEntry->SetIndex(nOfEntries);

		lastEntry	= firstEntry;
		greatestIndex	= nOfEntries;
	}
	else
	{
		lastEntry->SetNext(new Array_Entry<s>(value));
		lastEntry->GetNext()->SetPrev(lastEntry);

		lastEntry = lastEntry->GetNext();

		if (outlinedEntry)
		{
			lastEntry->SetIndex(++greatestIndex);
		}
		else
		{
			lastEntry->SetIndex(nOfEntries);

			greatestIndex = nOfEntries;
		}
	}

	nOfEntries++;

	prevAccessedEntry = lastEntry;

	return lastEntry->GetIndex();
}

template <class s> S::Bool S::Array_Backend<s>::AddEntry(const s &value, Int index)
{
	if (!IndexAvailable(index)) return False;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	AddEntry(value);

	prevAccessedEntry->SetIndex(index);

	if (index >= greatestIndex)	greatestIndex = index;
	else				greatestIndex--;

	outlinedEntry = True;

	return True;
}

template <class s> S::Int S::Array_Backend<s>::InsertEntryAfter(Int prevIndex, const s &value)
{
	if (GetArrayEntryByIndex(prevIndex) == ARRAY_NULLPOINTER) return False;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	Array_Entry<s>	*entry = new Array_Entry<s>(value);

	entry->SetNext(prevAccessedEntry->GetNext());
	prevAccessedEntry->SetNext(entry);
	entry->SetPrev(prevAccessedEntry);

	if (prevAccessedEntry == lastEntry)	lastEntry = entry;
	else					entry->GetNext()->SetPrev(entry);

	entry->SetIndex(++greatestIndex);

	prevAccessedEntry = entry;

	nOfEntries++;

	outlinedEntry = True;

	return entry->GetIndex();
}

template <class s> S::Bool S::Array_Backend<s>::InsertEntryAfter(Int prevIndex, const s &value, Int index)
{
	if (GetArrayEntryByIndex(prevIndex) == ARRAY_NULLPOINTER) return False;
	if (!IndexAvailable(index))				  return False;

	InsertEntryAfter(prevIndex, value);

	prevAccessedEntry->SetIndex(index);

	if (index >= greatestIndex)	greatestIndex = index;
	else				greatestIndex--;

	return True;
}

template <class s> S::Int S::Array_Backend<s>::InsertEntryAtPos(Int position, const s &value)
{
	if (nOfEntries < position) return False;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	Array_Entry<s>	*entry = new Array_Entry<s>(value);

	if (position == 0)
	{
		if (firstEntry != ARRAY_NULLPOINTER)
		{
			entry->SetNext(firstEntry);
			firstEntry->SetPrev(entry);
		}

		firstEntry = entry;
	}
	else
	{
		GetNthArrayEntry(position - 1);

		entry->SetNext(prevAccessedEntry->GetNext());
		prevAccessedEntry->SetNext(entry);
		entry->SetPrev(prevAccessedEntry);

		if (nOfEntries > position) entry->GetNext()->SetPrev(entry);
	}

	if (nOfEntries == position) lastEntry = entry;

	entry->SetIndex(++greatestIndex);

	prevAccessedEntry = entry;

	nOfEntries++;

	outlinedEntry = True;

	return entry->GetIndex();
}

template <class s> S::Bool S::Array_Backend<s>::InsertEntryAtPos(Int position, const s &value, Int index)
{
	if (nOfEntries < position)	return False;
	if (!IndexAvailable(index))	return False;

	InsertEntryAtPos(position, value);

	prevAccessedEntry->SetIndex(index);

	if (index >= greatestIndex)	greatestIndex = index;
	else				greatestIndex--;

	return True;
}

template <class s> S::Bool S::Array_Backend<s>::RemoveEntry(Int index)
{
	Array_Entry<s>	*entry = GetArrayEntryByIndex(index);

	if (entry == ARRAY_NULLPOINTER) return False;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	if (entry->GetNext() != ARRAY_NULLPOINTER && entry->GetPrev() != ARRAY_NULLPOINTER)
	{
		entry->GetPrev()->SetNext(entry->GetNext());
		entry->GetNext()->SetPrev(entry->GetPrev());
	}
	else if (entry->GetNext() != ARRAY_NULLPOINTER && entry->GetPrev() == ARRAY_NULLPOINTER)
	{
		firstEntry = entry->GetNext();
		entry->GetNext()->SetPrev(ARRAY_NULLPOINTER);
	}
	else if (entry->GetNext() == ARRAY_NULLPOINTER && entry->GetPrev() != ARRAY_NULLPOINTER)
	{
		entry->GetPrev()->SetNext(ARRAY_NULLPOINTER);
		lastEntry = entry->GetPrev();
	}
	else if (entry->GetNext() == ARRAY_NULLPOINTER && entry->GetPrev() == ARRAY_NULLPOINTER)
	{
		firstEntry = ARRAY_NULLPOINTER;
		lastEntry = ARRAY_NULLPOINTER;
	}

	if (entry->GetNext() != ARRAY_NULLPOINTER)	prevAccessedEntry = entry->GetNext();
	else if (entry->GetPrev() != ARRAY_NULLPOINTER)	prevAccessedEntry = entry->GetPrev();

	delete entry;

	nOfEntries--;

	outlinedEntry = True;

	return True;
}

template <class s> S::Bool S::Array_Backend<s>::RemoveAll()
{
	while (firstEntry != ARRAY_NULLPOINTER) RemoveEntry(firstEntry->GetIndex());

	nOfEntries		= 0;
	greatestIndex		= -1;
	outlinedEntry		= False;

	firstEntry		= ARRAY_NULLPOINTER;
	lastEntry		= ARRAY_NULLPOINTER;
	prevAccessedEntry	= ARRAY_NULLPOINTER;

	lastN			= -1024;
	lastNthEntry		= ARRAY_NULLPOINTER;

	return True;
}

template <class s> const s &S::Array_Backend<s>::GetEntry(Int index) const
{
	Array_Entry<s>	*entry = GetArrayEntryByIndex(index);

	if (entry != ARRAY_NULLPOINTER) return entry->GetValue();

	return nullValue;
}

template <class s> S::Bool S::Array_Backend<s>::SetEntry(Int index, const s &value)
{
	Array_Entry<s>	*entry = GetArrayEntryByIndex(index);

	if (entry != ARRAY_NULLPOINTER) return entry->SetValue(value);

	return False;
}

template <class s> const s &S::Array_Backend<s>::GetFirstEntry() const
{
	if (nOfEntries > 0 && firstEntry != ARRAY_NULLPOINTER)
	{
		prevAccessedEntry = firstEntry;

		return firstEntry->GetValue();
	}

	return nullValue;
}

template <class s> const s &S::Array_Backend<s>::GetLastEntry() const
{
	if (nOfEntries > 0 && lastEntry != ARRAY_NULLPOINTER)
	{
		prevAccessedEntry = lastEntry;

		return lastEntry->GetValue();
	}

	return nullValue;
}

template <class s> const s &S::Array_Backend<s>::GetNextEntry() const
{
	if (prevAccessedEntry != ARRAY_NULLPOINTER)
	{
		if (prevAccessedEntry->GetNext() != ARRAY_NULLPOINTER)
		{
			prevAccessedEntry = prevAccessedEntry->GetNext();

			return prevAccessedEntry->GetValue();
		}
	}

	return nullValue;
}

template <class s> const s &S::Array_Backend<s>::GetPrevEntry() const
{
	if (prevAccessedEntry != ARRAY_NULLPOINTER)
	{
		if (prevAccessedEntry->GetPrev() != ARRAY_NULLPOINTER)
		{
			prevAccessedEntry = prevAccessedEntry->GetPrev();

			return prevAccessedEntry->GetValue();
		}
	}

	return nullValue;
}

template <class s> const s &S::Array_Backend<s>::GetNthEntry(Int n) const
{
	Array_Entry<s>	*entry = GetNthArrayEntry(n);

	if (entry != ARRAY_NULLPOINTER) return entry->GetValue();

	return nullValue;
}

template <class s> S::Int S::Array_Backend<s>::GetNthEntryIndex(Int n) const
{
	Array_Entry<s>	*entry = GetNthArrayEntry(n);

	if (entry != ARRAY_NULLPOINTER) return entry->GetIndex();

	return -1;
}

#endif
