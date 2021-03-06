 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_LISTENTRY
#define H_OBJSMOOTH_LISTENTRY

namespace smooth
{
	namespace GUI
	{
		class ListEntry;
		class Hotspot;
	};
};

#include "../generic/entry.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI ListEntry : public Entry
		{
			private:
				Bool				 marked;
				Bool				 selected;
			protected:
				Hotspot				*hotspot;
				Hotspot				*markHotspot;

				String				 GetNthTabText(Int) const;
			public:
				static const Short		 classID;

				static const String		 tabDelimiter;

								 ListEntry(const String &);
				virtual				~ListEntry();

				virtual Int			 Paint(Int);

				virtual String			 ToString() const		{ return "a ListEntry"; }

				virtual Bool			 IsTypeCompatible(Short) const;
			accessors:
				Int				 SetMark(Bool);
				Bool				 IsMarked() const		{ return marked; }

				Int				 Select();
				Int				 Deselect();
				Bool				 IsSelected() const		{ return selected; }
			signals:
				static Signal2<Void, Int, Int>	 internalOnSelectEntry;

				Signal0<Void>			 onSelect;
				Signal0<Void>			 onDeselect;

				Signal1<Void, Bool>		 onToggleMark;
			slots:
				Void				 InitDrag();

				Void				 OnClickMarkHotspot();
				Void				 OnSelectEntry();

				Void				 OnSelectOtherEntry(Int, Int);
		};
	};
};

#endif
