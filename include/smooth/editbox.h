 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_EDITBOX_
#define _H_OBJSMOOTH_EDITBOX_

namespace smooth
{
	namespace GUI
	{
		class EditBox;
	};

	class Timer;
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		const Int EDB_ALPHANUMERIC	= 0;
		const Int EDB_NUMERIC		= 1;
		const Int EDB_ASTERISK		= 16;

		class SMOOTHAPI EditBox : public Widget
		{
			private:
				Timer		*timer;

				Void		 TimerProc();
			protected:
				Int		 promptPos;
				Bool		 promptVisible;
				Int		 markStart;
				Int		 oMarkStart;
				Int		 markEnd;
				Bool		 marking;
				Bool		 isAsterisk;
				Int		 maxSize;

				Void		 MarkText(Int, Int);
				Void		 DeleteSelectedText();
			public:
						 EditBox(String, Point, Size, Int, Int);
						~EditBox();

				virtual Int	 Paint(Int);
				Int		 Process(Int, Int, Int);

				Int		 Deactivate();

				Int		 SetText(String);
		};
	};

	SMOOTHVAR Int OBJ_EDITBOX;
};

#endif
