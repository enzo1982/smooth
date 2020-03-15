 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/gdi/droptarget.h>
#include <smooth/gui/window/backends/gdi/windowgdi.h>

const CLIPFORMAT	 smooth::GUI::DropTarget::CF_SHELLIDLIST = RegisterClipboardFormat(CFSTR_SHELLIDLIST);

smooth::GUI::DropTarget::DropTarget(WindowGDI *backend)
{
	refCount      = 1;

	data	      = NIL;

	this->backend = backend;
}

smooth::GUI::DropTarget::~DropTarget()
{
}

smooth::Array<smooth::String> smooth::GUI::DropTarget::GetFilesFromHDROP(HDROP hDrop)
{
	Array<String>	 fileNames;

	/* Query number of files dropped.
	 */
	Int	 nOfFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

	/* Query dropped files.
	 */
	Buffer<wchar_t>	 buffer(32768);

	for (Int i = 0; i < nOfFiles; i++)
	{
		DragQueryFile(hDrop, i, buffer, buffer.Size());

		fileNames.Add(String(buffer));
	}

	return fileNames;
}

smooth::Array<smooth::String> smooth::GUI::DropTarget::GetFilesFromCIDA(CIDA *cida)
{
	Array<String>	 fileNames;

	/* Get parent folder descriptor.
	 */
	LPCITEMIDLIST	 parentFolder = LPCITEMIDLIST(((LPBYTE) cida) + cida->aoffset[0]);

	for (UnsignedInt i = 0; i < cida->cidl; ++i)
	{
		/* Get shell item.
		 */
		IShellItem	*shellItem = NIL;

		if (SHCreateShellItem(parentFolder, NULL, LPCITEMIDLIST(((LPBYTE) cida) + cida->aoffset[i + 1]), &shellItem) != S_OK) continue;

		/* Query path from shell item.
		 */
		wchar_t	*path = NIL;

		if (shellItem->GetDisplayName(SIGDN_FILESYSPATH, &path) == S_OK)
		{
			fileNames.Add(path);

			CoTaskMemFree(path);
		}

		shellItem->Release();
	}

	return fileNames;
}

smooth::Array<smooth::String> smooth::GUI::DropTarget::GetDroppedFiles()
{
	if (data == NIL) return Array<String>();

	Array<String>	 fileNames;

	/* Try HDROP and ShellIDList clipboard formats.
	 */
	FORMATETC	 formatHDrop   = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	FORMATETC	 formatIDList  = { CF_SHELLIDLIST, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM	 storageMedium = { 0 };

	if (data->GetData(&formatIDList, &storageMedium) == S_OK)
	{
		/* Get files from ShellIDList objects.
		 */
		if (CIDA *cida = (CIDA *) GlobalLock(storageMedium.hGlobal))
		{
			fileNames = GetFilesFromCIDA(cida);

			GlobalUnlock(cida);
		}

		ReleaseStgMedium(&storageMedium);
	}
	else if (data->GetData(&formatHDrop, &storageMedium) == S_OK)
	{
		/* Get files from HDROP handle.
		 */
		if (HDROP hDrop = (HDROP) GlobalLock(storageMedium.hGlobal))
		{
			fileNames = GetFilesFromHDROP(hDrop);

			GlobalUnlock(hDrop);
		}

		ReleaseStgMedium(&storageMedium);
	}

	return fileNames;
}

ULONG STDMETHODCALLTYPE smooth::GUI::DropTarget::AddRef()
{
	return ++refCount;
}

ULONG STDMETHODCALLTYPE smooth::GUI::DropTarget::Release()
{
	if (--refCount == 0)
	{
		delete this;

		return 0;
	}

	return refCount;
}

STDMETHODIMP smooth::GUI::DropTarget::QueryInterface(REFIID riid, void **ppvObject)
{
	if	(riid == IID_IUnknown)	  *ppvObject = (IUnknown *) this;
	else if	(riid == IID_IDropTarget) *ppvObject = (IDropTarget *) this;
	else				  return E_NOINTERFACE;

	AddRef();

	return S_OK;
}

STDMETHODIMP smooth::GUI::DropTarget::DragEnter(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	/* Look for supported drop formats.
	 */
	Bool	 formatSupported = False;

	if (*pdwEffect & DROPEFFECT_COPY)
	{
		IEnumFORMATETC	*enumFormat = NIL;

		if (pDataObj->EnumFormatEtc(DATADIR_GET, &enumFormat) == S_OK)
		{
			FORMATETC format;

			while (enumFormat->Next(1, &format, NULL) == S_OK)
			{
				if (format.cfFormat != CF_HDROP && format.cfFormat != CF_SHELLIDLIST) continue;

				formatSupported = True;

				break;
			}

			enumFormat->Release();
		}
	}

	/* Check if we found a supported format.
	 */
	if (formatSupported)
	{
		*pdwEffect = DROPEFFECT_COPY;

		data = pDataObj;
		data->AddRef();
	}
	else
	{
		*pdwEffect = DROPEFFECT_NONE;
	}

	return S_OK;
}

STDMETHODIMP smooth::GUI::DropTarget::DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	if (*pdwEffect & DROPEFFECT_COPY) *pdwEffect = DROPEFFECT_COPY;
	else				  *pdwEffect = DROPEFFECT_NONE;

	return S_OK;
}

STDMETHODIMP smooth::GUI::DropTarget::DragLeave()
{
	if (data == NIL) return S_OK;

	data->Release();
	data = NIL;

	return S_OK;
}

STDMETHODIMP smooth::GUI::DropTarget::Drop(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	RECT	 windowRect;

	GetWindowRect((HWND) backend->GetSystemWindow(), &windowRect);

	backend->onEvent.Call(SM_DROPFILES, pt.x - windowRect.left, pt.y - windowRect.top);

	*pdwEffect = DROPEFFECT_COPY;

	data->Release();
	data = NIL;

	return S_OK;
}
