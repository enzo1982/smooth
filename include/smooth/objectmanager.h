 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECTMANAGER_
#define _H_OBJSMOOTH_OBJECTMANAGER_

namespace smooth
{
	class ObjectManager;
};

#include "application.h"
#include "container.h"

namespace smooth
{
	class SMOOTHAPI ObjectManager : public Container
	{
		protected:
			static Bool	 objectManagerExists;
			Bool		 iAmTheOne;
		public:
					 ObjectManager();
					~ObjectManager();

			Int		 RegisterObject(Object *);
			Int		 UnregisterObject(Object *);
	};

	SMOOTHVAR ObjectManager	*mainObjectManager;
};

#endif
