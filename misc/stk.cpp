 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/stk.h>
#include <smooth/color.h>

#ifdef __WIN32__
#include <smooth/application.h>
#include <smooth/graphics/window.h>
#include <smooth/layer.h>
#include <smooth/objectmanager.h>
#include <smooth/object.h>
#include <smooth/version.h>
#include <smooth/loop.h>
#include <smooth/objectproperties.h>
#include <smooth/toolwindow.h>
#include <smooth/divider.h>
#include <smooth/pciio.h>
#include <smooth/mdiwindow.h>

S::I18n::Translator	*S::SMOOTH::i18n = NIL;

HBITMAP S::SMOOTH::LoadImage(String file, Int id, String name)
{
	HBITMAP	 bmp;
	PCIIn	 pci = OpenPCIForInput((char *) file);

	if (pci->GetLastError() != IOLIB_ERROR_OK)
	{
		ClosePCI(pci);

		pci = OpenPCIForInput((char *) Application::GetStartupDirectory().Append(file));

		if (pci->GetLastError() != IOLIB_ERROR_OK)
		{
			ClosePCI(pci);

			pci = OpenPCIForInput((char *) Application::GetApplicationDirectory().Append(file));

			if (pci->GetLastError() != IOLIB_ERROR_OK)
			{
				ClosePCI(pci);

				return NIL;
			}
		}
	}

	PCIIO	*pio = new PCIIO();

	if (id >= 0)		pio->SelectImage(id);
	else if (name != NIL)	pio->SelectImage((char *) name);

	ReadPCI(pci, *pio);

	ClosePCI(pci);

	bmp = pio->GetBitmap();

	delete pio;

	if (bmp == NIL) return NIL;

	return bmp;
}
#endif
