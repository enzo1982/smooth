 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "translator.h"

Int smooth::Main()
{
	Translator	*app = new Translator();

	Loop();

	delete app;

	return 0;
}

Translator::Translator()
{
	filename = "";

	Point	 pos;
	Size	 size;

	wnd		= new Window(String("smooth Translator v").Append(SMOOTH_VERSION));
	title		= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	menubar		= new Menubar();
	statusbar	= new Statusbar("Ready");

	pos.x = 7;
	pos.y = 6;

	button_new	= new Button("New", NULL, pos, size);
	button_new->onClick.Connect(&Translator::NewEntry, this);

	pos.x += 87;
	pos.y += 4;

	text_id		= new Text("ID:", pos);

	pos.x += 20;
	pos.y -= 3;
	size.cx = 37;
	size.cy = 0;

	edit_id		= new EditBox("", pos, size, 5);
	edit_id->SetFlags(EDB_NUMERIC);

	pos.x += 44;
	pos.y += 3;

	text_original	= new Text("Original:", pos);

	pos.x += 45;
	pos.y -= 3;
	size.cx = 158;
	size.cy = 0;

	edit_original	= new EditBox("", pos, size, 0);

	pos.x += 165;
	pos.y += 3;

	text_translated	= new Text("Translation:", pos);

	pos.x += 62;
	pos.y -= 3;

	edit_translated	= new EditBox("", pos, size, 0);

	pos.x += 166;
	pos.y -= 1;
	size.cx = 0;

	button_save	= new Button("Save", NULL, pos, size);
	button_save->onClick.Connect(&Translator::SaveData, this);

	pos.x += 88;

	button_remove	= new Button("Remove", NULL, pos, size);
	button_remove->onClick.Connect(&Translator::RemoveEntry, this);

	pos.x = 7;
	pos.y += 27;
	size.cx = 757;
	size.cy = 189;

	list_entries	= new ListBox(pos, size);
	list_entries->onClick.Connect(&Translator::SelectEntry, this);
	list_entries->SetFlags(LF_ALLOWRESELECT);
	list_entries->AddTab("ID", 30);
	list_entries->AddTab("String");
	list_entries->AddTab("Translation");

	menu_file	= new Menu();

	menubar->AddEntry("File", NIL, menu_file);

	menu_file->AddEntry("New")->onClick.Connect(&Translator::NewFile, this);
	menu_file->AddEntry("Close")->onClick.Connect(&Translator::Close, this);
	menu_file->AddEntry();
	menu_file->AddEntry("Open")->onClick.Connect(&Translator::OpenFile, this);
	menu_file->AddEntry("Save")->onClick.Connect(&Translator::SaveFile, this);
	menu_file->AddEntry("Save as")->onClick.Connect(&Translator::SaveFileAs, this);
	menu_file->AddEntry();
	menu_file->AddEntry("Exit")->onClick.Connect(&Window::Close, wnd);

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

	RegisterObject(wnd);

	wnd->RegisterObject(text_id);
	wnd->RegisterObject(edit_id);
	wnd->RegisterObject(text_original);
	wnd->RegisterObject(edit_original);
	wnd->RegisterObject(text_translated);
	wnd->RegisterObject(edit_translated);
	wnd->RegisterObject(button_save);
	wnd->RegisterObject(button_remove);
	wnd->RegisterObject(button_new);
	wnd->RegisterObject(list_entries);
	wnd->RegisterObject(title);
	wnd->RegisterObject(menubar);
	wnd->RegisterObject(statusbar);

	wnd->SetMetrics(Point(50, 50), Size(777, 300));
	wnd->SetIcon(SI_DEFAULT);
	wnd->doQuit.Connect(&Translator::ExitProc, this);

	wnd->Show();
}

Translator::~Translator()
{
	for (int i = 0; i < entries.GetNOfEntries(); i++) delete entries.GetFirstEntry();

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

		Int	 id = SMOOTH::MessageBox(String("Do you want to save changes in ").Append(file).Append("?"), "smooth Translator", MB_YESNOCANCEL, IDI_QUESTION);

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
	}
	else if (filename != "" && !modified)
	{
		CloseFile();
	}

	return True;
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
		int		 lid = list_entries->AddEntry("Program:")->id;
		listEntry	*entry = new listEntry;

		entry->listid = lid;
		entry->id = -1;
		entry->original = "Program";
		entry->translation = "";

		entries.AddEntry(entry);

		lid = list_entries->AddEntry("Version:")->id;
		entry = new listEntry;

		entry->listid = lid;
		entry->id = -2;
		entry->original = "Version";
		entry->translation = "";

		entries.AddEntry(entry);

		lid = list_entries->AddEntry("Language:")->id;
		entry = new listEntry;

		entry->listid = lid;
		entry->id = -3;
		entry->original = "Language";
		entry->translation = "";

		entries.AddEntry(entry);

		lid = list_entries->AddEntry("Author:")->id;
		entry = new listEntry;

		entry->listid = lid;
		entry->id = -4;
		entry->original = "Author";
		entry->translation = "";

		entries.AddEntry(entry);

		lid = list_entries->AddEntry("URL:")->id;
		entry = new listEntry;

		entry->listid = lid;
		entry->id = -5;
		entry->original = "URL";
		entry->translation = "";

		entries.AddEntry(entry);
	}

	wnd->SetText(wnd->GetText().Append(" - unnamed"));
}

void Translator::Close()
{
	ExitProc();
}

void Translator::CloseFile()
{
	filename = "";

	wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION));

	for (int i = 0; i < entries.GetNOfEntries(); i++) delete entries.GetNthEntry(i);

	entries.RemoveAll();

	list_entries->RemoveAll();

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

	DialogFileSelection	*dialog = new DialogFileSelection();

	dialog->SetParentWindow(wnd);

	dialog->AddFilter("XML Language Files", "*.xml");
	dialog->AddFilter("All Files", "*.*");

	if (dialog->ShowDialog() == Success)
	{
		filename = dialog->GetFileName();

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

		for (int k = 0; k < info->GetNOfNodes(); k++)
		{
			String		 property = info->GetNthNode(k)->GetAttributeByName("name")->GetContent();
			listEntry	*entry = NULL;

			if (property == "program")	entry = entries.GetNthEntry(0);
			if (property == "version")	entry = entries.GetNthEntry(1);
			if (property == "language")	entry = entries.GetNthEntry(2);
			if (property == "author")	entry = entries.GetNthEntry(3);
			if (property == "url")		entry = entries.GetNthEntry(4);

			if (entry != NULL)
			{
				entry->translation = info->GetNthNode(k)->GetContent();

				list_entries->ModifyEntry(entry->listid, String(entry->original).Append(": ").Append(entry->translation));
			}
		}

		XML::Node	*xentry = doc->GetRootNode()->GetNodeByName("data")->GetNodeByName("entry");

		while (xentry != NIL)
		{
			if (xentry->GetName() == "entry")
			{
				listEntry	*entry = new listEntry;

				entry->id = xentry->GetAttributeByName("id")->GetContent().ToInt();
				entry->original = xentry->GetAttributeByName("string")->GetContent();
				entry->translation = xentry->GetContent();
				entry->listid = list_entries->AddEntry(String(xentry->GetAttributeByName("id")->GetContent()).Append("\t").Append(entry->original).Append("\t").Append(entry->translation))->id;

				entries.AddEntry(entry);
			}

			xentry = xentry->GetNextNode();
		}

		delete doc;
	}

	delete dialog;
}

void Translator::SaveFile()
{
	if (filename == "") return;

	if (filename != "unnamed")	SaveFileWithName(filename);
	else				SaveFileAs();
}

void Translator::SaveFileAs()
{
	if (filename == "") return;

	DialogFileSelection	*dialog = new DialogFileSelection();

	dialog->SetParentWindow(wnd);
	dialog->SetMode(SFM_SAVE);
	dialog->SetDefaultExtension("xml");

	dialog->AddFilter("XML Language Files", "*.xml");
	dialog->AddFilter("All Files", "*.*");

	if (dialog->ShowDialog() == Success)
	{
		filename = dialog->GetFileName();

		SaveFileWithName(filename);

		String	 file;
		Int	 lastBs = -1;

		for (int i = 0; i < filename.Length(); i++) if (filename[i] == '\\') lastBs = i;

		for (int j = ++lastBs; j < filename.Length(); j++) file[j - lastBs] = filename[j];

		wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION).Append(" - ").Append(file));
	}

	delete dialog;
}

void Translator::SaveFileWithName(String file)
{
	XML::Document	*doc = new XML::Document();
	XML::Node	*root = new XML::Node();

	root->SetName("LangFile");

	XML::Node	*info = root->AddNode("info", NIL);

	info->AddNode("property", entries.GetNthEntry(0)->translation)->SetAttribute("name", "program");
	info->AddNode("property", entries.GetNthEntry(1)->translation)->SetAttribute("name", "version");
	info->AddNode("property", entries.GetNthEntry(2)->translation)->SetAttribute("name", "language");
	info->AddNode("property", "UTF-8")->SetAttribute("name", "encoding");
	info->AddNode("property", entries.GetNthEntry(3)->translation)->SetAttribute("name", "author");
	info->AddNode("property", entries.GetNthEntry(4)->translation)->SetAttribute("name", "url");

	XML::Node	*data = root->AddNode("data", NIL);

	for (int i = 5; i < entries.GetNOfEntries(); i++)
	{
		listEntry	*entry = entries.GetNthEntry(i);

		XML::Node	*xentry = data->AddNode("entry", entry->translation);

		xentry->SetAttribute("id", String::FromInt(entry->id));
		xentry->SetAttribute("string", entry->original);
	}

	doc->SetEncoding("UTF-8");
	doc->SetRootNode(root);

	doc->SaveFile(file);

	modified = False;

	delete doc;
}

void Translator::SaveData()
{
	listEntry	*entry = NULL;

	if (edit_original->GetText() == "Program")
	{
		entry = entries.GetNthEntry(0);

		entry->translation = edit_translated->GetText();

		list_entries->ModifyEntry(entry->listid, String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNthEntry(1);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "Version")
	{
		entry = entries.GetNthEntry(1);

		entry->translation = edit_translated->GetText();

		list_entries->ModifyEntry(entry->listid, String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNthEntry(2);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "Language")
	{
		entry = entries.GetNthEntry(2);

		entry->translation = edit_translated->GetText();

		list_entries->ModifyEntry(entry->listid, String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNthEntry(3);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "Author")
	{
		entry = entries.GetNthEntry(3);

		entry->translation = edit_translated->GetText();

		list_entries->ModifyEntry(entry->listid, String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		entry = entries.GetNthEntry(4);

		edit_original->SetText(entry->original);
		edit_translated->SetText(entry->translation);
	}
	else if (edit_original->GetText() == "URL")
	{
		entry = entries.GetNthEntry(4);

		entry->translation = edit_translated->GetText();

		list_entries->ModifyEntry(entry->listid, String(edit_original->GetText()).Append(": ").Append(edit_translated->GetText()));

		text_id->Activate();
		edit_id->Activate();
		edit_original->Activate();
		button_remove->Activate();

		text_original->SetText("Original:");
		text_translated->SetText("Translation:");

		if (entries.GetNOfEntries() > 5)
		{
			entry = entries.GetNthEntry(5);

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
		for (Int i = 0; i < entries.GetNOfEntries(); i++)
		{
			if (entries.GetNthEntry(i)->id == edit_id->GetText().ToInt())
			{
				entry = entries.GetNthEntry(i);

				entry->id = edit_id->GetText().ToInt();
				entry->original = edit_original->GetText();
				entry->translation = edit_translated->GetText();

				list_entries->ModifyEntry(entry->listid, String(edit_id->GetText()).Append("\t").Append(edit_original->GetText()).Append("\t").Append(edit_translated->GetText()));

				break;
			}
		}

		if (entry == NULL)
		{
			Int	 lid = list_entries->AddEntry(String(edit_id->GetText()).Append("\t").Append(edit_original->GetText()).Append("\t").Append(edit_translated->GetText()))->id;

			entry = new listEntry;

			entry->listid = lid;
			entry->id = edit_id->GetText().ToInt();
			entry->original = edit_original->GetText();
			entry->translation = edit_translated->GetText();

			entries.AddEntry(entry);
		}

		entry = NULL;

		for (int j = 0; j < entries.GetNOfEntries(); j++)
		{
			if (entries.GetNthEntry(j)->id == edit_id->GetText().ToInt() + 1)
			{
				entry = entries.GetNthEntry(j);

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
	Int		 lid = list_entries->GetSelectedEntry()->id;
	listEntry	*entry = NULL;

	for (Int i = 0; i < entries.GetNOfEntries(); i++)
	{
		entry = entries.GetNthEntry(i);

		if (entry->listid == lid) break;
	}

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

void Translator::NewEntry()
{
	listEntry	*entry = entries.GetLastEntry();

	text_id->Activate();
	edit_id->Activate();
	edit_original->Activate();
	button_remove->Activate();

	text_original->SetText("Original:");
	text_translated->SetText("Translation:");

	edit_id->SetText(String::FromInt(max(entry->id, 0) + 1));
	edit_original->SetText("");
	edit_translated->SetText("");

	modified = True;
}

void Translator::RemoveEntry()
{
	Int	 lid = list_entries->GetSelectedEntry()->id;

	for (Int i = 0; i < entries.GetNOfEntries(); i++)
	{
		if (entries.GetNthEntry(i)->listid == lid)
		{
			entries.RemoveEntry(entries.GetNthEntryIndex(i));

			break;
		}
	}

	list_entries->RemoveEntry(lid);

	edit_original->SetText("");
	edit_translated->SetText("");

	modified = True;
}
