 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/haiku/backendhaiku.h>
#include <smooth/gui/application/application.h>
#include <smooth/files/file.h>

#include <interface/InterfaceDefs.h>

namespace smooth
{
	class HaikuApplication : public BApplication
	{
		public:
			HaikuApplication() : BApplication(String("application/x-vnd.").Append(File(GUI::Application::GetCommand()).GetFileName()))
			{
			}
	};
};

S::Backends::Backend *CreateBackendHaiku()
{
	return new S::Backends::BackendHaiku();
}

S::Int	 backenHaikuTmp = S::Backends::Backend::AddBackend(&CreateBackendHaiku);

S::Backends::BackendHaiku::BackendHaiku()
{
	type = BACKEND_HAIKU;
}

S::Backends::BackendHaiku::~BackendHaiku()
{
}

S::Int S::Backends::BackendHaiku::Init()
{
	/* Get default colors.
	 */
	UpdateColors();

	return Success();
}

S::Int S::Backends::BackendHaiku::Deinit()
{
	return Success();
}

S::Void S::Backends::BackendHaiku::UpdateColors()
{
	rgb_color	 panelBackground    = ui_color(B_PANEL_BACKGROUND_COLOR);
	rgb_color	 panelText	    = ui_color(B_PANEL_TEXT_COLOR);
	rgb_color	 documentBackground = ui_color(B_DOCUMENT_BACKGROUND_COLOR);
	rgb_color	 documentText	    = ui_color(B_DOCUMENT_TEXT_COLOR);
	rgb_color	 windowTab	    = ui_color(B_WINDOW_TAB_COLOR);
	rgb_color	 windowText	    = ui_color(B_WINDOW_TEXT_COLOR);
	rgb_color	 windowInactiveTab  = ui_color(B_WINDOW_INACTIVE_TAB_COLOR);
	rgb_color	 windowInactiveText = ui_color(B_WINDOW_INACTIVE_TEXT_COLOR);
	rgb_color	 tooltipBackground  = ui_color(B_TOOL_TIP_BACKGROUND_COLOR);
	rgb_color	 tooltipText	    = ui_color(B_TOOL_TIP_TEXT_COLOR);

	Setup::BackgroundColor		  = GUI::Color(panelBackground.red, panelBackground.green, panelBackground.blue);
	Setup::ClientColor		  = GUI::Color(documentBackground.red, documentBackground.green, documentBackground.blue);
	Setup::ClientTextColor		  = GUI::Color(documentText.red, documentText.green, documentText.blue);
	Setup::LightGrayColor		  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
	Setup::DividerLightColor	  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
	Setup::TextColor		  = GUI::Color(panelText.red, panelText.green, panelText.blue);
	Setup::GradientStartColor	  = GUI::Color(windowTab.red, windowTab.green, windowTab.blue);
	Setup::GradientEndColor		  = GUI::Color(Setup::GradientStartColor.GetRed() + (255 - Setup::GradientStartColor.GetRed()) * 0.6, Setup::GradientStartColor.GetGreen() + (255 - Setup::GradientStartColor.GetGreen()) * 0.6, Setup::GradientStartColor.GetBlue() + (255 - Setup::GradientStartColor.GetBlue()) * 0.6);
	Setup::GradientTextColor	  = GUI::Color(windowText.red, windowText.green, windowText.blue);
	Setup::InactiveGradientStartColor = GUI::Color(windowInactiveTab.red, windowInactiveTab.green, windowInactiveTab.blue);
	Setup::InactiveGradientEndColor	  = GUI::Color(Setup::InactiveGradientStartColor.GetRed() + (255 - Setup::InactiveGradientStartColor.GetRed()) * 0.6, Setup::InactiveGradientStartColor.GetGreen() + (255 - Setup::InactiveGradientStartColor.GetGreen()) * 0.6, Setup::InactiveGradientStartColor.GetBlue() + (255 - Setup::InactiveGradientStartColor.GetBlue()) * 0.6);
	Setup::InactiveGradientTextColor  = GUI::Color(windowInactiveText.red, windowInactiveText.green, windowInactiveText.blue);
	Setup::TooltipColor		  = GUI::Color(tooltipBackground.red, tooltipBackground.green, tooltipBackground.blue);
	Setup::TooltipTextColor		  = GUI::Color(tooltipText.red, tooltipText.green, tooltipText.blue);
}

BApplication *S::Backends::BackendHaiku::GetApplication()
{
	static HaikuApplication	 app;

	return be_app;
}
