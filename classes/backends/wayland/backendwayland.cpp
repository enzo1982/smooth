 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/wayland/backendwayland.h>
#include <smooth/backends/freedesktop/desktopsettings.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

#include <xdg-shell-client-protocol.h>
#include <libdecor.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <poll.h>

using namespace smooth;

S::Backends::Backend *CreateBackendWayland()
{
	return new S::Backends::BackendWayland();
}

S::Int	 backendWaylandTmp = S::Backends::Backend::AddBackend(&CreateBackendWayland);

wl_display				*S::Backends::BackendWayland::display		 = NIL;
wl_registry				*S::Backends::BackendWayland::registry		 = NIL;

wl_compositor				*S::Backends::BackendWayland::compositor	 = NIL;
wl_shm					*S::Backends::BackendWayland::shm		 = NIL;
xdg_wm_base				*S::Backends::BackendWayland::wmBase		 = NIL;
wl_seat					*S::Backends::BackendWayland::seat		 = NIL;
wl_data_device_manager			*S::Backends::BackendWayland::dataDeviceManager	 = NIL;

libdecor				*S::Backends::BackendWayland::decorContext	 = NIL;

S::Array<S::Backends::WaylandOutput *, S::Void *>	 S::Backends::BackendWayland::outputs;

S::UnsignedInt				 S::Backends::BackendWayland::seatCapabilities	 = 0;

const wl_pointer_listener		*S::Backends::BackendWayland::pointerListener	 = NIL;
const wl_keyboard_listener		*S::Backends::BackendWayland::keyboardListener	 = NIL;

wl_pointer				*S::Backends::BackendWayland::pointer		 = NIL;
wl_keyboard				*S::Backends::BackendWayland::keyboard		 = NIL;

/* xdg_wm_base listener.
 */
static void wmBasePing(void *data, xdg_wm_base *wmBase, uint32_t serial)
{
	xdg_wm_base_pong(wmBase, serial);
}

static const xdg_wm_base_listener wmBaseListener = { wmBasePing };

/* wl_output listener.
 */
static void outputGeometry(void *data, wl_output *output, int32_t x, int32_t y, int32_t physicalWidth, int32_t physicalHeight, int32_t subpixel, const char *make, const char *model, int32_t transform)
{
	Backends::WaylandOutput	*info = (Backends::WaylandOutput *) data;

	info->position = GUI::Point(x, y);
}

static void outputMode(void *data, wl_output *output, uint32_t flags, int32_t width, int32_t height, int32_t refresh)
{
	Backends::WaylandOutput	*info = (Backends::WaylandOutput *) data;

	if (flags & WL_OUTPUT_MODE_CURRENT) info->size = GUI::Size(width, height);
}

static void outputDone(void *data, wl_output *output)
{
}

static void outputScale(void *data, wl_output *output, int32_t factor)
{
	Backends::WaylandOutput	*info = (Backends::WaylandOutput *) data;

	info->scale = factor;
}

static const wl_output_listener outputListener = { outputGeometry, outputMode, outputDone, outputScale };

/* wl_seat listener.
 */
static void seatCapabilities(void *data, wl_seat *seat, uint32_t capabilities)
{
	Backends::BackendWayland::OnSeatCapabilities(seat, capabilities);
}

static void seatName(void *data, wl_seat *seat, const char *name)
{
}

static const wl_seat_listener seatListener = { seatCapabilities, seatName };

/* wl_registry listener.
 */
static void registryGlobal(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
	Backends::BackendWayland::OnRegistryGlobal(registry, name, interface, version);
}

static void registryGlobalRemove(void *data, wl_registry *registry, uint32_t name)
{
	Backends::BackendWayland::OnRegistryGlobalRemove(registry, name);
}

static const wl_registry_listener registryListener = { registryGlobal, registryGlobalRemove };

/* libdecor error handler.
 */
static void decorError(libdecor *context, enum libdecor_error error, const char *message)
{
	fprintf(stderr, "Error: libdecor: %s\n", message);
}

static libdecor_interface decorInterface = { decorError };

S::Backends::BackendWayland::BackendWayland()
{
	type = BACKEND_WAYLAND;
}

S::Backends::BackendWayland::~BackendWayland()
{
}

S::Int S::Backends::BackendWayland::Init()
{
	/* Connect to Wayland display.
	 */
	const char	*displayId = getenv("WAYLAND_DISPLAY");

	if (displayId == NIL) return Success();

	display = wl_display_connect(NIL);

	if (display == NIL)
	{
		fprintf(stderr, "Error: Unable to connect to Wayland display at %s.\n", displayId);

		return Error();
	}

	/* Bind globals. The second roundtrip collects
	 * output and seat information.
	 */
	registry = wl_display_get_registry(display);

	wl_registry_add_listener(registry, &registryListener, NIL);

	wl_display_roundtrip(display);
	wl_display_roundtrip(display);

	if (compositor == NIL || shm == NIL || wmBase == NIL)
	{
		fprintf(stderr, "Error: Wayland compositor lacks required interfaces (wl_compositor, wl_shm, xdg_wm_base).\n");

		Deinit();

		return Error();
	}

	/* Create decoration context. This is done before initializing GTK, as the
	 * libdecor GTK plugin wants to initialize GTK itself. Block SIGALRM first,
	 * so threads created by the plugin inherit the signal mask.
	 */
	DesktopSettings::BlockTimerInterrupts(True);

	decorContext = libdecor_new(display, &decorInterface);

	if (decorContext == NIL)
	{
		fprintf(stderr, "Error: Unable to create libdecor context.\n");

		Deinit();

		return Error();
	}

	/* Init the desktop settings helper.
	 */
	DesktopSettings::InitDependencies();

	/* Get default colors.
	 */
	DesktopSettings::UpdateColors();

	/* Set locale.
	 */
	setlocale(LC_ALL, "");

	return Success();
}

S::Int S::Backends::BackendWayland::Deinit()
{
	if (display == NIL) return Success();

	if (decorContext != NIL) libdecor_unref(decorContext);

	if (pointer  != NIL) wl_pointer_destroy(pointer);
	if (keyboard != NIL) wl_keyboard_destroy(keyboard);

	foreach (WaylandOutput *output, outputs)
	{
		wl_output_destroy(output->output);

		delete output;
	}

	outputs.RemoveAll();

	if (dataDeviceManager != NIL) wl_data_device_manager_destroy(dataDeviceManager);
	if (seat	      != NIL) wl_seat_destroy(seat);
	if (wmBase	      != NIL) xdg_wm_base_destroy(wmBase);
	if (shm		      != NIL) wl_shm_destroy(shm);
	if (compositor	      != NIL) wl_compositor_destroy(compositor);

	wl_registry_destroy(registry);

	wl_display_disconnect(display);

	decorContext	  = NIL;
	pointer		  = NIL;
	keyboard	  = NIL;
	dataDeviceManager = NIL;
	seat		  = NIL;
	wmBase		  = NIL;
	shm		  = NIL;
	compositor	  = NIL;
	registry	  = NIL;
	display		  = NIL;

	DesktopSettings::BlockTimerInterrupts(False);

	return Success();
}

S::Void S::Backends::BackendWayland::OnRegistryGlobal(wl_registry *registry, UnsignedInt name, const char *interface, UnsignedInt version)
{
	if (strcmp(interface, wl_compositor_interface.name) == 0)
	{
		compositor = (wl_compositor *) wl_registry_bind(registry, name, &wl_compositor_interface, Math::Min(version, 4));
	}
	else if (strcmp(interface, wl_shm_interface.name) == 0)
	{
		shm = (wl_shm *) wl_registry_bind(registry, name, &wl_shm_interface, 1);
	}
	else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
	{
		wmBase = (xdg_wm_base *) wl_registry_bind(registry, name, &xdg_wm_base_interface, Math::Min(version, 2));

		xdg_wm_base_add_listener(wmBase, &wmBaseListener, NIL);
	}
	else if (strcmp(interface, wl_seat_interface.name) == 0 && seat == NIL)
	{
		seat = (wl_seat *) wl_registry_bind(registry, name, &wl_seat_interface, Math::Min(version, 5));

		wl_seat_add_listener(seat, &seatListener, NIL);
	}
	else if (strcmp(interface, wl_data_device_manager_interface.name) == 0)
	{
		dataDeviceManager = (wl_data_device_manager *) wl_registry_bind(registry, name, &wl_data_device_manager_interface, Math::Min(version, 3));
	}
	else if (strcmp(interface, wl_output_interface.name) == 0)
	{
		WaylandOutput	*output = new WaylandOutput();

		output->name   = name;
		output->scale  = 1;
		output->output = (wl_output *) wl_registry_bind(registry, name, &wl_output_interface, Math::Min(version, 2));

		wl_output_add_listener(output->output, &outputListener, output);

		outputs.Add(output);
	}
}

S::Void S::Backends::BackendWayland::OnRegistryGlobalRemove(wl_registry *registry, UnsignedInt name)
{
	for (Int i = 0; i < outputs.Length(); i++)
	{
		WaylandOutput	*output = outputs.GetNth(i);

		if (output->name != name) continue;

		wl_output_destroy(output->output);

		delete output;

		outputs.RemoveNth(i);

		break;
	}
}

S::Void S::Backends::BackendWayland::OnSeatCapabilities(wl_seat *seat, UnsignedInt capabilities)
{
	seatCapabilities = capabilities;

	UpdateSeatDevices();
}

S::Void S::Backends::BackendWayland::SetSeatListeners(const wl_pointer_listener *nPointerListener, const wl_keyboard_listener *nKeyboardListener)
{
	pointerListener	 = nPointerListener;
	keyboardListener = nKeyboardListener;

	UpdateSeatDevices();
}

S::Void S::Backends::BackendWayland::UpdateSeatDevices()
{
	if (seat == NIL) return;

	/* Create or destroy pointer device.
	 */
	if ((seatCapabilities & WL_SEAT_CAPABILITY_POINTER) && pointer == NIL && pointerListener != NIL)
	{
		pointer = wl_seat_get_pointer(seat);

		wl_pointer_add_listener(pointer, pointerListener, NIL);
	}
	else if (!(seatCapabilities & WL_SEAT_CAPABILITY_POINTER) && pointer != NIL)
	{
		wl_pointer_destroy(pointer);

		pointer = NIL;
	}

	/* Create or destroy keyboard device.
	 */
	if ((seatCapabilities & WL_SEAT_CAPABILITY_KEYBOARD) && keyboard == NIL && keyboardListener != NIL)
	{
		keyboard = wl_seat_get_keyboard(seat);

		wl_keyboard_add_listener(keyboard, keyboardListener, NIL);
	}
	else if (!(seatCapabilities & WL_SEAT_CAPABILITY_KEYBOARD) && keyboard != NIL)
	{
		wl_keyboard_destroy(keyboard);

		keyboard = NIL;
	}
}

S::Int S::Backends::BackendWayland::Dispatch()
{
	if (display == NIL) return -1;

	/* Let libdecor dispatch if available, as
	 * its plugins may need to process events.
	 */
	if (decorContext != NIL) return libdecor_dispatch(decorContext, 0);

	/* Read and dispatch events without blocking.
	 */
	while (wl_display_prepare_read(display) != 0) wl_display_dispatch_pending(display);

	wl_display_flush(display);

	pollfd	 pfd = { wl_display_get_fd(display), POLLIN, 0 };

	if (poll(&pfd, 1, 0) > 0) wl_display_read_events(display);
	else			  wl_display_cancel_read(display);

	return wl_display_dispatch_pending(display);
}

S::Int S::Backends::BackendWayland::GetScale()
{
	/* Use the highest scale of all outputs, so content
	 * looks sharp on every monitor.
	 */
	Int	 scale = 1;

	foreach (WaylandOutput *output, outputs) scale = Math::Max(scale, output->scale);

	return scale;
}

S::Int S::Backends::BackendWayland::Roundtrip()
{
	if (display == NIL) return -1;

	return wl_display_roundtrip(display);
}
