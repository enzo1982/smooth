 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/application/xulloader.h>
#include <smooth/gui/application/application.h>
#include <smooth/xml/xul/renderer.h>
#include <smooth/misc/args.h>
#include <smooth/init.h>

#ifdef __WIN32__
#	include <smooth/init.win32.h>

	void WINAPI S::GUI::LoadXUL(HWND hWnd, HINSTANCE shInstance, LPSTR szCmdLine, int iCmdShow)
	{
		hInstance = shInstance;

		/* Parse command line arguments.
		 */
		ArgumentsParser	 args(GetCommandLineW());

		String		 command   = args.GetCommand();
		Array<String>	 arguments = args.GetArguments();

		if (arguments.GetFirst().EndsWith(".dll,LoadXUL"))
		{
			command = command.Append(" ").Append(arguments.GetFirst());
			arguments.RemoveNth(0);
		}

		/* Init application and start XUL loader.
		 */
		Application::SetCommand(command);
		Application::SetArguments(arguments);

		Application::GetStartupDirectory();
		Application::GetApplicationDirectory();

		Init();

		XULLoader	*loader = new XULLoader(arguments.GetLast());

		loader->Loop();

		Object::DeleteObject(loader);

		Free();
	}
#endif

S::GUI::XULLoader::XULLoader(const String &xulFile)
{
	xulRenderer = new XML::XUL::Renderer();

	if (xulRenderer->LoadXUL(xulFile) == Success()) Add(xulRenderer->GetWidget());
}

S::GUI::XULLoader::~XULLoader()
{
	delete xulRenderer;
}
