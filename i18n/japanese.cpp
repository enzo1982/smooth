 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_JAPANESE_
#define __OBJSMOOTH_JAPANESE_

#include <smooth/i18n/japanese.h>
#include <smooth/i18n.h>

void ActivateJapaneseLanguage()
{
	I18N_DEFAULTFONT = "MS PGothic";

	I18N_DEFAULTFONTSIZE = 10;
	I18N_SMALLFONTSIZE = 10;

	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "エラー";

	TXT_OK = "はい";
	TXT_CANCEL = "キャンセル";
	TXT_YES = "はい";
	TXT_NO = "いいえ";
	TXT_RETRY = "再試行";
	TXT_ABORT = "中止";
	TXT_IGNORE = "無視";

	TXT_SMOOTHAPPLICATION = "SMOOTH アプリケーション";
	TXT_LAYER = "レイヤ";

	TXT_COLORSELECTION = "色を選択";
	TXT_REDSHORT = "赤";
	TXT_GREENSHORT = "緑";
	TXT_BLUESHORT = "青";
	TXT_HUESHORT = "色";
	TXT_SATURATIONSHORT = "彩";
	TXT_VALUESHORT = "値";

	TXT_OPENFILE = "ファイルを開く";
	TXT_SAVEFILE = "ファイルを保存";
	TXT_SAVEFILEAS = "名前を付けて保存";

	TXT_SELECTDIR = "ディレクトリを選択:";
	TXT_SELECTFONT = "フォントを選択";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
