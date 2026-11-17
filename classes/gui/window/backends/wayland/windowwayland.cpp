 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/wayland/windowwayland.h>
#include <smooth/backends/platform.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/special/cursor.h>
#include <smooth/gui/widgets/basic/editbox.h>
#include <smooth/gui/application/application.h>
#include <smooth/input/pointer.h>
#include <smooth/threads/mutex.h>
#include <smooth/misc/math.h>
#include <smooth/misc/encoding/urlencode.h>
#include <smooth/foreach.h>
#include <smooth/init.h>
#include <smooth/backends/wayland/backendwayland.h>

#include <xdg-shell-client-protocol.h>
#include <primary-selection-unstable-v1-client-protocol.h>
#include <xdg-toplevel-icon-v1-client-protocol.h>
#include <viewporter-client-protocol.h>
#include <xdg-activation-v1-client-protocol.h>
#include <text-input-unstable-v3-client-protocol.h>
#include <wayland-cursor.h>
#include <libdecor.h>

#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-compose.h>

#include <linux/input-event-codes.h>

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace smooth;
using namespace smooth::GUI;

S::GUI::WindowBackend *CreateWindowWayland()
{
	return new S::GUI::WindowWayland();
}

static S::Int Register_windowWaylandTmp() { return S::GUI::WindowBackend::SetBackend(&CreateWindowWayland); }

S::Int	 windowWaylandTmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_WAYLAND, &Register_windowWaylandTmp);

S::Int	 addWindowWaylandInitTmp = S::AddInitFunction(&S::GUI::WindowWayland::Initialize);
S::Int	 addWindowWaylandFreeTmp = S::AddFreeFunction(&S::GUI::WindowWayland::Free);

S::Array<S::GUI::WindowWayland *, S::Void *>	 S::GUI::WindowWayland::windowBackends;

S::Int	 S::GUI::WindowWayland::focusWndID = -1;

/* Global input and data transfer state.
 */
namespace
{
	/* Offer information attached to wl_data_offer objects.
	 */
	struct OfferInfo
	{
		Array<String>		 mimeTypes;

		Bool HasMimeType(const String &mimeType) const
		{
			foreach (const String &type, mimeTypes) if (type == mimeType) return True;

			return False;
		}
	};

	Threads::Mutex		*bufferMutex	    = NIL;

	Array<Int>		 pendingDestroy;

	/* Pointer state.
	 */
	wl_surface		*pointerSurface	    = NIL;
	UnsignedInt		 pointerSerial	    = 0;
	Float			 axisAccumulator    = 0;

	Float			 frameAxisValue	    = 0;
	Int			 frameAxisDiscrete  = 0;

	wl_cursor_theme		*cursorTheme	    = NIL;
	wl_surface		*cursorSurface	    = NIL;

	/* Keyboard state.
	 */
	wl_surface		*keyboardSurface    = NIL;

	xkb_context		*xkbContext	    = NIL;
	xkb_keymap		*xkbKeymap	    = NIL;
	xkb_state		*xkbState	    = NIL;

	xkb_compose_table	*composeTable	    = NIL;
	xkb_compose_state	*composeState	    = NIL;

	Int			 repeatRate	    = 25;
	Int			 repeatDelay	    = 600;

	UnsignedInt		 repeatKey	    = 0;
	Int64			 repeatNext	    = 0;

	/* Serial of the last input event, needed for setting the selection.
	 */
	UnsignedInt		 inputSerial	    = 0;

	/* Data device state.
	 */
	wl_data_device		*dataDevice	    = NIL;

	wl_data_offer		*selectionOffer	    = NIL;

	wl_data_offer		*dragOffer	    = NIL;
	wl_surface		*dragSurface	    = NIL;
	Bool			 dragAccepted	    = False;

	wl_data_source		*selectionSource    = NIL;
	String			 clipboardText;

	/* Primary selection state.
	 */
	zwp_primary_selection_device_v1	*primaryDevice = NIL;
	zwp_primary_selection_offer_v1	*primaryOffer  = NIL;
	zwp_primary_selection_source_v1	*primarySource = NIL;

	String			 primaryText;

	/* Text input (input method) state.
	 */
	zwp_text_input_v3	*textInput	    = NIL;
	wl_surface		*textInputSurface   = NIL;
	Bool			 textInputEnabled   = False;

	Cursor			*activeCursor	    = NIL;
	Point			 activeCursorPos;

	String			 pendingPreedit;
	Int			 pendingPreeditCaret = -1;
	String			 pendingCommit;

	Bool			 preeditShown	    = False;

	/* Convert a byte offset in a UTF-8 string to a character offset.
	 */
	Int CharacterOffset(const char *utf8, Int byteOffset)
	{
		if (utf8 == NIL || byteOffset < 0) return -1;

		Int	 characters = 0;

		for (Int i = 0; i < byteOffset && utf8[i] != 0; i++)
		{
			if ((utf8[i] & 0xC0) != 0x80) characters++;
		}

		return characters;
	}

	Int64 GetMilliseconds()
	{
		timespec	 ts;

		clock_gettime(CLOCK_MONOTONIC, &ts);

		return Int64(ts.tv_sec) * 1000 + ts.tv_nsec / 1000000;
	}

	/* Text formats offered for clipboard and primary selection.
	 */
	const char		*textMimeTypes[] = { "text/plain;charset=utf-8", "text/plain", "UTF8_STRING", "TEXT", "STRING" };

	/* Get the offer information attached to a data or primary selection offer.
	 */
	template <class t> OfferInfo *GetOfferInfo(t *offer)
	{
		return (OfferInfo *) wl_proxy_get_user_data((wl_proxy *) offer);
	}

	/* Destroy a data or primary selection offer and its offer information.
	 */
	template <class t> Void DestroyOffer(t *&offer, void (*destroy)(t *))
	{
		if (offer == NIL) return;

		delete GetOfferInfo(offer);

		destroy(offer);

		offer = NIL;
	}

	Void ResetDrag()
	{
		DestroyOffer(dragOffer, &wl_data_offer_destroy);

		dragSurface  = NIL;
		dragAccepted = False;
	}

	/* Read UTF-8 text from the read end of a pipe until EOF or timeout.
	 */
	String ReadPipe(int fd)
	{
		Buffer<char>	 data(4096);
		Int		 bytes = 0;

		while (True)
		{
			pollfd	 pfd = { fd, POLLIN, 0 };

			if (poll(&pfd, 1, 1000) <= 0) break;

			if (data.Size() - bytes < 4096) data.Resize(data.Size() * 2);

			ssize_t	 count = read(fd, (char *) data + bytes, data.Size() - bytes - 1);

			if	(count < 0 && errno == EINTR) continue;
			else if (count <= 0)		      break;

			bytes += count;
		}

		close(fd);

		data[bytes] = 0;

		String	 result;

		result.ImportFrom("UTF-8", data);

		return result;
	}

	/* Receive data from a data or primary selection offer synchronously.
	 */
	template <class t> String ReceiveOffer(t *offer, const char *mimeType, void (*receive)(t *, const char *, int32_t))
	{
		int	 fds[2];

		if (pipe2(fds, O_CLOEXEC) != 0) return NIL;

		receive(offer, mimeType, fds[1]);
		close(fds[1]);

		wl_display_flush(Backends::BackendWayland::GetDisplay());

		return ReadPipe(fds[0]);
	}

	/* Receive text from an offer, choosing the best available text format.
	 */
	template <class t> String ReceiveText(t *offer, void (*receive)(t *, const char *, int32_t))
	{
		const OfferInfo	*info = GetOfferInfo(offer);

		if	(info->HasMimeType("text/plain;charset=utf-8")) return ReceiveOffer(offer, "text/plain;charset=utf-8", receive);
		else if (info->HasMimeType("UTF8_STRING"))		return ReceiveOffer(offer, "UTF8_STRING", receive);
		else if (info->HasMimeType("text/plain"))		return ReceiveOffer(offer, "text/plain", receive);

		return NIL;
	}

	/* Write UTF-8 text to a file descriptor and close it.
	 */
	Void WriteText(int fd, const String &string)
	{
		const char	*text	= string.ConvertTo("UTF-8");
		Int		 length = text != NIL ? strlen(text) : 0;
		Int		 offset = 0;

		while (offset < length)
		{
			ssize_t	 count = write(fd, text + offset, length - offset);

			if	(count < 0 && errno == EINTR) continue;
			else if (count <= 0)		      break;

			offset += count;
		}

		close(fd);
	}

	/* Load cursor theme on demand.
	 */
	wl_cursor *GetCursor(Input::Pointer::CursorType type, Int scale)
	{
		if (cursorTheme == NIL)
		{
			const char	*themeName = getenv("XCURSOR_THEME");
			const char	*themeSize = getenv("XCURSOR_SIZE");
			Int		 size	   = themeSize != NIL ? atoi(themeSize) : 24;

			if (size <= 0) size = 24;

			cursorTheme = wl_cursor_theme_load(themeName, size * scale, Backends::BackendWayland::GetShm());
		}

		if (cursorTheme == NIL) return NIL;

		const char	*names[2] = { "left_ptr", "default" };

		if	(type == Input::Pointer::CursorTextEdit) { names[0] = "xterm";		   names[1] = "text";	   }
		else if (type == Input::Pointer::CursorHand)	 { names[0] = "hand2";		   names[1] = "pointer";   }
		else if (type == Input::Pointer::CursorHSize)	 { names[0] = "sb_h_double_arrow"; names[1] = "ew-resize"; }
		else if (type == Input::Pointer::CursorVSize)	 { names[0] = "sb_v_double_arrow"; names[1] = "ns-resize"; }

		wl_cursor	*cursor = wl_cursor_theme_get_cursor(cursorTheme, names[0]);

		if (cursor == NIL) cursor = wl_cursor_theme_get_cursor(cursorTheme, names[1]);

		return cursor;
	}

	/* Create a 32 bit shared memory buffer and map its data.
	 */
	wl_buffer *CreateBuffer(const Size &size, UnsignedInt format, Void **data)
	{
		Int	 stride	  = size.cx * 4;
		Int	 dataSize = stride * size.cy;
		int	 fd	  = memfd_create("smooth-buffer", MFD_CLOEXEC);

		if (fd < 0) return NIL;

		while (ftruncate(fd, dataSize) < 0)
		{
			if (errno == EINTR) continue;

			close(fd);

			return NIL;
		}

		*data = mmap(NIL, dataSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

		if (*data == MAP_FAILED) { close(fd); return NIL; }

		wl_shm_pool	*pool	= wl_shm_create_pool(Backends::BackendWayland::GetShm(), fd, dataSize);
		wl_buffer	*buffer = wl_shm_pool_create_buffer(pool, 0, size.cx, size.cy, stride, format);

		wl_shm_pool_destroy(pool);
		close(fd);

		return buffer;
	}

	Void DestroyBuffer(WaylandBuffer *buffer)
	{
		wl_buffer_destroy(buffer->buffer);
		munmap(buffer->data, buffer->dataSize);

		delete buffer;
	}

	Rect UniteRects(const Rect &rect1, const Rect &rect2)
	{
		if (rect1.GetWidth() <= 0 || rect1.GetHeight() <= 0) return rect2;
		if (rect2.GetWidth() <= 0 || rect2.GetHeight() <= 0) return rect1;

		return Rect::EncloseRect(rect1, rect2);
	}
};

/* xdg_surface and xdg_popup listeners.
 */
static void xdgSurfaceConfigure(void *data, xdg_surface *surface, uint32_t serial)
{
	((WindowWayland *) data)->OnXdgSurfaceConfigure(serial);
}

static const xdg_surface_listener xdgSurfaceListener = { xdgSurfaceConfigure };

static void xdgPopupConfigure(void *data, xdg_popup *popup, int32_t x, int32_t y, int32_t width, int32_t height)
{
	((WindowWayland *) data)->OnPopupConfigure(Point(x, y));
}

static void xdgPopupDone(void *data, xdg_popup *popup)
{
	((WindowWayland *) data)->OnPopupDone();
}

static const xdg_popup_listener xdgPopupListener = { xdgPopupConfigure, xdgPopupDone };

/* libdecor frame listener.
 */
static void frameConfigure(libdecor_frame *frame, libdecor_configuration *configuration, void *data)
{
	((WindowWayland *) data)->OnFrameConfigure(frame, configuration);
}

static void frameClose(libdecor_frame *frame, void *data)
{
	((WindowWayland *) data)->OnFrameClose();
}

static void frameCommit(libdecor_frame *frame, void *data)
{
	((WindowWayland *) data)->OnFrameCommit();
}

static void frameDismissPopup(libdecor_frame *frame, const char *seatName, void *data)
{
}

static libdecor_frame_interface frameInterface = { frameConfigure, frameClose, frameCommit, frameDismissPopup };

/* xdg_activation_token_v1 listener. The window is referenced by ID,
 * as it might have been destroyed when the token arrives.
 */
static void activationTokenDone(void *data, xdg_activation_token_v1 *token, const char *tokenString)
{
	WindowWayland::ActivateWindow((Int) (intptr_t) data, tokenString);

	xdg_activation_token_v1_destroy(token);
}

static const xdg_activation_token_v1_listener activationTokenListener = { activationTokenDone };

/* wl_buffer listener.
 */
static void bufferRelease(void *data, wl_buffer *buffer)
{
	((WindowWayland *) data)->OnBufferRelease(buffer);
}

static const wl_buffer_listener bufferListener = { bufferRelease };

/* wl_pointer listener.
 */
static void pointerEnter(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface, wl_fixed_t x, wl_fixed_t y)
{
	pointerSurface = surface;
	pointerSerial  = serial;

	WindowWayland	*window = WindowWayland::GetWindowBackend(surface);

	if (window != NIL) window->OnPointerEnter(wl_fixed_to_double(x), wl_fixed_to_double(y));
}

static void pointerLeave(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface)
{
	WindowWayland	*window = WindowWayland::GetWindowBackend(surface);

	pointerSurface = NIL;

	if (window != NIL) window->OnPointerLeave();
}

static void pointerMotion(void *data, wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
	WindowWayland	*window = WindowWayland::GetWindowBackend(pointerSurface);

	if (window != NIL) window->OnPointerMotion(wl_fixed_to_double(x), wl_fixed_to_double(y));
}

static void pointerButton(void *data, wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
	if (state == WL_POINTER_BUTTON_STATE_PRESSED) inputSerial = serial;

	WindowWayland	*window = WindowWayland::GetWindowBackend(pointerSurface);

	if (window != NIL) window->OnPointerButton(button, time, state == WL_POINTER_BUTTON_STATE_PRESSED);
}

/* Emit wheel events for the scroll data collected in a pointer frame.
 */
static void processAxis()
{
	WindowWayland	*window = WindowWayland::GetWindowBackend(pointerSurface);

	if (window == NIL || (frameAxisDiscrete == 0 && frameAxisValue == 0)) { frameAxisDiscrete = 0; frameAxisValue = 0; return; }

	if (frameAxisDiscrete != 0)
	{
		/* Mouse wheel with discrete steps.
		 */
		for (Int i = 0; i < Math::Abs(frameAxisDiscrete); i++) window->OnPointerAxis(frameAxisDiscrete > 0 ? -120 : 120);

		axisAccumulator = 0;
	}
	else
	{
		/* Continuous scrolling. Reset when the direction changes and emit
		 * a wheel step for each 10 units (one notch on most systems),
		 * rounding to the nearest step.
		 */
		if ((axisAccumulator > 0 && frameAxisValue < 0) || (axisAccumulator < 0 && frameAxisValue > 0)) axisAccumulator = 0;

		axisAccumulator += frameAxisValue;

		while (axisAccumulator >=  5) { window->OnPointerAxis(-120); axisAccumulator -= 10; }
		while (axisAccumulator <= -5) { window->OnPointerAxis( 120); axisAccumulator += 10; }
	}

	frameAxisDiscrete = 0;
	frameAxisValue	  = 0;
}

static void pointerAxis(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
{
	if (axis != WL_POINTER_AXIS_VERTICAL_SCROLL) return;

	frameAxisValue += wl_fixed_to_double(value);

	/* Pointers before version 5 do not send frame events.
	 */
	if (wl_pointer_get_version(pointer) < WL_POINTER_FRAME_SINCE_VERSION) processAxis();
}

static void pointerFrame(void *data, wl_pointer *pointer)
{
	processAxis();
}

static void pointerAxisSource(void *data, wl_pointer *pointer, uint32_t source)
{
}

static void pointerAxisStop(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis)
{
	if (axis == WL_POINTER_AXIS_VERTICAL_SCROLL) axisAccumulator = 0;
}

static void pointerAxisDiscrete(void *data, wl_pointer *pointer, uint32_t axis, int32_t discrete)
{
	if (axis == WL_POINTER_AXIS_VERTICAL_SCROLL) frameAxisDiscrete += discrete;
}

static const wl_pointer_listener pointerListener = { pointerEnter, pointerLeave, pointerMotion, pointerButton, pointerAxis, pointerFrame, pointerAxisSource, pointerAxisStop, pointerAxisDiscrete };

/* wl_keyboard listener.
 */
static void keyboardKeymap(void *data, wl_keyboard *keyboard, uint32_t format, int32_t fd, uint32_t size)
{
	if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) { close(fd); return; }

	char	*map = (char *) mmap(NIL, size, PROT_READ, MAP_PRIVATE, fd, 0);

	close(fd);

	if (map == MAP_FAILED) return;

	xkb_keymap	*keymap = xkb_keymap_new_from_string(xkbContext, map, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);

	munmap(map, size);

	if (keymap == NIL) return;

	if (xkbState  != NIL) xkb_state_unref(xkbState);
	if (xkbKeymap != NIL) xkb_keymap_unref(xkbKeymap);

	xkbKeymap = keymap;
	xkbState  = xkb_state_new(xkbKeymap);
}

static void keyboardEnter(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface, wl_array *keys)
{
	keyboardSurface = surface;

	WindowWayland	*window = WindowWayland::GetWindowBackend(surface);

	if (window != NIL) window->OnKeyboardEnter();
}

static void keyboardLeave(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface)
{
	WindowWayland	*window = WindowWayland::GetWindowBackend(surface);

	keyboardSurface = NIL;
	repeatKey	= 0;

	if (window != NIL) window->OnKeyboardLeave();
}

static void keyboardKey(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
	Bool		 pressed = (state == WL_KEYBOARD_KEY_STATE_PRESSED);
	UnsignedInt	 keycode = key + 8;

	if (pressed) inputSerial = serial;

	/* Set up key repeat.
	 */
	if	(pressed && xkbKeymap != NIL && xkb_keymap_key_repeats(xkbKeymap, keycode) && repeatRate > 0) { repeatKey = keycode; repeatNext = GetMilliseconds() + repeatDelay; }
	else if (!pressed && repeatKey == keycode)								       { repeatKey = 0; }

	WindowWayland	*window = WindowWayland::GetWindowBackend(keyboardSurface);

	if (window != NIL) window->OnKey(keycode, pressed);
}

static void keyboardModifiers(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t depressed, uint32_t latched, uint32_t locked, uint32_t group)
{
	if (xkbState != NIL) xkb_state_update_mask(xkbState, depressed, latched, locked, 0, 0, group);
}

static void keyboardRepeatInfo(void *data, wl_keyboard *keyboard, int32_t rate, int32_t delay)
{
	repeatRate  = rate;
	repeatDelay = delay;
}

static const wl_keyboard_listener keyboardListener = { keyboardKeymap, keyboardEnter, keyboardLeave, keyboardKey, keyboardModifiers, keyboardRepeatInfo };

/* wl_data_offer listener.
 */
static void dataOfferOffer(void *data, wl_data_offer *offer, const char *mimeType)
{
	((OfferInfo *) data)->mimeTypes.Add(mimeType);
}

static void dataOfferSourceActions(void *data, wl_data_offer *offer, uint32_t actions)
{
}

static void dataOfferAction(void *data, wl_data_offer *offer, uint32_t action)
{
}

static const wl_data_offer_listener dataOfferListener = { dataOfferOffer, dataOfferSourceActions, dataOfferAction };

/* wl_data_device listener.
 */
static void dataDeviceDataOffer(void *data, wl_data_device *device, wl_data_offer *offer)
{
	wl_data_offer_add_listener(offer, &dataOfferListener, new OfferInfo());
}

static void dataDeviceEnter(void *data, wl_data_device *device, uint32_t serial, wl_surface *surface, wl_fixed_t x, wl_fixed_t y, wl_data_offer *offer)
{
	if (dragOffer != offer) ResetDrag();

	dragOffer    = offer;
	dragSurface  = surface;
	dragAccepted = False;

	WindowWayland	*window = WindowWayland::GetWindowBackend(surface);

	if (offer == NIL) return;

	/* Accept only file lists and only if the window wants them.
	 */
	Bool	 accept = (window != NIL && window->IsDropFilesEnabled() && GetOfferInfo(offer)->HasMimeType("text/uri-list"));

	if (accept)
	{
		wl_data_offer_accept(offer, serial, "text/uri-list");

		if (wl_data_offer_get_version(offer) >= 3) wl_data_offer_set_actions(offer, WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY, WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY);
	}
	else
	{
		wl_data_offer_accept(offer, serial, NIL);
	}

	dragAccepted = accept;

	if (window != NIL) window->UpdatePointerPosition(wl_fixed_to_double(x), wl_fixed_to_double(y));
}

static void dataDeviceLeave(void *data, wl_data_device *device)
{
	ResetDrag();
}

static void dataDeviceMotion(void *data, wl_data_device *device, uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
	WindowWayland	*window = WindowWayland::GetWindowBackend(dragSurface);

	if (window != NIL) window->UpdatePointerPosition(wl_fixed_to_double(x), wl_fixed_to_double(y));
}

static void dataDeviceDrop(void *data, wl_data_device *device)
{
	WindowWayland	*window = WindowWayland::GetWindowBackend(dragSurface);

	if (dragOffer != NIL && dragAccepted && window != NIL)
	{
		/* Receive and parse the URI list.
		 */
		String			 uriList = ReceiveOffer(dragOffer, "text/uri-list", &wl_data_offer_receive);
		const Array<String>	&lines	 = uriList.Explode("\n");
		Array<String>		 files;

		foreach (String line, lines)
		{
			line = line.Trim();

			if (line.StartsWith("#") || !line.StartsWith("file://")) continue;

			String	 host = line.SubString(7, line.Tail(line.Length() - 7).Find("/"));

			files.Add(Encoding::URLEncode::Decode(line.Tail(line.Length() - 7 - host.Length())));
		}

		if (wl_data_offer_get_version(dragOffer) >= 3) wl_data_offer_finish(dragOffer);

		window->OnDrop(files);
	}

	ResetDrag();
}

static void dataDeviceSelection(void *data, wl_data_device *device, wl_data_offer *offer)
{
	if (selectionOffer != offer) DestroyOffer(selectionOffer, &wl_data_offer_destroy);

	selectionOffer = offer;
}

static const wl_data_device_listener dataDeviceListener = { dataDeviceDataOffer, dataDeviceEnter, dataDeviceLeave, dataDeviceMotion, dataDeviceDrop, dataDeviceSelection };

/* wl_data_source listener.
 */
static void dataSourceTarget(void *data, wl_data_source *source, const char *mimeType)
{
}

static void dataSourceSend(void *data, wl_data_source *source, const char *mimeType, int32_t fd)
{
	WriteText(fd, clipboardText);
}

static void dataSourceCancelled(void *data, wl_data_source *source)
{
	if (source == selectionSource)
	{
		selectionSource = NIL;
		clipboardText	= NIL;
	}

	wl_data_source_destroy(source);
}

static const wl_data_source_listener dataSourceListener = { dataSourceTarget, dataSourceSend, dataSourceCancelled };

/* zwp_primary_selection_offer_v1 listener.
 */
static void primaryOfferOffer(void *data, zwp_primary_selection_offer_v1 *offer, const char *mimeType)
{
	((OfferInfo *) data)->mimeTypes.Add(mimeType);
}

static const zwp_primary_selection_offer_v1_listener primaryOfferListener = { primaryOfferOffer };

/* zwp_primary_selection_device_v1 listener.
 */
static void primaryDeviceDataOffer(void *data, zwp_primary_selection_device_v1 *device, zwp_primary_selection_offer_v1 *offer)
{
	zwp_primary_selection_offer_v1_add_listener(offer, &primaryOfferListener, new OfferInfo());
}

static void primaryDeviceSelection(void *data, zwp_primary_selection_device_v1 *device, zwp_primary_selection_offer_v1 *offer)
{
	if (primaryOffer != offer) DestroyOffer(primaryOffer, &zwp_primary_selection_offer_v1_destroy);

	primaryOffer = offer;
}

static const zwp_primary_selection_device_v1_listener primaryDeviceListener = { primaryDeviceDataOffer, primaryDeviceSelection };

/* zwp_primary_selection_source_v1 listener.
 */
static void primarySourceSend(void *data, zwp_primary_selection_source_v1 *source, const char *mimeType, int32_t fd)
{
	WriteText(fd, primaryText);
}

static void primarySourceCancelled(void *data, zwp_primary_selection_source_v1 *source)
{
	if (source == primarySource)
	{
		primarySource = NIL;
		primaryText   = NIL;
	}

	zwp_primary_selection_source_v1_destroy(source);
}

static const zwp_primary_selection_source_v1_listener primarySourceListener = { primarySourceSend, primarySourceCancelled };

/* zwp_text_input_v3 listener. Changes are collected and
 * applied atomically when the done event arrives.
 */
static void textInputEnter(void *data, zwp_text_input_v3 *input, wl_surface *surface)
{
	textInputSurface = surface;
	textInputEnabled = False;

	WindowWayland::UpdateTextInput();
}

static void textInputLeave(void *data, zwp_text_input_v3 *input, wl_surface *surface)
{
	/* Leaving resets the text input state on the compositor side.
	 */
	textInputSurface = NIL;
	textInputEnabled = False;

	pendingPreedit	    = NIL;
	pendingPreeditCaret = -1;
	pendingCommit	    = NIL;

	WindowWayland::ApplyTextInput();
}

static void textInputPreeditString(void *data, zwp_text_input_v3 *input, const char *text, int32_t cursorBegin, int32_t cursorEnd)
{
	pendingPreedit	    = NIL;
	pendingPreeditCaret = -1;

	if (text == NIL) return;

	pendingPreedit.ImportFrom("UTF-8", text);
	pendingPreeditCaret = CharacterOffset(text, cursorBegin);
}

static void textInputCommitString(void *data, zwp_text_input_v3 *input, const char *text)
{
	pendingCommit = NIL;

	if (text != NIL) pendingCommit.ImportFrom("UTF-8", text);
}

static void textInputDeleteSurroundingText(void *data, zwp_text_input_v3 *input, uint32_t beforeLength, uint32_t afterLength)
{
	/* We do not provide surrounding text, so input
	 * methods are not expected to delete any.
	 */
}

static void textInputDone(void *data, zwp_text_input_v3 *input, uint32_t serial)
{
	WindowWayland::ApplyTextInput();
}

static const zwp_text_input_v3_listener textInputListener = { textInputEnter, textInputLeave, textInputPreeditString, textInputCommitString, textInputDeleteSurroundingText, textInputDone };

S::Int S::GUI::WindowWayland::Initialize()
{
	if (Backends::BackendWayland::GetDisplay() == NIL) return Success();

	bufferMutex = new Threads::Mutex();

	/* Set up keyboard handling.
	 */
	xkbContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

	const char	*locale = getenv("LC_ALL");

	if (locale == NIL || *locale == 0) locale = getenv("LC_CTYPE");
	if (locale == NIL || *locale == 0) locale = getenv("LANG");
	if (locale == NIL || *locale == 0) locale = "C";

	composeTable = xkb_compose_table_new_from_locale(xkbContext, locale, XKB_COMPOSE_COMPILE_NO_FLAGS);

	if (composeTable != NIL) composeState = xkb_compose_state_new(composeTable, XKB_COMPOSE_STATE_NO_FLAGS);

	/* Register seat listeners.
	 */
	Backends::BackendWayland::SetSeatListeners(&pointerListener, &keyboardListener);

	/* Create data device for clipboard and drag and drop.
	 */
	if (Backends::BackendWayland::GetSeat() != NIL && Backends::BackendWayland::GetDataDeviceManager() != NIL)
	{
		dataDevice = wl_data_device_manager_get_data_device(Backends::BackendWayland::GetDataDeviceManager(), Backends::BackendWayland::GetSeat());

		wl_data_device_add_listener(dataDevice, &dataDeviceListener, NIL);
	}

	/* Create primary selection device if supported.
	 */
	if (Backends::BackendWayland::GetSeat() != NIL && Backends::BackendWayland::GetPrimarySelectionManager() != NIL)
	{
		primaryDevice = zwp_primary_selection_device_manager_v1_get_device(Backends::BackendWayland::GetPrimarySelectionManager(), Backends::BackendWayland::GetSeat());

		zwp_primary_selection_device_v1_add_listener(primaryDevice, &primaryDeviceListener, NIL);
	}

	/* Create text input for input method support.
	 */
	if (Backends::BackendWayland::GetSeat() != NIL && Backends::BackendWayland::GetTextInputManager() != NIL)
	{
		textInput = zwp_text_input_manager_v3_get_text_input(Backends::BackendWayland::GetTextInputManager(), Backends::BackendWayland::GetSeat());

		zwp_text_input_v3_add_listener(textInput, &textInputListener, NIL);
	}

	/* Register for cursor events.
	 */
	Cursor::internalSetCursor.Connect(&WindowWayland::SetCursor);
	Cursor::internalRemoveCursor.Connect(&WindowWayland::RemoveCursor);

	Backends::BackendWayland::Roundtrip();

	return Success();
}

S::Int S::GUI::WindowWayland::Free()
{
	if (Backends::BackendWayland::GetDisplay() == NIL) return Success();

	Backends::BackendWayland::SetSeatListeners(NIL, NIL);

	Cursor::internalSetCursor.Disconnect(&WindowWayland::SetCursor);
	Cursor::internalRemoveCursor.Disconnect(&WindowWayland::RemoveCursor);

	if (textInput != NIL) zwp_text_input_v3_destroy(textInput);

	textInput	 = NIL;
	textInputSurface = NIL;
	activeCursor	 = NIL;

	DestroyOffer(selectionOffer, &wl_data_offer_destroy);

	ResetDrag();

	if (selectionSource != NIL) wl_data_source_destroy(selectionSource);
	if (dataDevice	    != NIL) wl_data_device_destroy(dataDevice);

	selectionSource = NIL;
	dataDevice	= NIL;

	DestroyOffer(primaryOffer, &zwp_primary_selection_offer_v1_destroy);

	if (primarySource != NIL) zwp_primary_selection_source_v1_destroy(primarySource);
	if (primaryDevice != NIL) zwp_primary_selection_device_v1_destroy(primaryDevice);

	primarySource = NIL;
	primaryDevice = NIL;

	if (cursorSurface != NIL) wl_surface_destroy(cursorSurface);
	if (cursorTheme	  != NIL) wl_cursor_theme_destroy(cursorTheme);

	cursorSurface = NIL;
	cursorTheme   = NIL;

	if (composeState != NIL) xkb_compose_state_unref(composeState);
	if (composeTable != NIL) xkb_compose_table_unref(composeTable);
	if (xkbState	 != NIL) xkb_state_unref(xkbState);
	if (xkbKeymap	 != NIL) xkb_keymap_unref(xkbKeymap);
	if (xkbContext	 != NIL) xkb_context_unref(xkbContext);

	composeState = NIL;
	composeTable = NIL;
	xkbState     = NIL;
	xkbKeymap    = NIL;
	xkbContext   = NIL;

	delete bufferMutex;

	bufferMutex = NIL;

	return Success();
}

S::Bool S::GUI::WindowWayland::ProcessPendingEvents()
{
	Bool	 processed = False;

	/* Emit deferred destroy notifications.
	 */
	if (pendingDestroy.Length() > 0)
	{
		Array<Int>	 ids = pendingDestroy;

		pendingDestroy.RemoveAll();

		foreach (Int id, ids)
		{
			WindowWayland	*backend = windowBackends.Get(id);

			if (backend == NIL) continue;

			Application::Lock	 lock;

			backend->onDestroy.Emit();
		}

		processed = True;
	}

	/* Process key repeat.
	 */
	if (repeatKey != 0 && keyboardSurface != NIL && repeatRate > 0)
	{
		Int64	 now = GetMilliseconds();

		if (now >= repeatNext)
		{
			WindowWayland	*window = GetWindowBackend(keyboardSurface);

			if (window != NIL) window->OnKey(repeatKey, True);

			/* Do not try to catch up after long stalls.
			 */
			repeatNext = Math::Max(repeatNext + 1000 / repeatRate, now);

			processed = True;
		}
	}

	return processed;
}

S::GUI::WindowWayland::WindowWayland(Void *iWindow)
{
	type		= WINDOW_WAYLAND;

	if (Backends::BackendWayland::GetDisplay() == NIL)
	{
		fprintf(stderr, "Error: Unable to connect to Wayland display.\n");

		exit(EXIT_FAILURE);
	}

	id		= windowBackends.Add(this);

	opened		= False;

	surface		= NIL;

	frame		= NIL;
	xdgSurface	= NIL;
	xdgPopup	= NIL;

	parent		= NIL;

	configured	= False;

	presentPending	= False;
	presentSource	= NIL;

	scale		= Backends::BackendWayland::GetScale();
	viewport	= NIL;
	fontSize	= Surface().GetSurfaceDPI() / 96.0;

	minSize		= Size(160, 24);

	maximized	= False;

	flags		= 0;

	enableDropFiles	= False;

	cursor		= Input::Pointer::CursorArrow;
}

S::GUI::WindowWayland::~WindowWayland()
{
	DestroyRole();
	DestroyBuffers();

	if (presentSource != NIL) cairo_surface_destroy(presentSource);

	windowBackends.Remove(id);
}

S::Void *S::GUI::WindowWayland::GetSystemWindow() const
{
	if (opened) return (Void *) this;

	return NIL;
}

S::GUI::WindowWayland *S::GUI::WindowWayland::GetWindowBackend(wl_surface *surface)
{
	if (surface == NIL) return NIL;

	foreach (WindowWayland *window, windowBackends)
	{
		if (window != NIL && window->surface == surface) return window;
	}

	return NIL;
}

S::GUI::WindowWayland *S::GUI::WindowWayland::GetWindowBackend(const Window *window)
{
	if (window == NIL) return NIL;

	Void	*systemWindow = window->GetSystemWindow();

	if (systemWindow == NIL) return NIL;

	foreach (WindowWayland *backend, windowBackends)
	{
		if (backend == systemWindow) return backend;
	}

	return NIL;
}

S::Bool S::GUI::WindowWayland::IsFractional() const
{
	/* Fractional scales need viewports to map buffer pixels
	 * to logical surface size. Otherwise scale is an integer.
	 */
	return Backends::BackendWayland::GetViewporter() != NIL;
}

S::GUI::Size S::GUI::WindowWayland::ToLogical(const Size &real) const
{
	if (IsFractional()) return Size(Math::Max(1, Math::Round(real.cx / scale)), Math::Max(1, Math::Round(real.cy / scale)));

	Int	 integerScale = Math::Round(scale);

	return Size(Math::Max(1, (real.cx + integerScale - 1) / integerScale), Math::Max(1, (real.cy + integerScale - 1) / integerScale));
}

S::GUI::Size S::GUI::WindowWayland::ToReal(const Size &logical) const
{
	return Size(Math::Round(logical.cx * scale), Math::Round(logical.cy * scale));
}

S::GUI::Size S::GUI::WindowWayland::GetRealSize(const Size &windowSize) const
{
	/* Convert a window size in smooth units to real pixels.
	 */
	return Size(Math::Round(windowSize.cx * fontSize) + sizeModifier.cx, Math::Round(windowSize.cy * fontSize) + sizeModifier.cy);
}

S::GUI::Size S::GUI::WindowWayland::GetSnappedSize(const Size &real) const
{
	/* Snap to a size that maps exactly to a logical size, so the
	 * compositor does not need to resample fractionally scaled buffers.
	 */
	return IsFractional() ? ToReal(ToLogical(real)) : real;
}

S::GUI::Size S::GUI::WindowWayland::GetFloatingSize(Bool nMaximized) const
{
	/* Use the last floating size if the compositor lets us choose,
	 * e.g. when the window is restored from maximized state.
	 */
	return (!nMaximized && floatingSize.cx > 0 && floatingSize.cy > 0) ? floatingSize : realSize;
}

S::Bool S::GUI::WindowWayland::IsToolWindow() const
{
	return (flags & WF_TOPMOST) && (flags & WF_NOTASKBUTTON) && (flags & WF_THINBORDER);
}

S::Input::Keyboard::Key S::GUI::WindowWayland::ConvertKey(UnsignedInt keySym)
{
	Input::Keyboard::Key	 key = Input::Keyboard::KeyOther;

	switch (keySym)
	{
		case XKB_KEY_Left:	key = Input::Keyboard::KeyLeft;    break;
		case XKB_KEY_Up:	key = Input::Keyboard::KeyUp;	   break;
		case XKB_KEY_Right:	key = Input::Keyboard::KeyRight;   break;
		case XKB_KEY_Down:	key = Input::Keyboard::KeyDown;    break;

		case XKB_KEY_Home:	key = Input::Keyboard::KeyHome;    break;
		case XKB_KEY_End:	key = Input::Keyboard::KeyEnd;	   break;
		case XKB_KEY_Insert:	key = Input::Keyboard::KeyInsert;  break;
		case XKB_KEY_Delete:	key = Input::Keyboard::KeyDelete;  break;
		case XKB_KEY_Prior:	key = Input::Keyboard::KeyPrior;   break;
		case XKB_KEY_Next:	key = Input::Keyboard::KeyNext;    break;

		case XKB_KEY_Return:
		case XKB_KEY_KP_Enter:	key = Input::Keyboard::KeyReturn;  break;
		case XKB_KEY_BackSpace:	key = Input::Keyboard::KeyBack;    break;
		case XKB_KEY_Tab:
		case XKB_KEY_ISO_Left_Tab: key = Input::Keyboard::KeyTab;  break;

		case XKB_KEY_space:	key = Input::Keyboard::KeySpace;   break;

		case XKB_KEY_Shift_L:
		case XKB_KEY_Shift_R:	key = Input::Keyboard::KeyShift;   break;

		case XKB_KEY_Control_L:
		case XKB_KEY_Control_R:	key = Input::Keyboard::KeyControl; break;

		case XKB_KEY_Alt_L:
		case XKB_KEY_Alt_R:	key = Input::Keyboard::KeyAlt;	   break;

		case XKB_KEY_Escape:	key = Input::Keyboard::KeyEscape;  break;

		case XKB_KEY_F1:	key = Input::Keyboard::KeyF1;	   break;
		case XKB_KEY_F2:	key = Input::Keyboard::KeyF2;	   break;
		case XKB_KEY_F3:	key = Input::Keyboard::KeyF3;	   break;
		case XKB_KEY_F4:	key = Input::Keyboard::KeyF4;	   break;
		case XKB_KEY_F5:	key = Input::Keyboard::KeyF5;	   break;
		case XKB_KEY_F6:	key = Input::Keyboard::KeyF6;	   break;
		case XKB_KEY_F7:	key = Input::Keyboard::KeyF7;	   break;
		case XKB_KEY_F8:	key = Input::Keyboard::KeyF8;	   break;
		case XKB_KEY_F9:	key = Input::Keyboard::KeyF9;	   break;
		case XKB_KEY_F10:	key = Input::Keyboard::KeyF10;	   break;
		case XKB_KEY_F11:	key = Input::Keyboard::KeyF11;	   break;
		case XKB_KEY_F12:	key = Input::Keyboard::KeyF12;	   break;
		case XKB_KEY_F13:	key = Input::Keyboard::KeyF13;	   break;
		case XKB_KEY_F14:	key = Input::Keyboard::KeyF14;	   break;
		case XKB_KEY_F15:	key = Input::Keyboard::KeyF15;	   break;
		case XKB_KEY_F16:	key = Input::Keyboard::KeyF16;	   break;
		case XKB_KEY_F17:	key = Input::Keyboard::KeyF17;	   break;
		case XKB_KEY_F18:	key = Input::Keyboard::KeyF18;	   break;
		case XKB_KEY_F19:	key = Input::Keyboard::KeyF19;	   break;
		case XKB_KEY_F20:	key = Input::Keyboard::KeyF20;	   break;
		case XKB_KEY_F21:	key = Input::Keyboard::KeyF21;	   break;
		case XKB_KEY_F22:	key = Input::Keyboard::KeyF22;	   break;
		case XKB_KEY_F23:	key = Input::Keyboard::KeyF23;	   break;
		case XKB_KEY_F24:	key = Input::Keyboard::KeyF24;	   break;
	}

	if	(keySym >= '0' && keySym <= '9') key = (Input::Keyboard::Key)  keySym;
	else if	(keySym >= 'A' && keySym <= 'Z') key = (Input::Keyboard::Key)  keySym;
	else if	(keySym >= 'a' && keySym <= 'z') key = (Input::Keyboard::Key) (keySym + ('A' - 'a'));

	return key;
}

/* Window lifecycle.
 */
S::Int S::GUI::WindowWayland::Open(const String &nTitle, const Point &nPos, const Size &nSize, Int iFlags)
{
	if (opened) return Error();

	flags	     = iFlags;
	title	     = nTitle;

	pos	     = nPos;
	size	     = nSize;
	realSize     = GetSnappedSize(GetRealSize(size));
	floatingSize = realSize;

	restoredRect = Rect(pos, size);

	opened	     = True;

	onCreate.Emit();

	/* Create drawing surface.
	 */
	drawSurface = new Surface((Void *) this, realSize);
	drawSurface->SetSize(realSize);

	/* Set minimum and maximum size.
	 */
	if (flags & WF_NORESIZE)
	{
		minSize = size;
		maxSize = size;
	}

	return Success();
}

S::Int S::GUI::WindowWayland::Close()
{
	if (!opened) return Success();

	DestroyRole();
	DestroyBuffers();

	/* Delete surface.
	 */
	if (drawSurface != NIL) delete drawSurface;

	drawSurface = NIL;

	opened = False;

	/* Emit destroy notification from the event loop,
	 * like X11 does with its DestroyNotify event.
	 */
	pendingDestroy.Add(id);

	return Success();
}

S::Int S::GUI::WindowWayland::RequestClose()
{
	if (doClose.Call()) return Close();

	return Success();
}

S::GUI::WindowWayland *S::GUI::WindowWayland::FindLeaderWindow()
{
	/* The leader window is the newest non topmost toplevel window.
	 */
	foreachreverse (WindowWayland *backend, windowBackends)
	{
		if (  backend->id      <  id	&&
		      backend->surface != NIL	&&
		      backend->xdgPopup == NIL	&&
		    !(backend->flags   &  WF_TOPMOST)) return backend;
	}

	return NIL;
}

S::GUI::WindowWayland *S::GUI::WindowWayland::FindParentWindow()
{
	Window	*window = Window::GetWindow((Void *) this);

	if (window != NIL)
	{
		Widget	*container = window->GetContainer();

		if (container != NIL)
		{
			WindowWayland	*backend = GetWindowBackend(container->GetContainerWindow());

			if (backend != NIL && backend != this && backend->surface != NIL) return backend;
		}
	}

	return FindLeaderWindow();
}

S::Bool S::GUI::WindowWayland::IsModalWindowActive()
{
	/* Ignore modal windows if this is a topmost window.
	 */
	if (flags & WF_TOPMOST) return False;

	/* Look for modal windows opened after ourselves.
	 */
	foreachreverse (WindowWayland *backend, windowBackends)
	{
		if	(backend == this)				      return False;
		else if (backend->surface != NIL && backend->flags & WF_MODAL) return True;
	}

	return False;
}

S::Void S::GUI::WindowWayland::CreateRole()
{
	if (surface != NIL) return;

	surface	   = wl_compositor_create_surface(Backends::BackendWayland::GetCompositor());
	configured = False;

	/* Use a viewport to map buffer pixels to logical size.
	 */
	if (IsFractional()) viewport = wp_viewporter_get_viewport(Backends::BackendWayland::GetViewporter(), surface);

	if (IsToolWindow()) CreatePopup();

	if (xdgPopup == NIL) CreateToplevel();

	wl_display_flush(Backends::BackendWayland::GetDisplay());
}

S::Void S::GUI::WindowWayland::CreateToplevel()
{
	String		 appID;

	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		if (Object::GetNthObject(i)->GetObjectType() != Application::classID) continue;

		appID = ((Application *) Object::GetNthObject(i))->GetText();

		break;
	}

	WindowWayland	*parentWindow = FindParentWindow();

	if (parentWindow != NIL && parentWindow->xdgPopup != NIL) parentWindow = NIL;

	/* Use libdecor for all toplevel windows. Decorations are hidden for
	 * windows without a title bar. Setting this before mapping prevents
	 * client side decorations, server side decorations are disabled once
	 * the frame is configured (see OnFrameConfigure).
	 */
	frame = libdecor_decorate(Backends::BackendWayland::GetDecorContext(), surface, &frameInterface, this);

	if (frame == NIL) return;

	if (flags & WF_NOTITLE)	 libdecor_frame_set_visibility(frame, false);
	if (flags & WF_NORESIZE) libdecor_frame_unset_capabilities(frame, LIBDECOR_ACTION_RESIZE);

	libdecor_frame_set_title(frame, title.ConvertTo("UTF-8"));

	if (appID != NIL) libdecor_frame_set_app_id(frame, appID.ConvertTo("UTF-8"));

	if (parentWindow != NIL && parentWindow->frame != NIL) libdecor_frame_set_parent(frame, parentWindow->frame);

	UpdateSizeHints();

	libdecor_frame_map(frame);

	/* The xdg_toplevel is created when the frame is mapped.
	 */
	ApplyIcon();
}

S::Void S::GUI::WindowWayland::CreatePopup()
{
	parent = FindParentWindow();

	if (parent == NIL) return;

	xdg_surface	*parentSurface = (parent->frame != NIL ? libdecor_frame_get_xdg_surface(parent->frame) : parent->xdgSurface);

	if (parentSurface == NIL) { parent = NIL; return; }

	/* Position popup relative to its parent. Coordinates are
	 * translated to the parent's window geometry, which
	 * includes client side decorations.
	 */
	Size		 logicalSize = ToLogical(realSize);
	int		 x	     = Math::Round((pos.x - parent->pos.x) / scale);
	int		 y	     = Math::Round((pos.y - parent->pos.y) / scale);

	if (parent->frame != NIL) libdecor_frame_translate_coordinate(parent->frame, x, y, &x, &y);

	xdg_positioner	*positioner  = xdg_wm_base_create_positioner(Backends::BackendWayland::GetWMBase());

	xdg_positioner_set_size(positioner, logicalSize.cx, logicalSize.cy);
	xdg_positioner_set_anchor_rect(positioner, x, y, 1, 1);
	xdg_positioner_set_anchor(positioner, XDG_POSITIONER_ANCHOR_TOP_LEFT);
	xdg_positioner_set_gravity(positioner, XDG_POSITIONER_GRAVITY_BOTTOM_RIGHT);
	xdg_positioner_set_constraint_adjustment(positioner, XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_X | XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_Y);

	xdgSurface = xdg_wm_base_get_xdg_surface(Backends::BackendWayland::GetWMBase(), surface);
	xdgPopup   = xdg_surface_get_popup(xdgSurface, parentSurface, positioner);

	xdg_positioner_destroy(positioner);

	xdg_surface_add_listener(xdgSurface, &xdgSurfaceListener, this);
	xdg_popup_add_listener(xdgPopup, &xdgPopupListener, this);

	wl_surface_commit(surface);
}

S::Void S::GUI::WindowWayland::DestroyRole()
{
	if (surface == NIL) return;

	/* Popups must be destroyed before their parents.
	 */
	foreachreverse (WindowWayland *backend, windowBackends)
	{
		if (backend != NIL && backend->parent == this) backend->DestroyRole();
	}

	if (frame      != NIL) libdecor_frame_unref(frame);
	if (xdgPopup   != NIL) xdg_popup_destroy(xdgPopup);
	if (xdgSurface != NIL) xdg_surface_destroy(xdgSurface);

	frame	   = NIL;
	xdgPopup   = NIL;
	xdgSurface = NIL;

	if (pointerSurface  == surface) pointerSurface	= NIL;
	if (keyboardSurface == surface) keyboardSurface = NIL;
	if (dragSurface	    == surface) dragSurface	= NIL;

	if (textInputSurface == surface) { textInputSurface = NIL; textInputEnabled = False; preeditShown = False; }

	if (viewport != NIL) wp_viewport_destroy(viewport);

	viewport = NIL;

	wl_surface_destroy(surface);

	surface	   = NIL;
	parent	   = NIL;
	configured = False;

	/* Destroy buffers as they may still be in use by the compositor.
	 */
	DestroyBuffers();

	wl_display_flush(Backends::BackendWayland::GetDisplay());

	/* Clear internal focus window.
	 */
	if (focusWndID == id) focusWndID = -1;
}

S::Int S::GUI::WindowWayland::Show()
{
	if (!opened) return Success();

	CreateRole();

	return Success();
}

S::Int S::GUI::WindowWayland::Hide()
{
	if (!opened) return Success();

	DestroyRole();

	return Success();
}

S::Int S::GUI::WindowWayland::SetTitle(const String &nTitle)
{
	title = nTitle;

	if (frame != NIL) libdecor_frame_set_title(frame, title.ConvertTo("UTF-8"));

	return Success();
}

S::Int S::GUI::WindowWayland::SetIcon(const Bitmap &nIcon)
{
	icon = nIcon;

	ApplyIcon();

	return Success();
}

S::Void S::GUI::WindowWayland::ApplyIcon()
{
	xdg_toplevel_icon_manager_v1	*manager  = Backends::BackendWayland::GetToplevelIconManager();
	xdg_toplevel			*toplevel = (frame != NIL ? libdecor_frame_get_xdg_toplevel(frame) : NIL);

	if (manager == NIL || toplevel == NIL || icon == NIL || icon.GetBitsPerPixel() != 32) return;

	/* Icon buffers must be square, so center the
	 * bitmap in a square transparent buffer.
	 */
	Size	 iconSize = icon.GetSize();
	Int	 extent	  = Math::Max(iconSize.cx, iconSize.cy);

	if (extent <= 0) return;

	Int		 dataSize = extent * extent * 4;
	UnsignedInt	*data	  = NIL;
	wl_buffer	*buffer	  = CreateBuffer(Size(extent, extent), WL_SHM_FORMAT_ARGB8888, (Void **) &data);

	if (buffer == NIL) return;

	memset(data, 0, dataSize);

	/* Convert pixels to premultiplied ARGB. In 24 bit icons, the
	 * background color is treated as transparent like on X11.
	 */
	UnsignedInt	 background = (Setup::BackgroundColor.GetRed() << 16) | (Setup::BackgroundColor.GetGreen() << 8) | Setup::BackgroundColor.GetBlue();
	Point		 offset	    = Point((extent - iconSize.cx) / 2, (extent - iconSize.cy) / 2);
	Int		 depth	    = icon.GetDepth();

	for (Int y = 0; y < iconSize.cy; y++)
	{
		const UnsignedInt	*source = (const UnsignedInt *) (icon.GetBytes() + y * iconSize.cx * 4);
		UnsignedInt		*target = data + (y + offset.y) * extent + offset.x;

		for (Int x = 0; x < iconSize.cx; x++)
		{
			UnsignedInt	 value = source[x];

			if (depth == 32)
			{
				UnsignedInt	 alpha = value >> 24;

				target[x] = (alpha << 24) | ((((value >> 16) & 255) * alpha / 255) << 16) | ((((value >> 8) & 255) * alpha / 255) << 8) | ((value & 255) * alpha / 255);
			}
			else
			{
				value &= 0xFFFFFF;

				target[x] = (value == background ? 0 : 0xFF000000 | value);
			}
		}
	}

	munmap(data, dataSize);

	/* Assign icon.
	 */
	xdg_toplevel_icon_v1	*toplevelIcon = xdg_toplevel_icon_manager_v1_create_icon(manager);

	xdg_toplevel_icon_v1_add_buffer(toplevelIcon, buffer, 1);
	xdg_toplevel_icon_manager_v1_set_icon(manager, toplevel, toplevelIcon);

	/* The toplevel keeps its icon when the icon object is destroyed.
	 * The buffer must outlive the icon object.
	 */
	xdg_toplevel_icon_v1_destroy(toplevelIcon);
	wl_buffer_destroy(buffer);

	/* Apply icon if the window is already mapped.
	 */
	if (configured) wl_surface_commit(surface);

	wl_display_flush(Backends::BackendWayland::GetDisplay());
}

S::Int S::GUI::WindowWayland::EnableDropFiles(Bool nEnableDropFiles)
{
	enableDropFiles = nEnableDropFiles;

	return Success();
}

const S::Array<S::String> &S::GUI::WindowWayland::GetDroppedFiles() const
{
	return droppedFiles;
}

S::Void S::GUI::WindowWayland::UpdateSizeHints()
{
	Size	 logicalMin = GetRealSize(minSize) / scale;
	Size	 logicalMax = GetRealSize(maxSize) / scale;

	if (minSize == Size()) logicalMin = Size();
	if (maxSize == Size()) logicalMax = Size();

	if (frame == NIL) return;

	libdecor_frame_set_min_content_size(frame, logicalMin.cx, logicalMin.cy);
	libdecor_frame_set_max_content_size(frame, logicalMax.cx, logicalMax.cy);
}

S::Int S::GUI::WindowWayland::SetMinimumSize(const Size &nMinSize)
{
	minSize = nMinSize;

	UpdateSizeHints();

	if (size.cx < minSize.cx || size.cy < minSize.cy) SetMetrics(pos, Size(Math::Max(size.cx, minSize.cx), Math::Max(size.cy, minSize.cy)));

	return Success();
}

S::Int S::GUI::WindowWayland::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize;

	UpdateSizeHints();

	if (size.cx > maxSize.cx || size.cy > maxSize.cy) SetMetrics(pos, Size(Math::Min(size.cx, maxSize.cx), Math::Min(size.cy, maxSize.cy)));

	return Success();
}

S::Int S::GUI::WindowWayland::SetMetrics(const Point &nPos, const Size &nSize)
{
	if (!opened) return Success();

	Size	 nRealSize = GetRealSize(nSize);
	Bool	 changed   = (nPos != pos || nRealSize != realSize);

	if (!changed) return Success();

	/* Wayland does not let clients position windows, so the
	 * position is purely virtual except for popups, which are
	 * positioned relative to their parent.
	 */
	if (xdgPopup != NIL)
	{
		DestroyRole();

		pos = nPos;

		UpdateMetrics(nRealSize, False);

		CreateRole();

		return Success();
	}

	pos = nPos;

	if (maximized && surface != NIL)
	{
		/* Keep the size dictated by the compositor.
		 */
		UpdateMetrics(realSize, False);

		return Success();
	}

	UpdateMetrics(nRealSize, False);

	floatingSize = realSize;

	/* Tell libdecor about the new size.
	 */
	if (frame != NIL && configured)
	{
		Size		 logicalSize = ToLogical(realSize);
		libdecor_state	*state	     = libdecor_state_new(logicalSize.cx, logicalSize.cy);

		libdecor_frame_commit(frame, state, NIL);
		libdecor_state_free(state);
	}

	if (drawSurface != NIL) drawSurface->PaintRect(Rect(Point(0, 0), realSize));

	return Success();
}

S::Void S::GUI::WindowWayland::UpdateMetrics(const Size &nRealSize, Bool forcePaint)
{
	Application::Lock	 lock;

	Size	 prevRealSize = realSize;

	realSize = GetSnappedSize(nRealSize);
	size	 =(realSize - sizeModifier) / fontSize;

	if (drawSurface != NIL) drawSurface->SetSize(realSize);

	onEvent.Call(SM_WINDOWMETRICS, ((	     pos.x			     + 32768) << 16) | (	    pos.y			     + 32768),
				       ((Math::Round(realSize.cx / fontSize) + 32768) << 16) | (Math::Round(realSize.cy / fontSize) + 32768));

	/* Update window rect and emit paint event if necessary. Compare
	 * real sizes, as the window computes its size with different
	 * rounding and expects a paint event whenever that changes.
	 */
	if (realSize != prevRealSize || forcePaint)
	{
		if (!maximized) restoredRect = Rect(pos, size);

		updateRect = Rect(Point(0, 0), realSize);

		onEvent.Call(SM_PAINT, 0, 0);

		updateRect = Rect();
	}
}

S::Int S::GUI::WindowWayland::Minimize()
{
	if (frame != NIL) libdecor_frame_set_minimized(frame);

	return Success();
}

S::Int S::GUI::WindowWayland::Maximize()
{
	if (frame != NIL) libdecor_frame_set_maximized(frame);

	return Success();
}

S::Int S::GUI::WindowWayland::Restore()
{
	if (frame != NIL) libdecor_frame_unset_maximized(frame);

	return Success();
}

S::Int S::GUI::WindowWayland::Raise()
{
	xdg_activation_v1	*activation = Backends::BackendWayland::GetActivation();

	if (surface == NIL || activation == NIL) return Success();

	/* Nothing to do if we already have the keyboard focus.
	 */
	if (keyboardSurface == surface) return Success();

	/* Request an activation token. Compositors grant activation only for
	 * tokens tied to recent user input, so pass the serial of the last
	 * input event and the surface that received it.
	 */
	xdg_activation_token_v1	*token = xdg_activation_v1_get_activation_token(activation);

	xdg_activation_token_v1_add_listener(token, &activationTokenListener, (void *) (intptr_t) id);

	if (inputSerial != 0 && Backends::BackendWayland::GetSeat() != NIL) xdg_activation_token_v1_set_serial(token, inputSerial, Backends::BackendWayland::GetSeat());

	if	(keyboardSurface != NIL) xdg_activation_token_v1_set_surface(token, keyboardSurface);
	else if (pointerSurface	 != NIL) xdg_activation_token_v1_set_surface(token, pointerSurface);

	xdg_activation_token_v1_commit(token);

	wl_display_flush(Backends::BackendWayland::GetDisplay());

	return Success();
}

S::Void S::GUI::WindowWayland::ActivateWindow(Int windowID, const String &token)
{
	xdg_activation_v1	*activation = Backends::BackendWayland::GetActivation();
	WindowWayland		*window	    = windowBackends.Get(windowID);

	if (activation == NIL || window == NIL || window->surface == NIL || token == NIL) return;

	xdg_activation_v1_activate(activation, token.ConvertTo("UTF-8"), window->surface);

	wl_display_flush(Backends::BackendWayland::GetDisplay());
}

/* Configure handling.
 */
S::Void S::GUI::WindowWayland::OnConfigure(const Size &nRealSize, Bool nMaximized)
{
	Application::Lock	 lock;

	Bool	 first = !configured;

	configured = True;

	/* Update maximized state.
	 */
	if	( nMaximized && !maximized) { maximized = True;  onMaximize.Emit(); }
	else if (!nMaximized &&  maximized) { maximized = False; onRestore.Emit();  }

	/* Set internal focus window when mapped like X11 does.
	 */
	if (first) focusWndID = id;

	/* Choose our own size if the compositor does not dictate one.
	 */
	Size	 targetSize = (nRealSize.cx > 0 && nRealSize.cy > 0) ? nRealSize : GetFloatingSize(maximized);

	if (!maximized) floatingSize = targetSize;

	UpdateMetrics(targetSize, first);

	if (first) onEvent.Call(SM_GETFOCUS, 0, 0);

	/* Activate the first toplevel with the token passed by the
	 * launching application, so it gets focus on all compositors.
	 */
	if (first && xdgPopup == NIL)
	{
		String	 token = Backends::BackendWayland::TakeLaunchToken();

		if (token != NIL) ActivateWindow(id, token);
	}

	/* Make sure a buffer is committed in response to the configure.
	 */
	if (drawSurface != NIL) drawSurface->PaintRect(Rect(Point(0, 0), realSize));
}

S::Void S::GUI::WindowWayland::OnFrameConfigure(libdecor_frame *nFrame, libdecor_configuration *configuration)
{
	int	 width	= 0;
	int	 height = 0;

	libdecor_window_state	 windowState = LIBDECOR_WINDOW_STATE_NONE;
	Bool			 nMaximized  = False;

	if (libdecor_configuration_get_window_state(configuration, &windowState)) nMaximized = (windowState & LIBDECOR_WINDOW_STATE_MAXIMIZED) != 0;

	/* Hide decorations again on the first configure for windows without a
	 * title bar. libdecor can ask the compositor to drop server side
	 * decorations only once it knows the decoration mode.
	 */
	if ((flags & WF_NOTITLE) && !configured) libdecor_frame_set_visibility(nFrame, false);

	/* Choose our own size if the compositor does not dictate one.
	 */
	if (!libdecor_configuration_get_content_size(configuration, nFrame, &width, &height) || width <= 0 || height <= 0)
	{
		Size	 size = ToLogical(GetFloatingSize(nMaximized));

		width  = size.cx;
		height = size.cy;
	}

	libdecor_state	*state = libdecor_state_new(width, height);

	libdecor_frame_commit(nFrame, state, configuration);
	libdecor_state_free(state);

	OnConfigure(ToReal(Size(width, height)), nMaximized);
}

S::Void S::GUI::WindowWayland::OnFrameCommit()
{
	if (surface != NIL) wl_surface_commit(surface);
}

S::Void S::GUI::WindowWayland::OnFrameClose()
{
	Application::Lock	 lock;

	if (doClose.Call()) Close();
}

S::Void S::GUI::WindowWayland::OnPopupConfigure(const Point &offset)
{
	popupOffset = offset;
}

S::Void S::GUI::WindowWayland::OnXdgSurfaceConfigure(UnsignedInt serial)
{
	xdg_surface_ack_configure(xdgSurface, serial);

	/* Update position of popups that have been moved by the compositor.
	 * Toplevel windows are configured through libdecor instead.
	 */
	if (parent != NIL)
	{
		int	 x = 0;
		int	 y = 0;

		if (parent->frame != NIL) libdecor_frame_translate_coordinate(parent->frame, 0, 0, &x, &y);

		pos = parent->pos + Point(Math::Round((popupOffset.x - x) * scale), Math::Round((popupOffset.y - y) * scale));
	}

	OnConfigure(Size(), False);
}

S::Void S::GUI::WindowWayland::OnPopupDone()
{
	Application::Lock	 lock;

	/* Popup was dismissed by the compositor.
	 */
	onEvent.Call(SM_LOSEFOCUS, -1, 0);
}

/* Buffer management.
 */
S::GUI::WaylandBuffer *S::GUI::WindowWayland::GetFreeBuffer()
{
	/* Integer scaled buffers must be a multiple of the scale.
	 */
	Size	 logicalSize = ToLogical(realSize);
	Size	 bufferSize  = IsFractional() ? realSize : Size(logicalSize.cx * Math::Round(scale), logicalSize.cy * Math::Round(scale));

	if (bufferSize.cx <= 0 || bufferSize.cy <= 0) return NIL;

	/* Remove idle buffers with wrong size.
	 */
	for (Int i = buffers.Length() - 1; i >= 0; i--)
	{
		WaylandBuffer	*buffer = buffers.GetNth(i);

		if (buffer->busy || buffer->size == bufferSize) continue;

		DestroyBuffer(buffer);

		buffers.RemoveNth(i);
	}

	/* Find a free buffer.
	 */
	foreach (WaylandBuffer *buffer, buffers)
	{
		if (!buffer->busy && buffer->size == bufferSize) return buffer;
	}

	if (buffers.Length() >= 3) return NIL;

	/* Create new buffer.
	 */
	Void		*data	   = NIL;
	wl_buffer	*shmBuffer = CreateBuffer(bufferSize, WL_SHM_FORMAT_XRGB8888, &data);

	if (shmBuffer == NIL) return NIL;

	WaylandBuffer	*buffer = new WaylandBuffer();

	buffer->buffer	 = shmBuffer;
	buffer->data	 = data;
	buffer->dataSize = bufferSize.cx * bufferSize.cy * 4;
	buffer->size	 = bufferSize;
	buffer->busy	 = False;
	buffer->damage	 = Rect(Point(0, 0), bufferSize);

	wl_buffer_add_listener(buffer->buffer, &bufferListener, this);

	buffers.Add(buffer);

	return buffer;
}

S::Void S::GUI::WindowWayland::DestroyBuffers()
{
	if (bufferMutex != NIL) bufferMutex->Lock();

	foreach (WaylandBuffer *buffer, buffers) DestroyBuffer(buffer);

	buffers.RemoveAll();

	presentPending = False;

	if (bufferMutex != NIL) bufferMutex->Release();
}

S::Void S::GUI::WindowWayland::Present(cairo_surface_t *source, const Rect &rect)
{
	if (bufferMutex == NIL) return;

	bufferMutex->Lock();

	/* Remember source surface for deferred presentation.
	 */
	if (source != presentSource)
	{
		if (presentSource != NIL) cairo_surface_destroy(presentSource);

		presentSource = cairo_surface_reference(source);
	}

	/* Add damage to all buffers.
	 */
	foreach (WaylandBuffer *buffer, buffers) buffer->damage = UniteRects(buffer->damage, rect);

	if (surface == NIL || !configured) { bufferMutex->Release(); return; }

	WaylandBuffer	*buffer = GetFreeBuffer();

	if (buffer == NIL)
	{
		/* Present again when a buffer is released.
		 */
		presentPending = True;

		bufferMutex->Release();

		return;
	}

	presentPending = False;

	/* Copy damaged area from source surface.
	 */
	cairo_surface_flush(source);

	Int		 sourceWidth  = cairo_image_surface_get_width(source);
	Int		 sourceHeight = cairo_image_surface_get_height(source);
	Int		 sourceStride = cairo_image_surface_get_stride(source);
	UnsignedByte	*sourceData   = cairo_image_surface_get_data(source);

	Rect		 damage	      = Rect::OverlapRect(buffer->damage, Rect(Point(0, 0), Size(Math::Min(sourceWidth, buffer->size.cx), Math::Min(sourceHeight, buffer->size.cy))));
	Int		 stride	      = buffer->size.cx * 4;

	if (damage.GetWidth() > 0 && damage.GetHeight() > 0)
	{
		for (Int y = damage.top; y < damage.bottom; y++)
		{
			memcpy((UnsignedByte *) buffer->data + y * stride + damage.left * 4, sourceData + y * sourceStride + damage.left * 4, damage.GetWidth() * 4);
		}
	}

	/* Attach and commit buffer.
	 */
	wl_surface_attach(surface, buffer->buffer, 0, 0);

	/* Map buffer to logical size, either by viewport or buffer scale.
	 */
	if (viewport != NIL)
	{
		Size	 logicalSize = ToLogical(buffer->size);

		wp_viewport_set_destination(viewport, logicalSize.cx, logicalSize.cy);
	}
	else if (scale > 1)
	{
		wl_surface_set_buffer_scale(surface, Math::Round(scale));
	}

	if (wl_surface_get_version(surface) >= 4) wl_surface_damage_buffer(surface, buffer->damage.left, buffer->damage.top, buffer->damage.GetWidth(), buffer->damage.GetHeight());
	else					  wl_surface_damage(surface, 0, 0, INT32_MAX, INT32_MAX);

	wl_surface_commit(surface);

	buffer->busy   = True;
	buffer->damage = Rect();

	wl_display_flush(Backends::BackendWayland::GetDisplay());

	bufferMutex->Release();
}

S::Void S::GUI::WindowWayland::OnBufferRelease(wl_buffer *releasedBuffer)
{
	Bool	 present = False;

	bufferMutex->Lock();

	foreach (WaylandBuffer *buffer, buffers)
	{
		if (buffer->buffer == releasedBuffer) buffer->busy = False;
	}

	present = presentPending && presentSource != NIL;

	bufferMutex->Release();

	if (!present) return;

	/* Present the pending update. Acquire the application lock first, as other
	 * threads may currently be drawing to the source surface or replacing it.
	 */
	Application::Lock	 lock;

	bufferMutex->Lock();

	cairo_surface_t	*source = presentPending && presentSource != NIL ? cairo_surface_reference(presentSource) : NIL;

	bufferMutex->Release();

	if (source == NIL) return;

	Present(source, Rect());

	cairo_surface_destroy(source);
}

/* Pointer input.
 */
S::Void S::GUI::WindowWayland::SetPointerCursor(Input::Pointer::CursorType nCursor)
{
	cursor = nCursor;

	wl_pointer	*pointer = Backends::BackendWayland::GetPointer();

	if (pointer == NIL || surface == NIL || pointerSurface != surface) return;

	/* Use the next integer scale for cursors, as cursor themes
	 * only provide integer sizes.
	 */
	Int		 cursorScale   = Math::Ceil(scale);
	wl_cursor	*waylandCursor = GetCursor(cursor, cursorScale);

	if (waylandCursor == NIL || waylandCursor->image_count == 0) return;

	wl_cursor_image	*image = waylandCursor->images[0];

	if (cursorSurface == NIL) cursorSurface = wl_compositor_create_surface(Backends::BackendWayland::GetCompositor());

	wl_pointer_set_cursor(pointer, pointerSerial, cursorSurface, image->hotspot_x / cursorScale, image->hotspot_y / cursorScale);

	wl_surface_attach(cursorSurface, wl_cursor_image_get_buffer(image), 0, 0);

	if (cursorScale > 1) wl_surface_set_buffer_scale(cursorSurface, cursorScale);

	wl_surface_damage(cursorSurface, 0, 0, image->width, image->height);
	wl_surface_commit(cursorSurface);

	wl_display_flush(Backends::BackendWayland::GetDisplay());
}

S::Void S::GUI::WindowWayland::OnPointerEnter(Float x, Float y)
{
	SetPointerCursor(cursor);

	OnPointerMotion(x, y);
}

S::Void S::GUI::WindowWayland::OnPointerLeave()
{
	Application::Lock	 lock;

	Input::Pointer::UpdatePosition(NIL, Input::Pointer::GetPosition().x, Input::Pointer::GetPosition().y);

	onEvent.Call(SM_MOUSEMOVE, 0, 0);
}

S::Void S::GUI::WindowWayland::OnPointerMotion(Float x, Float y)
{
	Application::Lock	 lock;

	UpdatePointerPosition(x, y);

	onEvent.Call(SM_MOUSEMOVE, 0, 0);
}

S::Void S::GUI::WindowWayland::UpdatePointerPosition(Float x, Float y)
{
	Application::Lock	 lock;

	Input::Pointer::UpdatePosition(Window::GetWindow((Void *) this), pos.x + Math::Round(x * scale), pos.y + Math::Round(y * scale));
}

S::Void S::GUI::WindowWayland::OnPointerButton(UnsignedInt button, UnsignedInt time, Bool pressed)
{
	Application::Lock	 lock;

	/* Map left, middle and right buttons to their down, up and double click messages.
	 */
	static const Int	 messages[3][3] = { { SM_LBUTTONDOWN, SM_LBUTTONUP, SM_LBUTTONDBLCLK },
						    { SM_MBUTTONDOWN, SM_MBUTTONUP, SM_MBUTTONDBLCLK },
						    { SM_RBUTTONDOWN, SM_RBUTTONUP, SM_RBUTTONDBLCLK } };

	Int	 index = (button == BTN_LEFT ? 0 : (button == BTN_MIDDLE ? 1 : (button == BTN_RIGHT ? 2 : -1)));

	if (!pressed)
	{
		if (index >= 0) onEvent.Call(messages[index][1], 0, 0);

		return;
	}

	/* Reject if a modal window is active.
	 */
	if (IsModalWindowActive()) return;

	/* Detect double clicks.
	 */
	static UnsignedInt	 prevButton = 0;
	static UnsignedInt	 prevTime   = 0;
	static Point		 prevPos;

	Point	 position = Input::Pointer::GetPosition();

	if (index  >= 0				   &&
	    button == prevButton		   &&
	    time   <= prevTime + 500		   &&
	    Math::Abs(position.x - prevPos.x) <= 5 &&
	    Math::Abs(position.y - prevPos.y) <= 5)
	{
		onEvent.Call(messages[index][2], 0, 0);

		prevButton = 0;
	}
	else
	{
		if (index >= 0) onEvent.Call(messages[index][0], 0, 0);

		prevButton = button;
	}

	prevTime = time;
	prevPos	 = position;

	/* Move internal focus to this window if necessary.
	 */
	if (opened && focusWndID != id && index >= 0)
	{
		WindowWayland	*focusWnd = windowBackends.Get(focusWndID);
		Window		*window	  = Window::GetWindow((Void *) this);

		if (focusWnd != NIL && window != NIL) focusWnd->onEvent.Call(SM_LOSEFOCUS, window->GetHandle(), 0);

		focusWndID = id;

		onEvent.Call(SM_GETFOCUS, 0, 0);
	}
}

S::Void S::GUI::WindowWayland::OnPointerAxis(Int delta)
{
	Application::Lock	 lock;

	if (IsModalWindowActive()) return;

	onEvent.Call(SM_MOUSEWHEEL, delta, 0);
}

/* Keyboard input.
 */
S::Void S::GUI::WindowWayland::OnKeyboardEnter()
{
	Application::Lock	 lock;

	focusWndID = id;

	onEvent.Call(SM_GETFOCUS, 0, 0);
}

S::Void S::GUI::WindowWayland::OnKeyboardLeave()
{
	Application::Lock	 lock;

	Input::Keyboard::ResetKeyState();

	/* Popups do not receive keyboard focus, so make sure an internally
	 * focussed popup gets closed when the application loses focus.
	 */
	WindowWayland	*focusWnd = windowBackends.Get(focusWndID);

	if (focusWnd != NIL && focusWnd != this) focusWnd->onEvent.Call(SM_LOSEFOCUS, -1, 0);

	onEvent.Call(SM_LOSEFOCUS, -1, 0);
}

S::Void S::GUI::WindowWayland::OnKey(UnsignedInt keycode, Bool pressed)
{
	if (xkbState == NIL) return;

	Application::Lock	 lock;

	/* Get unshifted key symbol for key code conversion.
	 */
	const xkb_keysym_t	*syms	 = NIL;
	xkb_layout_index_t	 layout	 = xkb_state_key_get_layout(xkbState, keycode);
	Int			 numSyms = xkb_keymap_key_get_syms_by_level(xkbKeymap, keycode, layout == XKB_LAYOUT_INVALID ? 0 : layout, 0, &syms);
	Input::Keyboard::Key	 key	 = ConvertKey(numSyms > 0 ? syms[0] : XKB_KEY_NoSymbol);

	if (!pressed)
	{
		Input::Keyboard::UpdateKeyState(key, False);

		onEvent.Call(SM_KEYUP, key, 0);

		return;
	}

	Input::Keyboard::UpdateKeyState(key, True);

	onEvent.Call(SM_KEYDOWN, key, 0);

	/* Convert keyboard event to input string and
	 * call SM_CHAR event for each character.
	 */
	char		 text[64] = { 0 };
	xkb_keysym_t	 sym	  = xkb_state_key_get_one_sym(xkbState, keycode);

	if (composeState != NIL && xkb_compose_state_feed(composeState, sym) == XKB_COMPOSE_FEED_ACCEPTED)
	{
		switch (xkb_compose_state_get_status(composeState))
		{
			case XKB_COMPOSE_COMPOSING:
				return;
			case XKB_COMPOSE_COMPOSED:
				xkb_compose_state_get_utf8(composeState, text, sizeof(text));
				xkb_compose_state_reset(composeState);

				break;
			case XKB_COMPOSE_CANCELLED:
				xkb_compose_state_reset(composeState);

				return;
			case XKB_COMPOSE_NOTHING:
				xkb_state_key_get_utf8(xkbState, keycode, text, sizeof(text));

				break;
		}
	}
	else
	{
		xkb_state_key_get_utf8(xkbState, keycode, text, sizeof(text));
	}

	if (text[0] == 0) return;

	String	 string;

	string.ImportFrom("UTF-8", text);

	for (Int i = 0; i < string.Length(); i++) onEvent.Call(SM_CHAR, string[i], 0);
}

/* Drag and drop.
 */
S::Void S::GUI::WindowWayland::OnDrop(const Array<String> &files)
{
	Application::Lock	 lock;

	droppedFiles = files;

	Window	*window = Window::GetWindow((Void *) this);

	if (window == NIL || files.Length() == 0) return;

	Point	 position = window->GetMousePosition();

	onEvent.Call(SM_DROPFILES, position.x, position.y);
}

/* Clipboard.
 */
S::String S::GUI::WindowWayland::GetClipboardText()
{
	/* Return our own text if we own the selection, as reading
	 * from ourselves would block without dispatching events.
	 */
	if (selectionSource != NIL) return clipboardText;

	if (selectionOffer == NIL) return NIL;

	return ReceiveText(selectionOffer, &wl_data_offer_receive);
}

S::Bool S::GUI::WindowWayland::SetClipboardText(const String &text)
{
	if (dataDevice == NIL) return False;

	wl_data_source	*source = wl_data_device_manager_create_data_source(Backends::BackendWayland::GetDataDeviceManager());

	wl_data_source_add_listener(source, &dataSourceListener, NIL);

	for (UnsignedInt i = 0; i < sizeof(textMimeTypes) / sizeof(char *); i++) wl_data_source_offer(source, textMimeTypes[i]);

	wl_data_device_set_selection(dataDevice, source, inputSerial);

	if (selectionSource != NIL) wl_data_source_destroy(selectionSource);

	selectionSource = source;
	clipboardText	= text;

	wl_display_flush(Backends::BackendWayland::GetDisplay());

	return True;
}

S::String S::GUI::WindowWayland::GetSelectionText()
{
	/* Keep the selection application internal if the
	 * compositor does not support primary selections.
	 */
	if (primaryDevice == NIL) return primaryText;

	/* Return our own text if we own the selection, as reading
	 * from ourselves would block without dispatching events.
	 */
	if (primarySource != NIL) return primaryText;

	if (primaryOffer == NIL) return NIL;

	return ReceiveText(primaryOffer, &zwp_primary_selection_offer_v1_receive);
}

S::Bool S::GUI::WindowWayland::SetSelectionText(const String &text)
{
	primaryText = text;

	if (primaryDevice == NIL) return True;

	/* Offer selection to other clients.
	 */
	zwp_primary_selection_source_v1	*source = zwp_primary_selection_device_manager_v1_create_source(Backends::BackendWayland::GetPrimarySelectionManager());

	zwp_primary_selection_source_v1_add_listener(source, &primarySourceListener, NIL);

	for (UnsignedInt i = 0; i < sizeof(textMimeTypes) / sizeof(char *); i++) zwp_primary_selection_source_v1_offer(source, textMimeTypes[i]);

	zwp_primary_selection_device_v1_set_selection(primaryDevice, source, inputSerial);

	if (primarySource != NIL) zwp_primary_selection_source_v1_destroy(primarySource);

	primarySource = source;

	wl_display_flush(Backends::BackendWayland::GetDisplay());

	return True;
}

/* Input method support.
 */
S::Void S::GUI::WindowWayland::SetCursor(Cursor *cursor, const Point &point)
{
	if (cursor != activeCursor && activeCursor != NIL) RemoveCursor(activeCursor);

	activeCursor	= cursor;
	activeCursorPos = point;

	UpdateTextInput();
}

S::Void S::GUI::WindowWayland::RemoveCursor(Cursor *cursor)
{
	if (activeCursor != cursor) return;

	/* Clear preediting area.
	 */
	WindowWayland	*window = GetWindowBackend(cursor->GetContainerWindow());

	if (window != NIL && preeditShown && cursor->IsVisible()) window->ClearPreedit();

	preeditShown	= False;

	activeCursor	= NIL;
	activeCursorPos = Point();

	UpdateTextInput();
}

S::Void S::GUI::WindowWayland::UpdateTextInput()
{
	if (textInput == NIL) return;

	WindowWayland	*window = (activeCursor != NIL ? GetWindowBackend(activeCursor->GetContainerWindow()) : NIL);

	if (window != NIL && window->surface != NIL && window->surface == textInputSurface)
	{
		/* Enable text input and describe the text field.
		 */
		if (!textInputEnabled)
		{
			UnsignedInt	 hint	 = ZWP_TEXT_INPUT_V3_CONTENT_HINT_NONE;
			UnsignedInt	 purpose = ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_NORMAL;
			Widget		*field	 = activeCursor->GetContainer();

			if (field != NIL && field->GetObjectType() == EditBox::classID)
			{
				if	(field->GetFlags() & EDB_ASTERISK) { purpose = ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_PASSWORD; hint |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_SENSITIVE_DATA | ZWP_TEXT_INPUT_V3_CONTENT_HINT_HIDDEN_TEXT; }
				else if (field->GetFlags() & EDB_NUMERIC)  { purpose = ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_DIGITS; }
			}

			if (activeCursor->GetFlags() & CF_MULTILINE) hint |= ZWP_TEXT_INPUT_V3_CONTENT_HINT_MULTILINE;

			zwp_text_input_v3_enable(textInput);
			zwp_text_input_v3_set_content_type(textInput, hint, purpose);

			textInputEnabled = True;
		}

		/* Tell the input method where the caret is, so candidate
		 * windows can be placed next to it.
		 */
		Int	 height = activeCursor->GetFont().GetScaledTextSizeY();

		zwp_text_input_v3_set_cursor_rectangle(textInput, Math::Round(activeCursorPos.x / window->scale), Math::Round(activeCursorPos.y / window->scale), 1, Math::Max(1, Math::Round(height / window->scale)));
		zwp_text_input_v3_commit(textInput);
	}
	else if (textInputEnabled)
	{
		zwp_text_input_v3_disable(textInput);
		zwp_text_input_v3_commit(textInput);

		textInputEnabled = False;
	}

	wl_display_flush(Backends::BackendWayland::GetDisplay());
}

S::Void S::GUI::WindowWayland::ApplyTextInput()
{
	/* Take pending changes. Values are reset after each done event.
	 */
	String	 commit	= pendingCommit;
	String	 preedit = pendingPreedit;
	Int	 caret	= pendingPreeditCaret;

	pendingCommit	    = NIL;
	pendingPreedit	    = NIL;
	pendingPreeditCaret = -1;

	Application::Lock	 lock;

	WindowWayland	*window = (activeCursor != NIL ? GetWindowBackend(activeCursor->GetContainerWindow()) : NIL);

	if (window == NIL) { preeditShown = False; return; }

	/* Apply changes in the order required by the protocol: remove
	 * the old preedit text, insert committed text, show new preedit.
	 */
	if (preeditShown) window->ClearPreedit();

	for (Int i = 0; i < commit.Length(); i++) window->onEvent.Call(SM_CHAR, commit[i], 0);

	if (preedit != NIL && activeCursor != NIL) window->DrawPreedit(preedit, caret);
}

S::Void S::GUI::WindowWayland::DrawPreedit(const String &preedit, Int caret)
{
	if (activeCursor == NIL || drawSurface == NIL) return;

	/* Draw underlined preedit text at the cursor position.
	 */
	Rect	 rect = Rect::OverlapRect(Rect(activeCursorPos - Point(1, 1), activeCursor->GetRealSize()), Rect(activeCursor->GetRealPosition(), activeCursor->GetRealSize()));
	Font	 font = activeCursor->GetFont();

	font.SetStyle(font.GetStyle() | Font::Underline);

	activeCursor->SetIMECursor(True);

	drawSurface->StartPaint(rect);
	drawSurface->Box(rect, activeCursor->GetBackgroundColor(), Rect::Filled);
	drawSurface->SetText(preedit, rect, font);
	drawSurface->EndPaint();

	/* Place the blinking cursor at the preedit caret.
	 */
	if (caret < 0 || caret > preedit.Length()) caret = preedit.Length();

	activeCursor->SetIMEAdvance(font.GetScaledTextSizeX(preedit.Head(caret)));
	activeCursor->SetIMECursor(False);

	preeditShown = True;
}

S::Void S::GUI::WindowWayland::ClearPreedit()
{
	preeditShown = False;

	if (activeCursor == NIL || drawSurface == NIL) return;

	/* Restore the text covered by the preedit string.
	 */
	Rect	 rect = Rect::OverlapRect(Rect(activeCursorPos - Point(1, 1), activeCursor->GetRealSize()), Rect(activeCursor->GetRealPosition(), activeCursor->GetRealSize()));

	activeCursor->SetIMECursor(True);

	drawSurface->StartPaint(rect);
	drawSurface->Box(rect, activeCursor->GetBackgroundColor(), Rect::Filled);

	activeCursor->Paint(SP_PAINT);

	drawSurface->EndPaint();

	activeCursor->SetIMEAdvance(0);
	activeCursor->SetIMECursor(False);
}
