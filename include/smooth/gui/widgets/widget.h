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
		class Container;
	};
};

#include "../../basic/object.h"
#include "../../graphics/font.h"
#include "../../signals.h"
#include "../../graphics/rect.h"

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

				Bool				 clicked;
				Bool				 checked;

				Int				 orientation;

				String				 text;
				String				 tooltipText;

				Font				 font;

				Bool				 registered;
				Container			*container;

				Void				 GetTextSize();
			public:
				static const Int		 classID;

				Array<Int>			 possibleContainers;

				Int				 subtype;

				Point				 pos;
				Size				 size;

				Size				 textSize;

								 Widget();
				virtual				~Widget();

				Bool				 IsRegistered();

				Int				 SetContainer(Container *);
				Container			*GetContainer();

				Void				 SetRegisteredFlag(Bool);

				virtual Point			 GetRealPosition();

				virtual Int			 Show();
				virtual Int			 Hide();

				virtual Int			 Activate();
				virtual Int			 Deactivate();

				virtual Int			 Paint(Int);
				virtual Int			 Process(Int, Int, Int);

				Bool				 IsVisible();
				Bool				 IsActive();

				virtual Int			 SetText(const String &);
				virtual String			 GetText();

				virtual Int			 SetTooltipText(const String &);
				virtual String			 GetTooltipText();

				virtual Int			 SetFont(Font);
				virtual Font			 GetFont();

				virtual Int			 SetOrientation(Int);
				virtual Int			 GetOrientation();

				virtual Int			 SetPosition(Point);
				virtual Int			 SetMetrics(Point, Size);

				virtual Bool			 IsAffected(const Rect &);

				virtual Bool			 IsTypeCompatible(Int);
			signals:
				Signal0<Void>			 onShow;
				Signal0<Void>			 onHide;

				Signal2<Void, Int, Int>		 onClick;

				Signal1<Void, Container *>	 onRegister;
				Signal1<Void, Container *>	 onUnregister;
		};
	};
};

#endif
