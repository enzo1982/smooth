 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_ARRAY_
#define __OBJSMOOTH_ARRAY_

#include "array.h"
#include "entry.h"

template <class t> S::Array<t>::Array()
{
	nOfEntries	= 0;
	greatestIndex	= -1;
	outlinedEntry	= False;

	firstEntry	= ARRAY_NULLPOINTER;
	lastEntry	= ARRAY_NULLPOINTER;
	prevEntry	= ARRAY_NULLPOINTER;
	prevDeletedEntry= ARRAY_NULLPOINTER;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;
}

template <class t> S::Array<t>::~Array()
{
	RemoveAll();
}

template <class t> S::Bool S::Array<t>::IndexAvailable(Int index)
{
	if (GetEntry(index) == ARRAY_NULLVALUE)	return True;
	else					return False;
}

template <class t> S::Int S::Array<t>::AddEntry(t entry)
{
	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	if (nOfEntries == 0)
	{
		outlinedEntry = False;

		firstEntry = new Array_Entry<t>;

		firstEntry->Fill(entry);
		firstEntry->SetIndex(nOfEntries);

		lastEntry	= firstEntry;
		greatestIndex	= nOfEntries;
	}
	else
	{
		lastEntry->SetNext(new Array_Entry<t>);
		lastEntry->GetNext()->SetPrev(lastEntry);

		lastEntry = lastEntry->GetNext();

		lastEntry->Fill(entry);

		if (outlinedEntry)
		{
			lastEntry->SetIndex(greatestIndex + 1);

			greatestIndex++;
		}
		else
		{
			lastEntry->SetIndex(nOfEntries);

			greatestIndex = nOfEntries;
		}
	}

	nOfEntries++;

	prevEntry = lastEntry;

	return lastEntry->GetIndex();
}

template <class t> S::Bool S::Array<t>::AddEntry(t entry, Int index)
{
	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	if (index > greatestIndex)
	{
		AddEntry(entry);

		prevEntry->SetIndex(index);

		outlinedEntry = True;
		greatestIndex = index;
	}
	else
	{
		if (!IndexAvailable(index)) return False;

		AddEntry(entry);

		prevEntry->SetIndex(index);

		if (outlinedEntry) greatestIndex--;

		outlinedEntry = True;
	}

	return True;
}

template <class t> S::Int S::Array<t>::InsertEntryAfter(Int prev, t entry)
{
	if (GetEntry(prev) == ARRAY_NULLVALUE) return -1;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	Array_Entry<t>	*operat = new Array_Entry<t>;

	operat->SetNext(prevEntry->GetNext());
	prevEntry->SetNext(operat);
	operat->SetPrev(prevEntry);

	if (prevEntry == lastEntry)	lastEntry = operat;
	else				operat->GetNext()->SetPrev(operat);

	operat->Fill(entry);

	outlinedEntry = True;

	operat->SetIndex(greatestIndex + 1);
	greatestIndex++;

	nOfEntries++;

	prevEntry = operat;

	return operat->GetIndex();
}

template <class t> S::Bool S::Array<t>::InsertEntryAfter(Int prev, t entry, Int index)
{
	if (GetEntry(prev) == ARRAY_NULLVALUE) return -1;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	if (index > greatestIndex)
	{
		InsertEntryAfter(prev, entry);

		prevEntry->SetIndex(index);

		outlinedEntry = True;
		greatestIndex = index;
	}
	else
	{
		if (!IndexAvailable(index)) return False;

		InsertEntryAfter(prev, entry);

		prevEntry->SetIndex(index);

		greatestIndex--;
	}

	return True;
}

template <class t> S::Int S::Array<t>::InsertEntryAtPos(Int pos, t entry)
{
	if (nOfEntries < pos) return -1;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	Array_Entry<t>	*operat = new Array_Entry<t>;

	if (pos == 0)
	{
		if (firstEntry != ARRAY_NULLPOINTER)
		{
			operat->SetNext(firstEntry);
			firstEntry->SetPrev(operat);
		}

		firstEntry = operat;
	}
	else
	{
		GetNthEntry(pos - 1);

		operat->SetNext(prevEntry->GetNext());
		prevEntry->SetNext(operat);
		operat->SetPrev(prevEntry);

		if (nOfEntries > pos) operat->GetNext()->SetPrev(operat);
	}

	if (nOfEntries == pos) lastEntry = operat;

	operat->Fill(entry);

	outlinedEntry = True;

	operat->SetIndex(greatestIndex + 1);
	greatestIndex++;

	nOfEntries++;

	prevEntry = operat;

	return operat->GetIndex();
}

template <class t> S::Bool S::Array<t>::InsertEntryAtPos(Int pos, t entry, Int index)
{
	if (nOfEntries < pos) return -1;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	if (index > greatestIndex)
	{
		InsertEntryAtPos(pos, entry);
		prevEntry->SetIndex(index);

		outlinedEntry = True;
		greatestIndex = index;
	}
	else
	{
		if (!IndexAvailable(index)) return False;

		InsertEntryAtPos(pos, entry);
		prevEntry->SetIndex(index);

		greatestIndex--;
	}

	return True;
}

template <class t> S::Bool S::Array<t>::RemoveEntry(Int index)
{
	if (nOfEntries == 0) return False;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	Array_Entry<t>	*operat	= ARRAY_NULLPOINTER;
	Array_Entry<t>	*prev	= ARRAY_NULLPOINTER;

	if (prevDeletedEntry != ARRAY_NULLPOINTER)
	{
		if (prevDeletedEntry->GetNext() != ARRAY_NULLPOINTER)
		{
			if (prevDeletedEntry->GetNext()->GetIndex() == index)
			{
				operat = prevDeletedEntry->GetNext();

				if (operat->GetNext() == ARRAY_NULLPOINTER)
				{
					prevDeletedEntry->SetNext(ARRAY_NULLPOINTER);
					lastEntry = prevDeletedEntry;
				}
				else
				{
					prevDeletedEntry->SetNext(operat->GetNext());
					operat->GetNext()->SetPrev(prevDeletedEntry);
				}

				if (operat == prevEntry) prevEntry = ARRAY_NULLPOINTER;

				delete operat;

				nOfEntries--;
				outlinedEntry = True;

				return True;
			}
		}

		if (prevDeletedEntry->GetPrev() != ARRAY_NULLPOINTER)
		{
			if (prevDeletedEntry->GetPrev()->GetIndex() == index)
			{
				operat = prevDeletedEntry->GetPrev();

				if (operat->GetPrev() == ARRAY_NULLPOINTER)
				{
					prevDeletedEntry->SetPrev(ARRAY_NULLPOINTER);
					firstEntry = prevDeletedEntry;
				}
				else
				{
					prevDeletedEntry->SetPrev(operat->GetPrev());
					operat->GetPrev()->SetNext(prevDeletedEntry);
				}

				if (operat == prevEntry) prevEntry = ARRAY_NULLPOINTER;

				delete operat;

				nOfEntries--;
				outlinedEntry = True;

				return True;
			}
		}
	}

	for (Int i = 0; ; i++)
	{
		if (i == 0)
		{
			operat = firstEntry;
		}
		else if (operat->GetNext() != ARRAY_NULLPOINTER)	
		{
			prev	= operat;
			operat	= operat->GetNext();
		}
		else
		{
			break;
		}

		if (operat->GetIndex() == index)
		{
			if (operat->GetNext() != ARRAY_NULLPOINTER && prev != ARRAY_NULLPOINTER)
			{
				prev->SetNext(operat->GetNext());
				operat->GetNext()->SetPrev(prev);
			}
			else if (operat->GetNext() != ARRAY_NULLPOINTER && prev == ARRAY_NULLPOINTER)
			{
				firstEntry = operat->GetNext();
				operat->GetNext()->SetPrev(ARRAY_NULLPOINTER);
			}
			else if (operat->GetNext() == ARRAY_NULLPOINTER && prev != ARRAY_NULLPOINTER)
			{
				prev->SetNext(ARRAY_NULLPOINTER);
				lastEntry = prev;
			}

			if (operat == prevEntry) prevEntry = ARRAY_NULLPOINTER;
			if (operat->GetNext() == ARRAY_NULLPOINTER && operat == firstEntry) firstEntry = ARRAY_NULLPOINTER;
			if (operat == prevDeletedEntry) prevDeletedEntry = ARRAY_NULLPOINTER;

			prevDeletedEntry = prev;

			delete operat;

			nOfEntries--;
			outlinedEntry = True;

			return True;
		}
	}

	return False;
}

template <class t> S::Bool S::Array<t>::RemoveAll()
{
	if (nOfEntries == 0) return False;

	while (firstEntry != ARRAY_NULLPOINTER) RemoveEntry(firstEntry->GetIndex());

	nOfEntries	= 0;
	greatestIndex	= -1;
	outlinedEntry	= False;

	firstEntry	= ARRAY_NULLPOINTER;
	lastEntry	= ARRAY_NULLPOINTER;
	prevEntry	= ARRAY_NULLPOINTER;
	prevDeletedEntry= ARRAY_NULLPOINTER;

	lastN		= -1024;
	lastNthEntry	= ARRAY_NULLPOINTER;

	return True;
}

template <class t> t S::Array<t>::GetEntry(Int index)
{
	if (nOfEntries == 0) return ARRAY_NULLVALUE;

	Array_Entry<t>	*operat = ARRAY_NULLPOINTER;

	if (prevEntry != ARRAY_NULLPOINTER)
	{
		if (prevEntry->GetNext() != ARRAY_NULLPOINTER)
		{
			if (prevEntry->GetNext()->GetIndex() == index)
			{
				prevEntry = prevEntry->GetNext();

				return prevEntry->GetValue();
			}
		}

		if (prevEntry->GetPrev() != ARRAY_NULLPOINTER)
		{
			if (prevEntry->GetPrev()->GetIndex() == index)
			{
				prevEntry = prevEntry->GetPrev();

				return prevEntry->GetValue();
			}
		}
	}

	for (Int i = 0; ; i++)
	{
		if (i == 0)
		{
			operat = firstEntry;
		}
		else
		{
			if (operat->GetIndex() == index)
			{
				prevEntry = operat;
				return operat->GetValue();
			}

			if (operat->GetNext() != ARRAY_NULLPOINTER)	operat = operat->GetNext();
			else						break;
		}
	}

	return ARRAY_NULLVALUE;
}

template <class t> S::Bool S::Array<t>::SetEntry(Int index, t entry)
{
	if (nOfEntries == 0) return False;

	Array_Entry<t>	*operat = ARRAY_NULLPOINTER;

	if (prevEntry != ARRAY_NULLPOINTER)
	{
		if (prevEntry->GetNext() != ARRAY_NULLPOINTER)
		{
			if (prevEntry->GetNext()->GetIndex() == index)
			{
				prevEntry = prevEntry->GetNext();

				prevEntry->Fill(entry);

				return True;
			}
		}

		if (prevEntry->GetPrev() != ARRAY_NULLPOINTER)
		{
			if (prevEntry->GetPrev()->GetIndex() == index)
			{
				prevEntry = prevEntry->GetPrev();

				prevEntry->Fill(entry);

				return True;
			}
		}
	}

	for (Int i = 0; ; i++)
	{
		if (i == 0)
		{
			operat = firstEntry;
		}
		else
		{
			if (operat->GetIndex() == index)
			{
				prevEntry = operat;
				operat->Fill(entry);
				return True;
			}

			if (operat->GetNext() != ARRAY_NULLPOINTER)	operat = operat->GetNext();
			else						break;
		}
	}

	return False;
}

template <class t> t S::Array<t>::GetFirstEntry()
{
	if (nOfEntries > 0 && firstEntry != ARRAY_NULLPOINTER)
	{
		prevEntry = firstEntry;

		return firstEntry->GetValue();
	}
	else	return ARRAY_NULLVALUE;
}

template <class t> t S::Array<t>::GetLastEntry()
{
	if (nOfEntries > 0 && lastEntry != ARRAY_NULLPOINTER)
	{
		prevEntry = lastEntry;

		return lastEntry->GetValue();
	}
	else	return ARRAY_NULLVALUE;
}

template <class t> t S::Array<t>::GetNextEntry()
{
	Array_Entry<t>	*ret;

	if (prevEntry != ARRAY_NULLPOINTER)
	{
		if (prevEntry->GetNext() != ARRAY_NULLPOINTER)
		{
			ret = prevEntry->GetNext();

			prevEntry = ret;

			return ret->GetValue();
		}
	}

	return ARRAY_NULLVALUE;
}

template <class t> t S::Array<t>::GetPrevEntry()
{
	Array_Entry<t>	*ret;

	if (prevEntry != ARRAY_NULLPOINTER)
	{
		if (prevEntry->GetPrev() != ARRAY_NULLPOINTER)
		{
			ret = prevEntry->GetPrev();

			prevEntry = ret;

			return ret->GetValue();
		}
	}

	return ARRAY_NULLVALUE;
}

template <class t> t S::Array<t>::GetNthEntry(Int n)
{
	if (n >= nOfEntries) return ARRAY_NULLVALUE;

	t	 ret;

	if (lastNthEntry == prevEntry)
	{
		if (n == lastN) return prevEntry->GetValue();

		if (n == lastN + 1)
		{
			ret = GetNextEntry();

			lastN++;
			lastNthEntry = prevEntry;

			return ret;
		}

		if (n == lastN - 1)
		{
			ret = GetPrevEntry();

			lastN--;
			lastNthEntry = prevEntry;

			return ret;
		}
	}

	ret = GetFirstEntry();

	for (Int i = 0; i < n; i++) ret = GetNextEntry();

	lastN		= n;
	lastNthEntry	= prevEntry;

	return ret;
}

template <class t> S::Int S::Array<t>::GetNthEntryIndex(Int n)
{
	if (n >= nOfEntries) return -1;

	Array_Entry<t>	*entry = firstEntry;

	for (Int i = 0; i < n; i++) entry = entry->GetNext();

	return entry->GetIndex();
}

template <class t> t S::Array<t>::GetNthEntry(const Array<t> *array, Int n)
{
	if (n >= array->GetNOfEntries()) return ARRAY_NULLVALUE;

	Array_Entry<t> *pEntry = array->firstEntry;

	for (Int i = 0; i < n; i++) pEntry = pEntry->GetNext();

	return pEntry->GetValue();
}

#endif
