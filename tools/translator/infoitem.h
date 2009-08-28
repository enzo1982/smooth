 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_TRANSLATOR_INFOITEM
#define H_OBJSMOOTH_TRANSLATOR_INFOITEM

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class InfoItem : public ListEntry
{
	protected:
		String			 name;
		String			 value;

		Int			 UpdateText();
	public:
		static const Int	 classID;

					 InfoItem(const String &, const String &);
		virtual			~InfoItem();

		Bool			 IsTypeCompatible(Int) const;
	accessors:
		const String		&GetName()			{ return name; }
		Void			 SetName(const String &nName)	{ name = nName; UpdateText(); }

		const String		&GetValue()			{ return value; }
		Void			 SetValue(const String &nValue)	{ value = nValue; UpdateText(); }
};

#endif
