 /* PCIFF library
  * Copyright (C) 1997-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_LIBPCI_COMPRESS_
#define _H_LIBPCI_COMPRESS_

#include <pciio.h>

bool	 CompressPCI(PCIOut, PCIIO &);
bool	 DecompressPCI(PCIIn, PCIIO &);
bool	 WriteLine(PCIOut, PCIIO &, int);
bool	 ReadLine(PCIIn, PCIIO &, int);
int	 GetSLAL(int [], int [], int);
int	 DecompressSLAL(int [], int [], int);
int	 GetRLE(int [], int, int, int);
int	 GetVRLE(int [], int [], int, int, int);
int	 ProbeComp(int, int, int [], int [], int, int);
int	 ProbeCompNVRLE(int, int, int [], int [], int, int);
int	 GetSAA(int [], int [], int);
int	 GetDelta(int [], int [], int, int);
int	 CompressDelta(int [], int [], int, int, PCIOut);
int	 DecompressDelta(int [], int [], int, int, PCIIn);
int	 GetMinimumBits(int);
int	 GetMinimumTriples(int);
int	 GetMinimumDecades(int);
int	 RotatePalette(int);
int	 RotatePaletteEntry(int);
int	 GetPaletteEntry(int);

#endif
