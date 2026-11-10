 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_WINDOWWAYLAND
#define H_OBJSMOOTH_WINDOWWAYLAND

namespace smooth
{
	namespace GUI
	{
		class WindowWayland;
	};
};

#include "../windowbackend.h"
#include "../../../../graphics/forms/rect.h"
#include "../../../../input/keyboard.h"
#include "../../../../input/pointer.h"

#include <wayland-client.h>
#include <cairo/cairo.h>

struct xdg_surface;
struct xdg_popup;
struct libdecor_frame;
struct libdecor_configuration;

namespace smooth
{
	namespace GUI
	{
		const Short	 WINDOW_WAYLAND	= 5;

		/* Shared memory buffer used to present window contents.
		 */
		struct WaylandBuffer
		{
			wl_buffer	*buffer;

			Void		*data;
			Int		 dataSize;

			Size		 size;

			Bool		 busy;
			Rect		 damage;
		};

		class WindowWayland : public WindowBackend
		{
			private:
				static Array<WindowWayland *, Void *>	 windowBackends;

				static Int				 focusWndID;

				Array<WaylandBuffer *, Void *>		 buffers;
				Bool					 presentPending;
				cairo_surface_t				*presentSource;

				WindowWayland				*FindLeaderWindow();
				WindowWayland				*FindParentWindow();

				Bool					 IsModalWindowActive();
				Bool					 IsToolWindow() const;

				Void					 CreateRole();
				Void					 DestroyRole();

				Void					 CreateToplevel();
				Void					 CreatePopup();

				WaylandBuffer				*GetFreeBuffer();
				Void					 DestroyBuffers();

				Void					 UpdateSizeHints();

				Size					 GetRealSize(const Size &) const;
				Size					 GetFloatingSize(Bool) const;

				Void					 UpdateMetrics(const Size &, Bool);

				Input::Keyboard::Key			 ConvertKey(UnsignedInt);
			protected:
				Int					 id;

				Bool					 opened;

				wl_surface				*surface;

				/* Toplevel windows use a libdecor frame,
				 * popups use an xdg_surface with an xdg_popup.
				 */
				libdecor_frame				*frame;
				xdg_surface				*xdgSurface;
				xdg_popup				*xdgPopup;

				WindowWayland				*parent;

				Bool					 configured;
				Point					 popupOffset;

				Int					 scale;
				Float					 fontSize;

				String					 title;

				Point					 pos;
				Size					 size;
				Size					 realSize;
				Size					 floatingSize;

				Size					 minSize;
				Size					 maxSize;

				Bool					 maximized;

				Rect					 restoredRect;

				Int					 flags;

				Bool					 enableDropFiles;
				Array<String>				 droppedFiles;

				Input::Pointer::CursorType		 cursor;
			public:
				static Int				 Initialize();
				static Int				 Free();

				/* Process events queued for deferred delivery and
				 * key repeat. Return True if something was processed.
				 */
				static Bool				 ProcessPendingEvents();

									 WindowWayland(Void * = NIL);
									~WindowWayland();

				Void					*GetSystemWindow() const;

				static WindowWayland			*GetWindowBackend(wl_surface *);
				static WindowWayland			*GetWindowBackend(const Window *);

				/* Present the contents of a backbuffer.
				 */
				Void					 Present(cairo_surface_t *, const Rect &);

				Int					 Open(const String &, const Point &, const Size &, Int);
				Int					 Close();

				Int					 RequestClose();

				Int					 SetTitle(const String &);

				Int					 EnableDropFiles(Bool);
				const Array<String>			&GetDroppedFiles() const;

				Int					 SetMinimumSize(const Size &);
				Int					 SetMaximumSize(const Size &);

				Int					 Show();
				Int					 Hide();

				Rect					 GetRestoredWindowRect() const		{ return restoredRect; }

				Int					 SetMetrics(const Point &, const Size &);

				Int					 Minimize();

				Int					 Maximize();
				Int					 Restore();

				Int					 Raise();

				Void					 SetPointerCursor(Input::Pointer::CursorType);

				/* Callbacks used by protocol listeners.
				 */
				Void					 OnConfigure(const Size &, Bool);
				Void					 OnFrameConfigure(libdecor_frame *, libdecor_configuration *);
				Void					 OnFrameCommit();
				Void					 OnFrameClose();
				Void					 OnPopupConfigure(const Point &);
				Void					 OnXdgSurfaceConfigure(UnsignedInt);
				Void					 OnPopupDone();
				Void					 OnBufferRelease(wl_buffer *);

				Void					 OnPointerEnter(Float, Float);
				Void					 OnPointerLeave();
				Void					 OnPointerMotion(Float, Float);
				Void					 OnPointerButton(UnsignedInt, UnsignedInt, Bool);
				Void					 OnPointerAxis(Int);

				Void					 OnKeyboardEnter();
				Void					 OnKeyboardLeave();
				Void					 OnKey(UnsignedInt, Bool);

				Void					 UpdatePointerPosition(Float, Float);
				Void					 OnDrop(const Array<String> &);

				/* Clipboard access.
				 */
				static String				 GetClipboardText();
				static Bool				 SetClipboardText(const String &);

				static String				 GetSelectionText();
				static Bool				 SetSelectionText(const String &);

				Bool					 IsDropFilesEnabled() const		{ return enableDropFiles; }
				Int					 GetScale() const			{ return scale; }
				const Point				&GetPosition() const			{ return pos; }
		};
	};
};

#endif
