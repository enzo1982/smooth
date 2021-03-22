/* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/imageloader/imageloader.h>
#include <smooth/graphics/imageloader/pci.h>
#include <smooth/graphics/imageloader/png.h>
#include <smooth/graphics/imageloader/jpeg.h>
#include <smooth/graphics/imageloader/webp.h>
#include <smooth/graphics/imageloader/icon.h>
#include <smooth/gui/application/application.h>

S::GUI::ImageLoader::ImageLoader(const String &iFileName)
{
	fileName    = iFileName;

	gotFileName = True;
	gotBuffer   = False;
}

S::GUI::ImageLoader::ImageLoader(const Buffer<UnsignedByte> &iBuffer)
{
	buffer	    = iBuffer;

	gotBuffer   = True;
	gotFileName = False;
}

S::GUI::ImageLoader::~ImageLoader()
{
}

S::GUI::Bitmap S::GUI::ImageLoader::Load(const String &fileName)
{
	ImageLoader	*loader = NIL;

	if	(fileName.ToLower().Contains(".pci:"))	loader = new ImageLoaderPCI(fileName);
	else if (fileName.ToLower().EndsWith(".png"))	loader = new ImageLoaderPNG(fileName);
	else if (fileName.ToLower().EndsWith(".jpg") ||
		(fileName.ToLower().EndsWith(".jpeg")))	loader = new ImageLoaderJPEG(fileName);
	else if (fileName.ToLower().EndsWith(".webp"))	loader = new ImageLoaderWebP(fileName);
#ifdef __WIN32__
	else if (fileName.StartsWith("Icon:"))		loader = new ImageLoaderIcon(fileName);
#endif

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

S::GUI::Bitmap S::GUI::ImageLoader::Load(const Buffer<UnsignedByte> &buffer, Short format)
{
	ImageLoader	*loader = NIL;

	if	(format == IMAGE_FORMAT_PCI)  loader = new ImageLoaderPCI(buffer);
	else if (format == IMAGE_FORMAT_PNG)  loader = new ImageLoaderPNG(buffer);
	else if (format == IMAGE_FORMAT_JPEG) loader = new ImageLoaderJPEG(buffer);
	else if (format == IMAGE_FORMAT_WEBP) loader = new ImageLoaderWebP(buffer);

	if (loader == NIL) return NIL;

	Bitmap	 bitmap = loader->Load();

	delete loader;

	return bitmap;
}
