 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/mcp/tools.h>
#include <smooth/misc/mcp/server.h>
#include <smooth/misc/encoding/base64.h>
#include <smooth/system/system.h>

#include <smooth.h>
#include <smooth/gui/widgets/multi/generic/container.h>
#include <smooth/gui/widgets/multi/generic/entry.h>
#include <smooth/gui/widgets/multi/list/listboxheader.h>
#include <smooth/gui/widgets/multi/list/listentryseparator.h>
#include <smooth/gui/widgets/multi/menu/menuentry.h>
#include <smooth/gui/widgets/multi/menu/menubarentry.h>
#include <smooth/gui/widgets/multi/menu/popupmenuentry.h>
#include <smooth/gui/widgets/multi/menu/popupmenuentrycheck.h>
#include <smooth/gui/widgets/multi/menu/popupmenuentryoption.h>

#include <png.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::MCP;

namespace
{
	/* Time to wait for queries and for actions, which may open modal dialogs.
	 */
	const Int	 queryTimeout	= 10000;
	const Int	 actionTimeout	= 2000;

	const Int	 maxFindResults	= 100;

	/* Names of widget classes. Derived classes must precede their base classes.
	 */
	struct TypeName
	{
		const Short	*classID;
		const char	*name;
	};

	const TypeName	 typeNames[] = { { &ToolWindow::classID,	   "ToolWindow"		  },
					 { &Window::classID,		   "Window"		  },
					 { &Titlebar::classID,		   "Titlebar"		  },
					 { &Statusbar::classID,		   "Statusbar"		  },
					 { &Divider::classID,		   "Divider"		  },
					 { &Client::classID,		   "Client"		  },
					 { &GroupBox::classID,		   "GroupBox"		  },
					 { &TabWidget::classID,		   "TabWidget"		  },
					 { &Button::classID,		   "Button"		  },
					 { &CheckBox::classID,		   "CheckBox"		  },
					 { &OptionBox::classID,		   "OptionBox"		  },
					 { &Hyperlink::classID,		   "Hyperlink"		  },
					 { &ActiveArea::classID,	   "ActiveArea"		  },
					 { &Arrows::classID,		   "Arrows"		  },
					 { &Scrollbar::classID,		   "Scrollbar"		  },
					 { &Slider::classID,		   "Slider"		  },
					 { &Progressbar::classID,	   "Progressbar"	  },
					 { &EditBox::classID,		   "EditBox"		  },
					 { &MultiEdit::classID,		   "MultiEdit"		  },
					 { &Text::classID,		   "Text"		  },
					 { &Image::classID,		   "Image"		  },
					 { &Cursor::classID,		   "Cursor"		  },
					 { &Tooltip::classID,		   "Tooltip"		  },
					 { &DropArea::classID,		   "DropArea"		  },
					 { &DragControl::classID,	   "DragControl"	  },
					 { &Shortcut::classID,		   "Shortcut"		  },
					 { &ComboBox::classID,		   "ComboBox"		  },
					 { &ListBox::classID,		   "ListBox"		  },
					 { &ListBoxHeader::classID,	   "ListBoxHeader"	  },
					 { &ImageBox::classID,		   "ImageBox"		  },
					 { &Tree::classID,		   "Tree"		  },
					 { &ListEntrySeparator::classID,   "ListEntrySeparator"	  },
					 { &ImageEntry::classID,	   "ImageEntry"		  },
					 { &PopupMenuEntryCheck::classID,  "PopupMenuEntryCheck"  },
					 { &PopupMenuEntryOption::classID, "PopupMenuEntryOption" },
					 { &PopupMenuEntry::classID,	   "PopupMenuEntry"	  },
					 { &MenubarEntry::classID,	   "MenubarEntry"	  },
					 { &MenuEntry::classID,		   "MenuEntry"		  },
					 { &ListEntry::classID,		   "ListEntry"		  },
					 { &MicroMenu::classID,		   "MicroMenu"		  },
					 { &PopupMenu::classID,		   "PopupMenu"		  },
					 { &Menubar::classID,		   "Menubar"		  },
					 { &List::classID,		   "List"		  },
					 { &Entry::classID,		   "Entry"		  },
					 { &Container::classID,		   "Container"		  },
					 { &HotspotSimpleButton::classID,  "HotspotSimpleButton"  },
					 { &Hotspot::classID,		   "Hotspot"		  },
					 { &Layer::classID,		   "Layer"		  } };

	/* Check whether a widget is a window, including windows
	 * of application defined classes.
	 */
	Bool IsWindow(const Widget *widget)
	{
		for (Int i = 0; i < Window::GetNOfWindows(); i++) if (Window::GetNthWindow(i) == widget) return True;

		return False;
	}

	/* Check whether a widget is a list. Lists do not report
	 * inheritance through their object types.
	 */
	Bool IsList(const Widget *widget)
	{
		const ObjectType	&type = widget->GetObjectType();

		return (type == List::classID || type == ListBox::classID || type == ComboBox::classID || type == ImageBox::classID);
	}

	/* Read the state of option boxes, which have no public accessor
	 * for it. Pointers to their protected members can be formed in a
	 * derived class and then be used with any option box.
	 */
	class OptionBoxState : public OptionBox
	{
		public:
			static Bool IsChecked(const OptionBox *optionBox)
			{
				Int *OptionBox::*variable = &OptionBoxState::variable;
				Int  OptionBox::*code	  = &OptionBoxState::code;

				return *(optionBox->*variable) == optionBox->*code;
			}
	};

	Bool IsHotspot(const Widget *widget)
	{
		const ObjectType	&type = widget->GetObjectType();

		return (type == Hotspot::classID || type == HotspotSimpleButton::classID);
	}

	String GetTypeName(const Widget *widget)
	{
		for (UnsignedInt i = 0; i < sizeof(typeNames) / sizeof(TypeName); i++)
		{
			if (widget->GetObjectType() == *typeNames[i].classID) return typeNames[i].name;
		}

		return IsWindow(widget) ? "Window" : "Widget";
	}

	/* Get the name of an object if one was set. Unnamed
	 * objects return a generated name based on their handle.
	 */
	String GetObjectName(const Object *object)
	{
		const String	&name = object->GetName();

		if (name == String("Object::").Append(String::FromInt(object->GetHandle()))) return NIL;

		return name;
	}

	/* Widgets that only implement other widgets and are hidden by default.
	 */
	Bool IsInternal(const Widget *widget)
	{
		Short	 type = widget->GetObjectType();

		return (IsHotspot(widget) || type == Cursor::classID || type == Shortcut::classID || type == DragControl::classID || type == Tooltip::classID);
	}

	/* Result helpers.
	 */
	Bool Fail(JSON &result, const String &text)
	{
		result = JSON(text);

		return False;
	}

	JSON TextContent(const String &text)
	{
		return JSON::NewArray().Add(JSON::NewObject().Set("type", "text").Set("text", text));
	}

	JSON JSONContent(const JSON &value)
	{
		char	*text = value.Serialize();
		String	 string;

		string.ImportFrom("UTF-8", text);

		delete [] text;

		return TextContent(string);
	}

	/* Check whether a widget is part of the given parent. Widgets may be
	 * listed by several widgets, e.g. popup menu entries by the menu and
	 * by the window showing it, but are part of their container only.
	 * Windows are listed separately.
	 */
	Bool IsOwnChild(const Widget *parent, const Widget *child)
	{
		return (child->GetContainer() == parent && !IsWindow(child));
	}

	/* Look up widgets and windows by ID. Search the widget trees, as
	 * object types do not reflect inheritance for most classes.
	 */
	Widget *FindWidget(Widget *widget, Int handle)
	{
		if (widget->GetHandle() == handle) return widget;

		for (Int i = 0; i < widget->GetNOfObjects(); i++)
		{
			if (!IsOwnChild(widget, widget->GetNthObject(i))) continue;

			Widget	*found = FindWidget(widget->GetNthObject(i), handle);

			if (found != NIL) return found;
		}

		return NIL;
	}

	Widget *GetWidget(const JSON &id)
	{
		if (!id.IsNumber()) return NIL;

		for (Int i = 0; i < Window::GetNOfWindows(); i++)
		{
			Widget	*found = FindWidget(Window::GetNthWindow(i), (Int) id.GetNumber());

			if (found != NIL) return found;
		}

		return NIL;
	}

	Window *GetWindow(const JSON &id)
	{
		if (!id.IsNumber()) return NIL;

		for (Int i = 0; i < Window::GetNOfWindows(); i++)
		{
			if (Window::GetNthWindow(i)->GetHandle() == (Int) id.GetNumber()) return Window::GetNthWindow(i);
		}

		return NIL;
	}

	Window *GetContainerWindow(Widget *widget)
	{
		if (IsWindow(widget)) return (Window *) widget;

		return widget->GetContainerWindow();
	}

	JSON GetBounds(const Widget *widget)
	{
		Point	 position = widget->GetRealPosition();
		Size	 size	  = widget->GetRealSize();

		return JSON::NewObject().Set("x", position.x).Set("y", position.y).Set("width", size.cx).Set("height", size.cy);
	}

	/* Describe a widget and its children.
	 */
	struct TreeOptions
	{
		Int	 maxDepth;
		Bool	 includeHidden;
		Bool	 includeInternal;
	};

	/* Add the state of a list entry. Tree items are expanded
	 * when marked, other entries have a check box.
	 */
	Void AddEntryState(const ListEntry *entry, JSON &info)
	{
		if (entry->IsSelected()) info.Set("selected", True);

		if	(entry->GetObjectType() == Tree::classID) info.Set("expanded", entry->IsMarked());
		else if (entry->IsMarked())			  info.Set("checked", True);
	}

	/* Describe the entries of a list, including
	 * the entries of tree items.
	 */
	JSON DescribeEntries(List *list)
	{
		JSON	 entries = JSON::NewArray();

		for (Int i = 0; i < list->Length(); i++)
		{
			ListEntry	*entry = list->GetNthEntry(i);
			JSON		 item  = JSON::NewObject().Set("id", entry->GetHandle()).Set("text", entry->GetText());

			AddEntryState(entry, item);

			if (entry->GetObjectType() == Tree::classID) item.Set("entries", DescribeEntries(((Tree *) entry)->GetList()));

			entries.Add(item);
		}

		return entries;
	}

	Void AddWidgetState(const Widget *widget, JSON &info)
	{
		const ObjectType	&type = widget->GetObjectType();

		if	(type == CheckBox::classID)    info.Set("checked", ((CheckBox *) widget)->IsChecked());
		else if (type == OptionBox::classID)   info.Set("checked", OptionBoxState::IsChecked((OptionBox *) widget));
		else if (type == Slider::classID)      info.Set("value", ((Slider *) widget)->GetValue());
		else if (type == Arrows::classID)      info.Set("value", ((Arrows *) widget)->GetValue());
		else if (type == Progressbar::classID) info.Set("value", ((Progressbar *) widget)->GetValue());
		else if (type == Hyperlink::classID)   info.Set("url", ((Hyperlink *) widget)->GetURL());

		/* Menu entries.
		 */
		if (type == MenuEntry::classID)
		{
			MenuEntry	*entry = (MenuEntry *) widget;

			if (entry->GetPopupMenu() != NIL)	info.Set("submenu", True);
			if (entry->GetCheckVariable() != NIL)	info.Set("checked", *entry->GetCheckVariable());
			if (entry->GetOptionVariable() != NIL)	info.Set("checked", *entry->GetOptionVariable() == entry->GetOptionCode());
		}

		/* Entries are listed with their list or tree item.
		 */
		if (type == ListEntry::classID) AddEntryState((ListEntry *) widget, info);

		if	(IsList(widget))	info.Set("entries", DescribeEntries((List *) widget));
		else if (type == Tree::classID) info.Set("entries", DescribeEntries(((Tree *) widget)->GetList()));

		/* Tabs are listed with their tab widget.
		 */
		if (type == TabWidget::classID)
		{
			TabWidget	*tabWidget = (TabWidget *) widget;
			JSON		 tabs	   = JSON::NewArray();

			for (Int i = 0; i < tabWidget->GetNOfObjects(); i++)
			{
				Widget	*tab  = tabWidget->GetNthObject(i);
				JSON	 item = JSON::NewObject().Set("id", tab->GetHandle()).Set("text", tab->GetText());

				if (tab == tabWidget->GetSelectedTab()) item.Set("selected", True);

				tabs.Add(item);
			}

			info.Set("tabs", tabs);
		}
	}

	Bool IsListed(const Widget *widget, const Widget *child, const TreeOptions &options)
	{
		if (!IsOwnChild(widget, child))				     return False;
		if (!options.includeHidden   && !child->IsVisible()) return False;
		if (!options.includeInternal && IsInternal(child))   return False;

		/* Entries are listed with their lists.
		 */
		if (IsList(widget) && child->GetObjectType() == ListEntry::classID) return False;
		if (widget->GetObjectType() == Tree::classID && IsList(child))	    return False;

		return True;
	}

	JSON DescribeWidget(const Widget *widget, Int depth, const TreeOptions &options)
	{
		JSON	 info = JSON::NewObject();

		info.Set("id", widget->GetHandle());
		info.Set("type", GetTypeName(widget));

		if (GetObjectName(widget)    != NIL) info.Set("name", GetObjectName(widget));
		if (widget->GetText()	     != NIL) info.Set("text", widget->GetText());
		if (widget->GetTooltipText() != NIL) info.Set("tooltip", widget->GetTooltipText());
		if (widget->GetStatusText()  != NIL) info.Set("status", widget->GetStatusText());

		info.Set("bounds", GetBounds(widget));

		if (!widget->IsVisible())  info.Set("visible", False);
		if (!widget->IsActive())   info.Set("enabled", False);
		if ( widget->IsFocussed()) info.Set("focused", True);

		AddWidgetState(widget, info);

		/* Add children.
		 */
		if (options.maxDepth >= 0 && depth >= options.maxDepth) return info;

		JSON	 children = JSON::NewArray();

		for (Int i = 0; i < widget->GetNOfObjects(); i++)
		{
			Widget	*child = widget->GetNthObject(i);

			if (IsListed(widget, child, options)) children.Add(DescribeWidget(child, depth + 1, options));
		}

		if (children.Length() > 0) info.Set("children", children);

		return info;
	}

	/* Find widgets matching the given criteria.
	 */
	Void FindWidgets(const Widget *widget, const JSON &arguments, const TreeOptions &options, Int windowID, JSON &results)
	{
		for (Int i = 0; i < widget->GetNOfObjects() && results.Length() < maxFindResults; i++)
		{
			Widget	*child = widget->GetNthObject(i);

			if (!IsOwnChild(widget, child))				     continue;
			if (!options.includeHidden   && !child->IsVisible()) continue;
			if (!options.includeInternal && IsInternal(child))   continue;

			Bool	 match = True;

			if (arguments.Has("text") && !child->GetText().ToLower().Contains(arguments.Get("text").GetString().ToLower())) match = False;
			if (arguments.Has("type") &&  GetTypeName(child).ToLower() != arguments.Get("type").GetString().ToLower())	 match = False;
			if (arguments.Has("name") &&  child->GetName() != arguments.Get("name").GetString())				 match = False;

			if (match)
			{
				TreeOptions	 flat = options;

				flat.maxDepth = 0;

				results.Add(DescribeWidget(child, 0, flat).Set("window_id", windowID));
			}

			FindWidgets(child, arguments, options, windowID, results);
		}
	}

	/* Simulate a click on a widget by emitting the mouse button signals of
	 * its hotspot, which is how most widgets receive clicks.
	 */
	Widget *GetClickTarget(Widget *widget)
	{
		if (IsHotspot(widget)) return widget;

		for (Int i = 0; i < widget->GetNOfObjects(); i++)
		{
			Widget	*child = widget->GetNthObject(i);

			if (IsHotspot(child) && child->IsVisible() && child->IsActive()) return child;
		}

		return widget;
	}

	Bool ClickWidget(Widget *widget, JSON &result)
	{
		if (!widget->IsVisible()) return Fail(result, "The widget is not visible.");
		if (!widget->IsActive())  return Fail(result, "The widget is disabled.");

		Widget	*target = GetClickTarget(widget);
		Int	 handle = target->GetHandle();
		Point	 point	= target->GetRealPosition() + Point(target->GetRealSize().cx / 2, target->GetRealSize().cy / 2);

		/* Handlers may delete the target, so check it still exists before each signal.
		 */
		if (Object::GetObject(handle) == target) target->onLeftButtonDown.Emit(point);
		if (Object::GetObject(handle) == target) target->onLeftButtonUp.Emit(point);
		if (Object::GetObject(handle) == target) target->onLeftButtonClick.Emit(point);

		result = TextContent("Clicked the widget.");

		return True;
	}

	/* Encode a bitmap as PNG.
	 */
	Void WritePNGData(png_structp png, png_bytep data, png_size_t length)
	{
		Buffer<UnsignedByte>	*buffer = (Buffer<UnsignedByte> *) png_get_io_ptr(png);
		Int			 size	= buffer->Size();

		buffer->Resize(size + length);

		memcpy(((UnsignedByte *) *buffer) + size, data, length);
	}

	Void FlushPNGData(png_structp)
	{
	}

	Bool EncodePNG(const Bitmap &bitmap, Buffer<UnsignedByte> &buffer)
	{
		png_structp	 png  = png_create_write_struct(PNG_LIBPNG_VER_STRING, NIL, NIL, NIL);
		png_infop	 info = png != NIL ? png_create_info_struct(png) : NIL;

		if (info == NIL) { png_destroy_write_struct(&png, NIL); return False; }

		const Size	&size = bitmap.GetSize();
		png_bytep	 row  = new png_byte [size.cx * 3];

		if (setjmp(png_jmpbuf(png)))
		{
			png_destroy_write_struct(&png, &info);

			delete [] row;

			return False;
		}

		png_set_write_fn(png, &buffer, &WritePNGData, &FlushPNGData);
		png_set_IHDR(png, info, size.cx, size.cy, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png, info);

		for (Int y = 0; y < size.cy; y++)
		{
			for (Int x = 0; x < size.cx; x++)
			{
				Color	 color = bitmap.GetPixel(Point(x, y));

				row[x * 3 + 0] = color.GetRed();
				row[x * 3 + 1] = color.GetGreen();
				row[x * 3 + 2] = color.GetBlue();
			}

			png_write_row(png, row);
		}

		png_write_end(png, info);
		png_destroy_write_struct(&png, &info);

		delete [] row;

		return True;
	}

	/* Tool implementations running on the GUI thread.
	 */
	Bool ListWindows(const JSON &, JSON &result)
	{
		JSON	 windows = JSON::NewArray();

		for (Int i = 0; i < Window::GetNOfWindows(); i++)
		{
			Window	*window = Window::GetNthWindow(i);
			JSON	 info	= JSON::NewObject();

			info.Set("id", window->GetHandle());
			info.Set("type", GetTypeName(window));

			if (GetObjectName(window) != NIL) info.Set("name", GetObjectName(window));
			if (window->GetText() != NIL) info.Set("title", window->GetText());

			/* Report the screen position and the size in pixels,
			 * which widget bounds and screenshots are based on.
			 */
			info.Set("bounds", JSON::NewObject().Set("x", window->GetPosition().x).Set("y", window->GetPosition().y).Set("width", window->GetRealSize().cx).Set("height", window->GetRealSize().cy));
			info.Set("visible", window->IsVisible());

			windows.Add(info);
		}

		result = JSONContent(windows);

		return True;
	}

	Bool GetWidgetTree(const JSON &arguments, JSON &result)
	{
		Widget	*widget = arguments.Has("widget_id") ? GetWidget(arguments.Get("widget_id")) : GetWindow(arguments.Get("window_id"));

		if (widget == NIL) return Fail(result, "No widget or window with the given ID exists.");

		TreeOptions	 options;

		options.maxDepth	= arguments.Get("max_depth").IsNumber() ? (Int) arguments.Get("max_depth").GetNumber() : -1;
		options.includeHidden	= arguments.Get("include_hidden").GetBoolean();
		options.includeInternal = arguments.Get("include_internal").GetBoolean();

		result = JSONContent(DescribeWidget(widget, 0, options));

		return True;
	}

	JSON CollectWidgets(const JSON &arguments)
	{
		TreeOptions	 options;

		options.maxDepth	= 0;
		options.includeHidden	= arguments.Get("include_hidden").GetBoolean();
		options.includeInternal = arguments.Get("include_internal").GetBoolean();

		JSON	 results = JSON::NewArray();

		for (Int i = 0; i < Window::GetNOfWindows(); i++)
		{
			Window	*window = Window::GetNthWindow(i);

			if (arguments.Has("window_id") && window->GetHandle() != (Int) arguments.Get("window_id").GetNumber()) continue;
			if (!options.includeHidden && !window->IsVisible())						       continue;

			FindWidgets(window, arguments, options, window->GetHandle(), results);
		}

		return results;
	}

	Bool FindWidgets(const JSON &arguments, JSON &result)
	{
		result = JSONContent(CollectWidgets(arguments));

		return True;
	}

	Bool Click(const JSON &arguments, JSON &result)
	{
		Widget	*widget = GetWidget(arguments.Get("widget_id"));

		if (widget == NIL) return Fail(result, "No widget with the given ID exists.");

		return ClickWidget(widget, result);
	}

	Bool SetValue(const JSON &arguments, JSON &result)
	{
		Widget	*widget = GetWidget(arguments.Get("widget_id"));

		if (widget == NIL) return Fail(result, "No widget with the given ID exists.");

		if (!widget->IsActive()) return Fail(result, "The widget is disabled.");

		const JSON		&value = arguments.Get("value");
		const ObjectType	&type  = widget->GetObjectType();

		if (type == EditBox::classID || type == MultiEdit::classID)
		{
			/* Set the text and notify listeners like user input does.
			 */
			if (!value.IsString()) return Fail(result, "The value must be a string.");

			widget->SetText(value.GetString());

			if (type == EditBox::classID) ((EditBox *) widget)->onInput.Emit(value.GetString());
			else			      ((MultiEdit *) widget)->onInput.Emit(value.GetString());
		}
		else if (type == CheckBox::classID || type == OptionBox::classID || (type == MenuEntry::classID && (((MenuEntry *) widget)->GetCheckVariable() != NIL || ((MenuEntry *) widget)->GetOptionVariable() != NIL)))
		{
			/* Click to change the state like the user does.
			 */
			Bool	 checked = False;

			if	(type == CheckBox::classID)					 checked = ((CheckBox *) widget)->IsChecked();
			else if (type == OptionBox::classID)					 checked = OptionBoxState::IsChecked((OptionBox *) widget);
			else if (((MenuEntry *) widget)->GetCheckVariable() != NIL)		 checked = *((MenuEntry *) widget)->GetCheckVariable();
			else									 checked = *((MenuEntry *) widget)->GetOptionVariable() == ((MenuEntry *) widget)->GetOptionCode();

			if (!value.IsBoolean()) return Fail(result, "The value must be a boolean.");

			Bool	 option = (type == OptionBox::classID || (type == MenuEntry::classID && ((MenuEntry *) widget)->GetOptionVariable() != NIL));

			if (option && !value.GetBoolean()) return Fail(result, "Options cannot be unchecked. Check another option instead.");

			if (checked != value.GetBoolean() && !ClickWidget(widget, result)) return False;
		}
		else if (type == Slider::classID || type == Arrows::classID)
		{
			if (!value.IsNumber()) return Fail(result, "The value must be a number.");

			if (type == Slider::classID) ((Slider *) widget)->SetValue((Int) value.GetNumber());
			else			     ((Arrows *) widget)->SetValue((Int) value.GetNumber());
		}
		else if (type == ListEntry::classID)
		{
			/* Expand or collapse tree items and check or uncheck entries
			 * of lists with check boxes like clicking the mark does.
			 */
			ListEntry	*entry = (ListEntry *) widget;

			if (!value.IsBoolean()) return Fail(result, "The value must be a boolean.");

			if (type != Tree::classID && !(entry->GetContainer()->GetFlags() & LF_MULTICHECKBOX)) return Fail(result, "The entry has no check box. Use click to select it.");

			entry->SetMark(value.GetBoolean());
		}
		else if (IsList(widget))
		{
			/* Select an entry by index or text.
			 */
			List	*list = (List *) widget;

			if	(value.IsNumber()) { if (value.GetNumber() < 0 || value.GetNumber() >= list->Length()) return Fail(result, "The entry index is out of range."); list->SelectNthEntry((Int) value.GetNumber()); }
			else if (value.IsString()) { if (list->GetEntry(value.GetString()) == NIL)		       return Fail(result, "No entry with the given text exists."); list->SelectEntry(value.GetString()); }
			else			   return Fail(result, "The value must be an entry index or text.");
		}
		else if (type == TabWidget::classID)
		{
			/* Select a tab by ID or text.
			 */
			TabWidget	*tabWidget = (TabWidget *) widget;
			Widget		*tab	   = NIL;

			for (Int i = 0; i < tabWidget->GetNOfObjects(); i++)
			{
				Widget	*child = tabWidget->GetNthObject(i);

				if ((value.IsNumber() && child->GetHandle() == (Int) value.GetNumber()) ||
				    (value.IsString() && child->GetText() == value.GetString())) tab = child;
			}

			if (tab == NIL) return Fail(result, "No tab with the given ID or text exists.");

			tabWidget->SelectTab(tab);
		}
		else
		{
			return Fail(result, String("Setting a value is not supported for widgets of type ").Append(GetTypeName(widget)).Append("."));
		}

		result = TextContent("Set the value.");

		return True;
	}

	Bool Focus(const JSON &arguments, JSON &result)
	{
		Widget	*widget = GetWidget(arguments.Get("widget_id"));

		if (widget == NIL) return Fail(result, "No widget with the given ID exists.");

		if (!widget->IsVisible() || !widget->IsActive()) return Fail(result, "The widget is not visible or disabled.");

		widget->SetFocus();

		result = TextContent("Focused the widget.");

		return True;
	}

	struct KeyName
	{
		const char	*name;
		Int		 key;
	};

	const KeyName	 keyNames[] = { { "Enter",     Input::Keyboard::KeyReturn },
					{ "Tab",       Input::Keyboard::KeyTab	  },
					{ "Escape",    Input::Keyboard::KeyEscape },
					{ "Backspace", Input::Keyboard::KeyBack	  },
					{ "Delete",    Input::Keyboard::KeyDelete },
					{ "Insert",    Input::Keyboard::KeyInsert },
					{ "Space",     Input::Keyboard::KeySpace  },
					{ "Left",      Input::Keyboard::KeyLeft	  },
					{ "Right",     Input::Keyboard::KeyRight  },
					{ "Up",	       Input::Keyboard::KeyUp	  },
					{ "Down",      Input::Keyboard::KeyDown	  },
					{ "Home",      Input::Keyboard::KeyHome	  },
					{ "End",       Input::Keyboard::KeyEnd	  },
					{ "PageUp",    Input::Keyboard::KeyPrior  },
					{ "PageDown",  Input::Keyboard::KeyNext	  } };

	Bool SendKeys(const JSON &arguments, JSON &result)
	{
		Widget	*widget = GetWidget(arguments.Get("widget_id"));

		if (widget == NIL) return Fail(result, "No widget with the given ID exists.");

		Window	*window = GetContainerWindow(widget);

		if (window == NIL) return Fail(result, "The widget is not part of a window.");

		/* Check key names before sending anything.
		 */
		const JSON	&keys = arguments.Get("keys");
		Array<Int>	 codes;

		for (Int i = 0; i < keys.Length(); i++)
		{
			const String	&name = keys.GetNth(i).GetString();
			Int		 code = -1;

			for (UnsignedInt j = 0; j < sizeof(keyNames) / sizeof(KeyName); j++) if (name.ToLower() == String(keyNames[j].name).ToLower()) code = keyNames[j].key;

			if (name.StartsWith("F") && name.Length() <= 3 && name.Tail(name.Length() - 1).ToInt() >= 1 && name.Tail(name.Length() - 1).ToInt() <= 12) code = Input::Keyboard::KeyF1 + name.Tail(name.Length() - 1).ToInt() - 1;

			if (code < 0) return Fail(result, String("Unknown key name: ").Append(name));

			codes.Add(code);
		}

		if (widget != window && !widget->IsFocussed()) widget->SetFocus();

		/* Type text, then press keys.
		 */
		const String	&text = arguments.Get("text").GetString();

		for (Int i = 0; i < text.Length(); i++) window->Process(SM_CHAR, text[i], 0);

		for (Int i = 0; i < codes.Length(); i++)
		{
			window->Process(SM_KEYDOWN, codes.GetNth(i), 0);
			window->Process(SM_KEYUP, codes.GetNth(i), 0);
		}

		result = TextContent("Sent the keys.");

		return True;
	}

	Bool Screenshot(const JSON &arguments, JSON &result)
	{
		Widget	*widget = arguments.Has("widget_id") ? GetWidget(arguments.Get("widget_id")) : GetWindow(arguments.Get("window_id"));

		if (widget == NIL) return Fail(result, "No widget or window with the given ID exists.");

		Window	*window = GetContainerWindow(widget);

		if (window == NIL || !window->IsVisible()) return Fail(result, "The window is not visible.");

		/* Copy the area from the window's surface.
		 */
		Surface	*surface = window->GetDrawSurface();
		Rect	 area	 = (widget == window) ? Rect(Point(0, 0), surface->GetSize()) : Rect(widget->GetRealPosition(), widget->GetRealSize());

		area = Rect::OverlapRect(area, Rect(Point(0, 0), surface->GetSize()));

		if (area.GetWidth() <= 0 || area.GetHeight() <= 0) return Fail(result, "The widget has no visible area.");

		Bitmap			 bitmap(area.GetSize(), 24);
		Buffer<UnsignedByte>	 png;

		surface->BlitToBitmap(area, bitmap, Rect(Point(0, 0), area.GetSize()));

		if (!EncodePNG(bitmap, png)) return Fail(result, "Encoding the screenshot failed.");

		result = JSON::NewArray().Add(JSON::NewObject().Set("type", "image").Set("data", Encoding::Base64(png).Encode(png.Size())).Set("mimeType", "image/png"));

		return True;
	}

	Bool CloseWindow(const JSON &arguments, JSON &result)
	{
		Window	*window = GetWindow(arguments.Get("window_id"));

		if (window == NIL) return Fail(result, "No window with the given ID exists.");

		window->Close();

		result = TextContent("Closed the window.");

		return True;
	}

	/* Count widgets for wait_for. Returns the number of
	 * matches as the result.
	 */
	Bool CountWidgets(const JSON &arguments, JSON &result)
	{
		if (arguments.Has("widget_id"))
		{
			Widget	*widget = GetWidget(arguments.Get("widget_id"));

			result = JSON(widget != NIL && widget->IsVisible() ? 1 : 0);

			return True;
		}

		result = JSON(CollectWidgets(arguments).Length());

		return True;
	}

	/* Tool definitions.
	 */
	struct Tool
	{
		const char	*name;
		const char	*description;
		const char	*schema;

		GUIFunction	 function;
		Int		 timeout;
	};

	const Tool	 tools[] = { { "list_windows",
				       "List the application's windows with their IDs, types, titles, bounds and visibility.",
				       "{\"type\":\"object\",\"properties\":{}}",
				       &ListWindows, queryTimeout },

				     { "get_widget_tree",
				       "Get the widget hierarchy of a window or widget. Each widget has an ID, its type, text, tooltip, status text, bounds within its window and state like visibility, enabled state, focus, checked state, values, list entries (nested for tree items, with their expanded state) and tabs. Hidden and internal widgets are left out unless requested.",
				       "{\"type\":\"object\",\"properties\":{\"window_id\":{\"type\":\"integer\",\"description\":\"ID of the window to describe.\"},\"widget_id\":{\"type\":\"integer\",\"description\":\"ID of a widget to describe instead of a whole window.\"},\"max_depth\":{\"type\":\"integer\",\"description\":\"Maximum depth of children to include. Unlimited by default.\"},\"include_hidden\":{\"type\":\"boolean\",\"description\":\"Include hidden widgets, e.g. on unselected tabs.\"},\"include_internal\":{\"type\":\"boolean\",\"description\":\"Include internal widgets like hotspots.\"}}}",
				       &GetWidgetTree, queryTimeout },

				     { "find_widgets",
				       "Find widgets by text (case insensitive substring), type and/or name in all windows or a given window. Returns a flat list of matches with their window IDs.",
				       "{\"type\":\"object\",\"properties\":{\"text\":{\"type\":\"string\"},\"type\":{\"type\":\"string\",\"description\":\"Widget type, e.g. Button, EditBox, ListBox.\"},\"name\":{\"type\":\"string\"},\"window_id\":{\"type\":\"integer\"},\"include_hidden\":{\"type\":\"boolean\"},\"include_internal\":{\"type\":\"boolean\"}}}",
				       &FindWidgets, queryTimeout },

				     { "click",
				       "Click a widget, e.g. a button, check box, menu entry, list entry, combo box or hyperlink. The widget receives the click like from the mouse, so all its handlers run. If the click opens a modal dialog, the call returns while the dialog is open.",
				       "{\"type\":\"object\",\"properties\":{\"widget_id\":{\"type\":\"integer\"}},\"required\":[\"widget_id\"]}",
				       &Click, actionTimeout },

				     { "set_value",
				       "Set the value of a widget: text for edit boxes, true or false for check boxes, option boxes and checkable menu entries, a number for sliders and arrows, an entry index or text for lists and combo boxes, true or false to expand or collapse tree items and to check or uncheck entries of lists with check boxes, and a tab ID or text for tab widgets.",
				       "{\"type\":\"object\",\"properties\":{\"widget_id\":{\"type\":\"integer\"},\"value\":{\"type\":[\"string\",\"number\",\"boolean\"]}},\"required\":[\"widget_id\",\"value\"]}",
				       &SetValue, actionTimeout },

				     { "focus",
				       "Give the keyboard focus to a widget.",
				       "{\"type\":\"object\",\"properties\":{\"widget_id\":{\"type\":\"integer\"}},\"required\":[\"widget_id\"]}",
				       &Focus, actionTimeout },

				     { "send_keys",
				       "Focus a widget, type text into it and then press keys. Key names are Enter, Tab, Escape, Backspace, Delete, Insert, Space, Left, Right, Up, Down, Home, End, PageUp, PageDown and F1 to F12. Modifier keys are not supported.",
				       "{\"type\":\"object\",\"properties\":{\"widget_id\":{\"type\":\"integer\"},\"text\":{\"type\":\"string\",\"description\":\"Text to type.\"},\"keys\":{\"type\":\"array\",\"items\":{\"type\":\"string\"},\"description\":\"Keys to press after typing.\"}},\"required\":[\"widget_id\"]}",
				       &SendKeys, actionTimeout },

				     { "screenshot",
				       "Take a PNG screenshot of a window or a single widget. The image is rendered by the application, so it works even if the window is covered.",
				       "{\"type\":\"object\",\"properties\":{\"window_id\":{\"type\":\"integer\"},\"widget_id\":{\"type\":\"integer\"}}}",
				       &Screenshot, queryTimeout },

				     { "wait_for",
				       "Wait until a widget matching the given criteria appears, or disappears if gone is true. Criteria are a widget ID, or text, type and/or name like for find_widgets.",
				       "{\"type\":\"object\",\"properties\":{\"widget_id\":{\"type\":\"integer\"},\"text\":{\"type\":\"string\"},\"type\":{\"type\":\"string\"},\"name\":{\"type\":\"string\"},\"window_id\":{\"type\":\"integer\"},\"gone\":{\"type\":\"boolean\"},\"timeout_ms\":{\"type\":\"integer\",\"description\":\"Maximum time to wait, 5000 by default.\"}}}",
				       &CountWidgets, queryTimeout },

				     { "close_window",
				       "Close a window, like using its close button.",
				       "{\"type\":\"object\",\"properties\":{\"window_id\":{\"type\":\"integer\"}},\"required\":[\"window_id\"]}",
				       &CloseWindow, actionTimeout } };

	const Tool *GetTool(const String &name)
	{
		for (UnsignedInt i = 0; i < sizeof(tools) / sizeof(Tool); i++) if (name == tools[i].name) return &tools[i];

		return NIL;
	}

	JSON ToolResult(const JSON &content, Bool error)
	{
		JSON	 result = JSON::NewObject().Set("content", content);

		if (error) result.Set("isError", True);

		return result;
	}

	/* Poll until the wait condition is met.
	 */
	JSON WaitFor(const JSON &arguments)
	{
		Int	 timeout = arguments.Get("timeout_ms").IsNumber() ? Math::Min(Math::Max((Int) arguments.Get("timeout_ms").GetNumber(), 0), 60000) : 5000;
		Bool	 gone	 = arguments.Get("gone").GetBoolean();

		for (Int waited = 0; True; waited += 100)
		{
			JSON	 count;
			Bool	 success = False;

			if (!Server::RunOnGUIThread(&CountWidgets, arguments, count, success, queryTimeout)) return ToolResult(TextContent("The application did not respond in time."), True);

			if (( gone && count.GetNumber() == 0) ||
			    (!gone && count.GetNumber() >  0)) return ToolResult(TextContent(gone ? "The widget is gone." : "The widget is there."), False);

			if (waited >= timeout) return ToolResult(TextContent("Timed out waiting."), True);

			System::System::Sleep(100);
		}
	}
};

S::String S::MCP::Tools::GetInstructions()
{
	return "This server controls a smooth application through its widgets. Start with list_windows, then use get_widget_tree or find_widgets to "
	       "get widget IDs. Use click, set_value and send_keys to interact, wait_for to wait for changes and screenshot to see the user interface. "
	       "IDs stay valid as long as the widget exists. Clicking a menubar entry or combo box opens a popup window; use list_windows to find it.";
}

S::MCP::JSON S::MCP::Tools::GetToolList()
{
	JSON	 list = JSON::NewArray();

	for (UnsignedInt i = 0; i < sizeof(tools) / sizeof(Tool); i++)
	{
		JSON	 schema;

		JSON::Parse(tools[i].schema, schema);

		list.Add(JSON::NewObject().Set("name", tools[i].name).Set("description", tools[i].description).Set("inputSchema", schema));
	}

	return list;
}

S::Bool S::MCP::Tools::HasTool(const String &name)
{
	return GetTool(name) != NIL;
}

S::MCP::JSON S::MCP::Tools::CallTool(const String &name, const JSON &arguments)
{
	const Tool	*tool = GetTool(name);

	if (tool->function == &CountWidgets) return WaitFor(arguments);

	JSON	 result;
	Bool	 success = False;

	if (!Server::RunOnGUIThread(tool->function, arguments, result, success, tool->timeout))
	{
		if (tool->timeout == actionTimeout) return ToolResult(TextContent("The action is still running, probably because it opened a modal dialog. Use list_windows to find new windows."), False);
		else				    return ToolResult(TextContent("The application did not respond in time. It may be busy or showing a native dialog."), True);
	}

	if (!success) return ToolResult(TextContent(result.GetString()), True);

	return ToolResult(result, False);
}
