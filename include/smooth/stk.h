 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_STK_
#define _H_OBJSMOOTH_STK_

class SMOOTH;
class SMOOTHWindow;
class SMOOTHObject;

#include "definitions.h"

class SMOOTHAPI SMOOTH
{
	private:
		static SMOOTHInt		 nextID;
		static SMOOTHInt		 nextGUID;
	public:
		class SMOOTHAPI Setup
		{
			public:
				static SMOOTHInt	 BackgroundColor;
				static SMOOTHInt	 ClientColor;
				static SMOOTHInt	 ClientTextColor;
				static SMOOTHInt	 LightGrayColor;
				static SMOOTHInt	 DividerLightColor;
				static SMOOTHInt	 DividerDarkColor;
				static SMOOTHInt	 TextColor;
				static SMOOTHInt	 GrayTextColor;
				static SMOOTHInt	 ShadowColor;
				static SMOOTHInt	 GradientStartColor;
				static SMOOTHInt	 GradientEndColor;
				static SMOOTHInt	 GradientTextColor;
				static SMOOTHInt	 InactiveGradientStartColor;
				static SMOOTHInt	 InactiveGradientEndColor;
				static SMOOTHInt	 InactiveGradientTextColor;
				static SMOOTHInt	 TooltipColor;
				static SMOOTHInt	 TooltipTextColor;

				static SMOOTHBool	 enableUnicode;

				static SMOOTHFloat	 FontSize;
		};

		static SMOOTHInt		 Success;
		static SMOOTHInt		 Error;
		static SMOOTHInt		 Break;

		static SMOOTHBool		 True;
		static SMOOTHBool		 False;

		static SMOOTHString		 StartDirectory;

		static SMOOTHObject		*GetObject(SMOOTHInt, SMOOTHInt);
		static SMOOTHWindow		*GetWindow(HWND);

		static SMOOTHInt		 DeleteObject(SMOOTHObject *);

		static SMOOTHVoid		 SetLanguage(SMOOTHInt);
		static SMOOTHBool		 SetStartDirectory(SMOOTHString);

		static HBITMAP			 LoadImage(SMOOTHString, SMOOTHInt, SMOOTHString);
		static SMOOTHInt		 RequestGUID();
		static SMOOTHInt		 RequestObjectID();
		static SMOOTHInt		 RequestObjectHandle();
		static SMOOTHVoid		 CloseWindow(SMOOTHWindow *);
		static SMOOTHString		 GetVersionString();

		static SMOOTHVoid		 SendMessage(SMOOTHWindow *, SMOOTHInt, SMOOTHInt, SMOOTHInt);

		static SMOOTHInt		 MessageBox(SMOOTHString, SMOOTHString, SMOOTHInt, char *);
		static SMOOTHInt		 MessageBox(SMOOTHString, SMOOTHString, SMOOTHInt, wchar_t *);
		static SMOOTHInt		 SplashScreen(HBITMAP, SMOOTHInt);

		static SMOOTHVoid		 Init();
		static SMOOTHVoid		 Free();
		static SMOOTHInt		 Loop();

		SMOOTHVoid			 Main();
		SMOOTHVoid			 AttachDLL();
		SMOOTHVoid			 DetachDLL();
};

#ifdef __SMOOTH_DLL__
HBITMAP		 GrayscaleBitmap(HBITMAP);
HBITMAP		 DetectTransparentRegions(HBITMAP);
SMOOTHBool	 Affected(SMOOTHObject *, SMOOTHRect &);
#endif

#endif
