 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DROPAREA_
#define _H_OBJSMOOTH_DROPAREA_

namespace smooth
{
	namespace GUI
	{
		class DropArea;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI DropArea : public Widget
		{
			private:
				Void			 OnRegister(Container *);

				Void			 Init();
			public:
							 DropArea(Point, Size);
							~DropArea();

				virtual Int		 Hide();

				virtual Int		 Process(Int, Int, Int);
			signals:
				Signal1<Void, String *>	 onDropFile;
		};
	};

	SMOOTHVAR Int OBJ_DROPAREA;
};

#endif
