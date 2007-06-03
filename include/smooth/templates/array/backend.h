 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ARRAY_BACKEND_
#define _H_OBJSMOOTH_ARRAY_BACKEND_

#include "entry.h"

namespace smooth
{
	template <class s> class Array_Backend
	{
		private:
			static s		 nullValue;

			Int			 nOfEntries;
			Int			 greatestIndex;
			Bool			 outlinedEntry;

			Array_Entry<s>		*firstEntry;
			Array_Entry<s>		*lastEntry;

			mutable Array_Entry<s>	*prevAccessedEntry;

			mutable Int		 lastN;
			mutable Array_Entry<s>	*lastNthEntry;

			Bool IndexAvailable(Int index) const
			{
				if (index > greatestIndex) return True;

				if (GetArrayEntryByIndex(index) == ARRAY_NULLPOINTER)	return True;
				else							return False;
			}

			Array_Entry<s> *GetArrayEntryByIndex(Int index) const
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

			Array_Entry<s> *GetNthArrayEntry(Int n) const
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

		public:
			Array_Backend()
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

			virtual	~Array_Backend()
			{
				RemoveAll();
			}

			Int Add(const s &value)
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

			Bool Add(const s &value, Int index)
			{
				if (!IndexAvailable(index)) return False;

				lastN		= -1024;
				lastNthEntry	= ARRAY_NULLPOINTER;

				Add(value);

				prevAccessedEntry->SetIndex(index);

				if (index >= greatestIndex)	greatestIndex = index;
				else				greatestIndex--;

				outlinedEntry = True;

				return True;
			}

			Int InsertAfter(Int prevIndex, const s &value)
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

			Bool InsertAfter(Int prevIndex, const s &value, Int index)
			{
				if (GetArrayEntryByIndex(prevIndex) == ARRAY_NULLPOINTER) return False;
				if (!IndexAvailable(index))				  return False;

				InsertAfter(prevIndex, value);

				prevAccessedEntry->SetIndex(index);

				if (index >= greatestIndex)	greatestIndex = index;
				else				greatestIndex--;

				return True;
			}

			Int InsertAtPos(Int position, const s &value)
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

			Bool InsertAtPos(Int position, const s &value, Int index)
			{
				if (nOfEntries < position)	return False;
				if (!IndexAvailable(index))	return False;

				InsertAtPos(position, value);

				prevAccessedEntry->SetIndex(index);

				if (index >= greatestIndex)	greatestIndex = index;
				else				greatestIndex--;

				return True;
			}

			Bool Remove(Int index)
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

			Bool RemoveAll()
			{
				while (firstEntry != ARRAY_NULLPOINTER) Remove(firstEntry->GetIndex());

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

			const s &Get(Int index) const
			{
				Array_Entry<s>	*entry = GetArrayEntryByIndex(index);

				if (entry != ARRAY_NULLPOINTER) return entry->GetValue();

				return nullValue;
			}

			Bool Set(Int index, const s &value)
			{
				Array_Entry<s>	*entry = GetArrayEntryByIndex(index);

				if (entry != ARRAY_NULLPOINTER) return entry->SetValue(value);

				return False;
			}

			const s &GetFirst() const
			{
				if (nOfEntries > 0 && firstEntry != ARRAY_NULLPOINTER)
				{
					prevAccessedEntry = firstEntry;

					return firstEntry->GetValue();
				}

				return nullValue;
			}

			const s &GetLast() const
			{
				if (nOfEntries > 0 && lastEntry != ARRAY_NULLPOINTER)
				{
					prevAccessedEntry = lastEntry;

					return lastEntry->GetValue();
				}

				return nullValue;
			}

			const s &GetNext() const
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

			const s &GetPrev() const
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

			const s &GetNth(Int n) const
			{
				Array_Entry<s>	*entry = GetNthArrayEntry(n);

				if (entry != ARRAY_NULLPOINTER) return entry->GetValue();

				return nullValue;
			}

			Int GetNthIndex(Int n) const
			{
				Array_Entry<s>	*entry = GetNthArrayEntry(n);

				if (entry != ARRAY_NULLPOINTER) return entry->GetIndex();

				return -1;
			}

			inline Int Length() const
			{
				return nOfEntries;
			}
	};
};

template <class s> s S::Array_Backend<s>::nullValue = ARRAY_NULLVALUE;

#endif
