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

size_t	 (*ex_iconv)(iconv_t, const char **, size_t *, char **, size_t *) = NIL;
iconv_t	 (*ex_iconv_open)(const char *, const char *) = NIL;
int	 (*ex_iconv_close)(iconv_t) = NIL;

#ifdef __WIN32__
#include <smooth/application.h>
#include <smooth/graphics/window.h>
#include <smooth/layer.h>
#include <smooth/objectmanager.h>
#include <smooth/toolkit.h>
#include <smooth/object.h>
#include <smooth/version.h>
#include <smooth/loop.h>
#include <smooth/objectproperties.h>
#include <smooth/toolwindow.h>
#include <smooth/divider.h>
#include <smooth/pciio.h>
#include <smooth/mdiwindow.h>

HMODULE	 iconvdll = NIL;

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

S::Bool S::LoadIconvDLL()
{
#ifdef __WIN32__
	iconvdll = LoadLibraryA(Application::GetApplicationDirectory().Append("iconv.dll"));

	if (iconvdll == NIL) return false;

	ex_iconv	= (size_t (*)(iconv_t, const char **, size_t *, char **, size_t *)) GetProcAddress(iconvdll, "libiconv");
	ex_iconv_open	= (iconv_t (*)(const char *, const char *)) GetProcAddress(iconvdll, "libiconv_open");
	ex_iconv_close	= (int (*)(iconv_t)) GetProcAddress(iconvdll, "libiconv_close");

	if (ex_iconv == NIL)		{ FreeIconvDLL(); return false; }
	if (ex_iconv_open == NIL)	{ FreeIconvDLL(); return false; }
	if (ex_iconv_close == NIL)	{ FreeIconvDLL(); return false; }
#else
	ex_iconv	= iconv;
	ex_iconv_open	= iconv_open;
	ex_iconv_close	= iconv_close;
#endif

	return true;
}

S::Void S::FreeIconvDLL()
{
#ifdef __WIN32__
	FreeLibrary(iconvdll);
#endif
	ex_iconv = NIL;
	ex_iconv_open = NIL;
	ex_iconv_close = NIL;
}
