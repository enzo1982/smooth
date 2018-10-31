 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/backend.h>
#include <smooth/templates/nonblocking.h>
#include <smooth/i18n/translator_internal.h>
#include <smooth/threads/thread.h>
#include <smooth/init.h>
#include <smooth/foreach.h>
#include <smooth/resources.h>

#if defined __WIN32__
#	include <smooth/init.win32.h>

#	include <smooth/backends/win32/backendwin32.h>
#endif

#include <iconv.h>

static S::Array<S::Int (*)(), S::Void *>	*initFunctions = NIL;
static S::Array<S::Int (*)(), S::Void *>	*freeFunctions = NIL;

#if defined __WIN32__
__declspec (dllexport) HINSTANCE	 S::hInstance	  = NIL;
__declspec (dllexport) HINSTANCE	 S::hPrevInstance = NIL;

__declspec (dllexport) HICON		 S::SMOOTHICON	  = NIL;
#endif

namespace smooth
{
	SMOOTHAPI S::Array<S::Caller *, Void *> S::NonBlocking::callers;
};

S::Bool	 S::initializing = S::True;

S::Int	 initCount = 0;

S::Int S::AddInitFunction(Int (*initFunction)())
{
	if (initFunction == NIL) return Error();

	if (initFunctions == NIL) initFunctions = new Array<Int (*)(), Void *>;

	initFunctions->Add(initFunction);

	return Success();
}

S::Int S::AddFreeFunction(Int (*freeFunction)())
{
	if (freeFunction == NIL) return Error();

	if (freeFunctions == NIL) freeFunctions = new Array<Int (*)(), Void *>;

	freeFunctions->Add(freeFunction);

	return Success();
}

S::Bool S::Init()
{
	if (initCount++) return True;

	/* Init system backends.
	 */
	if (Backends::Backend::InitBackends() == Error()) return False;

	/* Call registered init functions.
	 */
	foreach (Int (*initFunction)(), (*initFunctions))
	{
		if (initFunction() != Success())
		{
			/* Deinit system backends.
			 */
			Backends::Backend::DeinitBackends();

			return False;
		}
	}

#if defined __WIN32__
	if (hDllInstance == NIL) hDllInstance = hInstance;

	SMOOTHICON = (HICON) LoadImage(hDllInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	if (LoadIconvDLL() == True) Setup::useIconv = True;

	use_iconv = (Setup::useIconv ? 1 : 0);
#endif

	/* Init internationalization system.
	 */
	I18n::Translator::defaultTranslator = new I18n::TranslatorInternal();
	I18n::Translator::defaultTranslator->SelectUserDefaultLanguage();

	Setup::rightToLeft = I18n::Translator::defaultTranslator->IsActiveLanguageRightToLeft();

	/* Finished initializing.
	 */
	initializing = False;

	/* Start waiting threads.
	 */
	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object	*object = Object::GetNthObject(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Threads::Thread::classID &&
		    ((Threads::Thread *) object)->GetStatus() == Threads::THREAD_STARTME) ((Threads::Thread *) object)->Start();
	}

	return True;
}

S::Void S::Free()
{
	/* Delete all remaining callers left
	 * from nonblocking function calls.
	 */
	NonBlocking::CleanUp();

	/* Call ObjectCleanup() to make sure other
	 * DLLs are unloaded before we continue.
	 */
	Object::ObjectCleanup();

	if (--initCount) return;

	/* Stop running threads.
	 */
	if (Threads::Thread::GetNOfRunningThreads() > 0)
	{
		for (Int i = 0; i < Object::GetNOfObjects(); i++)
		{
			Object	*object = Object::GetNthObject(i);

			if (object == NIL) continue;

			if (object->GetObjectType() == Threads::Thread::classID &&
			    !(object->GetFlags() & Threads::THREAD_KILLFLAG_WAIT)) ((Threads::Thread *) object)->Stop();
		}
	}

	/* Wait for remaining threads to finish.
	 */
	while (Threads::Thread::GetNOfRunningThreads() > 0) System::System::Sleep(10);

	/* Delete translator object and free iconv library.
	 */
	delete I18n::Translator::defaultTranslator;

#if defined __WIN32__
	if (Setup::useIconv) FreeIconvDLL();
#endif

	/* Call registered free functions.
	 */
	foreach (Int (*freeFunction)(), (*freeFunctions))
	{
		freeFunction();
	}

	initFunctions->RemoveAll();
	freeFunctions->RemoveAll();

	delete initFunctions;
	delete freeFunctions;

	/* Deinit system backends.
	 */
	Backends::Backend::DeinitBackends();

	/* All pending objects should be marked for
	 * deletion now, so let's call ObjectCleanup()
	 * again to actually free them.
	 */
	Object::ObjectCleanup();
}
