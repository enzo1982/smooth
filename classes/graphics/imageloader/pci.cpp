 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/imageloader/pci.h>
#include <smooth/pciio.h>

S::GUI::ImageLoaderPCI::ImageLoaderPCI(const String &iFileName) : ImageLoader(iFileName)
{
}

S::GUI::ImageLoaderPCI::ImageLoaderPCI(const Buffer<UnsignedByte> &iBuffer) : ImageLoader(iBuffer)
{
}

S::GUI::ImageLoaderPCI::~ImageLoaderPCI()
{
}

const S::GUI::Bitmap &S::GUI::ImageLoaderPCI::Load()
{
	String	 pciFile = fileName;
	String	 idString;

	pciFile[fileName.ToLower().Find(".pci:") + 4] = 0;

	for (Int i = 0; i < fileName.Length() - fileName.ToLower().Find(".pci:") - 5; i++) idString[i] = fileName[fileName.ToLower().Find(".pci:") + 5 + i];

	Int	 id = idString.ToInt();
	PCIIn	 pci = OpenPCIForInput(pciFile);

	if (pci->GetLastError() != IO::IO_ERROR_OK)
	{
		ClosePCI(pci);

		bitmap = NIL;

		return bitmap;
	}

	PCIIO	*pio = new PCIIO();

	pio->SelectImage(id);

	ReadPCI(pci, *pio);

	ClosePCI(pci);

	bitmap = pio->GetBitmap();

	delete pio;

	return bitmap;
}
