 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WIDGET_
#define _H_OBJSMOOTH_WIDGET_

namespace smooth
{
	namespace GUI
	{
		class Widget;
	};

	class SMOOTH;
};

#include "object.h"
#include "font.h"
#include "signals.h"

namespace smooth
{
	const Int SP_PAINT	= 0;
	const Int SP_MOUSEIN	= 1;
	const Int SP_MOUSEOUT	= 2;
	const Int SP_MOUSEDOWN	= 3;
	const Int SP_MOUSEUP	= 4;
	const Int SP_UPDATE	= 5;
	const Int SP_DELAYED	= 6;
	const Int SP_SHOW	= 7;
	const Int SP_HIDE	= 8;

	namespace GUI
	{
		class SMOOTHAPI Widget : public Object
		{
			protected:
				Bool				 visible;
				Bool				 active;

				Void				 GetTextSize();
			public:
				static const Int		 classID;

				Int				 subtype;

								 Widget();
				virtual				~Widget();

				virtual Point			 GetRealPosition();

				virtual Int			 Show();
				virtual Int			 Hide();

				virtual Int			 Activate();
				virtual Int			 Deactivate();

				virtual Int			 Paint(Int);
				virtual Int			 Process(Int, Int, Int);

				Bool				 IsVisible();
				Bool				 IsActive();

				virtual Int			 SetText(String);
				virtual String			 GetText();

				virtual Int			 SetTooltip(String);
				virtual String			 GetTooltip();

				virtual Int			 SetFont(Font);
				virtual Int			 SetOrientation(Int);
				virtual Int			 SetPosition(Point);
				virtual Int			 SetMetrics(Point, Size);

				virtual Bool			 IsTypeCompatible(Int);
			signals:
				Signal2<Void, Int, Int>		 onClick;

				Signal1<Void, Container *>	 onRegister;
				Signal1<Void, Container *>	 onUnregister;
		};
	};
};

#endif
