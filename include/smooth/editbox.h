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
	class EditBox;
	class Timer;
};

#include "object.h"

namespace smooth
{
	const Int EDB_ALPHANUMERIC	= 0;
	const Int EDB_NUMERIC		= 1;
	const Int EDB_MULTILINE		= 2;
	const Int EDB_HSCROLL		= 4;
	const Int EDB_VSCROLL		= 8;
	const Int EDB_ASTERISK		= 16;

	class SMOOTHAPI EditBox : public Object
	{
		private:
			Timer		*timer;

			Void		 TimerProc();

			Int		 CountLines();
			String		 GetLine(Int);
		protected:
			Int		 promptPos;	// the current position of the cursor in the whole text
			Int		 linePromptPos;	// the current cursor position in the active line
			Int		 currLine;	// the current line (1st line = 0)
			Bool		 promptVisible;
			Int		 markStart;
			Int		 markEnd;
			Int		 leftCut;	// how much text of the current line is invisible
			String		 lcText;	// the invisible text
			String		 visText;	// the visible text of the current line
			Bool		 isRight;
			Bool		 isLeft;
			Bool		 isAsterisk;
			Int		 maxSize;
			Int		 nOfLines;	// the number of lines
			Array<String>	 lines;
		public:
					 EditBox(String, Point, Size, Int, Int, ProcParam, Void *);
					~EditBox();

			virtual Int	 Paint(Int);
			Int		 Process(Int, Int, Int);

			Int		 Deactivate();

			Int		 SetText(String);
	};

	SMOOTHVAR Int OBJ_EDITBOX;
};

#endif
