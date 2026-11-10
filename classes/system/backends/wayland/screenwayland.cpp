 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/wayland/screenwayland.h>
#include <smooth/backends/wayland/backendwayland.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>

S::System::ScreenBackend *CreateScreenWayland()
{
	return new S::System::ScreenWayland();
}

S::Int	 screenWaylandTmp = S::System::ScreenBackend::SetBackend(&CreateScreenWayland);

S::System::ScreenWayland::ScreenWayland()
{
	type = SCREEN_WAYLAND;
}

S::System::ScreenWayland::~ScreenWayland()
{
}

S::GUI::Rect S::System::ScreenWayland::GetActiveScreenMetrics()
{
	/* Wayland does not expose global pointer coordinates, so the
	 * active screen is guessed from the virtual pointer position.
	 */
	const GUI::Point	&point = Input::Pointer::GetPosition();

	for (Int i = 0; i < Backends::BackendWayland::GetNOfOutputs(); i++)
	{
		const Backends::WaylandOutput	*output = Backends::BackendWayland::GetNthOutput(i);
		GUI::Rect			 rect	= GUI::Rect(output->position, output->size);

		if (point.x >= rect.left && point.x < rect.right &&
		    point.y >= rect.top	 && point.y < rect.bottom) return rect;
	}

	if (Backends::BackendWayland::GetNOfOutputs() > 0)
	{
		const Backends::WaylandOutput	*output = Backends::BackendWayland::GetNthOutput(0);

		return GUI::Rect(output->position, output->size);
	}

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(1024, 768));
}

S::GUI::Rect S::System::ScreenWayland::GetActiveScreenWorkArea()
{
	return GetActiveScreenMetrics();
}

S::GUI::Rect S::System::ScreenWayland::GetVirtualScreenMetrics()
{
	if (Backends::BackendWayland::GetNOfOutputs() == 0) return GetActiveScreenMetrics();

	GUI::Rect	 metrics;

	for (Int i = 0; i < Backends::BackendWayland::GetNOfOutputs(); i++)
	{
		const Backends::WaylandOutput	*output = Backends::BackendWayland::GetNthOutput(i);

		metrics = GUI::Rect::EncloseRect(metrics, GUI::Rect(output->position, output->size));
	}

	return metrics;
}
