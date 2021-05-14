 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gtk/gtk.h>

#include <smooth/backends/xlib/backendxlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

using namespace X11;

#ifndef GTK_STATE_FLAG_LINK
#	define GTK_STATE_FLAG_LINK GtkStateFlags(1 << 9)
#endif

static void toggleTimerInterrupts(bool allowed)
{
	sigset_t	 ss;

	sigemptyset(&ss);
	sigaddset(&ss, SIGALRM);

	pthread_sigmask(allowed ? SIG_UNBLOCK : SIG_BLOCK, &ss, NIL);
}

static GdkRGBA get_gtk_color(GType type, const gchar *className, GtkStateFlags stateFlags, bool background)
{
	/* Create dummy widget of requested type.
	 */
	GtkWidget	*widget = NIL;

	if	(type == GTK_TYPE_WINDOW)      widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	else if	(type == GTK_TYPE_TOOLTIP)     widget = gtk_window_new(GTK_WINDOW_POPUP);
	else if	(type == GTK_TYPE_BUTTON)      widget = gtk_button_new();
	else if	(type == GTK_TYPE_TEXT_VIEW)   widget = gtk_text_view_new();
	else if	(type == GTK_TYPE_LINK_BUTTON) widget = gtk_link_button_new("");

	if (widget == NIL) return { 0, 0, 0, 0 };

	/* Get GtkStyleContext to query color.
	 */
	GtkStyleContext *style = gtk_widget_get_style_context(widget);

	gtk_style_context_add_class(style, className);

	/* Query color from style context.
	 */
	GdkRGBA	 color;

	if (background) gtk_style_context_get_background_color(style, stateFlags, &color);
	else		gtk_style_context_get_color(style, stateFlags, &color);

	g_object_ref_sink(widget);
	g_object_unref(widget);

	return color;
}

S::Backends::Backend *CreateBackendXLib()
{
	return new S::Backends::BackendXLib();
}

S::Int	 backendXLibTmp = S::Backends::Backend::AddBackend(&CreateBackendXLib);

Display	*S::Backends::BackendXLib::display = NIL;
XIM	 S::Backends::BackendXLib::im	   = NIL;

S::Backends::BackendXLib::BackendXLib()
{
	type = BACKEND_XLIB;
}

S::Backends::BackendXLib::~BackendXLib()
{
}

S::Int S::Backends::BackendXLib::Init()
{
	XInitThreads();

	/* Open display.
	 */
	const char	*displayId = getenv("DISPLAY");

	if (displayId != NIL)
	{
		display	= XOpenDisplay(NIL);

		if (display == NIL)
		{
			fprintf(stderr, "Error: Unable to open display at %s.\n", displayId);

			return Error();
		}

		/* Init the GTK with SIGALRM signals disabled to make
		 * threads created by gtk_init inherit this SIGMASK.
		 */
		toggleTimerInterrupts(false);

		gtk_init(NULL, NULL);

		toggleTimerInterrupts(true);

		/* Get default colors.
		 */
		UpdateColors();

		/* Set locale and open input method.
		 */
		setlocale(LC_ALL, "");

		XSetLocaleModifiers("");

		im = XOpenIM(display, NIL, NIL, NIL);
	}

	return Success();
}

S::Int S::Backends::BackendXLib::Deinit()
{
	if (im	    != NIL) XCloseIM(im);
	if (display != NIL) XCloseDisplay(display);

	return Success();
}

S::Void S::Backends::BackendXLib::UpdateColors()
{
	/* Query UI colors defined in GTK theme.
	 */
	GdkRGBA	 backgroundColor    = get_gtk_color(GTK_TYPE_WINDOW, GTK_STYLE_CLASS_BACKGROUND, GTK_STATE_FLAG_NORMAL, true);

	GdkRGBA	 textColor	    = get_gtk_color(GTK_TYPE_BUTTON, GTK_STYLE_CLASS_VIEW, GTK_STATE_FLAG_NORMAL, false);

	GdkRGBA	 clientColor	    = get_gtk_color(GTK_TYPE_TEXT_VIEW, GTK_STYLE_CLASS_VIEW, GTK_STATE_FLAG_NORMAL, true);
	GdkRGBA	 clientTextColor    = get_gtk_color(GTK_TYPE_TEXT_VIEW, GTK_STYLE_CLASS_VIEW, GTK_STATE_FLAG_NORMAL, false);

	GdkRGBA	 highlightColor	    = get_gtk_color(GTK_TYPE_TEXT_VIEW, GTK_STYLE_CLASS_VIEW, GtkStateFlags(GTK_STATE_FLAG_SELECTED | GTK_STATE_FLAG_FOCUSED), true);
	GdkRGBA	 highlightTextColor = get_gtk_color(GTK_TYPE_TEXT_VIEW, GTK_STYLE_CLASS_VIEW, GtkStateFlags(GTK_STATE_FLAG_SELECTED | GTK_STATE_FLAG_FOCUSED), false);

	GdkRGBA	 tooltipColor	    = get_gtk_color(GTK_TYPE_TOOLTIP, GTK_STYLE_CLASS_VIEW, GTK_STATE_FLAG_NORMAL, true);
	GdkRGBA	 tooltipTextColor   = get_gtk_color(GTK_TYPE_TOOLTIP, GTK_STYLE_CLASS_VIEW, GTK_STATE_FLAG_NORMAL, false);

	GdkRGBA	 linkColor	    = get_gtk_color(GTK_TYPE_LINK_BUTTON, GTK_STYLE_CLASS_VIEW, GTK_STATE_FLAG_LINK, false);
	GdkRGBA	 linkHighlightColor = get_gtk_color(GTK_TYPE_LINK_BUTTON, GTK_STYLE_CLASS_VIEW, GtkStateFlags(GTK_STATE_FLAG_LINK | GTK_STATE_FLAG_PRELIGHT), false);

	/* Convert to smooth colors.
	 */
	Setup::BackgroundColor	  = GUI::Color(255 * backgroundColor.red, 255 * backgroundColor.green, 255 * backgroundColor.blue);
	Setup::LightGrayColor	  = GUI::Color(255 * (backgroundColor.red + (1.0 - backgroundColor.red) * 0.6), 255 * (backgroundColor.green + (1.0 - backgroundColor.green) * 0.6), 255 * (backgroundColor.blue + (1.0 - backgroundColor.blue) * 0.6));

	Setup::ClientColor	  = GUI::Color(255 * clientColor.red, 255 * clientColor.green, 255 * clientColor.blue);
	Setup::ClientTextColor	  = GUI::Color(255 * clientTextColor.red, 255 * clientTextColor.green, 255 * clientTextColor.blue);

	Setup::DividerLightColor  = GUI::Color(255 * (backgroundColor.red + (1.0 - backgroundColor.red) * 0.6), 255 * (backgroundColor.green + (1.0 - backgroundColor.green) * 0.6), 255 * (backgroundColor.blue + (1.0 - backgroundColor.blue) * 0.6));
	Setup::DividerDarkColor	  = GUI::Color(255 * backgroundColor.red * 0.7, 255 * backgroundColor.green * 0.7, 255 * backgroundColor.blue * 0.7);

	Setup::TextColor	  = GUI::Color(255 * textColor.red, 255 * textColor.green, 255 * textColor.blue);
	Setup::InactiveTextColor  = Setup::TextColor.Average(Setup::BackgroundColor);

	Setup::HighlightColor	  = GUI::Color(255 * highlightColor.red, 255 * highlightColor.green, 255 * highlightColor.blue);
	Setup::HighlightTextColor = GUI::Color(255 * highlightTextColor.red, 255 * highlightTextColor.green, 255 * highlightTextColor.blue);

	Setup::GradientStartColor = GUI::Color(255 * (highlightColor.red - highlightColor.red * 0.15), 255 * (highlightColor.green - highlightColor.green * 0.15), 255 * (highlightColor.blue - highlightColor.blue * 0.15));
	Setup::GradientEndColor	  = GUI::Color(255 * (highlightColor.red + (1.0 - highlightColor.red) * 0.15), 255 * (highlightColor.green + (1.0 - highlightColor.green) * 0.15), 255 * (highlightColor.blue + (1.0 - highlightColor.blue) * 0.15));
	Setup::GradientTextColor  = GUI::Color(255 * highlightTextColor.red, 255 * highlightTextColor.green, 255 * highlightTextColor.blue);

	Setup::TooltipColor	  = GUI::Color(255 * tooltipColor.red, 255 * tooltipColor.green, 255 * tooltipColor.blue);
	Setup::TooltipTextColor	  = GUI::Color(255 * tooltipTextColor.red, 255 * tooltipTextColor.green, 255 * tooltipTextColor.blue);

	if (GUI::Color(255 * linkColor.red, 255 * linkColor.green, 255 * linkColor.blue) != Setup::TextColor)
	{
		Setup::LinkColor	  = GUI::Color(255 * linkColor.red, 255 * linkColor.green, 255 * linkColor.blue);
		Setup::LinkHighlightColor = GUI::Color(255 * linkHighlightColor.red, 255 * linkHighlightColor.green, 255 * linkHighlightColor.blue);
	}
}

Display *S::Backends::BackendXLib::GetDisplay()
{
	return display;
}

XIM S::Backends::BackendXLib::GetIM()
{
	return im;
}
