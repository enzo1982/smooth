 /* LiSA - Library for System Adaptation
  * Copyright (C) 2002-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public
  * License along with this library; if not, write to the Free
  * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  * MA 02111-1307, USA */

#ifndef _H_LISA_
#define _H_LISA_

#include <wchar.h>

/* LiSAInit - Initializes LiSA ***********************************
 *								 *
 * Syntax:	void LiSAInit();				 *
 *								 *
 * Explanation:	LiSAInit must be called before any other LiSA	 *
 *		function can be called.				 *
 *								 *
 *****************************************************************/

#define LInit LiSAInit

void LiSAInit();

/* LiSADeinit - Deinitializes LiSA *******************************
 *								 *
 * Syntax:	void LiSADeinit();				 *
 *								 *
 * Explanation:	LiSADeinit should be called as the last LiSA	 *
 *		function before your program quits.		 *
 *								 *
 *****************************************************************/

#define LDeinit LiSADeinit

void LiSADeinit();

/* LiSASleep - Sleep for a given time ****************************
 *								 *
 * Syntax:	void LiSASleep(int mSeconds);			 *
 *								 *
 * Explanation:	LiSASleep suspends the execution of your program *
 *		for the given amount of milliseconds.		 *
 *								 *
 *****************************************************************/

#define LSleep LiSASleep

void LiSASleep(int);

/* LiSASetMouseCursor - Set the mouse cursor picture *********************
 *									 *
 * Syntax:	int LiSASetMouseCursor(LiSAWindow *, int mouseCursor);	 *
 *									 *
 * Explanation:	LiSASetMouseCursor changes the picture used as		 *
 *		your mouse cursor.					 *
 *									 *
 * mouseCursor:	The mouse cursor ID. One of the following:		 *
 *									 *
 *		LiSA_MOUSE_ARROW	the standard arrow		 *
 *		LiSA_MOUSE_TEXTEDIT	the cursor used for		 *
 *					marking text etc.		 *
 *		LiSA_MOUSE_HAND		the hand cursor used for	 *
 *					hyperlinks			 *
 *		LiSA_MOUSE_HSIZE	a horizontal resizer		 *
 *		LiSA_MOUSE_VSIZE	a vertical resizer		 *
 *									 *
 * Return:	LiSASetMouseCursor returns 0 if everything goes		 *
 *		well. A positive integer is returned in case of		 *
 *		an error.						 *
 *									 *
 *************************************************************************/

typedef void LiSAWindow;

#define LSetMouseCursor LiSASetMouseCursor

int LiSASetMouseCursor(LiSAWindow *, int);

const int	 LiSA_MOUSE_ARROW	= 0;
const int	 LiSA_MOUSE_TEXTEDIT	= 1;
const int	 LiSA_MOUSE_HAND	= 2;
const int	 LiSA_MOUSE_HSIZE	= 3;
const int	 LiSA_MOUSE_VSIZE	= 4;

/* LiSAOpenURL - Open a URL in a browser *************************
 *								 *
 * Syntax:	int LiSAOpenURL(char *url);			 *
 *		int LiSAOpenURL(wchar_t *url);			 *
 *								 *
 * Explanation:	LiSAOpenURL opens the given URL in the systems	 *
 *		default web browser.				 *
 *								 *
 * Return:	LiSAOpenURL returns 0 if everything goes well. A *
 *		positive integer is returned in case of an error.*
 *								 *
 *****************************************************************/

int LiSAOpenURLA(char *);
int LiSAOpenURLW(wchar_t *);

#if defined UNICODE
#define LOpenURL LiSAOpenURLW
#define LiSAOpenURL LiSAOpenURLW
#else
#define LOpenURL LiSAOpenURLA
#define LiSAOpenURL LiSAOpenURLA
#endif

#define LGetDisplaySizeX LiSAGetDisplaySizeX

int LiSAGetDisplaySizeX();

#define LGetDisplaySizeY LiSAGetDisplaySizeY

int LiSAGetDisplaySizeY();

#endif
