#ifndef SHLOBJ_MINI_H
#define SHLOBJ_MINI_H

#include <windows.h>

#define BIF_RETURNONLYFSDIRS 1

typedef struct _SHITEMID {
	USHORT	cb;
	BYTE	abID[1];
} SHITEMID, * LPSHITEMID;

typedef const SHITEMID *LPCSHITEMID;

typedef struct _ITEMIDLIST {
	SHITEMID mkid;
} ITEMIDLIST,*LPITEMIDLIST;

typedef const ITEMIDLIST *LPCITEMIDLIST;
typedef int (CALLBACK* BFFCALLBACK)(HWND,UINT,LPARAM,LPARAM);

typedef struct _browseinfoA {
	HWND		hwndOwner;
	LPCITEMIDLIST	pidlRoot;
	LPSTR		pszDisplayName;
	LPCSTR		lpszTitle;
	UINT		ulFlags;
	BFFCALLBACK	lpfn;
	LPARAM		lParam;
	int		iImage;
} BROWSEINFOA,*PBROWSEINFOA,*LPBROWSEINFOA;

typedef struct _browseinfoW {
	HWND		hwndOwner;
	LPCITEMIDLIST	pidlRoot;
	LPWSTR		pszDisplayName;
	LPCWSTR		lpszTitle;
	UINT		ulFlags;
	BFFCALLBACK	lpfn;
	LPARAM		lParam;
	int		iImage;
} BROWSEINFOW,*PBROWSEINFOW,*LPBROWSEINFOW;

extern "C"
{
	LPITEMIDLIST WINAPI SHBrowseForFolderW(PBROWSEINFOW);
	LPITEMIDLIST WINAPI SHBrowseForFolderA(PBROWSEINFOA);
	BOOL WINAPI SHGetPathFromIDListW(LPCITEMIDLIST,LPWSTR);
	BOOL WINAPI SHGetPathFromIDListA(LPCITEMIDLIST,LPSTR);
}

#endif
