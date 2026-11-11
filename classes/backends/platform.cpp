 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/platform.h>

#include <stdio.h>
#include <string.h>

namespace
{
	/* Registration data is kept in plain arrays, as components register
	 * during static initialization in undefined order.
	 */
	const int	 maxComponents = 64;
	const int	 maxProbes     = 8;

	struct Component
	{
		S::Short	 platform;
		S::Int		 (*registerFunction)();
	};

	struct Probe
	{
		S::Short	 platform;
		S::Int		 priority;
		S::Bool		 (*probeFunction)();
	};

	Component	 components[maxComponents];
	int		 numComponents = 0;

	Probe		 probes[maxProbes];
	int		 numProbes     = 0;
};

S::Short	 S::Backends::Platform::selected	       = S::Backends::PLATFORM_NONE;
char		 S::Backends::Platform::requested[32] = { 0 };

S::Void S::Backends::Platform::SetRequested(const char *platform)
{
	strncpy(requested, platform != NIL ? platform : "", sizeof(requested) - 1);

	requested[sizeof(requested) - 1] = 0;
}

S::Int S::Backends::Platform::AddComponent(Short platform, Int (*registerFunction)())
{
	if (registerFunction == NIL || numComponents >= maxComponents) return Error();

	components[numComponents].platform	   = platform;
	components[numComponents].registerFunction = registerFunction;

	numComponents++;

	return Success();
}

S::Int S::Backends::Platform::AddProbe(Short platform, Int priority, Bool (*probeFunction)())
{
	if (probeFunction == NIL || numProbes >= maxProbes) return Error();

	probes[numProbes].platform	= platform;
	probes[numProbes].priority	= priority;
	probes[numProbes].probeFunction = probeFunction;

	numProbes++;

	return Success();
}

S::Int S::Backends::Platform::GetProbeIndex(Short platform)
{
	for (int i = 0; i < numProbes; i++) if (probes[i].platform == platform) return i;

	return -1;
}

const char *S::Backends::Platform::GetPlatformName(Short platform)
{
	if	(platform == PLATFORM_X11)     return "x11";
	else if (platform == PLATFORM_WAYLAND) return "wayland";

	return "none";
}

S::Short S::Backends::Platform::Select()
{
	if (selected != PLATFORM_NONE || numProbes == 0) return selected;

	/* Honor platform requested by the user if available.
	 */
	if (requested[0] != 0)
	{
		Short	 platform = PLATFORM_NONE;

		if	(strcmp(requested, "x11")     == 0) platform = PLATFORM_X11;
		else if (strcmp(requested, "wayland") == 0) platform = PLATFORM_WAYLAND;

		Int	 index = GetProbeIndex(platform);

		if	(index < 0)			     fprintf(stderr, "Warning: Display platform '%s' is not supported by this build of smooth.\n", requested);
		else if (!probes[index].probeFunction())     fprintf(stderr, "Warning: Display platform '%s' requested, but it is not available.\n", requested);
		else					     selected = platform;
	}

	/* Otherwise use the available platform with the highest priority.
	 */
	if (selected == PLATFORM_NONE)
	{
		Int	 bestPriority = -1;

		for (int i = 0; i < numProbes; i++)
		{
			if (probes[i].priority <= bestPriority || !probes[i].probeFunction()) continue;

			selected     = probes[i].platform;
			bestPriority = probes[i].priority;
		}
	}

	/* Without any display, fall back to X11 if built, as its
	 * backends handle running without a display gracefully.
	 */
	if (selected == PLATFORM_NONE) selected = (GetProbeIndex(PLATFORM_X11) >= 0) ? PLATFORM_X11 : probes[0].platform;

	/* Install components of the selected platform.
	 */
	for (int i = 0; i < numComponents; i++)
	{
		if (components[i].platform == selected) components[i].registerFunction();
	}

	return selected;
}
