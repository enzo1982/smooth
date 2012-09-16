 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <Pt.h>

#include <smooth/backends/photon/backendphoton.h>

S::Backends::Backend *CreateBackendPhoton()
{
	return new S::Backends::BackendPhoton();
}

S::Int	 backendPhotonTmp = S::Backends::Backend::AddBackend(&CreateBackendPhoton);

S::Backends::BackendPhoton::BackendPhoton()
{
	type = BACKEND_PHOTON;
}

S::Backends::BackendPhoton::~BackendPhoton()
{
}

S::Int S::Backends::BackendPhoton::Init()
{
	/* Init the Photon API.
	 */
	PtInit(NULL);

	return Success();
}
