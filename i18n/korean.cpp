 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_KOREAN_
#define __OBJSMOOTH_KOREAN_

#include <smooth/i18n/korean.h>
#include <smooth/i18n.h>

void ActivateKoreanLanguage()
{
	TXT_ERROR = _TEXT("오류");

	TXT_OK = _TEXT("예");
	TXT_CANCEL = _TEXT("취소");
	TXT_YES = _TEXT("예");
	TXT_NO = _TEXT("아니오");
	TXT_RETRY = _TEXT("복원");
	TXT_ABORT = _TEXT("중지");
	TXT_IGNORE = _TEXT("┰�+");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH 작동");

	TXT_COLORSELECTION = _TEXT("색상 선택");

	TXT_OPENFILE = _TEXT("파일 열기");
	TXT_SAVEFILE = _TEXT("파일 저장");
	TXT_SAVEFILEAS = _TEXT("다른 이름으로 저장");

	TXT_SELECTDIR = _TEXT("새 디렉토리:");
	TXT_SELECTFONT = _TEXT("글꼴 선택");
}

#endif
