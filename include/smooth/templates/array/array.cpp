 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __SMOOTHARRAY_ARRAY_
#define __SMOOTHARRAY_ARRAY_

#include "array.h"
#include "entry.h"

template <class t> SMOOTHArray<t>::SMOOTHArray()
{
	nOfEntries = 0;
	greatestIndex = -1;
	outlinedEntry = false;
	firstEntry = ARRAY_NULLPOINTER;
	lastEntry = ARRAY_NULLPOINTER;
	prevEntry = ARRAY_NULLPOINTER;
	prevDeletedEntry = ARRAY_NULLPOINTER;
}

template <class t> SMOOTHArray<t>::~SMOOTHArray()
{
	DeleteAll();
}

template <class t> bool SMOOTHArray<t>::Cleanup(SMOOTHArray_Entry<t> *entry)
{
	if (entry != ARRAY_NULLPOINTER)
	{
		if (entry->gotNext) Cleanup(entry->GetNext());
		DeleteEntry(entry->GetIndex());

		return true;
	}
	else
	{
		return false;
	}
}

template <class t> bool SMOOTHArray<t>::IndexAvailable(long index)
{
	if (GetEntry(index) == ARRAY_NULLVALUE)	return true;
	else					return false;
}

template <class t> long SMOOTHArray<t>::AddEntry(t entry)
{
	if (nOfEntries == 0)
	{
		outlinedEntry = false;

		firstEntry = new SMOOTHArray_Entry<t>;
		firstEntry->Fill(entry);
		firstEntry->SetIndex(nOfEntries);
		lastEntry = firstEntry;
		greatestIndex = nOfEntries;
	}
	else
	{
		lastEntry->SetNext(new SMOOTHArray_Entry<t>);
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

template <class t> bool SMOOTHArray<t>::AddEntry(t entry, long index)
{
	if (index > greatestIndex)
	{
		AddEntry(entry);
		prevEntry->SetIndex(index);

		outlinedEntry = true;

		greatestIndex = index;

		return true;
	}
	else
	{
		if (IndexAvailable(index))
		{
			AddEntry(entry);
			prevEntry->SetIndex(index);

			if (outlinedEntry)
			{
				greatestIndex--;
			}
			else
			{
				outlinedEntry = true;
			}

			return true;
		}
		else
		{
			return false;
		}
	}
}

template <class t> long SMOOTHArray<t>::InsertEntryAfter(long prev, t entry)
{
	if (GetEntry(prev) == ARRAY_NULLVALUE) return -1;

	SMOOTHArray_Entry<t>	*operat = new SMOOTHArray_Entry<t>;

	operat->SetNext(prevEntry->GetNext());
	prevEntry->SetNext(operat);
	operat->SetPrev(prevEntry);

	if (prevEntry == lastEntry)
	{
		lastEntry = operat;
	}
	else
	{
		operat->GetNext()->SetPrev(operat);
	}

	operat->Fill(entry);

	outlinedEntry = true;

	operat->SetIndex(greatestIndex + 1);
	greatestIndex++;

	nOfEntries++;

	prevEntry = operat;

	return operat->GetIndex();
}

template <class t> bool SMOOTHArray<t>::InsertEntryAfter(long prev, t entry, long index)
{
	if (GetEntry(prev) == ARRAY_NULLVALUE) return -1;

	if (index > greatestIndex)
	{
		InsertEntryAfter(prev, entry);
		prevEntry->SetIndex(index);

		outlinedEntry = true;

		greatestIndex = index;

		return true;
	}
	else
	{
		if (IndexAvailable(index))
		{
			InsertEntryAfter(prev, entry);
			prevEntry->SetIndex(index);

			greatestIndex--;

			return true;
		}
		else
		{
			return false;
		}
	}
}

template <class t> long SMOOTHArray<t>::InsertEntryAtPos(long pos, t entry)
{
	if (nOfEntries < pos) return -1;

	SMOOTHArray_Entry<t>	*operat;

	for (int i = 0; i < pos; i++)
	{
		if (i == 0)	GetFirstEntry();
		else		GetNextEntry();
	}

	operat = new SMOOTHArray_Entry<t>;

	if (pos == 0)
	{
		operat->SetNext(firstEntry);
		firstEntry->SetPrev(operat);

		firstEntry = operat;
	}
	else
	{
		operat->SetNext(prevEntry->GetNext());
		prevEntry->SetNext(operat);
		operat->SetPrev(prevEntry);

		if (nOfEntries > pos) operat->GetNext()->SetPrev(operat);
	}

	if (nOfEntries == pos) lastEntry = operat;

	operat->Fill(entry);

	outlinedEntry = true;

	operat->SetIndex(greatestIndex + 1);
	greatestIndex++;

	nOfEntries++;

	prevEntry = operat;

	return operat->GetIndex();
}

template <class t> bool SMOOTHArray<t>::InsertEntryAtPos(long pos, t entry, long index)
{
	if (nOfEntries < pos) return -1;

	if (index > greatestIndex)
	{
		InsertEntryAtPos(pos, entry);
		prevEntry->SetIndex(index);

		outlinedEntry = true;

		greatestIndex = index;

		return true;
	}
	else
	{
		if (IndexAvailable(index))
		{
			InsertEntryAtPos(pos, entry);
			prevEntry->SetIndex(index);

			greatestIndex--;

			return true;
		}
		else
		{
			return false;
		}
	}
}

template <class t> bool SMOOTHArray<t>::DeleteEntry(long index)
{
	if (nOfEntries == 0) return false;

	SMOOTHArray_Entry<t>	*operat = ARRAY_NULLPOINTER;
	SMOOTHArray_Entry<t>	*prev = ARRAY_NULLPOINTER;

	if (prevDeletedEntry != ARRAY_NULLPOINTER)
	{
		if (prevDeletedEntry->gotNext)
		{
			if (prevDeletedEntry->GetNext()->GetIndex() == index)
			{
				operat = prevDeletedEntry->GetNext();

				if (!operat->gotNext)
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

				operat->Clear();
				delete operat;

				nOfEntries--;
				outlinedEntry = true;

				return true;
			}
		}

		if (prevDeletedEntry->gotPrev)
		{
			if (prevDeletedEntry->GetPrev()->GetIndex() == index)
			{
				operat = prevDeletedEntry->GetPrev();

				if (!operat->gotPrev)
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

				operat->Clear();
				delete operat;

				nOfEntries--;
				outlinedEntry = true;

				return true;
			}
		}
	}

	for (long i = 0; ; i++)
	{
		if (i == 0)
		{
			operat = firstEntry;
		}
		else
		{
			if (operat->GetIndex() == index)
			{
				if (operat->gotNext && prev != ARRAY_NULLPOINTER)
				{
					prev->SetNext(operat->GetNext());
					operat->GetNext()->SetPrev(prev);
				}
				else if (operat->gotNext && prev == ARRAY_NULLPOINTER)
				{
					firstEntry = operat->GetNext();
					operat->GetNext()->SetPrev(ARRAY_NULLPOINTER);
				}
				else if (!operat->gotNext && prev != ARRAY_NULLPOINTER)
				{
					prev->SetNext(ARRAY_NULLPOINTER);
					lastEntry = prev;
				}

				if (operat == prevEntry) prevEntry = ARRAY_NULLPOINTER;
				if (!operat->gotNext && operat == firstEntry) firstEntry = ARRAY_NULLPOINTER;
				if (operat == prevDeletedEntry) prevDeletedEntry = ARRAY_NULLPOINTER;

				prevDeletedEntry = prev;

				operat->Clear();
				delete operat;

				nOfEntries--;
				outlinedEntry = true;

				return true;
			}

			if (operat->gotNext)	
			{
				prev = operat;
				operat = operat->GetNext();
			}
			else
			{
				break;
			}
		}
	}
	return false;
}

template <class t> bool SMOOTHArray<t>::DeleteAll()
{
	if (nOfEntries == 0) return false;

	Cleanup(firstEntry);

	firstEntry = ARRAY_NULLPOINTER;

	nOfEntries = 0;

	return true;
}

template <class t> t SMOOTHArray<t>::GetEntry(long index)
{
	SMOOTHArray_Entry<t>	*operat = ARRAY_NULLPOINTER;

	if (nOfEntries == 0) return ARRAY_NULLVALUE;

	if (prevEntry != ARRAY_NULLPOINTER)
	{
		if (prevEntry->gotNext)
		{
			if (prevEntry->GetNext()->GetIndex() == index)
			{
				prevEntry = prevEntry->GetNext();

				return prevEntry->GetValue();
			}
		}

		if (prevEntry->gotPrev)
		{
			if (prevEntry->GetPrev()->GetIndex() == index)
			{
				prevEntry = prevEntry->GetPrev();

				return prevEntry->GetValue();
			}
		}
	}

	for (long i = 0; ; i++)
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

			if (operat->gotNext)	operat = operat->GetNext();
			else			break;
		}
	}

	return ARRAY_NULLVALUE;
}

template <class t> bool SMOOTHArray<t>::SetEntry(long index, t entry)
{
	SMOOTHArray_Entry<t>	*operat = ARRAY_NULLPOINTER;

	if (prevEntry != ARRAY_NULLPOINTER)
	{
		if (prevEntry->gotNext)
		{
			if (prevEntry->GetNext()->GetIndex() == index)
			{
				prevEntry = prevEntry->GetNext();

				prevEntry->Fill(entry);

				return true;
			}
		}

		if (prevEntry->gotPrev)
		{
			if (prevEntry->GetPrev()->GetIndex() == index)
			{
				prevEntry = prevEntry->GetPrev();

				prevEntry->Fill(entry);

				return true;
			}
		}
	}

	for (long i = 0; ; i++)
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
				return true;
			}

			if (operat->gotNext)	operat = operat->GetNext();
			else			break;
		}
	}

	return false;
}

template <class t> long SMOOTHArray<t>::GetNOfEntries()
{
	return nOfEntries;
}

template <class t> t SMOOTHArray<t>::GetFirstEntry()
{
	if (nOfEntries > 0 && firstEntry != ARRAY_NULLPOINTER)
	{
		prevEntry = firstEntry;

		return firstEntry->GetValue();
	}
	else	return ARRAY_NULLVALUE;
}

template <class t> t SMOOTHArray<t>::GetLastEntry()
{
	if (nOfEntries > 0 && lastEntry != ARRAY_NULLPOINTER)
	{
		prevEntry = lastEntry;

		return lastEntry->GetValue();
	}
	else	return ARRAY_NULLVALUE;
}

template <class t> t SMOOTHArray<t>::GetNextEntry()
{
	SMOOTHArray_Entry<t>	*ret;

	if (prevEntry != ARRAY_NULLPOINTER)
	{
		if (prevEntry->gotNext)
		{
			ret = prevEntry->GetNext();

			prevEntry = ret;

			return ret->GetValue();
		}
		else	return ARRAY_NULLVALUE;
	}

	return ARRAY_NULLVALUE;
}

template <class t> t SMOOTHArray<t>::GetPrevEntry()
{
	SMOOTHArray_Entry<t>	*ret;

	if (prevEntry != ARRAY_NULLPOINTER)
	{
		if (prevEntry->gotPrev)
		{
			ret = prevEntry->GetPrev();

			prevEntry = ret;

			return ret->GetValue();
		}
		else	return ARRAY_NULLVALUE;
	}

	return ARRAY_NULLVALUE;
}

template <class t> t SMOOTHArray<t>::GetNthEntry(int n)
{
	if (n >= nOfEntries) return ARRAY_NULLVALUE;

	t	 ret = GetFirstEntry();

	for (int i = 0; i < n; i++)
	{
		ret = GetNextEntry();
	}

	return ret;
}

#endif
