 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include <smooth/args.h>
#include "translator.h"

Int smooth::Main(const Array<String> &args)
{
	Translator	*app = new Translator(args.GetNth(0));

	app->Loop();

	delete app;

	return 0;
}

Translator::Translator(const String &openFile)
{
	filename = "";

	Point	 pos;
	Size	 size;

	wnd		= new GUI::Window(String("smooth Translator v").Append(SMOOTH_VERSION), Point(50, 50), Size(700, 404));
	title		= new Titlebar();
	menubar		= new Menubar();
	statusbar	= new Statusbar("Ready");

	pos.x = 7;
	pos.y = 7;
	size.cx = 757;
	size.cy = 191;

	list_entries	= new ListBox(pos, size);
	list_entries->onSelectEntry.Connect(&Translator::SelectEntry, this);
	list_entries->SetFlags(LF_ALLOWRESELECT);
	list_entries->AddTab("ID", 30);
	list_entries->AddTab("String");
	list_entries->AddTab("Translation");

	pos.x = 7;
	pos.y = 164;
	size.cx = 0;
	size.cy = 0;

	button_new	= new Button("New", NIL, pos, size);
	button_new->onAction.Connect(&Translator::NewEntry, this);
	button_new->SetOrientation(OR_LOWERLEFT);

	pos.x += 87;
	pos.y -= 4;

	text_id		= new Text("ID:", pos);
	text_id->SetOrientation(OR_LOWERLEFT);

	pos.x += 62;
	pos.y += 3;
	size.cx = 37;
	size.cy = 0;

	edit_id		= new EditBox("", pos, size, 5);
	edit_id->SetFlags(EDB_NUMERIC);
	edit_id->SetOrientation(OR_LOWERLEFT);

	pos.x = 175;
	pos.y += 1;
	size.cx = 0;
	size.cy = 0;

	button_save	= new Button("Save", NIL, pos, size);
	button_save->onAction.Connect(&Translator::SaveData, this);
	button_save->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	button_remove	= new Button("Remove", NIL, pos, size);
	button_remove->onAction.Connect(&Translator::RemoveEntry, this);
	button_remove->SetOrientation(OR_LOWERRIGHT);

	pos.x = 94;
	pos.y -= 31;

	text_original	= new Text("Original:", pos);
	text_original->SetOrientation(OR_LOWERLEFT);

	pos.x += 62;
	pos.y += 3;
	size.cx = 777 - pos.x - 13;
	size.cy = 60;

	edit_original	= new MultiEdit("", pos, size, 0);
	edit_original->SetOrientation(OR_LOWERLEFT);

	pos.x -= 62;
	pos.y -= edit_original->GetHeight() + 11;

	text_translated	= new Text("Translation:", pos);
	text_translated->SetOrientation(OR_LOWERLEFT);

	pos.x += 62;
	pos.y += 3;

	edit_translated	= new MultiEdit("", pos, size, 0);
	edit_translated->SetOrientation(OR_LOWERLEFT);

	menu_file	= new PopupMenu();

	menubar->AddEntry("File", NIL, menu_file);

	menu_file->AddEntry("New")->onAction.Connect(&Translator::NewFile, this);
	menu_file->AddEntry("Close")->onAction.Connect(&Translator::Close, this);
	menu_file->AddEntry();
	menu_file->AddEntry("Open...")->onAction.Connect(&Translator::OpenFile, this);
	menu_file->AddEntry("Save")->onAction.Connect(&Translator::SaveFile, this);
	menu_file->AddEntry("Save as...")->onAction.Connect(&Translator::SaveFileAs, this);
	menu_file->AddEntry();
	menu_file->AddEntry("Exit")->onAction.Connect(&GUI::Window::Close, wnd);

	text_id->Deactivate();
	edit_id->Deactivate();
	text_original->Deactivate();
	edit_original->Deactivate();
	text_translated->Deactivate();
	edit_translated->Deactivate();
	button_save->Deactivate();
	button_remove->Deactivate();
	button_new->Deactivate();
	list_entries->Deactivate();

	Add(wnd);

	wnd->Add(text_id);
	wnd->Add(edit_id);
	wnd->Add(text_original);
	wnd->Add(edit_original);
	wnd->Add(text_translated);
	wnd->Add(edit_translated);
	wnd->Add(button_save);
	wnd->Add(button_remove);
	wnd->Add(button_new);
	wnd->Add(list_entries);
	wnd->Add(title);
	wnd->Add(menubar);
	wnd->Add(statusbar);

	wnd->SetMinimumSize(Size(400, 350));
	wnd->SetIcon(NIL);
	wnd->doClose.Connect(&Translator::ExitProc, this);
	wnd->onChangeSize.Connect(&Translator::ResizeProc, this);

	ResizeProc();

	if (openFile != NIL) OpenFileName(openFile);

	wnd->Show();
}

Translator::~Translator()
{
	for (int i = 0; i < entries.Length(); i++) delete entries.GetFirst();

	entries.RemoveAll();

	DeleteObject(title);
	DeleteObject(wnd);
	DeleteObject(menubar);
	DeleteObject(statusbar);
	DeleteObject(text_id);
	DeleteObject(edit_id);
	DeleteObject(text_original);
	DeleteObject(edit_original);
	DeleteObject(text_translated);
	DeleteObject(edit_translated);
	DeleteObject(button_save);
	DeleteObject(button_remove);
	DeleteObject(button_new);
	DeleteObject(list_entries);

	delete menu_file;
}

Bool Translator::ExitProc()
{
	if (filename != "" && modified)
	{
		String	 file;
		Int	 lastBs = -1;

		for (int i = 0; i < filename.Length(); i++) if (filename[i] == '\\') lastBs = i;

		for (int j = ++lastBs; j < filename.Length(); j++) file[j - lastBs] = filename[j];

#ifdef __WIN32__
		Int	 id = QuickMessage(String("Do you want to save changes in ").Append(file).Append("?"), "smooth Translator", MB_YESNOCANCEL, IDI_QUESTION);

		switch (id)
		{
			case IDYES:
				SaveFile();
				CloseFile();

				return True;
			case IDNO:
				CloseFile();

				return True;
			case IDCANCEL:
			case IDCLOSE:
				return False;
		}
#endif
	}
	else if (filename != "" && !modified)
	{
		CloseFile();
	}

	return True;
}

Void Translator::ResizeProc()
{
	Rect	 clientRect = wnd->GetClientRect();
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_entries->SetSize(Size(clientSize.cx - 14, clientSize.cy - 178));

	edit_original->SetSize(Size(clientSize.cx - 163, edit_original->GetHeight()));
	edit_translated->SetSize(Size(clientSize.cx - 163, edit_translated->GetHeight()));
}

void Translator::NewFile()
{
	if (!ExitProc()) return;

	filename = "unnamed";
	modified = False;

	text_id->Activate();
	edit_id->Activate();
	text_original->Activate();
	edit_original->Activate();
	text_translated->Activate();
	edit_translated->Activate();
	button_save->Activate();
	button_remove->Activate();
	button_new->Activate();
	list_entries->Activate();

	{
		GUI::Font	 entryFont;

		entryFont.SetColor(Color(0, 0, 255));

		listEntry	*entry = new listEntry;

		entry->entry = list_entries->AddEntry("Program:");
		entry->entry->SetFont(entryFont);
		entry->id = -1;
		entry->original = "Program";
		entry->translation = "";

		entries.Add(entry);

		entry = new listEntry;

		entry->entry = list_entries->AddEntry("Version:");
		entry->entry->SetFont(entryFont);
		entry->id = -2;
		entry->original = "Version";
		entry->translation = "";

		entries.Add(entry);

		entry = new listEntry;

		entry->entry = list_entries->AddEntry("Language:");
		entry->entry->SetFont(entryFont);
		entry->id = -3;
		entry->original = "Language";
		entry->translation = "";

		entries.Add(entry);

		entry = new listEntry;

		entry->entry = list_entries->AddEntry("RightToLeft:");
		entry->entry->SetFont(entryFont);
		entry->id = -4;
		entry->original = "RightToLeft";
		entry->translation = "";

		entries.Add(entry);

		entry = new listEntry;

		entry->entry = list_entries->AddEntry("Author:");
		entry->entry->SetFont(entryFont);
		entry->id = -5;
		entry->original = "Author";
		entry->translation = "";

		entries.Add(entry);

		entry = new listEntry;

		entry->entry = list_entries->AddEntry("URL:");
		entry->entry->SetFont(entryFont);
		entry->id = -6;
		entry->original = "URL";
		entry->translation = "";

		entries.Add(entry);
	}

	wnd->SetText(String(wnd->GetText()).Append(" - unnamed"));

	list_entries->SelectNthEntry(0);

	SelectEntry();
}

void Translator::Close()
{
	ExitProc();
}

void Translator::CloseFile()
{
	filename = "";

	wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION));

	for (int i = 0; i < entries.Length(); i++) delete entries.GetNth(i);

	entries.RemoveAll();

	list_entries->RemoveAllEntries();

	text_original->SetText("Original:");
	text_translated->SetText("Translation:");

	edit_id->SetText("");
	edit_original->SetText("");
	edit_translated->SetText("");

	text_id->Deactivate();
	edit_id->Deactivate();
	text_original->Deactivate();
	edit_original->Deactivate();
	text_translated->Deactivate();
	edit_translated->Deactivate();
	button_save->Deactivate();
	button_remove->Deactivate();
	button_new->Deactivate();
	list_entries->Deactivate();
}

void Translator::OpenFile()
{
	if (!ExitProc()) return;

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(wnd);

	dialog->AddFilter("XML Language Files", "*.xml");
	dialog->AddFilter("All Files", "*.*");

	if (dialog->ShowDialog() == Success())
	{
		OpenFileName(dialog->GetFileName());
	}
}

void Translator::OpenFileName(const String &openFile)
{
	filename = openFile;

	String	 file = filename;

	filename = "";

	NewFile();

	filename = file;
	file = "";

	Int	 lastBs = -1;

	for (int i = 0; i < filename.Length(); i++) if (filename[i] == '\\') lastBs = i;

	for (int j = ++lastBs; j < filename.Length(); j++) file[j - lastBs] = filename[j];

	wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION).Append(" - ").Append(file));

	XML::Document	*doc = new XML::Document();

	doc->LoadFile(filename);

	XML::Node	*info = doc->GetRootNode()->GetNodeByName("info");

	for (Int k = 0; k < info->GetNOfNodes(); k++)
	{
		String		 property = info->GetNthNode(k)->GetAttributeByName("name")->GetContent();
		listEntry	*entry = NULL;

		if (property == "program")	entry = entries.GetNth(0);
		if (property == "version")	entry = entries.GetNth(1);
		if (property == "language")	entry = entries.GetNth(2);
		if (property == "righttoleft")	entry = entries.GetNth(3);
		if (property == "author")	entry = entries.GetNth(4);
		if (property == "url")		entry = entries.GetNth(5);

		if (entry != NULL)
		{
			entry->translation = info->GetNthNode(k)->GetContent();

			entry->entry->SetText(String(entry->original).Append(": ").Append(entry->translation));
		}
	}

	XML::Node	*data = doc->GetRootNode()->GetNodeByName("data");

	for (Int l = 0; l < data->GetNOfNodes(); l++)
	{
		XML::Node	*xentry = data->GetNthNode(l);

		if (xentry->GetName() == "entry")
		{
			GUI::Font	 redFont;

			redFont.SetColor(Color(255, 0, 0));

			listEntry	*entry = new listEntry;

			entry->id = xentry->GetAttributeByName("id")->GetContent().ToInt();
			entry->original = xentry->GetAttributeByName("string")->GetContent();
			entry->translation = xentry->GetContent();
			entry->entry = list_entries->AddEntry(String(xentry->GetAttributeByName("id")->GetContent()).Append("\t").Append(entry->original).Append("\t").Append(entry->translation));

			if (entry->translation == NIL) entry->entry->SetFont(redFont);

			entries.Add(entry);
		}
	}

	delete doc;

	list_entries->SelectNthEntry(0);

	SelectEntry();
}

void Translator::SaveFile()
{
	if (filename == "") return;

	if (filename != "unnamed")	SaveFileName(filename);
	else				SaveFileAs();
}

void Translator::SaveFileAs()
{
	if (filename == "") return;

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(wnd);
	dialog->SetMode(SFM_SAVE);
	dialog->SetDefaultExtension("xml");

	dialog->AddFilter("XML Language Files", "*.xml");
	dialog->AddFilter("All Files", "*.*");

	if (dialog->ShowDialog() == Success())
	{
		filename = dialog->GetFileName();

		SaveFileName(filename);

		String	 file;
		Int	 lastBs = -1;

		for (int i = 0; i < filename.Length(); i++) if (filename[i] == '\\') lastBs = i;

		for (int j = ++lastBs; j < filename.Length(); j++) file[j - lastBs] = filename[j];

		wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION).Append(" - ").Append(file));
	}

	delete dialog;
}

void Translator::SaveFileName(const String &file)
{
	XML::Document	*doc = new XML::Document();
	XML::Node	*root = new XML::Node("LangFile");
	XML::Node	*info = root->AddNode("info");

	info->AddNode("property", entries.GetNth(0)->translation)->SetAttribute("name", "program");
	info->AddNode("property", entries.GetNth(1)->translation)->SetAttribute("name", "version");
	info->AddNode("property", entries.GetNth(2)->translation)->SetAttribute("name", "language");
	info->AddNode("property", entries.GetNth(3)->translation)->SetAttribute("name", "righttoleft");
	info->AddNode("property", "UTF-8")->SetAttribute("name", "encoding");
	info->AddNode("property", entries.GetNth(4)->translation)->SetAttribute("name", "author");
	info->AddNode("property", entries.GetNth(5)->translation)->SetAttribute("name", "url");

	XML::Node	*data = root->AddNode("data");

	for (Int i = 6; i < entries.Length(); i++)
	{
		listEntry	*entry = entries.GetNth(i);
		XML::Node	*xentry = data->AddNode("entry", entry->translation);

		xentry->SetAttribute("id", String::FromInt(entry->id));
		xentry->SetAttribute("string", entry->original);
	}

	doc->SetEncoding("UTF-8");
	doc->SetRootNode(root);

	doc->SaveFile(file);

	modified = False;

	delete doc;
	delete root;
}

void Translator::SaveData()
{
	listEntry	*entry = NULL;

	if (edit_original->GetText() == "Program")
	{
		entry = entries.GetNth(0);

		entry->translation = edit_translated->GetText();

		entry->entry->SetText(String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNth(1);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "Version")
	{
		entry = entries.GetNth(1);

		entry->translation = edit_translated->GetText();

		entry->entry->SetText(String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNth(2);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "Language")
	{
		entry = entries.GetNth(2);

		entry->translation = edit_translated->GetText();

		entry->entry->SetText(String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNth(3);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "RightToLeft")
	{
		entry = entries.GetNth(3);

		entry->translation = edit_translated->GetText();

		entry->entry->SetText(String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNth(4);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "Author")
	{
		entry = entries.GetNth(4);

		entry->translation = edit_translated->GetText();

		entry->entry->SetText(String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNth(5);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "URL")
	{
		entry = entries.GetNth(5);

		entry->translation = edit_translated->GetText();

		entry->entry->SetText(String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		text_id->Activate();
		edit_id->Activate();
		edit_original->Activate();
		button_remove->Activate();

		text_original->SetText("Original:");
		text_translated->SetText("Translation:");

		if (entries.Length() > 6)
		{
			entry = entries.GetNth(6);

			edit_id->SetText(String::FromInt(entry->id));
			edit_original->SetText(entry->original);
			edit_translated->SetText(entry->translation);
		}
		else
		{
			edit_id->SetText("1");
			edit_original->SetText("");
			edit_translated->SetText("");
		}
	}
	else
	{
		for (Int i = 0; i < entries.Length(); i++)
		{
			if (entries.GetNth(i)->id == edit_id->GetText().ToInt())
			{
				entry = entries.GetNth(i);

				entry->id = edit_id->GetText().ToInt();
				entry->original = edit_original->GetText();
				entry->translation = edit_translated->GetText();

				entry->entry->SetText(String(edit_id->GetText()).Append("\t").Append(edit_original->GetText()).Append("\t").Append(edit_translated->GetText()));

				break;
			}
		}

		if (entry == NULL)
		{
			entry = new listEntry;

			entry->entry = list_entries->AddEntry(String(edit_id->GetText()).Append("\t").Append(edit_original->GetText()).Append("\t").Append(edit_translated->GetText()));
			entry->id = edit_id->GetText().ToInt();
			entry->original = edit_original->GetText();
			entry->translation = edit_translated->GetText();

			entries.Add(entry);
		}

		GUI::Font	 redFont;
		GUI::Font	 blackFont;

		redFont.SetColor(Color(255, 0, 0));

		if (entry->translation == NIL)	entry->entry->SetFont(redFont);
		else				entry->entry->SetFont(blackFont);

		entry = NULL;

		for (int j = 0; j < entries.Length(); j++)
		{
			if (entries.GetNth(j)->id == edit_id->GetText().ToInt() + 1)
			{
				entry = entries.GetNth(j);

				edit_id->SetText(String::FromInt(entry->id));
				edit_original->SetText(entry->original);
				edit_translated->SetText(entry->translation);

				break;
			}
		}

		if (entry == NULL)
		{
			edit_id->SetText(String::FromInt(edit_id->GetText().ToInt() + 1));
			edit_original->SetText("");
			edit_translated->SetText("");
		}
	}

	modified = True;
}

void Translator::SelectEntry()
{
	ListEntry	*lid = list_entries->GetSelectedEntry();
	listEntry	*entry = NIL;

	for (Int i = 0; i < entries.Length(); i++)
	{
		entry = entries.GetNth(i);

		if (entry->entry == lid) break;
	}

	if (entry != NIL)
	{
		if (entry->id < 0)
		{
			edit_id->SetText("");
			edit_original->SetText(entry->original);
			edit_translated->SetText(entry->translation);

			text_id->Deactivate();
			edit_id->Deactivate();
			edit_original->Deactivate();
			button_remove->Deactivate();

			text_original->SetText("Field:");
			text_translated->SetText("Value:");
		}
		else
		{
			text_id->Activate();
			edit_id->Activate();
			edit_original->Activate();
			button_remove->Activate();

			text_original->SetText("Original:");
			text_translated->SetText("Translation:");

			edit_id->SetText(String::FromInt(entry->id));
			edit_original->SetText(entry->original);
			edit_translated->SetText(entry->translation);
		}
	}
}

void Translator::NewEntry()
{
	listEntry	*entry = entries.GetLast();

	text_id->Activate();
	edit_id->Activate();
	edit_original->Activate();
	button_remove->Activate();

	text_original->SetText("Original:");
	text_translated->SetText("Translation:");

	edit_id->SetText(String::FromInt(Math::Max(entry->id, 0) + 1));
	edit_original->SetText("");
	edit_translated->SetText("");

	modified = True;
}

void Translator::RemoveEntry()
{
	ListEntry	*lid = list_entries->GetSelectedEntry();

	for (Int i = 0; i < entries.Length(); i++)
	{
		if (entries.GetNth(i)->entry == lid)
		{
			entries.Remove(entries.GetNthIndex(i));

			break;
		}
	}

	list_entries->Remove(lid);

	edit_original->SetText("");
	edit_translated->SetText("");

	modified = True;
}
