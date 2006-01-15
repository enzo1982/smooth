 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MENUENTRY_
#define _H_OBJSMOOTH_MENUENTRY_

namespace smooth
{
	namespace GUI
	{
		class MenuEntry;
		class Menu;

		class Shortcut;
	};
};

#include "../../widget.h"
#include "../../../../graphics/bitmap.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI MenuEntry : public Widget
		{
			private:
				Shortcut		*shortcut;
			protected:
				Menu			*popup;

				Bitmap			 bitmap;
				Bitmap			 graymap;

				Bool			*bVar;
				Int			*iVar;
				Int			 iCode;
			public:
				static const Int	 classID;

							 MenuEntry(const String & = NIL, const Bitmap & = NIL, Menu * = NIL, Bool * = NIL, Int * = NIL, Int = 0);
				virtual			~MenuEntry();
			accessors:
				Int			 SetShortcut(Int, Int);

				Int			 SetBitmap(const Bitmap &);
				const Bitmap		&GetBitmap();
		};
	};
};

#endif
