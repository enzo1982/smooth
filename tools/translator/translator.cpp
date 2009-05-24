 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
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
#include "infoitem.h"
#include "stringitem.h"

Int smooth::Main(const Array<String> &args)
{
	Translator	*app = new Translator(args.GetNth(0));

	app->Loop();

	delete app;

	return 0;
}

Translator::Translator(const String &openFile)
{
	fileName = NIL;
	templateName = NIL;

	wnd		= new GUI::Window(String("smooth Translator v").Append(SMOOTH_VERSION), Point(50, 50), Size(700, 434));
	title		= new Titlebar();
	menubar		= new Menubar();
	statusbar	= new Statusbar("Ready");

	list_entries	= new ListBox(Point(7, 7), Size(757, 191));
	list_entries->onSelectEntry.Connect(&Translator::SelectEntry, this);
	list_entries->SetFlags(LF_ALLOWRESELECT);
	list_entries->AddTab("ID", 30);
	list_entries->AddTab("String");
	list_entries->AddTab("Translation");

	button_new	= new Button("New", NIL, Point(7, 164), Size());
	button_new->onAction.Connect(&Translator::NewEntry, this);
	button_new->SetOrientation(OR_LOWERLEFT);

	text_id		= new Text("ID:", Point(94, 160));
	text_id->SetOrientation(OR_LOWERLEFT);

	edit_id		= new EditBox(NIL, Point(156, 163), Size(37, 0), 5);
	edit_id->SetFlags(EDB_NUMERIC);
	edit_id->SetOrientation(OR_LOWERLEFT);

	button_save	= new Button("Save", NIL, Point(175, 164), Size());
	button_save->onAction.Connect(&Translator::SaveData, this);
	button_save->SetOrientation(OR_LOWERRIGHT);

	button_remove	= new Button("Remove", NIL, Point(87, 164), Size());
	button_remove->onAction.Connect(&Translator::RemoveEntry, this);
	button_remove->SetOrientation(OR_LOWERRIGHT);

	text_original	= new Text("Original:", Point(94, 133));
	text_original->SetOrientation(OR_LOWERLEFT);

	edit_original	= new MultiEdit(NIL, Point(156, 136), Size(608, 60), 0);
	edit_original->SetOrientation(OR_LOWERLEFT);

	text_translated	= new Text("Translation:", Point(94, 65));
	text_translated->SetOrientation(OR_LOWERLEFT);

	edit_translated	= new MultiEdit(NIL, Point(156, 68), Size(608, 60), 0);
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
	CloseFile();

	DeleteObject(title);
	DeleteObject(wnd);
	DeleteObject(menubar);
	DeleteObject(menu_file);
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
}

String Translator::GetShortFileName(const String &fileName)
{
	return fileName.Tail(fileName.Length() - fileName.FindLast("\\") - 1);
}

Bool Translator::ExitProc()
{
	if (fileName != NIL && modified)
	{
		Int	 id = QuickMessage(String("Do you want to save changes in ").Append(GetShortFileName(fileName)).Append("?"), "smooth Translator", MB_YESNOCANCEL, IDI_QUESTION);

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
	else if (fileName != NIL && !modified)
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

	edit_original->SetWidth(clientSize.cx - 163);
	edit_translated->SetWidth(clientSize.cx - 163);
}

Void Translator::NewFile()
{
	if (!ExitProc()) return;

	fileName = "unnamed";
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
		ListEntry	*entry = NIL;
		GUI::Font	 entryFont;

		entryFont.SetColor(Color(0, 0, 255));

		/* Adding "Program" entry.
		 */
		entry = new InfoItem("Program", NIL);
		entry->SetFont(entryFont);

		list_entries->Add(entry);

		entries.Add(entry, -1);

		/* Adding "Version" entry.
		 */
		entry = new InfoItem("Version", NIL);
		entry->SetFont(entryFont);

		list_entries->Add(entry);

		entries.Add(entry, -2);

		/* Adding "Language" entry.
		 */
		entry = new InfoItem("Language", NIL);
		entry->SetFont(entryFont);

		list_entries->Add(entry);

		entries.Add(entry, -3);

		/* Adding "RightToLeft" entry.
		 */
		entry = new InfoItem("RightToLeft", NIL);
		entry->SetFont(entryFont);

		list_entries->Add(entry);

		entries.Add(entry, -4);

		/* Adding "Template" entry.
		 */
		entry = new InfoItem("Template", NIL);
		entry->SetFont(entryFont);

		list_entries->Add(entry);

		entries.Add(entry, -5);

		/* Adding "Author" entry.
		 */
		entry = new InfoItem("Author", NIL);
		entry->SetFont(entryFont);

		list_entries->Add(entry);

		entries.Add(entry, -6);

		/* Adding "URL" entry.
		 */
		entry = new InfoItem("URL", NIL);
		entry->SetFont(entryFont);

		list_entries->Add(entry);

		entries.Add(entry, -7);
	}

	wnd->SetText(String(wnd->GetText()).Append(" - unnamed"));

	list_entries->SelectNthEntry(0);

	SelectEntry();
}

Void Translator::Close()
{
	ExitProc();
}

Void Translator::CloseFile()
{
	fileName = NIL;
	templateName = NIL;

	wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION));

	list_entries->RemoveAllEntries();

	for (Int i = entries.Length() - 1; i >= 0; i--) DeleteObject(entries.GetNth(i));

	entries.RemoveAll();

	text_original->SetText("Original:");
	text_translated->SetText("Translation:");

	edit_id->SetText(NIL);
	edit_original->SetText(NIL);
	edit_translated->SetText(NIL);

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

Void Translator::OpenFile()
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

Int Translator::OpenTemplate(const String &fileName)
{
	if (!File(fileName).Exists()) return Error();

	XML::Document	*doc = new XML::Document();

	doc->LoadFile(fileName);

	XML::Node	*data = doc->GetRootNode()->GetNodeByName("data");

	for (Int l = 0; l < data->GetNOfNodes(); l++)
	{
		XML::Node	*xentry = data->GetNthNode(l);

		if (xentry->GetName() == "entry")
		{
			GUI::Font	 redFont;

			redFont.SetColor(Color(255, 0, 0));

			StringItem	*entry = new StringItem(xentry->GetAttributeByName("id")->GetContent().ToInt(),
								 xentry->GetAttributeByName("string")->GetContent(),
								 NIL);

			entry->SetFont(redFont);

			list_entries->Add(entry);

			entries.Add(entry, entry->GetID());
		}
	}

	delete doc;

	return Success();
}

Void Translator::OpenFileName(const String &openFile)
{
	NewFile();

	fileName = openFile;

	wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION).Append(" - ").Append(GetShortFileName(fileName)));

	XML::Document	*doc = new XML::Document();

	doc->LoadFile(fileName);

	XML::Node	*info = doc->GetRootNode()->GetNodeByName("info");

	for (Int k = 0; k < info->GetNOfNodes(); k++)
	{
		String		 property = info->GetNthNode(k)->GetAttributeByName("name")->GetContent();
		InfoItem	*entry = NULL;

		if	(property == "program")		entry = (InfoItem *) entries.GetNth(0);
		else if (property == "version")		entry = (InfoItem *) entries.GetNth(1);
		else if (property == "language")	entry = (InfoItem *) entries.GetNth(2);
		else if (property == "righttoleft")	entry = (InfoItem *) entries.GetNth(3);

		else if (property == "template")
		{
			templateName = info->GetNthNode(k)->GetContent();

			if (OpenTemplate(String(File(fileName).GetFilePath()).Append("\\").Append(templateName)) != Success())
			{
				templateName = NIL;
			}

			entry = (InfoItem *) entries.GetNth(4);
		}

		else if (property == "author")		entry = (InfoItem *) entries.GetNth(5);
		else if (property == "url")		entry = (InfoItem *) entries.GetNth(6);

		if (entry != NIL)
		{
			entry->SetValue(info->GetNthNode(k)->GetContent());
		}
	}

	XML::Node	*data = doc->GetRootNode()->GetNodeByName("data");

	for (Int l = 0; l < data->GetNOfNodes(); l++)
	{
		XML::Node	*xentry = data->GetNthNode(l);

		if (xentry->GetName() == "entry")
		{
			GUI::Font	 blackFont;
			GUI::Font	 redFont;
			GUI::Font	 orangeFont;

			redFont.SetColor(Color(255, 0, 0));
			orangeFont.SetColor(Color(255, 127, 36));

			if (entries.Get(xentry->GetAttributeByName("id")->GetContent().ToInt()) != NIL)
			{
				StringItem	*entry = (StringItem *) entries.Get(xentry->GetAttributeByName("id")->GetContent().ToInt());

				entry->SetTranslation(xentry->GetContent());

				if	(entry->GetTranslation() == NIL)						entry->SetFont(redFont);
				else if (entry->GetOriginal() != xentry->GetAttributeByName("string")->GetContent())	entry->SetFont(orangeFont);
				else											entry->SetFont(blackFont);
			}
			else
			{
				StringItem	*entry = new StringItem(xentry->GetAttributeByName("id")->GetContent().ToInt(),
									xentry->GetAttributeByName("string")->GetContent(),
									xentry->GetContent());

				if (entry->GetTranslation() == NIL)	entry->SetFont(redFont);
				else					entry->SetFont(orangeFont);

				list_entries->Add(entry);

				entries.Add(entry, entry->GetID());
			}
		}
	}

	delete doc;

	list_entries->SelectNthEntry(0);

	SelectEntry();
}

Void Translator::SaveFile()
{
	if (fileName == NIL) return;

	if (fileName != "unnamed")	SaveFileName(fileName);
	else				SaveFileAs();
}

Void Translator::SaveFileAs()
{
	if (fileName == NIL) return;

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(wnd);
	dialog->SetMode(SFM_SAVE);
	dialog->SetDefaultExtension("xml");

	dialog->AddFilter("XML Language Files", "*.xml");
	dialog->AddFilter("All Files", "*.*");

	if (dialog->ShowDialog() == Success())
	{
		fileName = dialog->GetFileName();

		SaveFileName(fileName);

		wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION).Append(" - ").Append(GetShortFileName(fileName)));
	}

	delete dialog;
}

Void Translator::SaveFileName(const String &file)
{
	XML::Document	*doc = new XML::Document();
	XML::Node	*root = new XML::Node("LangFile");
	XML::Node	*info = root->AddNode("info");

	info->AddNode("property", ((InfoItem *) entries.GetNth(0))->GetValue())->SetAttribute("name", "program");
	info->AddNode("property", ((InfoItem *) entries.GetNth(1))->GetValue())->SetAttribute("name", "version");
	info->AddNode("property", ((InfoItem *) entries.GetNth(2))->GetValue())->SetAttribute("name", "language");
	info->AddNode("property", ((InfoItem *) entries.GetNth(3))->GetValue())->SetAttribute("name", "righttoleft");
	info->AddNode("property", "UTF-8")->SetAttribute("name", "encoding");
	info->AddNode("property", ((InfoItem *) entries.GetNth(4))->GetValue())->SetAttribute("name", "template");
	info->AddNode("property", ((InfoItem *) entries.GetNth(5))->GetValue())->SetAttribute("name", "author");
	info->AddNode("property", ((InfoItem *) entries.GetNth(6))->GetValue())->SetAttribute("name", "url");

	XML::Node	*data = root->AddNode("data");

	for (Int i = numInfoItems; i < entries.Length(); i++)
	{
		StringItem	*entry = (StringItem *) entries.GetNth(i);

		/* If we have a template, save only
		 * entries with a translation.
		 */
		if (templateName == NIL || entry->GetTranslation() != NIL)
		{
			XML::Node	*xentry = data->AddNode("entry", entry->GetTranslation());

			xentry->SetAttribute("id", String::FromInt(entry->GetID()));
			xentry->SetAttribute("string", entry->GetOriginal());
		}
	}

	doc->SetEncoding("UTF-8");
	doc->SetRootNode(root);

	doc->SaveFile(file);

	modified = False;

	delete doc;
	delete root;
}

Void Translator::SaveData()
{
	if (edit_id->GetText() == NIL)
	{
		Int	 index = -1;

		if	(edit_original->GetText() == "Program")		index = 0;
		else if (edit_original->GetText() == "Version")		index = 1;
		else if (edit_original->GetText() == "Language")	index = 2;
		else if (edit_original->GetText() == "RightToLeft")	index = 3;
		else if (edit_original->GetText() == "Template")	index = 4;
		else if (edit_original->GetText() == "Author")		index = 5;
		else if (edit_original->GetText() == "URL")		index = 6;

		if (index >= 0)
		{
			InfoItem	*entry = (InfoItem *) entries.GetNth(index);

			entry->SetValue(edit_translated->GetText());

			if (index < numInfoItems - 1)
			{
				entry = (InfoItem *) entries.GetNth(index + 1);

				edit_original->SetText(entry->GetName());
				edit_translated->SetText(entry->GetValue());
			}
			else
			{
				text_id->Activate();
				edit_id->Activate();
				edit_original->Activate();
				button_remove->Activate();

				text_original->SetText("Original:");
				text_translated->SetText("Translation:");

				if (entries.Length() > numInfoItems)
				{
					StringItem	*entry = (StringItem *) entries.GetNth(numInfoItems);

					edit_id->SetText(String::FromInt(entry->GetID()));
					edit_original->SetText(entry->GetOriginal());
					edit_translated->SetText(entry->GetTranslation());
				}
				else
				{
					edit_id->SetText("1");
					edit_original->SetText(NIL);
					edit_translated->SetText(NIL);
				}
			}
		}
	}
	else
	{
		StringItem	*entry = NIL;

		for (Int i = numInfoItems; i < entries.Length(); i++)
		{
			if (((StringItem *) entries.GetNth(i))->GetID() == edit_id->GetText().ToInt())
			{
				entry = (StringItem *) entries.GetNth(i);

				entry->SetOriginal(edit_original->GetText());
				entry->SetTranslation(edit_translated->GetText());

				break;
			}
		}

		if (entry == NIL)
		{
			entry = new StringItem(edit_id->GetText().ToInt(), edit_original->GetText(), edit_translated->GetText());

			list_entries->Add(entry);

			entries.Add(entry, entry->GetID());
		}

		GUI::Font	 redFont;
		GUI::Font	 blackFont;

		redFont.SetColor(Color(255, 0, 0));

		if (entry->GetTranslation() == NIL)	entry->SetFont(redFont);
		else					entry->SetFont(blackFont);

		entry = NULL;

		for (Int j = numInfoItems; j < entries.Length(); j++)
		{
			if (((StringItem *) entries.GetNth(j))->GetID() == edit_id->GetText().ToInt() + 1)
			{
				entry = (StringItem *) entries.GetNth(j);

				edit_id->SetText(String::FromInt(entry->GetID()));
				edit_original->SetText(entry->GetOriginal());
				edit_translated->SetText(entry->GetTranslation());

				break;
			}
		}

		if (entry == NULL)
		{
			edit_id->SetText(String::FromInt(edit_id->GetText().ToInt() + 1));
			edit_original->SetText(NIL);
			edit_translated->SetText(NIL);
		}
	}

	modified = True;
}

Void Translator::SelectEntry()
{
	ListEntry	*entry = list_entries->GetSelectedEntry();

	if (entry != NIL)
	{
		if (entry->GetObjectType() == InfoItem::classID)
		{
			edit_id->SetText(NIL);
			edit_original->SetText(((InfoItem *) entry)->GetName());
			edit_translated->SetText(((InfoItem *) entry)->GetValue());

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

			edit_id->SetText(String::FromInt(((StringItem *) entry)->GetID()));
			edit_original->SetText(((StringItem *) entry)->GetOriginal());
			edit_translated->SetText(((StringItem *) entry)->GetTranslation());
		}
	}
}

Void Translator::NewEntry()
{
	StringItem	*entry = (StringItem *) entries.GetLast();

	text_id->Activate();
	edit_id->Activate();
	edit_original->Activate();
	button_remove->Activate();

	text_original->SetText("Original:");
	text_translated->SetText("Translation:");

	edit_id->SetText(String::FromInt(Math::Max(entry->GetID(), 0) + 1));
	edit_original->SetText(NIL);
	edit_translated->SetText(NIL);

	modified = True;
}

Void Translator::RemoveEntry()
{
	StringItem	*entry = (StringItem *) list_entries->GetSelectedEntry();

	entries.Remove(entry->GetID());

	list_entries->Remove(entry);

	DeleteObject(entry);

	edit_original->SetText(NIL);
	edit_translated->SetText(NIL);

	modified = True;
}
