 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
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
#include "../misc/i18n.h"

namespace smooth
{
#ifndef __WIN32__
	const Int	 FW_NORMAL	= 400;
	const Int	 FW_BOLD	= 700;
#endif

	namespace GUI
	{
		class SMOOTHAPI Font
		{
			protected:
				String		 fontName;
				Int		 fontSize;
				Int		 fontColor;
				Int		 fontWeight;
				Bool		 fontItalic;
				Bool		 fontUnderline;
				Bool		 fontStrikeOut;
			public:
						 Font(String = I18N_DEFAULTFONT, Int = I18N_DEFAULTFONTSIZE, Int = 0, Int = FW_NORMAL, Bool = 0, Bool = 0, Bool = 0);
						 Font(const Font &);
						~Font();

				Font &operator	 =(const Font &);
				Bool operator	 ==(const Font &);
				Bool operator	 !=(const Font &);

				Int		 SetName(String);
				Int		 SetSize(Int);
				Int		 SetColor(Int);
				Int		 SetWeight(Int);
				Int		 SetItalic(Bool);
				Int		 SetUnderline(Bool);
				Int		 SetStrikeOut(Bool);

				String		 GetName();
				Int		 GetSize();
				Int		 GetColor();
				Int		 GetWeight();
				Bool		 GetItalic();
				Bool		 GetUnderline();
				Bool		 GetStrikeOut();

				Int		 GetLineSizeX(String, Int);
				Int		 GetLineSizeY(String);

				Int		 GetTextSizeX(String, Int);
				Int		 GetTextSizeX(String);
				Int		 GetTextSizeY(String);
		};
	};
};

#endif
