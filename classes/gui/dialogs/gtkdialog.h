 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_GTKDIALOG
#define H_OBJSMOOTH_GTKDIALOG

#include <gtk/gtk.h>

#include <smooth/gui/application/application.h>
#include <smooth/system/system.h>

namespace smooth
{
	namespace GUI
	{
		namespace Dialogs
		{
			/* GTK must not be used by multiple threads concurrently, but dialogs
			 * may be shown from any thread while other threads use GTK as well
			 * (e.g. libdecor plugins drawing window decorations on Wayland).
			 *
			 * Code using GTK therefore holds the application lock. Instead of
			 * gtk_dialog_run, which would block other users of GTK for as long
			 * as the dialog is shown, RunGTKDialog processes GTK events in small
			 * steps and releases the application lock in between.
			 *
			 * The application lock must be held when calling RunGTKDialog.
			 */
			static void OnGTKDialogResponse(GtkDialog *dialog, gint response, gpointer data)
			{
				*(gint *) data = response;
			}

			static gint RunGTKDialog(GtkDialog *dialog)
			{
				gint	 response = GTK_RESPONSE_NONE;
				gulong	 handler  = g_signal_connect(dialog, "response", G_CALLBACK(OnGTKDialogResponse), &response);

				gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
				gtk_widget_show(GTK_WIDGET(dialog));

				/* Process events until the dialog is closed.
				 */
				Int	 suspendCount = Application::Lock::SuspendLock();

				while (response == GTK_RESPONSE_NONE)
				{
					{
						Application::Lock	 lock;

						while (gtk_events_pending()) gtk_main_iteration_do(FALSE);
					}

					if (response == GTK_RESPONSE_NONE) S::System::System::Sleep(10);
				}

				Application::Lock::ResumeLock(suspendCount);

				g_signal_handler_disconnect(dialog, handler);

				return response;
			}
		};
	};
};

#endif
