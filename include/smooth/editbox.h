 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_EDITBOX_
#define _H_OBJSMOOTH_EDITBOX_

#define SEditBox SMOOTHEditBox

class SMOOTHEditBox;
class SMOOTHTimer;

#include "object.h"

const SMOOTHInt EDB_ALPHANUMERIC	= 0;
const SMOOTHInt EDB_NUMERIC		= 1;
const SMOOTHInt EDB_MULTILINE		= 2;
const SMOOTHInt EDB_HSCROLL		= 4;
const SMOOTHInt EDB_VSCROLL		= 8;
const SMOOTHInt EDB_ASTERISK		= 16;

class SMOOTHAPI SMOOTHEditBox : public SMOOTHObject
{
	private:
		SMOOTHTimer			*timer;

		SMOOTHVoid			 TimerProc();

		SMOOTHInt			 CountLines();
		SMOOTHString			 GetLine(SMOOTHInt);
	protected:
		SMOOTHInt			 promptPos;	// the current position of the cursor in the whole text
		SMOOTHInt			 linePromptPos;	// the current cursor position in the active line
		SMOOTHInt			 currLine;	// the current line (1st line = 0)
		SMOOTHBool			 promptVisible;
		SMOOTHInt			 markStart;
		SMOOTHInt			 markEnd;
		SMOOTHInt			 leftCut;	// how much text of the current line is invisible
		SMOOTHString			 lcText;	// the invisible text
		SMOOTHString			 visText;	// the visible text of the current line
		SMOOTHBool			 isRight;
		SMOOTHBool			 isLeft;
		SMOOTHBool			 isAsterisk;
		SMOOTHInt			 maxSize;
		SMOOTHInt			 nOfLines;	// the number of lines
		SMOOTHArray<SMOOTHString>	 lines;
	public:
						 SMOOTHEditBox(SMOOTHString, SMOOTHPoint, SMOOTHSize, SMOOTHInt, SMOOTHInt, SMOOTHProcParam, SMOOTHVoid *);
						~SMOOTHEditBox();

		virtual SMOOTHInt		 Paint(SMOOTHInt);
		SMOOTHInt			 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt			 Deactivate();

		SMOOTHInt			 SetText(SMOOTHString);
};

SMOOTHVAR SMOOTHInt OBJ_EDITBOX;

#endif
