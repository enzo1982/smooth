 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_FONTDLG_
#define _H_OBJSMOOTH_FONTDLG_

namespace smooth
{
	class DialogFontSelection;
};

#include "definitions.h"
#include "dialog.h"
#include "graphics/font.h"

namespace smooth
{
	class SMOOTHAPI DialogFontSelection : public Dialog
	{
		private:
			GUI::Font	 font;
		public:
					 DialogFontSelection();
					~DialogFontSelection();

			Int		 ShowDialog();

			GUI::Font	 GetFont();
	};
};

#endif
