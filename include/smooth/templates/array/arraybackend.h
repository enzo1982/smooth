 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_ARRAY_BACKEND
#define H_OBJSMOOTH_ARRAY_BACKEND

#include "../../misc/array.h"

#include <memory.h>
#include <string.h>

namespace smooth
{
	template <class s> class ArrayEntry
	{
		public:
			s	 value;

				 ArrayEntry(const s &iValue) : value(iValue) { }
	};

	template <class s> class ArrayBackend : public IndexArray
	{
		private:
			static s		 nullValue;

			Buffer<ArrayEntry<s> *>	 entries;
		public:
			ArrayBackend()
			{
			}

			ArrayBackend(const ArrayBackend<s> &oArray) : IndexArray()
			{
				*this = oArray;
			}

			virtual	~ArrayBackend()
			{
				RemoveAll();
			}

			ArrayBackend<s> &operator =(const ArrayBackend<s> &oArray)
			{
				if (&oArray == this) return *this;

				ReadLock	 read(oArray);
				WriteLock	 write(*this);

				RemoveAll();

				for (Int i = 0; i < oArray.Length(); i++) Add(oArray.GetNthReference(i), oArray.GetNthIndex(i));

				return *this;
			}

			Int Add(const s &value)
			{
				WriteLock	 lock(*this);
				Int		 index = greatestIndex + 1;

				if (!Add(value, index))	index = -1;

				return index;
			}

			Bool Add(const s &value, Int index)
			{
				WriteLock	 lock(*this);

				if (!IndexAvailable(index)) return False;

				if (entries.Size() == nOfEntries) entries.Resize(8 > nOfEntries * 1.25 ? 8 : nOfEntries * 1.25);

				entries[nOfEntries] = new ArrayEntry<s>(value);

				return IndexArray::InsertAtPos(nOfEntries, index);
			}

			Int InsertAfter(Int prevIndex, const s &value)
			{
				WriteLock	 lock(*this);
				Int		 index = greatestIndex + 1;

				if (!InsertAfter(prevIndex, value, index)) index = -1;

				return index;
			}

			Bool InsertAfter(Int prevIndex, const s &value, Int index)
			{
				return InsertAtPos(GetEntryNumberByIndex(prevIndex) + 1, value, index);
			}

			Int InsertAtPos(Int position, const s &value)
			{
				WriteLock	 lock(*this);
				Int		 index = greatestIndex + 1;

				if (!InsertAtPos(position, value, index)) index = -1;

				return index;
			}

			Bool InsertAtPos(Int position, const s &value, Int index)
			{
				WriteLock	 lock(*this);

				if (position > nOfEntries || position < 0) return False;

				if (!IndexAvailable(index)) return False;

				if (entries.Size() == nOfEntries) entries.Resize(8 > nOfEntries * 1.25 ? 8 : nOfEntries * 1.25);

				memmove(entries + position + 1, entries + position, (nOfEntries - position) * sizeof(ArrayEntry<s> *));

				entries[position] = new ArrayEntry<s>(value);

				return IndexArray::InsertAtPos(position, index);
			}

			Bool MoveNth(Int n, Int m)
			{
				WriteLock	 lock(*this);

				if (nOfEntries <= n || n < 0 ||
				    nOfEntries <= m || m < 0) return False;

				ArrayEntry<s>	*backupEntry = entries[n];

				if (m < n) memmove(entries + m + 1, entries + m, (n - m) * sizeof(ArrayEntry<s> *));
				else	   memmove(entries + n, entries + n + 1, (m - n) * sizeof(ArrayEntry<s> *));

				entries[m] = backupEntry;

				return IndexArray::MoveNth(n, m);
			}

			Bool RemoveNth(Int n)
			{
				WriteLock	 lock(*this);

				if (nOfEntries <= n || n < 0) return False;

				if (nOfEntries == 1) return RemoveAll();

				delete entries[n];

				memmove(entries + n, entries + n + 1, (nOfEntries - n - 1) * sizeof(ArrayEntry<s> *));

				return IndexArray::RemoveNth(n);
			}

			Bool RemoveAll()
			{
				if (nOfEntries == 0) return True;

				WriteLock	 lock(*this);

				for (Int i = 0; i < nOfEntries; i++) delete entries[i];

				entries.Free();

				return IndexArray::RemoveAll();
			}

			const s &Get(Int index) const
			{
				return GetNth(GetEntryNumberByIndex(index));
			}

			s &GetReference(Int index)
			{
				return GetNthReference(GetEntryNumberByIndex(index));
			}

			const s &GetReference(Int index) const
			{
				return GetNthReference(GetEntryNumberByIndex(index));
			}

			Bool Set(Int index, const s &value)
			{
				return SetNth(GetEntryNumberByIndex(index), value);
			}

			const s &GetNth(Int n) const
			{
				ReadLock	 lock(*this);

				if (nOfEntries > n && n >= 0)
				{
					const s	&entry = entries[n]->value;

					lastAccessedEntry = n;

					return entry;
				}

				return nullValue;
			}

			s &GetNthReference(Int n)
			{
				ReadLock	 lock(*this);

				if (nOfEntries > n && n >= 0)
				{
					s	&entry = entries[n]->value;

					lastAccessedEntry = n;

					return entry;
				}

				return nullValue;
			}

			const s &GetNthReference(Int n) const
			{
				ReadLock	 lock(*this);

				if (nOfEntries > n && n >= 0)
				{
					const s	&entry = entries[n]->value;

					lastAccessedEntry = n;

					return entry;
				}

				return nullValue;
			}

			Bool SetNth(Int n, const s &value)
			{
				WriteLock	 lock(*this);

				if (nOfEntries > n && n >= 0)
				{
					entries[n]->value = value;

					lastAccessedEntry = n;

					return True;
				}

				return False;
			}

			const s &GetFirst() const
			{
				ReadLock	 lock(*this);

				if (nOfEntries > 0)
				{
					const s	&entry = entries[0]->value;

					lastAccessedEntry = 0;

					return entry;
				}

				return nullValue;
			}

			const s &GetLast() const
			{
				ReadLock	 lock(*this);

				if (nOfEntries > 0)
				{
					const s	&entry = entries[nOfEntries - 1]->value;

					lastAccessedEntry = nOfEntries - 1;

					return entry;
				}

				return nullValue;
			}

			const s &GetNext() const
			{
				ReadLock	 lock(*this);
				Int		 lastAccessed = lastAccessedEntry;

				if (lastAccessed < nOfEntries - 1)
				{
					const s	&entry = entries[++lastAccessed]->value;

					lastAccessedEntry = lastAccessed;

					return entry;
				}

				return nullValue;
			}

			const s &GetPrev() const
			{
				ReadLock	 lock(*this);
				Int		 lastAccessed = lastAccessedEntry;

				if (lastAccessed > 0)
				{
					const s	&entry = entries[--lastAccessed]->value;

					lastAccessedEntry = lastAccessed;

					return entry;
				}

				return nullValue;
			}
	};
};

template <class s> s S::ArrayBackend<s>::nullValue = (s) 0;

#endif
