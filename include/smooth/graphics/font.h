 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_FONT_
#define _H_OBJSMOOTH_FONT_

namespace smooth
{
	namespace GUI
	{
		class Font;
	};
};

#include "../definitions.h"
#include "../i18n/i18n.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Font
		{
			protected:
				String		 fontName;
				Int		 fontSize;
				Int		 fontColor;
				Int		 fontWeight;
				Int		 fontStyle;
			constants:
				static String	 Default;
				static Int	 DefaultSize;

				static Int	 Normal;
				static Int	 Bold;

				static Int	 Italic;
				static Int	 Underline;
				static Int	 StrikeOut;
			public:
						 Font(const String & = Font::Default, Int = Font::DefaultSize, Int = Font::Normal, Int = Font::Normal, Int = 0);
						 Font(const Font &);
				virtual		~Font();

				Font &operator	 =(const Font &);
				Bool operator	 ==(const Font &) const;
				Bool operator	 !=(const Font &) const;

				Int		 SetName(const String &);
				Int		 SetSize(Int);
				Int		 SetColor(Int);
				Int		 SetWeight(Int);
				Int		 SetStyle(Int);

				const String	&GetName() const;
				Int		 GetSize() const;
				Int		 GetColor() const;
				Int		 GetWeight() const;
				Int		 GetStyle() const;

				Int		 GetLineSizeX(const String &, Int) const;
				Int		 GetLineSizeY(const String &) const;

				Int		 GetTextSizeX(const String &, Int) const;
				Int		 GetTextSizeX(const String &) const;
				Int		 GetTextSizeY(const String &) const;
		};
	};
};

#endif
