 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Widget : public Object
		{
			protected:
				Bool		 visible;
				Bool		 active;
			public:
				Int		 subtype;

						 Widget();
				virtual		~Widget();

				virtual Int	 Show();
				virtual Int	 Hide();

				virtual Int	 Activate();
				virtual Int	 Deactivate();

				virtual Int	 Paint(Int);
				virtual Int	 Process(Int, Int, Int);

				Bool		 IsVisible();
				Bool		 IsActive();

				virtual Int	 SetText(String);
				virtual String	 GetText();

				virtual Int	 SetTooltip(String);
				virtual String	 GetTooltip();

				virtual Int	 SetFont(String, Int, Int, Int);
				virtual Int	 SetOrientation(Int);
				virtual Int	 SetPosition(Point);
				virtual Int	 SetMetrics(Point, Size);

				virtual Bool	 IsTypeCompatible(Int);
			signals:
				Signal0<Void>	 onClick;
		};
	};

	SMOOTHVAR Int OBJ_WIDGET;
};

#endif
