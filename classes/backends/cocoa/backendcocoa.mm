 /* The smooth Class Library
  * Copyright (C) 1998-2025 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/cocoa/backendcocoa.h>
#include <smooth/system/backends/cocoa/eventcocoa.h>

#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/special/droparea.h>

using namespace smooth;
using namespace smooth::GUI;

S::Backends::Backend *CreateBackendCocoa()
{
	return new S::Backends::BackendCocoa();
}

S::Int	 backendCocoaTmp = S::Backends::Backend::AddBackend(&CreateBackendCocoa);

#if defined MAC_OS_X_VERSION_10_6 && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface CocoaApplicationDelegate : NSObject <NSApplicationDelegate> { }
#else
@interface CocoaApplicationDelegate : NSObject { }
#endif
	- (NSApplicationTerminateReply)	 applicationShouldTerminate: (NSApplication *) sender;

	- (void)			 application:		     (NSApplication *) sender openFiles: (NSArray *) fileNames;

	- (DropArea *)			 findDropArea:		     (Widget *) container;
@end

@implementation CocoaApplicationDelegate
	- (NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication *) sender
	{
		/* Send a dummy event to allow NSApp::run to exit.
		 */
		NSEvent	*event = [NSEvent otherEventWithType: NSApplicationDefined
						    location: NSMakePoint(0, 0)
					       modifierFlags: 0
						   timestamp: 0
						windowNumber: 0
						     context: nil
						     subtype: 0
						       data1: 0
						       data2: 0];
		[NSApp postEvent: event
			 atStart: YES];

		[NSApp stop: self];

		/* Request application to quit.
		 */
		System::EventCocoa::RequestApplicationQuit();

		/* Application will terminate after all windows are closed.
		 */
		return NSTerminateCancel;
	}

	- (void) application: (NSApplication *) sender openFiles: (NSArray *) fileNames
	{
		/* Look for a window with a drop area.
		 */
		DropArea	*dropArea = NIL;

		for (Int n = 0; n < Window::GetNOfWindows(); n++)
		{
			Window		*window = Window::GetNthWindow(n);

			if (!window->IsVisible()) continue;

			if ((dropArea = [self findDropArea: window]) != NIL) break;
		}

		/* Check whether a drop area has been found.
		 */
		if (dropArea == NIL)
		{
			[sender replyToOpenOrPrint: NSApplicationDelegateReplyFailure];

			return;
		}

		/* Simulate arrival of the files via drag & drop.
		 */
		Array<String>	 files;

		for (UnsignedInt i = 0; i < [fileNames count]; i++)
		{
			String	 file;

			file.ImportFrom("UTF-8", [[fileNames objectAtIndex: i] UTF8String]);
			files.Add(file);

			dropArea->onDropFile.Emit(file);
		}

		dropArea->onDropFiles.Emit(files);

		[sender replyToOpenOrPrint: NSApplicationDelegateReplySuccess];
	}

	- (DropArea *) findDropArea: (Widget *) container
	{
		for (Int n = 0; n < container->GetNOfObjects(); n++)
		{
			Widget		*widget	  = container->GetNthObject(n);

			if (widget->GetObjectType() == DropArea::classID && widget->IsActive()) return (DropArea *) widget;

			DropArea	*dropArea = [self findDropArea: widget];

			if (dropArea != NIL) return dropArea;
		}

		return NIL;		
	}
@end

S::Backends::BackendCocoa::BackendCocoa()
{
	type = BACKEND_COCOA;

	pool = nil;
}

S::Backends::BackendCocoa::~BackendCocoa()
{
}

S::Int S::Backends::BackendCocoa::Init()
{
	/* Init the Cocoa app.
	 */
	[NSApplication sharedApplication];

	/* Create a garbage collection pool.
	 */
	pool = [[NSAutoreleasePool alloc] init];

	/* Set application delegate.
	 */
	[NSApp setDelegate: [[[CocoaApplicationDelegate alloc] init] autorelease]];

	/* Get default colors.
	 */
	UpdateColors();

	/* Set default language if not set.
	 */
	NSUserDefaults	*defaults  = [NSUserDefaults standardUserDefaults];
	NSArray		*languages = [defaults objectForKey: @"AppleLanguages"];
	NSString	*language  = [languages objectAtIndex: 0];

	setenv("LANG", [language UTF8String], False);

	return Success();
}

S::Int S::Backends::BackendCocoa::Deinit()
{
	/* Release our garbage collection pool.
	 */
	[pool release];

	return Success();
}

S::Void S::Backends::BackendCocoa::UpdateColors()
{
	/* Create a dummy window to retrieve its background color.
	 *
	 * This does not work in macOS 10.10 to 10.13, so we check if it returned
	 * a meaningful color and assign a light gray default value otherwise.
	 */
	NSWindow	*window = [[NSWindow alloc] initWithContentRect: NSMakeRect(0, 0, 0, 0)
							      styleMask: NSTitledWindowMask
								backing: NSBackingStoreBuffered
								  defer: YES];

	NSColor	*windowBackground	      = [[window backgroundColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];

	if ([windowBackground redComponent]   == 0 &&
	    [windowBackground greenComponent] == 0 &&
	    [windowBackground blueComponent]  == 0) windowBackground = [NSColor colorWithDeviceRed: 0.9 green: 0.9 blue: 0.9 alpha: 1.0];

	[window release];

	/* Query the other UI colors.
	 */
	NSColor	*text			      = [[NSColor textColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];
	NSColor	*textBackground		      = [[NSColor textBackgroundColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];

	NSColor	*controlText		      = [[NSColor controlTextColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];
	NSColor	*disabledControlText	      = [[NSColor disabledControlTextColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];
	
	NSColor	*selectedText		      = [[NSColor selectedTextColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];
	NSColor	*selectedTextBackground       = [[NSColor selectedTextBackgroundColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];

	NSColor	*alternateSelectedControl     = [[NSColor alternateSelectedControlColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];
	NSColor	*alternateSelectedControlText = [[NSColor alternateSelectedControlTextColor] colorUsingColorSpaceName: NSDeviceRGBColorSpace];

	if ([disabledControlText redComponent]   == 0 &&
	    [disabledControlText greenComponent] == 0 &&
	    [disabledControlText blueComponent]  == 0) disabledControlText = [NSColor colorWithDeviceRed: 0.5 green: 0.5 blue: 0.5 alpha: 1.0];

	/* Query link color if selector exists.
	 */
	static SEL	 linkColorSelector = @selector(linkColor);
	static Bool	 canQueryLinkColor = [[NSColor class] respondsToSelector: linkColorSelector];

	NSColor	*linkColor		      = nil;

	if (canQueryLinkColor) linkColor = [((NSColor *(*)(id, SEL)) [[NSColor class] methodForSelector: linkColorSelector])([NSColor class], linkColorSelector) colorUsingColorSpaceName: NSDeviceRGBColorSpace];

	/* Convert to smooth colors.
	 */
	Setup::BackgroundColor	  = GUI::Color([windowBackground redComponent] * 255, [windowBackground greenComponent] * 255, [windowBackground blueComponent] * 255);
	Setup::LightGrayColor	  = GUI::Color(([windowBackground redComponent] + (1.0 - [windowBackground redComponent]) * 0.6) * 255, ([windowBackground greenComponent] + (1.0 - [windowBackground greenComponent]) * 0.6) * 255, ([windowBackground blueComponent] + (1.0 - [windowBackground blueComponent]) * 0.6) * 255);

	Setup::ClientColor	  = GUI::Color([textBackground redComponent] * 255, [textBackground greenComponent] * 255, [textBackground blueComponent] * 255);
	Setup::ClientTextColor	  = GUI::Color([text redComponent] * 255, [text greenComponent] * 255, [text blueComponent] * 255);

	Setup::DividerLightColor  = GUI::Color(([windowBackground redComponent] + (1.0 - [windowBackground redComponent]) * 0.6) * 255, ([windowBackground greenComponent] + (1.0 - [windowBackground greenComponent]) * 0.6) * 255, ([windowBackground blueComponent] + (1.0 - [windowBackground blueComponent]) * 0.6) * 255);
	Setup::DividerDarkColor	  = GUI::Color([windowBackground redComponent] * 0.7 * 255, [windowBackground greenComponent] * 0.7 * 255, [windowBackground blueComponent] * 0.7 * 255);

	Setup::TextColor	  = GUI::Color([controlText redComponent] * 255, [controlText greenComponent] * 255, [controlText blueComponent] * 255);
	Setup::InactiveTextColor  = GUI::Color([disabledControlText redComponent] * 255, [disabledControlText greenComponent] * 255, [disabledControlText blueComponent] * 255);

	Setup::HighlightColor	  = GUI::Color([selectedTextBackground redComponent] * 255, [selectedTextBackground greenComponent] * 255, [selectedTextBackground blueComponent] * 255);
	Setup::HighlightTextColor = GUI::Color([selectedText redComponent] * 255, [selectedText greenComponent] * 255, [selectedText blueComponent] * 255);

	Setup::GradientStartColor = GUI::Color(([alternateSelectedControl redComponent] - [alternateSelectedControl redComponent] * 0.25) * 255, ([alternateSelectedControl greenComponent] - [alternateSelectedControl greenComponent] * 0.25) * 255, ([alternateSelectedControl blueComponent] - [alternateSelectedControl blueComponent] * 0.25) * 255);
	Setup::GradientEndColor	  = GUI::Color(([alternateSelectedControl redComponent] + (1.0 - [alternateSelectedControl redComponent]) * 0.25) * 255, ([alternateSelectedControl greenComponent] + (1.0 - [alternateSelectedControl greenComponent]) * 0.25) * 255, ([alternateSelectedControl blueComponent] + (1.0 - [alternateSelectedControl blueComponent]) * 0.25) * 255);
	Setup::GradientTextColor  = GUI::Color([alternateSelectedControlText redComponent] * 255, [alternateSelectedControlText greenComponent] * 255, [alternateSelectedControlText blueComponent] * 255);

	if (linkColor != nil)
	{
		Setup::LinkColor	  = GUI::Color([linkColor redComponent] * 255, [linkColor greenComponent] * 255, [linkColor blueComponent] * 255);
		Setup::LinkHighlightColor = GUI::Color([linkColor redComponent] * 255, [linkColor greenComponent] * 255, [linkColor blueComponent] * 255);
	}

	if (Setup::InactiveTextColor == Setup::TextColor)
	{
		Setup::InactiveTextColor = Setup::InactiveTextColor.Average(Setup::BackgroundColor);
	}
}

S::Bool S::Backends::BackendCocoa::IsOSXVersionAtLeast(Int majorVersion, Int minorVersion, Int microVersion)
{
	static SInt32	 osMajorVersion = 0;
	static SInt32	 osMinorVersion = 0;
	static SInt32	 osMicroVersion = 0;

	if (osMajorVersion == 0)
	{
		Gestalt(gestaltSystemVersionMajor, &osMajorVersion);
		Gestalt(gestaltSystemVersionMinor, &osMinorVersion);
		Gestalt(gestaltSystemVersionBugFix, &osMicroVersion);
	}

	if ( osMajorVersion >  majorVersion ||
	    (osMajorVersion == majorVersion && ( osMinorVersion >  minorVersion ||
						(osMinorVersion == minorVersion && osMicroVersion >= microVersion)))) return True;

	return False;
}
