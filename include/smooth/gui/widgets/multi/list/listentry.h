 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LISTENTRY_
#define _H_OBJSMOOTH_LISTENTRY_

namespace smooth
{
	namespace GUI
	{
		class ComboBox;
		class List;
		class ListBox;
		class Tooltip;
	};

	namespace System
	{
		class Timer;
	};
};

#include "../../widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI ListEntry : public Widget
		{
			friend class ComboBox;
			friend class List;
			friend class ListBox;
			private:
				Bool		 marked;

				Tooltip		*tooltip;
				System::Timer	*tipTimer;

				Void		 DrawEntryText(Int);
			public:
						 ListEntry(String);
						~ListEntry();

				Int		 Paint(Int);
				Int		 Process(Int, Int, Int);

				Int		 SetText(const String &);

				Int		 SetMark(Bool);
				Bool		 IsMarked();
			signals:
				Signal0<Void>	 onMouseOver;
				Signal0<Void>	 onMouseOut;
			slots:
				Void		 OnRegister();
				Void		 OnUnregister();

				Void		 ActivateTooltip();
				Void		 SelectEntry();
		};
	};
};

#endif
