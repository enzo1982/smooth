 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "xultest.h"

Int smooth::Main()
{
	XULTest	*app = new XULTest();

	app->Loop();

	Object::DeleteObject(app);

	return 0;
}

XULTest::XULTest()
{
	xulRenderer = new Renderer();

	if (xulRenderer->LoadXUL("layout.xul") == Success()) Add(xulRenderer->GetWidget());
}

XULTest::~XULTest()
{
	delete xulRenderer;
}
