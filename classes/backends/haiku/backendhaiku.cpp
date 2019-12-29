 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
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

	rgb_color	 tooltipBackground  = ui_color(B_TOOL_TIP_BACKGROUND_COLOR);
	rgb_color	 tooltipText	    = ui_color(B_TOOL_TIP_TEXT_COLOR);

	rgb_color	 link		    = ui_color(B_LINK_TEXT_COLOR);
	rgb_color	 linkHover	    = ui_color(B_LINK_HOVER_COLOR);

	Setup::BackgroundColor		  = GUI::Color(panelBackground.red, panelBackground.green, panelBackground.blue);
	Setup::LightGrayColor		  = GUI::Color(panelBackground.red + (255 - panelBackground.red) * 0.6, panelBackground.green + (255 - panelBackground.green) * 0.6, panelBackground.blue + (255 - panelBackground.blue) * 0.6);

	Setup::ClientColor		  = GUI::Color(documentBackground.red, documentBackground.green, documentBackground.blue);
	Setup::ClientTextColor		  = GUI::Color(documentText.red, documentText.green, documentText.blue);

	Setup::DividerLightColor	  = GUI::Color(panelBackground.red + (255 - panelBackground.red) * 0.6, panelBackground.green + (255 - panelBackground.green) * 0.6, panelBackground.blue + (255 - panelBackground.blue) * 0.6);
	Setup::DividerDarkColor		  = GUI::Color(panelBackground.red * 0.7, panelBackground.green * 0.7, panelBackground.blue * 0.7);

	Setup::TextColor		  = GUI::Color(panelText.red, panelText.green, panelText.blue);
	Setup::InactiveTextColor	  = GUI::Color(panelText.red + (255 - panelText.red) * 0.5, panelText.green + (255 - panelText.green) * 0.5, panelText.blue + (255 - panelText.blue) * 0.5);

	Setup::HighlightColor		  = GUI::Color(windowTab.red + (255 - windowTab.red) * 0.5, windowTab.green + (255 - windowTab.green) * 0.5, windowTab.blue + (255 - windowTab.blue) * 0.5);
	Setup::HighlightTextColor	  = GUI::Color(documentText.red, documentText.green, documentText.blue);

	Setup::GradientStartColor	  = GUI::Color(windowTab.red, windowTab.green, windowTab.blue);
	Setup::GradientEndColor		  = GUI::Color(windowTab.red + (255 - windowTab.red) * 0.6, windowTab.green + (255 - windowTab.green) * 0.6, windowTab.blue + (255 - windowTab.blue) * 0.6);
	Setup::GradientTextColor	  = GUI::Color(windowText.red, windowText.green, windowText.blue);

	Setup::TooltipColor		  = GUI::Color(tooltipBackground.red, tooltipBackground.green, tooltipBackground.blue);
	Setup::TooltipTextColor		  = GUI::Color(tooltipText.red, tooltipText.green, tooltipText.blue);

	Setup::LinkColor		  = GUI::Color(link.red, link.green, link.blue);
	Setup::LinkHighlightColor	  = GUI::Color(linkHover.red, linkHover.green, linkHover.blue);
}

BApplication *S::Backends::BackendHaiku::GetApplication()
{
	static HaikuApplication	 app;

	return be_app;
}
