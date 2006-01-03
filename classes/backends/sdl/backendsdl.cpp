 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/sdl/backendsdl.h>

#include <SDL/SDL.h>

S::Backends::Backend *CreateBackendSDL()
{
	return new S::Backends::BackendSDL();
}

S::Int	 backendSDLTmp = S::Backends::Backend::AddBackend(&CreateBackendSDL);

S::Backends::BackendSDL::BackendSDL()
{
	type = BACKEND_SDL;
}

S::Backends::BackendSDL::~BackendSDL()
{
}

S::Int S::Backends::BackendSDL::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)	return Success();
	else					return Error();
}

S::Int S::Backends::BackendSDL::Deinit()
{
	SDL_Quit();

	return Success();
}
