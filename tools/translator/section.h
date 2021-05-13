 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_TRANSLATOR_SECTION
#define H_OBJSMOOTH_TRANSLATOR_SECTION

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class StringItem;

class Section
{
	private:
		String					 name;

		List					*list;
	public:
		Array<StringItem *>			 items;
		Array<Tree *>				 trees;

		Array<Section *>			 sections;

							 Section();
							~Section();

		Bool					 Parse(XML::Node *, List *, Array<ListEntry *> &);
		Bool					 Save(XML::Node *, const String &);

		Bool					 Export(IO::OutStream &, Int);

		Bool					 Filter(const String &, List *);
	accessors:
		const String				&GetName() const	{ return name; }
	signals:
		static Signal1<Void, ListEntry *>	 onSelectItem;
	slots:
		Void					 OnSelectEntry(ListEntry *);
};

#endif
