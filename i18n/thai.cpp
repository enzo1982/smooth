 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_THAI_
#define __OBJSMOOTH_THAI_

#include <smooth/i18n/thai.h>
#include <smooth/i18n.h>

void ActivateThaiLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "ข้อผิดพลาด";

	TXT_OK = "ตกลง";
	TXT_CANCEL = "ยกเลิก";
	TXT_YES = "ใช่";
	TXT_NO = "ไม่";
	TXT_ABORT = "ยกเลิก";
	TXT_IGNORE = "ไม่สนใจ";

	TXT_SMOOTHAPPLICATION = "SMOOTH กิจกรรม";

	TXT_COLORSELECTION = "เลือกสี";

	TXT_OPENFILE = "เปิด ไฟล์";
	TXT_SAVEFILE = "บันทึกไฟล์ข้อมูล";
	TXT_SAVEFILEAS = "บันทึกในชื่อ";

	TXT_SELECTDIR = "ไดเร็กทอรีใหม่:";
	TXT_SELECTFONT = "เลือกฟ้อนท์";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
