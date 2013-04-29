 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/dynamicloader.h>
#include <smooth/files/directory.h>
#include <smooth/io/instream.h>
#include <smooth/gui/application/application.h>
#include <smooth/foreach.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <dlfcn.h>
#endif

const S::Short	 S::System::DynamicLoader::classID = S::Object::RequestClassID();

S::System::DynamicLoader::DynamicLoader(const String &module) : handle(NIL)
{
#if defined __WIN32__
	static String	 dllExt		= ".dll";
	static String	 versionPattern	= dllExt;
#elif defined __APPLE__
	static String	 dllExt		= ".dylib";
	static String	 versionPattern	= String(".*").Append(dllExt);
#else
	static String	 dllExt		= ".so";
	static String	 versionPattern	= String(dllExt).Append(".*");
#endif

#ifdef __WIN32__
	if (module[1] != ':')
	{
		/* Try the supplied module name in application directory.
		 */
		if (Setup::enableUnicode) handle = LoadLibraryW(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
		else			  handle = LoadLibraryA(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
	}

	if (handle == NIL)
	{
		/* Try the supplied module name system wide.
		 */
		if (Setup::enableUnicode) handle = LoadLibraryW(String(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
		else			  handle = LoadLibraryA(String(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
	}
#else
	Int	 dlopenFlags = RTLD_NOW | RTLD_LOCAL;

#if defined __FreeBSD__ || defined __NetBSD__
	dlopenFlags |= RTLD_NODELETE;
#endif

	if (!module.StartsWith("/"))
	{
		/* Try the supplied module name in application directory.
		 */
		handle = dlopen(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(dllExt) || module.Find(String(dllExt).Append(".")) >= 0 ? String() : dllExt), dlopenFlags);
	}

	if (handle == NIL)
	{
		/* Try the supplied module name system wide.
		 */
		handle = dlopen(String(module).Append(module.EndsWith(dllExt) || module.Find(String(dllExt).Append(".")) >= 0 ? String() : dllExt), dlopenFlags);
	}

	if (handle == NIL)
	{
		const Array<String> &directories = GetLibraryDirectories();

		/* Try loading an unversioned library.
		 */
		foreach (const String &path, directories)
		{
			Directory		 directory(path);
			const Array<File>	&files = directory.GetFilesByPattern(String(module.StartsWith("lib") || module.Find("/") >= 0 ? String() : "lib").Append(module).Append(module.EndsWith(dllExt) || module.Find(String(dllExt).Append(".")) >= 0 ? String() : dllExt));

			if (files.Length() > 0)
			{
				handle = dlopen((String) files.GetFirst(), dlopenFlags);
			}

			if (handle != NIL) return;
		}

		/* Try loading a versioned library.
		 */
		foreach (const String &path, directories)
		{
			Directory		 directory(path);
			const Array<File>	&files = directory.GetFilesByPattern(String(module.StartsWith("lib") || module.Find("/") >= 0 ? String() : "lib").Append(module).Append(module.EndsWith(dllExt) || module.Find(String(dllExt).Append(".")) >= 0 ? String() : versionPattern));

			if (files.Length() > 0)
			{
				handle = dlopen((String) files.GetFirst(), dlopenFlags);
			}

			if (handle != NIL) return;
		}
	}
#endif
}

S::System::DynamicLoader::~DynamicLoader()
{
#ifdef __WIN32__
	if (handle != NIL) FreeLibrary((HINSTANCE) handle);
#else
#ifdef __HAIKU__
	/* Haiku does not support RTLD_NODELETE yet, so we
	 * cannot close .so files without risking a crash.
	 */
#else
	if (handle != NIL) dlclose(handle);
#endif
#endif
}

S::Void *S::System::DynamicLoader::GetFunctionAddress(const String &functionName) const
{
	if (handle == NIL) return NIL;

#ifdef __WIN32__
	return (Void *) GetProcAddress((HINSTANCE) handle, functionName);
#else
	return (Void *) dlsym(handle, functionName);
#endif
}

S::Void *S::System::DynamicLoader::GetSystemModuleHandle() const
{
	return handle;
}

const S::Array<S::String> &S::System::DynamicLoader::GetLibraryDirectories()
{
	static Array<String>	 directories;

	if (directories.Length() == 0)
	{
		/* Look for /usr/lib and /usr/local/lib on all systems.
		 */
		if (Directory("/usr/lib").Exists())	    directories.Add("/usr/lib");
		if (Directory("/usr/local/lib").Exists())   directories.Add("/usr/local/lib");

#if defined __APPLE__
		/* Look for library directories of ports projects.
		 */
		if (Directory("/opt/local/lib").Exists())   directories.Add("/opt/local/lib");
		if (Directory("/sw/lib").Exists())	    directories.Add("/sw/lib");
#elif defined __HAIKU__
		/* Haiku puts libraries into /boot/common/lib.
		 */
							    directories.Add("/boot/common/lib");
#elif defined __NetBSD__
		/* Packages live in /usr/pkg on NetBSD.
		 */
		if (Directory("/usr/pkg/lib").Exists())	    directories.Add("/usr/pkg/lib");
#else
		/* Take care of lib32 and lib64 directories on multilib systems.
		 */
#if defined __x86_64__
		if (Directory("/usr/lib64").Exists())	    directories.Add("/usr/lib64");
		if (Directory("/usr/local/lib64").Exists()) directories.Add("/usr/local/lib64");
#elif defined __i386__
		if (Directory("/usr/lib32").Exists())	    directories.Add("/usr/lib32");
		if (Directory("/usr/local/lib32").Exists()) directories.Add("/usr/local/lib32");
#endif
		/* Parse /etc/ld.so.conf if it exists.
		 */
		if (File("/etc/ld.so.conf").Exists())	    ParseDirectoryList("/etc/ld.so.conf", directories);
#endif
	}

	return directories;
}

S::Void S::System::DynamicLoader::ParseDirectoryList(const String &pattern, Array<String> &directories)
{
	Directory		 directory(File(pattern).GetFilePath());
	const Array<File>	&files = directory.GetFilesByPattern(File(pattern).GetFileName());

	foreach (const File &file, files)
	{
		IO::InStream	 in(IO::STREAM_FILE, file);

		while (in.GetPos() < in.Size())
		{
			String	 line = in.InputLine();

			if	(line == NIL)			continue;
			else if (line.StartsWith("#"))		continue;
			else if (line.StartsWith("include "))	ParseDirectoryList(line.Tail(line.Length() - 8), directories);
			else					directories.Add(line);
		}
	}
}
