 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifdef __EXPLICIT_TEMPLATES__

#include <smooth.h>
#include <smooth/backends/backend.h>
#include <smooth/gui/window/backends/gdi/windowgdi.h>
#include <smooth/system/backends/eventbackend.h>
#include <smooth/xml/xul/popupmenu.h>

template class smooth::Array<bool>;
template class smooth::Array<char *>;
template class smooth::Array<long>;
template class smooth::Array<smooth::Backends::Backend *, void *>;
template class smooth::Array<smooth::Backends::Backend * (*)(), void *>;
template class smooth::Array<smooth::Directory>;
template class smooth::Array<smooth::File>;
template class smooth::Array<smooth::GUI::BitmapBackend * (*)(int), void *>;
template class smooth::Array<smooth::GUI::BitmapBackend * (*)(long, long, long), void *>;
template class smooth::Array<smooth::GUI::BitmapBackend * (*)(smooth::GUI::BitmapBackend const &), void *>;
template class smooth::Array<smooth::GUI::BitmapBackend * (*)(void *), void *>;
template class smooth::Array<smooth::GUI::SurfaceBackend * (*)(void *), void *>;
template class smooth::Array<smooth::GUI::Widget *, void *>;
template class smooth::Array<smooth::GUI::WindowBackend * (*)(), void *>;
template class smooth::Array<smooth::GUI::WindowGDI *, void *>;
template class smooth::Array<smooth::I18n::Language *, void *>;
template class smooth::Array<smooth::IO::Filter *, void *>;
template class smooth::Array<smooth::Object *, void *>;
template class smooth::Array<smooth::String>;
template class smooth::Array<smooth::System::EventBackend * (*)(), void *>;
template class smooth::Array<smooth::System::TimerBackend * (*)(), void *>;
template class smooth::Array<smooth::Threads::MutexBackend * (*)(void *), void *>;
template class smooth::Array<smooth::Threads::SemaphoreBackend * (*)(void *), void *>;
template class smooth::Array<smooth::Threads::ThreadBackend * (*)(void *), void *>;
template class smooth::Array<smooth::Tree::Entry *, void *>;
template class smooth::Array<smooth::XML::Attribute *, void *>;
template class smooth::Array<smooth::XML::Node *, void *>;
template class smooth::Array<smooth::XML::XUL::PopupMenu *, void *>;
template class smooth::Array<smooth::XML::XUL::Widget *, void *>;
template class smooth::Array<void *>;
template class smooth::Array_Backend<bool>;
template class smooth::Array_Backend<char *>;
template class smooth::Array_Backend<long>;
template class smooth::Array_Backend<smooth::Directory>;
template class smooth::Array_Backend<smooth::File>;
template class smooth::Array_Backend<smooth::String>;
template class smooth::Array_Backend<void *>;
template class smooth::Array_Entry<bool>;
template class smooth::Array_Entry<char *>;
template class smooth::Array_Entry<long>;
template class smooth::Array_Entry<smooth::Directory>;
template class smooth::Array_Entry<smooth::File>;
template class smooth::Array_Entry<smooth::String>;
template class smooth::Array_Entry<void *>;
template class smooth::Buffer<char>;
template class smooth::Callback0<bool>;
template class smooth::Callback1<bool, smooth::GUI::Point const &>;
template class smooth::Callback1<long, smooth::Threads::Thread *>;
template class smooth::Callback2<smooth::GUI::Menu *, long, long>;
template class smooth::Callback3<long, long, long, long>;
template class smooth::Signal0<void>;
template class smooth::Signal1<void, long>;
template class smooth::Signal1<void, smooth::GUI::Point const &>;
template class smooth::Signal1<void, smooth::GUI::Widget *>;
template class smooth::Signal1<void, smooth::String const &>;
template class smooth::Signal2<void, long, long>;
template class smooth::Signal3<void, long, long, long>;
template class smooth::SlotBase0<void>;
template class smooth::SlotBase1<void, long>;
template class smooth::SlotBase1<void, smooth::GUI::Point const &>;
template class smooth::SlotBase1<void, smooth::GUI::Widget *>;
template class smooth::SlotBase2<void, long, long>;
template class smooth::SlotBase3<void, long, long, long>;
template class smooth::SlotClass0<smooth::GUI::Arrows, void>;
template class smooth::SlotClass0<smooth::GUI::BackgroundApplication, void>;
template class smooth::SlotClass0<smooth::GUI::CheckBox, void>;
template class smooth::SlotClass0<smooth::GUI::ComboBox, void>;
template class smooth::SlotClass0<smooth::GUI::Dialogs::ColorSelection, void>;
template class smooth::SlotClass0<smooth::GUI::Dialogs::MessageDlg, void>;
template class smooth::SlotClass0<smooth::GUI::Dialogs::SplashScreen, void>;
template class smooth::SlotClass0<smooth::GUI::Dialogs::TipOfTheDay, void>;
template class smooth::SlotClass0<smooth::GUI::EditBox, void>;
template class smooth::SlotClass0<smooth::GUI::Hyperlink, void>;
template class smooth::SlotClass0<smooth::GUI::ListBox, void>;
template class smooth::SlotClass0<smooth::GUI::ListEntry, void>;
template class smooth::SlotClass0<smooth::GUI::Menubar, void>;
template class smooth::SlotClass0<smooth::GUI::MenuEntry, void>;
template class smooth::SlotClass0<smooth::GUI::MultiEdit, void>;
template class smooth::SlotClass0<smooth::GUI::OptionBox, void>;
template class smooth::SlotClass0<smooth::GUI::PopupView, void>;
template class smooth::SlotClass0<smooth::GUI::Titlebar, void>;
template class smooth::SlotClass0<smooth::GUI::Tooltip, void>;
template class smooth::SlotClass0<smooth::GUI::Widget, void>;
template class smooth::SlotClass0<smooth::GUI::Window, void>;
template class smooth::SlotClass1<smooth::GUI::DragControl, void, smooth::GUI::Point const &>;
template class smooth::SlotClass1<smooth::GUI::ListBox, void, smooth::GUI::Widget *>;
template class smooth::SlotClass1<smooth::GUI::Scrollbar, void, long>;
template class smooth::SlotClass1<smooth::GUI::Scrollbar, void, smooth::GUI::Point const &>;
template class smooth::SlotClass1<smooth::GUI::Slider, void, smooth::GUI::Point const &>;
template class smooth::SlotClass1<smooth::GUI::Titlebar, void, smooth::GUI::Point const &>;
template class smooth::SlotClass3<smooth::GUI::Dialogs::ColorSelection, void, long, long, long>;
template class smooth::SlotRBase0<bool>;
template class smooth::SlotRBase0<long>;
template class smooth::SlotRBase0<smooth::GUI::Menu *>;
template class smooth::SlotRBase1<bool, smooth::GUI::Point const &>;
template class smooth::SlotRBase1<long, smooth::Threads::Thread *>;
template class smooth::SlotRBase2<smooth::GUI::Menu *, long, long>;
template class smooth::SlotRBase3<long, long, long, long>;
template class smooth::SlotRClass0<smooth::GUI::Dialogs::ColorSelection, bool>;
template class smooth::SlotRClass0<smooth::GUI::Dialogs::MessageDlg, bool>;
template class smooth::SlotRClass0<smooth::GUI::Dialogs::SplashScreen, bool>;
template class smooth::SlotRClass1<smooth::GUI::Titlebar, bool, smooth::GUI::Point const &>;
template class smooth::SlotRClass1<smooth::GUI::Widget, bool, smooth::GUI::Point const &>;
template class smooth::SlotRClass3<smooth::GUI::Window, long, long, long, long>;
template class smooth::SlotRGlobal0<bool>;
template class smooth::SlotRGlobal0<long>;
template class smooth::SlotRGlobal0<smooth::GUI::Menu *>;
template class smooth::SlotRGlobal1<bool, smooth::GUI::Point const &>;
template class smooth::SlotRGlobal1<long, smooth::Threads::Thread *>;
template class smooth::SlotRGlobal2<smooth::GUI::Menu *, long, long>;
template class smooth::SlotRGlobal3<long, long, long, long>;
template class smooth::SlotRSignal0<bool>;
template class smooth::SlotRSignal0<long>;
template class smooth::SlotRSignal0<smooth::GUI::Menu *>;
template class smooth::SlotRSignal1<bool, smooth::GUI::Point const &>;
template class smooth::SlotRSignal1<long, smooth::Threads::Thread *>;
template class smooth::SlotRSignal2<smooth::GUI::Menu *, long, long>;
template class smooth::SlotRSignal3<long, long, long, long>;
template class smooth::SlotRValue0<bool>;
template class smooth::SlotRValue0<long>;
template class smooth::SlotRValue0<smooth::GUI::Menu *>;
template class smooth::SlotSignal0<void>;
template class smooth::SlotSignal2<void, long, long>;

#endif
