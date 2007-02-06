 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/imageloader/imageloader.h>
#include <smooth/graphics/imageloader/pci.h>
#include <smooth/graphics/imageloader/icon.h>
#include <smooth/gui/application/application.h>

S::GUI::ImageLoader::ImageLoader(const String &iFileName)
{
	fileName = iFileName;
}

S::GUI::ImageLoader::~ImageLoader()
{
}

S::GUI::Bitmap S::GUI::ImageLoader::Load(const String &fileName)
{
	ImageLoader	*loader = NIL;

	if	(fileName.ToLower().Find(".pci:") != -1) loader = new ImageLoaderPCI(fileName);
	else if (fileName.StartsWith("Icon:"))		 loader = new ImageLoaderIcon(fileName);

	if (loader == NIL) return NIL;

	Bitmap	 bitmap = loader->Load();

	delete loader;

	if (!fileName.StartsWith(Application::GetStartupDirectory()) && !fileName.StartsWith(Application::GetApplicationDirectory()))
	{
		if (bitmap == NIL) bitmap = Load(Application::GetStartupDirectory().Append(fileName));
		if (bitmap == NIL) bitmap = Load(Application::GetApplicationDirectory().Append(fileName));
	}

	return bitmap;
}
