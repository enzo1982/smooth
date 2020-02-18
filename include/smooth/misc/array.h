 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>

#ifndef H_OBJSMOOTH_INDEX_ARRAY
#define H_OBJSMOOTH_INDEX_ARRAY

namespace smooth
{
	class IndexArray;
};

#include <smooth/templates/buffer.h>
#include <smooth/threads/rwlock.h>

namespace smooth
{
	class SMOOTHAPI IndexArray
	{
		protected:
			Int			 nOfEntries;
			Int			 greatestIndex;

			Bool			 sorted;

			mutable Int		 lastAccessedEntry;

			Buffer<Int>		 indices;

			mutable Bool		 lockingEnabled;
			mutable Threads::RWLock	*lock;

			Bool			 IndexAvailable(Int) const;
			Int			 GetEntryNumberByIndex(Int) const;
		public:
						 IndexArray();
			virtual			~IndexArray();

			Bool			 InsertAtPos(Int, Int);

			Bool			 Move(Int, Int);
			virtual Bool		 MoveNth(Int, Int);

			Bool			 Remove(Int);
			virtual Bool		 RemoveNth(Int);
			virtual Bool		 RemoveAll();

			inline Int		 Length() const		{ return nOfEntries; }

			Int			 GetNthIndex(Int) const;

			/* Locking and unlocking for synchronization.
			 */
			Bool			 EnableLocking() const;
			Bool			 DisableLocking() const;

			inline Void		 LockForRead() const	{ if (lockingEnabled) lock->LockForRead(); }
			inline Void		 LockForWrite() const	{ if (lockingEnabled) lock->LockForWrite(); }

			inline Void		 Unlock() const		{ if (lockingEnabled) lock->Release(); }

			/* Simple scoped lockers for arrays.
			 */
			class ReadLock
			{
				private:
					const IndexArray	&array;
				public:
								 ReadLock(const IndexArray &a) : array(a) { array.LockForRead(); }
								~ReadLock() { array.Unlock(); }
			};

			class WriteLock
			{
				private:
					IndexArray		&array;
				public:
								 WriteLock(IndexArray &a) : array(a) { array.LockForWrite(); }
								~WriteLock() { array.Unlock(); }
			};
	};
};

#endif
