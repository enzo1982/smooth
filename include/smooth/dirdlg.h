 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DIRDLG_
#define _H_OBJSMOOTH_DIRDLG_

namespace smooth
{
	class DialogDirSelection;
};

#include "definitions.h"
#include "dialog.h"

namespace smooth
{
	class SMOOTHAPI DialogDirSelection : public Dialog
	{
		private:
			String	 directory;
		public:
				 DialogDirSelection();
				~DialogDirSelection();
			Int	 ShowDialog();
			String	 GetDirName();
	};
};

#endif
