 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/german.h>
#include <smooth/i18n.h>

void S::ActivateGermanLanguage()
{
	String	 inputFormat = String::SetInputFormat("UTF-8");

	TXT_ERROR = "Fehler";
	TXT_ERROR_OBJECTMANAGEREXISTS = "ObjectManager existiert bereits! Kann keine neue Instanz erzeugen!";
	TXT_ERROR_THREADMANAGEREXISTS = "ThreadManager existiert bereits! Kann keine neue Instanz erzeugen!";
	TXT_ERROR_POPUPMANAGEREXISTS = "PopupManager existiert bereits! Kann keine neue Instanz erzeugen!";

	TXT_OK = "OK";
	TXT_CANCEL = "Abbrechen";
	TXT_YES = "Ja";
	TXT_NO = "Nein";
	TXT_RETRY = "Wiederholen";
	TXT_ABORT = "Abbrechen";
	TXT_IGNORE = "Ignorieren";

	TXT_SMOOTHAPPLICATION = "smooth Anwendung";
	TXT_LAYER = "Layer";

	TXT_COLORSELECTION = "Farbauswahl";
	TXT_HTMLCODE = "HTML-Code:";
	TXT_REDSHORT = "R";
	TXT_GREENSHORT = "G";
	TXT_BLUESHORT = "B";
	TXT_HUESHORT = "F";
	TXT_SATURATIONSHORT = "S";
	TXT_VALUESHORT = "H";

	TXT_OPENFILE = "Datei öffnen";
	TXT_SAVEFILE = "Datei speichern";
	TXT_SAVEFILEAS = "Datei speichern unter";

	TXT_SELECTDIR = "Verzeichnis auswählen:";
	TXT_SELECTFONT = "Schriftart auswählen";

	TXT_SPLASHSCREEN = "Splashscreen";

	TXT_SMOOTHBACKGROUNDAPPLICATION = "smooth Hintergrundanwendung";
	TXT_SMOOTHTOOLWINDOW = "smooth::ToolWindow";

	String::SetInputFormat(inputFormat);
}
