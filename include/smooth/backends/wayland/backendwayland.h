 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BACKENDWAYLAND
#define H_OBJSMOOTH_BACKENDWAYLAND

namespace smooth
{
	namespace Backends
	{
		class BackendWayland;
	};
};

#include "../backend.h"
#include "../../graphics/forms/rect.h"

#include <wayland-client.h>

struct xdg_wm_base;
struct libdecor;
struct zwp_primary_selection_device_manager_v1;
struct xdg_toplevel_icon_manager_v1;
struct wp_viewporter;
struct zxdg_output_manager_v1;
struct zxdg_output_v1;

namespace smooth
{
	namespace Backends
	{
		const Short	 BACKEND_WAYLAND = 6;

		/* Information about a Wayland output (monitor).
		 */
		struct WaylandOutput
		{
			wl_output	*output;
			zxdg_output_v1	*xdgOutput;
			UnsignedInt	 name;

			GUI::Point	 position;
			GUI::Size	 size;
			GUI::Size	 logicalSize;

			Int		 scale;
		};

		class BackendWayland : public Backend
		{
			private:
				static wl_display			*display;
				static wl_registry			*registry;

				static wl_compositor			*compositor;
				static wl_shm				*shm;
				static xdg_wm_base			*wmBase;
				static wl_seat				*seat;
				static wl_data_device_manager		*dataDeviceManager;
				static zwp_primary_selection_device_manager_v1 *primarySelectionManager;
				static xdg_toplevel_icon_manager_v1 *toplevelIconManager;
				static wp_viewporter		*viewporter;
				static zxdg_output_manager_v1	*xdgOutputManager;

				static Float			 scale;

				static Void			 CreateXdgOutput(WaylandOutput *);

				static libdecor				*decorContext;

				static Array<WaylandOutput *, Void *>	 outputs;

				static UnsignedInt			 seatCapabilities;

				static const wl_pointer_listener	*pointerListener;
				static const wl_keyboard_listener	*keyboardListener;

				static wl_pointer			*pointer;
				static wl_keyboard			*keyboard;

				static Void				 UpdateSeatDevices();
			public:
									 BackendWayland();
				virtual					~BackendWayland();

				Int					 Init();
				Int					 Deinit();

				/* Dispatch pending Wayland events without blocking.
				 */
				static Int				 Dispatch();

				/* Perform a roundtrip to the compositor.
				 */
				static Int				 Roundtrip();

				/* Register listeners for seat devices. The pointer and keyboard
				 * objects are created once listeners are set, so no events get lost.
				 */
				static Void				 SetSeatListeners(const wl_pointer_listener *, const wl_keyboard_listener *);

				static wl_display			*GetDisplay()		{ return display; }

				static wl_compositor			*GetCompositor()	{ return compositor; }
				static wl_shm				*GetShm()		{ return shm; }
				static xdg_wm_base			*GetWMBase()		{ return wmBase; }
				static wl_seat				*GetSeat()		{ return seat; }
				static wl_data_device_manager		*GetDataDeviceManager()	{ return dataDeviceManager; }
				static zwp_primary_selection_device_manager_v1 *GetPrimarySelectionManager() { return primarySelectionManager; }
				static xdg_toplevel_icon_manager_v1 *GetToplevelIconManager() { return toplevelIconManager; }
				static wp_viewporter	*GetViewporter()		{ return viewporter; }

				static libdecor				*GetDecorContext()	{ return decorContext; }

				static wl_pointer			*GetPointer()		{ return pointer; }

				static Int				 GetNOfOutputs()	{ return outputs.Length(); }
				static const WaylandOutput		*GetNthOutput(Int n)	{ return outputs.GetNth(n); }

				/* Get the scale factor to render windows at. This is fractional
				 * if the compositor supports viewports and integer otherwise.
				 * It is determined once and stays fixed for the session.
				 */
				static Float		 GetScale();

				/* Internal registry callbacks.
				 */
				static Void				 OnRegistryGlobal(wl_registry *, UnsignedInt, const char *, UnsignedInt);
				static Void				 OnRegistryGlobalRemove(wl_registry *, UnsignedInt);

				static Void				 OnSeatCapabilities(wl_seat *, UnsignedInt);
		};
	};
};

#endif
