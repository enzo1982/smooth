 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CLIENT_
#define _H_OBJSMOOTH_CLIENT_

namespace smooth
{
	namespace GUI
	{
		class Client;
	};
};

#include "widget.h"
#include "rect.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Client : public Widget
		{
			protected:
				Rect		 updateRect;
			public:
						 Client();
						~Client();

				virtual Int	 Paint(Int);

				Rect		 GetUpdateRect();

				virtual Size	 GetSize();

				virtual Int	 BlitFromBitmap(HBITMAP, Rect, Rect);
				virtual Int	 BlitToBitmap(Rect, HBITMAP, Rect);
			signals:
				Signal0<>	 onPaint;
		};
	};

	SMOOTHVAR Int OBJ_CLIENT;
};

#endif
