 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
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

#define IDI_ICON	100
#define IDB_ICON	200

using namespace smooth::Input;

Int smooth::Main(const Array<String> &args)
{
	return Translator(args.GetNth(0)).Loop();
}

Translator::Translator(const String &openFile) : Application("smooth Translator")
{
	fileName	= NIL;
	templateName	= NIL;

	dataSection	= NIL;
	modified	= False;

	splitFactor	= 0.58;
	updateSplitter	= False;

	Rect	 workArea = System::Screen::GetActiveScreenWorkArea();

	wnd		= new GUI::Window(String("smooth Translator v").Append(SMOOTH_VERSION), Point(workArea.left, workArea.top) + Point(50, 50), Size(740, 490));
	title		= new Titlebar();
	menubar		= new Menubar();
	statusbar	= new Statusbar("Ready");

	Section::onSelectItem.Connect(&Translator::SelectEntry, this);

	text_filter	= new Text("Filter:", Point(377, 10));
	text_filter->SetOrientation(OR_UPPERRIGHT);

	edit_filter	= new EditBox(Point(370 - text_filter->GetUnscaledTextWidth(), 7), Size(275 - text_filter->GetUnscaledTextWidth(), 0), 0);
	edit_filter->onInput.Connect(&Translator::FilterEntries, this);
	edit_filter->SetOrientation(OR_UPPERRIGHT);

	button_clear	= new Button("Clear", Point(87, 6), Size());
	button_clear->onAction.Connect(&Translator::ClearFilter, this);
	button_clear->SetOrientation(OR_UPPERRIGHT);

	list_entries	= new ListBox(Point(7, 34), Size(757, 161));
	list_entries->onSelectEntry.Connect(&Translator::SelectEntry, this);
	list_entries->SetFlags(LF_ALLOWRESELECT);
	list_entries->AddTab("ID", 80);
	list_entries->AddTab("String");
	list_entries->AddTab("Translation");

	list_filtered	= new ListBox(list_entries->GetPosition(), list_entries->GetSize());
	list_filtered->onSelectEntry.Connect(&Translator::SelectEntry, this);
	list_filtered->SetFlags(LF_ALLOWRESELECT);
	list_filtered->AddTab("ID", 80);
	list_filtered->AddTab("String");
	list_filtered->AddTab("Translation");
	list_filtered->Hide();

	droparea = new DropArea(list_entries->GetPosition(), list_entries->GetSize());
	droparea->onDropFiles.Connect(&Translator::HandleDropFile, this);

	splitter	= new Divider(200, OR_HORZ | DIV_MOVABLE);
	splitter->onDrag.Connect(&Translator::OnDragSplitter, this);

	layer_edit	= new Layer();
	layer_edit->SetMetrics(Point(8, 202), Size(500, 200));
	layer_edit->onChangeSize.Connect(&Translator::OnChangeEditLayerSize, this);

	button_new	= new Button("New", Point(7, 7), Size());
	button_new->onAction.Connect(&Translator::NewEntry, this);
	button_new->SetTooltipText("Create a new entry");

	text_id		= new Text("ID:", Point(94, 11));

	button_remove	= new Button("Remove", Point(263, 7), Size());
	button_remove->onAction.Connect(&Translator::RemoveEntry, this);
	button_remove->SetTooltipText("Remove entry from translation");
	button_remove->SetOrientation(OR_UPPERRIGHT);

	button_copy	= new Button("Copy", Point(175, 7), Size());
	button_copy->onAction.Connect(&Translator::CopyOriginal, this);
	button_copy->SetTooltipText("Copy original to translation");
	button_copy->SetOrientation(OR_UPPERRIGHT);

	button_save	= new Button("Save", Point(87, 7), Size());
	button_save->onAction.Connect(&Translator::SaveData, this);
	button_save->SetTooltipText("Save entry and show next");
	button_save->SetOrientation(OR_UPPERRIGHT);

	text_original	= new Text("Original:", Point(94, 38));
	text_translated	= new Text("Translation:", Point(94, 106));

	edit_id		= new EditBox(Point(221 - text_translated->GetUnscaledTextWidth(), 8), Size(37, 0), 6);
	edit_id->SetWidth(edit_id->GetFont().GetUnscaledTextSizeX("000000") + 6); 
	edit_id->SetFlags(EDB_NUMERIC);

	edit_original	= new MultiEdit(Point(221 - text_translated->GetUnscaledTextWidth(), 35), Size(608, 60), 0);
	edit_translated	= new MultiEdit(Point(221 - text_translated->GetUnscaledTextWidth(), 103), Size(608, 60), 0);

	MenuEntry	*entry = NIL;

	menu_file	= new PopupMenu();

	menubar->AddEntry("File", menu_file);

	entry = menu_file->AddEntry("New");
	entry->onAction.Connect(&Translator::NewFile, this);
	entry->SetShortcut(SC_CONTROL, 'N', wnd);

	entry = menu_file->AddEntry("Close");
	entry->onAction.Connect(&Translator::Close, this);
	entry->Deactivate();

	menu_file->AddEntry();

	entry = menu_file->AddEntry("Open...");
	entry->onAction.Connect(&Translator::OpenFile, this);
	entry->SetShortcut(SC_CONTROL, 'O', wnd);

	entry = menu_file->AddEntry("Save");
	entry->onAction.Connect(&Translator::SaveFile, this);
	entry->SetShortcut(SC_CONTROL, 'S', wnd);
	entry->Deactivate();

	entry = menu_file->AddEntry("Save as...");
	entry->onAction.Connect(&Translator::SaveFileAs, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, 'S', wnd);
	entry->Deactivate();

	menu_file->AddEntry();

	entry = menu_file->AddEntry("Export as...");
	entry->onAction.Connect(&Translator::ExportAs, this);
	entry->Deactivate();

	menu_file->AddEntry();

	entry = menu_file->AddEntry("Exit");
	entry->onAction.Connect(&GUI::Window::Close, wnd);
	entry->SetShortcut(SC_ALT, Keyboard::KeyF4, wnd);

	menu_entry	= new PopupMenu();

	entry = menubar->AddEntry("Entry", menu_entry);
	entry->Deactivate();

	entry = menu_entry->AddEntry("New");
	entry->onAction.Connect(&Translator::NewEntry, this);
	entry->SetShortcut(SC_ALT, 'N', wnd);

	menu_entry->AddEntry();

	entry = menu_entry->AddEntry("Save");
	entry->onAction.Connect(&Translator::SaveData, this);
	entry->SetShortcut(SC_ALT, 'S', wnd);

	entry = menu_entry->AddEntry("Remove");
	entry->onAction.Connect(&Translator::RemoveEntry, this);
	entry->SetShortcut(SC_ALT, 'R', wnd);
	entry->Deactivate();

	list_entries->Deactivate();
	list_filtered->Deactivate();

	layer_edit->Deactivate();

	text_filter->Deactivate();
	edit_filter->Deactivate();
	button_clear->Deactivate();

	Add(wnd);

	wnd->Add(text_filter);
	wnd->Add(edit_filter);
	wnd->Add(button_clear);

	wnd->Add(list_entries);
	wnd->Add(list_filtered);

	wnd->GetMainLayer()->Add(droparea);

	wnd->Add(splitter);

	wnd->Add(layer_edit);

	layer_edit->Add(text_id);
	layer_edit->Add(edit_id);
	layer_edit->Add(text_original);
	layer_edit->Add(edit_original);
	layer_edit->Add(text_translated);
	layer_edit->Add(edit_translated);
	layer_edit->Add(button_save);
	layer_edit->Add(button_remove);
	layer_edit->Add(button_new);
	layer_edit->Add(button_copy);

	wnd->Add(title);
	wnd->Add(menubar);
	wnd->Add(statusbar);

	wnd->SetMinimumSize(Size(488, 308));
	wnd->SetIcon(NIL);

#ifdef __WIN32__
	wnd->SetIcon(ImageLoader::Load(String("Icon:").Append(String::FromInt(IDI_ICON))));
	wnd->SetIconDirect(LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
#endif

	wnd->doClose.Connect(&Translator::ExitProc, this);
	wnd->onChangeSize.Connect(&Translator::OnChangeSize, this);

	OnChangeSize(wnd->GetSize());

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
	DeleteObject(menu_entry);
	DeleteObject(statusbar);

	DeleteObject(text_filter);
	DeleteObject(edit_filter);
	DeleteObject(button_clear);

	DeleteObject(list_entries);
	DeleteObject(list_filtered);

	DeleteObject(droparea);

	DeleteObject(splitter);

	DeleteObject(layer_edit);

	DeleteObject(text_id);
	DeleteObject(edit_id);
	DeleteObject(text_original);
	DeleteObject(edit_original);
	DeleteObject(text_translated);
	DeleteObject(edit_translated);
	DeleteObject(button_save);
	DeleteObject(button_remove);
	DeleteObject(button_new);
	DeleteObject(button_copy);
}

String Translator::GetShortFileName(const String &fileName)
{
	return fileName.Tail(fileName.Length() - fileName.FindLast(Directory::GetDirectoryDelimiter()) - 1);
}

Bool Translator::ExitProc()
{
	if (entries.Length() == 0) return True;

	if (!modified)
	{
		CloseFile();

		return True;
	}

	Int	 id = QuickMessage(String("Do you want to save changes in ").Append(GetShortFileName(fileName)).Append("?"), "smooth Translator", Message::Buttons::YesNoCancel, Message::Icon::Question);

	switch (id)
	{
		case Message::Button::Yes:
			SaveFile();
			CloseFile();

			return True;
		case Message::Button::No:
			CloseFile();

			return True;
		case Message::Button::Cancel:
		case Message::Button::Close:
			return False;
	}

	return True;
}

Void Translator::OnChangeSize(const Size &size)
{
	Size	 clientSize  = wnd->GetClientRect().GetSize();

	Float	 splitFactor = this->splitFactor;
	Int	 splitterPos = Float(clientSize.cy) * splitFactor;

	updateSplitter = True;

	OnDragSplitter(splitterPos);

	this->splitFactor = splitFactor;
}

Void Translator::OnChangeEditLayerSize(const Size &size)
{
	edit_original->SetSize(Size(size.cx - edit_original->GetX() - 7, (size.cy - 50) / 2));

	text_translated->SetY(edit_original->GetY() + edit_original->GetHeight() + 11);
	edit_translated->SetY(text_translated->GetY() - 3);

	edit_translated->SetSize(Size(edit_original->GetWidth(), edit_original->GetHeight() + (size.cy & 1)));
}

Void Translator::OnDragSplitter(Int splitterPos)
{
	Rect	 clientRect = wnd->GetClientRect();
	Size	 clientSize = clientRect.GetSize();

	if (splitterPos > clientSize.cy - 118) splitterPos = clientSize.cy - 118;
	if (splitterPos <		  110) splitterPos =		     110;

	if (splitter->GetPos() == splitterPos && !updateSplitter) return;

	Surface	*surface = wnd->GetDrawSurface();

	surface->StartPaint(clientRect);

	Bool	 filtered = list_filtered->IsVisible();

	splitter->Hide();
	list_entries->Hide();
	list_filtered->Hide();
	layer_edit->Hide();

	list_entries->SetSize(Size(clientSize.cx - 14, splitterPos - 42));
	list_filtered->SetSize(list_entries->GetSize());

	droparea->SetSize(list_entries->GetSize());

	layer_edit->SetY(clientRect.top + splitterPos + 2);
	layer_edit->SetSize(clientSize - Size(0, splitterPos + 2));

	updateSplitter = False;
	splitFactor    = Float(splitterPos) / clientSize.cy;

	splitter->SetPos(splitterPos);
	splitter->Show();

	if (filtered) list_filtered->Show();
	else	      list_entries->Show();

	layer_edit->Show();

	surface->EndPaint();
}

Void Translator::NewFile()
{
	if (!ExitProc()) return;

	dataSection = new Section();

	fileName = "unnamed";
	modified = False;

	list_entries->Activate();
	list_filtered->Activate();

	layer_edit->Activate();

	text_filter->Activate();
	edit_filter->Activate();

	menu_file->GetNthEntry(1)->Activate();
	menu_file->GetNthEntry(4)->Activate();
	menu_file->GetNthEntry(5)->Activate();
	menu_file->GetNthEntry(7)->Activate();

	menubar->GetNthEntry(1)->Activate();

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

	wnd->SetText(wnd->GetText().Append(" - unnamed"));

	list_entries->SelectNthEntry(0);

	SelectEntry(entries.GetFirst());
}

Void Translator::Close()
{
	ExitProc();
}

Void Translator::CloseFile()
{
	if (entries.Length() == 0) return;

	fileName     = NIL;
	templateName = NIL;

	wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION));

	ClearFilter(list_filtered);

	list_entries->RemoveAllEntries();

	for (Int i = 0; i < numInfoItems;	     i++) DeleteObject(entries.GetNth(i));
	for (Int i = 0; i < createdEntries.Length(); i++) DeleteObject(createdEntries.GetNth(i));

	entries.RemoveAll();
	createdEntries.RemoveAll();

	delete dataSection;

	dataSection = NIL;
	modified    = False;

	SelectEntry(NIL);

	list_entries->Deactivate();
	list_filtered->Deactivate();

	layer_edit->Deactivate();

	text_filter->Deactivate();
	edit_filter->Deactivate();

	menu_file->GetNthEntry(1)->Deactivate();
	menu_file->GetNthEntry(4)->Deactivate();
	menu_file->GetNthEntry(5)->Deactivate();
	menu_file->GetNthEntry(7)->Deactivate();

	menubar->GetNthEntry(1)->Deactivate();
}

Void Translator::OpenFile()
{
	if (!ExitProc()) return;

	FileSelection	 dialog;

	dialog.SetParentWindow(wnd);

	dialog.AddFilter("XML Language Files", "*.xml");
	dialog.AddFilter("All Files", "*.*");

	if (dialog.ShowDialog() == Success())
	{
		OpenFileName(dialog.GetFileName());
	}
}

Int Translator::OpenTemplate(const String &fileName)
{
	if (!File(fileName).Exists()) return Error();

	XML::Document	 doc;

	if (doc.LoadFile(fileName) == Error()) return Error();

	XML::Node	*data = doc.GetRootNode()->GetNodeByName("data");

	dataSection->Parse(data, list_entries, entries);

	return Success();
}

Void Translator::OpenFileName(const String &openFile)
{
	/* Open document and check format.
	 */
	XML::Document	 doc;

	if (doc.LoadFile(openFile) == Error())
	{
		QuickMessage(String("Invalid file format: ").Append(GetShortFileName(openFile)), "smooth Translator", Message::Buttons::Ok, Message::Icon::Error);

		return;
	}

	/* Setup new file and load document.
	 */
	NewFile();

	fileName = openFile;

	wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION).Append(" - ").Append(GetShortFileName(fileName)));

	XML::Node	*info = doc.GetRootNode()->GetNodeByName("info");

	for (Int k = 0; k < info->GetNOfNodes(); k++)
	{
		String		 property = info->GetNthNode(k)->GetAttributeByName("name")->GetContent();
		InfoItem	*entry = NULL;

		if	(property == "program")	    entry = (InfoItem *) entries.GetNth(0);
		else if (property == "version")	    entry = (InfoItem *) entries.GetNth(1);
		else if (property == "language")    entry = (InfoItem *) entries.GetNth(2);
		else if (property == "righttoleft") entry = (InfoItem *) entries.GetNth(3);

		else if (property == "template")
		{
			templateName = info->GetNthNode(k)->GetContent();

			if (OpenTemplate(File(fileName).GetFilePath().Append(Directory::GetDirectoryDelimiter()).Append(templateName)) != Success())
			{
				templateName = NIL;
			}

			entry = (InfoItem *) entries.GetNth(4);
		}

		else if (property == "author")	    entry = (InfoItem *) entries.GetNth(5);
		else if (property == "url")	    entry = (InfoItem *) entries.GetNth(6);

		if (entry != NIL)
		{
			entry->SetValue(info->GetNthNode(k)->GetContent());
		}
	}

	XML::Node	*data = doc.GetRootNode()->GetNodeByName("data");

	dataSection->Parse(data, list_entries, entries);

	/* Look for entries with the same original text and suggest a translation.
	 */
	for (Int i = 0; i < entries.Length(); i++)
	{
		if (entries.GetNthIndex(i) < 0) continue;

		StringItem	*item = (StringItem *) entries.GetNth(i);

		if (item->GetTranslation() != NIL) continue;

		for (Int j = 0; j < entries.Length(); j++)
		{
			if (entries.GetNthIndex(j) < 0 || i == j) continue;

			StringItem	*item2 = (StringItem *) entries.GetNth(j);

			if (item->GetOriginal() == item2->GetOriginal() && item2->GetTranslation() != NIL)
			{
				item->SetTranslation(item2->GetTranslation());

				break;
			}
		}
	}

	list_entries->SelectNthEntry(0);

	SelectEntry(entries.GetFirst());
}

Void Translator::SaveFile()
{
	if (fileName == NIL) return;

	if (fileName != "unnamed") SaveFileName(fileName);
	else			   SaveFileAs();
}

Void Translator::SaveFileAs()
{
	if (fileName == NIL) return;

	FileSelection	 dialog;

	dialog.SetParentWindow(wnd);
	dialog.SetMode(SFM_SAVE);
	dialog.SetDefaultExtension("xml");

	dialog.AddFilter("XML Language Files", "*.xml");
	dialog.AddFilter("All Files", "*.*");

	if (dialog.ShowDialog() == Success())
	{
		fileName = dialog.GetFileName();

		SaveFileName(fileName);

		wnd->SetText(String("smooth Translator v").Append(SMOOTH_VERSION).Append(" - ").Append(GetShortFileName(fileName)));
	}
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

	dataSection->Save(data, templateName);

	doc->SetEncoding("UTF-8");
	doc->SetRootNode(root);

	doc->SaveFile(file);

	modified = False;

	delete doc;
	delete root;

	ReplaceLineEndings(file);
	FormatLines(file);
}

Void Translator::ExportAs()
{
	if (fileName == NIL) return;

	FileSelection	 dialog;

	dialog.SetParentWindow(wnd);
	dialog.SetMode(SFM_SAVE);
	dialog.SetDefaultExtension("txt");

	dialog.AddFilter("Plain Text Files", "*.txt");
	dialog.AddFilter("All Files", "*.*");

	if (dialog.ShowDialog() == Success())
	{
		IO::OutStream	 stream(IO::STREAM_FILE, dialog.GetFileName(), IO::OS_REPLACE);

		stream.OutputLine(((InfoItem *) entries.GetNth(0))->GetValue().Append(" ").Append(((InfoItem *) entries.GetNth(1))->GetValue()));
		stream.OutputLine(((InfoItem *) entries.GetNth(2))->GetValue());
		stream.OutputLine(((InfoItem *) entries.GetNth(5))->GetValue());
		stream.OutputString("\n");

		dataSection->Export(stream, 0);
	}
}

Void Translator::HandleDropFile(const Array<String> &files)
{
	OpenFileName(files.GetFirst());
}

Void Translator::ReplaceLineEndings(const String &file)
{
	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	IO::InStream	 in(IO::STREAM_FILE, file, IO::IS_READ);

	String		 xmlString = in.InputString(in.Size());

	/* Replace line endings.
	 */
	xmlString.Replace("\n", "&#10;");
	xmlString.Replace(">&#10;", ">\n");

	IO::OutStream	 out(IO::STREAM_FILE, file, IO::OS_REPLACE);

	out.OutputString(xmlString);

	out.Close();
}

Void Translator::FormatLines(const String &file)
{
	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	IO::InStream	 in(IO::STREAM_FILE, file, IO::IS_READ);

	String		 xmlString;

	while (in.GetPos() < in.Size())
	{
		String	 line = in.InputLine();

		if (line.Trim().StartsWith("<section ")) xmlString.Append("\n");

		while (line.StartsWith("    "))	{ xmlString.Append("\t"); line = line.Tail(line.Length() - 4); }
		while (line.StartsWith(" "))	{ xmlString.Append("  "); line = line.Tail(line.Length() - 1); }

		xmlString.Append(line).Append("\n");
	}

	IO::OutStream	 out(IO::STREAM_FILE, file, IO::OS_REPLACE);

	out.OutputString(xmlString);

	out.Close();
}

Void Translator::SaveData()
{
	if (edit_id->GetText() == NIL)
	{
		Int	 index = -1;

		if	(edit_original->GetText() == "Program")	    index = 0;
		else if (edit_original->GetText() == "Version")	    index = 1;
		else if (edit_original->GetText() == "Language")    index = 2;
		else if (edit_original->GetText() == "RightToLeft") index = 3;
		else if (edit_original->GetText() == "Template")    index = 4;
		else if (edit_original->GetText() == "Author")	    index = 5;
		else if (edit_original->GetText() == "URL")	    index = 6;

		if (index >= 0)
		{
			InfoItem	*entry = (InfoItem *) entries.GetNth(index);

			entry->SetValue(edit_translated->GetText());

			if (index < numInfoItems - 1)
			{
				entry = (InfoItem *) entries.GetNth(index + 1);

				entry->Select();
			}
			else
			{
				text_id->Activate();
				edit_id->Activate();
				edit_original->Activate();
				button_remove->Activate();
				button_copy->Activate();

				menu_entry->GetNthEntry(3)->Activate();

				text_original->SetText("Original:");
				text_translated->SetText("Translation:");

				if (entries.Length() > numInfoItems)
				{
					StringItem	*entry = (StringItem *) entries.GetNth(numInfoItems);

					entry->Select();
				}
				else
				{
					edit_id->SetText("1");
					edit_original->SetText(NIL);
					edit_translated->SetText(NIL);

					edit_translated->MarkAll();
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
			createdEntries.Add(entry, entry->GetID());
		}

		GUI::Font	 redFont;
		GUI::Font	 blackFont;

		redFont.SetColor(Color(255, 0, 0));

		if (entry->GetTranslation() == NIL) entry->SetFont(redFont);
		else				    entry->SetFont(blackFont);

		/* Select next entry if no filter is active.
		 */
		if (edit_filter->GetText() == NIL)
		{
			entry = (StringItem *) entries.GetNext();

			if (entry != NIL)
			{
				entry->Select();
			}
			else
			{
				edit_id->SetText(String::FromInt(edit_id->GetText().ToInt() + 1));
				edit_original->SetText(NIL);
				edit_translated->SetText(NIL);

				edit_translated->MarkAll();
			}
		}
	}

	if (edit_filter->GetText() != NIL) FilterEntries();

	modified = True;
}

Void Translator::SelectEntry(ListEntry *entry)
{
	if (entry == NIL)
	{
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
		button_copy->Deactivate();

		menu_entry->GetNthEntry(2)->Deactivate();
		menu_entry->GetNthEntry(3)->Deactivate();

		text_original->SetText("Original:");
		text_translated->SetText("Translation:");
	}
	else if (entry->GetObjectType() == InfoItem::classID)
	{
		edit_id->SetText(NIL);
		edit_original->SetText(((InfoItem *) entry)->GetName());
		edit_translated->SetText(((InfoItem *) entry)->GetValue());

		text_id->Deactivate();
		edit_id->Deactivate();
		text_original->Activate();
		edit_original->Deactivate();
		text_translated->Activate();
		edit_translated->Activate();
		button_save->Activate();
		button_remove->Deactivate();
		button_copy->Deactivate();

		menu_entry->GetNthEntry(2)->Activate();
		menu_entry->GetNthEntry(3)->Deactivate();

		text_original->SetText("Field:");
		text_translated->SetText("Value:");

		edit_translated->MarkAll();
	}
	else
	{
		edit_id->SetText(String::FromInt(((StringItem *) entry)->GetID()));
		edit_original->SetText(((StringItem *) entry)->GetOriginal());
		edit_translated->SetText(((StringItem *) entry)->GetTranslation());

		text_id->Activate();
		edit_id->Activate();
		text_original->Activate();
		edit_original->Activate();
		text_translated->Activate();
		edit_translated->Activate();
		button_save->Activate();
		button_remove->Activate();
		button_copy->Activate();

		menu_entry->GetNthEntry(2)->Activate();
		menu_entry->GetNthEntry(3)->Activate();

		text_original->SetText("Original:");
		text_translated->SetText("Translation:");

		edit_translated->MarkAll();
	}
}

Void Translator::NewEntry()
{
	Int	 id = 1;

	if (entries.Length() > 7)
	{
		StringItem	*entry = (StringItem *) entries.GetLast();

		id = Math::Max(entry->GetID(), 0) + 1;
	}

	text_id->Activate();
	edit_id->Activate();
	text_original->Activate();
	edit_original->Activate();
	text_translated->Activate();
	edit_translated->Activate();
	button_save->Activate();
	button_remove->Activate();
	button_copy->Activate();

	menu_entry->GetNthEntry(2)->Activate();
	menu_entry->GetNthEntry(3)->Activate();

	text_original->SetText("Original:");
	text_translated->SetText("Translation:");

	edit_id->SetText(String::FromInt(id));
	edit_original->SetText(NIL);
	edit_translated->SetText(NIL);

	modified = True;
}

Void Translator::RemoveEntry()
{
	/* Get selected entry.
	 */
	StringItem	*entry = NIL;

	if (edit_filter->GetText() == NIL) entry = (StringItem *) GetSelectedEntry(list_entries);
	else				   entry = (StringItem *) GetSelectedEntry(list_filtered);

	if (entry == NIL) return;

	/* Find list corresponding to selected entry.
	 */
	Int	 id   = entry->GetID();
	List	*list = GetEntryList(list_entries, id);

	entry = (StringItem *) entries.Get(id);

	/* Remove entry.
	 */
	entries.Remove(id);

	list->Remove(entry);

	if (createdEntries.Get(id) != NIL)
	{
		DeleteObject(entry);

		createdEntries.Remove(id);
	}

	SelectEntry(NIL);

	if (edit_filter->GetText() != NIL) FilterEntries();
	else				   list_entries->Paint(SP_PAINT);

	modified = True;
}

ListEntry *Translator::GetSelectedEntry(List *list)
{
	/* Recursively search for the list with the selected entry.
	 */
	for (Int i = list->Length() - 1; i >= 0; i--)
	{
		ListEntry	*entry = list->GetNthEntry(i);

		if (entry->GetObjectType() == Tree::classID)
		{
			ListEntry	*selected = GetSelectedEntry(((Tree *) entry)->GetList());

			if (selected != NIL) return selected;
		}
		else if (entry->IsSelected())
		{
			return entry;
		}
	}

	return NIL;
}

List *Translator::GetEntryList(List *list, Int id)
{
	/* Recursively search for the list with the requested entry.
	 */
	for (Int i = list->Length() - 1; i >= 0; i--)
	{
		ListEntry	*entry = list->GetNthEntry(i);

		if (entry->GetObjectType() == Tree::classID)
		{
			List	*selected = GetEntryList(((Tree *) entry)->GetList(), id);

			if (selected != NIL) return selected;
		}
		else if (entry->GetObjectType() == StringItem::classID)
		{
			if (((StringItem *) entry)->GetID() == id) return list;
		}
	}

	return NIL;
}

Void Translator::CopyOriginal()
{
	edit_translated->SetText(edit_original->GetText());
}

Void Translator::FilterEntries()
{
	/* Clean up if filter text is empty.
	 */
	if (edit_filter->GetText() == NIL)
	{
		ClearFilter();

		return;
	}

	/* Update entries and exchange list widgets.
	 */
	Surface	*surface = wnd->GetDrawSurface();

	surface->StartPaint(Rect(list_entries->GetRealPosition(), list_entries->GetRealSize()));

	Int	 scrollbarPos = -1;

	if (list_filtered->IsVisible()) scrollbarPos = ((Scrollbar *) list_filtered->GetNthObject(0))->GetValue();

	list_entries->Hide();

	ClearFilter(list_filtered);

	dataSection->Filter(edit_filter->GetText().ToLower(), list_filtered);

	((Scrollbar *) list_filtered->GetNthObject(0))->SetValue(scrollbarPos);

	list_filtered->Show();

	surface->EndPaint();

	/* Activate clear button.
	 */
	button_clear->Activate();
}

Void Translator::ClearFilter()
{
	/* Clear filter box.
	 */
	edit_filter->SetText(NIL);
	button_clear->Deactivate();

	/* Clean up entries and exchange list widgets.
	 */
	Surface	*surface = wnd->GetDrawSurface();

	surface->StartPaint(Rect(list_filtered->GetRealPosition(), list_filtered->GetRealSize()));

	list_filtered->Hide();

	ClearFilter(list_filtered);

	list_entries->Show();

	surface->EndPaint();
}

Void Translator::ClearFilter(List *list)
{
	/* Recursively clear this and subordinate sections.
	 */
	for (Int i = list->Length() - 1; i >= 0; i--)
	{
		ListEntry	*entry = list->GetNthEntry(i);

		if (entry->GetObjectType() == Tree::classID) ClearFilter(((Tree *) entry)->GetList());

		list->Remove(entry);

		delete entry;
	}
}
