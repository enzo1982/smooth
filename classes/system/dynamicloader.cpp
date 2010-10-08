 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/dynamicloader.h>
#include <smooth/gui/application/application.h>

#ifndef __WIN32__
#	include <dlfcn.h>
#endif

const S::Short	 S::System::DynamicLoader::classID = S::Object::RequestClassID();

S::System::DynamicLoader::DynamicLoader(const String &module)
{
#ifdef __WIN32__
	if (Setup::enableUnicode)	handle = LoadLibraryW(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(".dll") ? "" : ".dll"));
	else				handle = LoadLibraryA(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(".dll") ? "" : ".dll"));

	if (handle == NIL)
	{
		if (Setup::enableUnicode)	handle = LoadLibraryW(String(module).Append(module.EndsWith(".dll") ? "" : ".dll"));
		else				handle = LoadLibraryA(String(module).Append(module.EndsWith(".dll") ? "" : ".dll"));
	}
#else
	handle = dlopen(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(".so") ? "" : ".so"), RTLD_NOW | RTLD_GLOBAL);

	if (handle == NIL)
	{
		handle = dlopen(String(module).Append(module.EndsWith(".so") ? "" : ".so"), RTLD_NOW | RTLD_GLOBAL);
	}
#endif
}

S::System::DynamicLoader::~DynamicLoader()
{
#ifdef __WIN32__
	if (handle != NIL) FreeLibrary(handle);
#else
	if (handle != NIL) dlclose(handle);
#endif
}

S::Void *S::System::DynamicLoader::GetFunctionAddress(const String &functionName) const
{
	if (handle == NIL) return NIL;

#ifdef __WIN32__
	return (Void *) GetProcAddress(handle, functionName);
#else
	return (Void *) dlsym(handle, functionName);
#endif
}

S::Void *S::System::DynamicLoader::GetSystemModuleHandle() const
{
	return handle;
}
