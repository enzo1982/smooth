 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECTMANAGER_
#define _H_OBJSMOOTH_OBJECTMANAGER_

#define SObjectManager SMOOTHObjectManager

class SMOOTHObjectManager;

#include "application.h"
#include "container.h"

class SMOOTHAPI SMOOTHObjectManager : public SMOOTHContainer
{
	protected:
		static SMOOTHBool	 objectManagerExists;
		SMOOTHBool		 iAmTheOne;
	public:
					 SMOOTHObjectManager();
					~SMOOTHObjectManager();

		SMOOTHInt		 RegisterObject(SMOOTHObject *);
		SMOOTHInt		 UnregisterObject(SMOOTHObject *);
};

SMOOTHVAR SMOOTHObjectManager	*mainObjectManager;

#endif
