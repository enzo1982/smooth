 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_TRANSLATOR_STRINGITEM
#define H_OBJSMOOTH_TRANSLATOR_STRINGITEM

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class StringItem : public ListEntry
{
	protected:
		Int			 id;

		String			 original;
		String			 translation;

		Int			 UpdateText();
	public:
		static const Short	 classID;

					 StringItem(Int, const String &, const String &);
		virtual			~StringItem();

		Bool			 IsTypeCompatible(Short) const;
	accessors:
		Int			 GetID()					{ return id; }

		const String		&GetOriginal()					{ return original; }
		Void			 SetOriginal(const String &nOriginal)		{ original = nOriginal; UpdateText(); }

		const String		&GetTranslation()				{ return translation; }
		Void			 SetTranslation(const String &nTranslation)	{ translation = nTranslation; UpdateText(); }
};

#endif
