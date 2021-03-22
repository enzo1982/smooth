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
#include <smooth/io/instream.h>

using namespace smooth::IO;

namespace smooth
{
	namespace GUI
	{
		static Short DetectImageFormat(const Buffer<UnsignedByte> &buffer)
		{
			Short	 format = IMAGE_FORMAT_AUTO;

			if (buffer.Size() < 12) return format;

			if	(buffer[ 0] == 0x50 && buffer[ 1] == 0x43 &&
				 buffer[ 2] == 0x49 && buffer[ 3] == 0x46) format = IMAGE_FORMAT_PCI;
			else if (buffer[ 0] == 0xFF && buffer[ 1] == 0xD8) format = IMAGE_FORMAT_JPEG;
			else if (buffer[ 0] == 0x89 && buffer[ 1] == 0x50 &&
				 buffer[ 2] == 0x4E && buffer[ 3] == 0x47 &&
				 buffer[ 4] == 0x0D && buffer[ 5] == 0x0A &&
				 buffer[ 6] == 0x1A && buffer[ 7] == 0x0A) format = IMAGE_FORMAT_PNG;
			else if (buffer[ 0] == 0x52 && buffer[ 1] == 0x49 &&
				 buffer[ 2] == 0x46 && buffer[ 3] == 0x46 &&
				 buffer[ 8] == 0x57 && buffer[ 9] == 0x45 &&
				 buffer[10] == 0x42 && buffer[11] == 0x50) format = IMAGE_FORMAT_WEBP;

			return format;
		}
	};
};

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

	if	(fileName.ToLower().Contains(".pci:")) loader = new ImageLoaderPCI(fileName);
#ifdef __WIN32__
	else if (fileName.StartsWith("Icon:"))	       loader = new ImageLoaderIcon(fileName);
#endif

	if (loader == NIL)
	{
		/* Auto-detect image format.
		 */
		Buffer<UnsignedByte>	 buffer(12);
		InStream		 in(STREAM_FILE, fileName, IS_READ);

		in.InputData(buffer, 12);

		Short	 format = DetectImageFormat(buffer);

		if	(format == IMAGE_FORMAT_PCI)  loader = new ImageLoaderPCI(fileName);
		else if (format == IMAGE_FORMAT_PNG)  loader = new ImageLoaderPNG(fileName);
		else if (format == IMAGE_FORMAT_JPEG) loader = new ImageLoaderJPEG(fileName);
		else if (format == IMAGE_FORMAT_WEBP) loader = new ImageLoaderWebP(fileName);
	}

	/* Load image.
	 */
	Bitmap	 bitmap;

	if (loader != NIL)
	{
		bitmap = loader->Load();

		delete loader;
	}

	/* Look in the startup and application folders if no image has been found yet.
	 */
	if (!fileName.StartsWith(Application::GetStartupDirectory()) && !fileName.StartsWith(Application::GetApplicationDirectory()))
	{
		if (bitmap == NIL) bitmap = Load(Application::GetStartupDirectory().Append(fileName));
		if (bitmap == NIL) bitmap = Load(Application::GetApplicationDirectory().Append(fileName));
	}

	return bitmap;
}

S::GUI::Bitmap S::GUI::ImageLoader::Load(const Buffer<UnsignedByte> &buffer, Short format)
{
	/* Auto-detect image format.
	 */
	if (format == IMAGE_FORMAT_AUTO) format = DetectImageFormat(buffer);

	/* Create image loader.
	 */
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
