 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_DROPTARGET
#define H_OBJSMOOTH_DROPTARGET

#include <shlobj.h>

namespace smooth
{
	namespace GUI
	{
		class WindowGDI;
	};
};

#include "../../../../definitions.h"

namespace smooth
{
	namespace GUI
	{
		class DropTarget : public IDropTarget
		{
			private:
				static const CLIPFORMAT	 CF_SHELLIDLIST;

				ULONG			 refCount;

				WindowGDI		*backend;
				IDataObject		*data;
			public:
				static Array<String>	 GetFilesFromHDROP(HDROP);
				static Array<String>	 GetFilesFromCIDA(CIDA *);

							 DropTarget(WindowGDI *);
				virtual			~DropTarget();

				Array<String>		 GetDroppedFiles();

				/* IUnknown methods.
				 */
				ULONG STDMETHODCALLTYPE	 AddRef();
				ULONG STDMETHODCALLTYPE	 Release();

				STDMETHOD		 (QueryInterface)(REFIID, void **);
						
				/* IDropTarget methods.
				 */
				STDMETHOD		 (DragEnter)(LPDATAOBJECT, DWORD, POINTL, LPDWORD);
				STDMETHOD		 (DragOver)(DWORD, POINTL, LPDWORD);
				STDMETHOD		 (DragLeave)();
				STDMETHOD		 (Drop)(LPDATAOBJECT, DWORD, POINTL, LPDWORD);
		};
	};
};

#endif
