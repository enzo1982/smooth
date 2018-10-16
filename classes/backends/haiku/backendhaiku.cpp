 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
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
	return Success();
}

S::Int S::Backends::BackendHaiku::Deinit()
{
	return Success();
}

BApplication *S::Backends::BackendHaiku::GetApplication()
{
	static HaikuApplication	 app;

	return be_app;
}
