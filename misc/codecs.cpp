 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/codecs.h>
#include <smooth/graphics/color.h>
#include <smooth/pciio.h>
#include <smooth/misc/math.h>
#include <smooth/io/filters/filter_bzip2.h>

using namespace smooth;
using namespace smooth::GUI;

int	*lastline;
int	 palette[388];
int	 palentries = 0;
int	 bits = 0;
int	 bitssaa = 0;
int	 bitsnrle = 0;
int	 bitssaanrle = 0;

bool CompressPCI(PCIOut out, PCIIO &ior)
{
	IO::FilterBZip2	*filter = NULL;

	palentries = 0;
	bits = 0;
	bitssaa = 0;
	bitsnrle = 0;
	bitssaanrle = 0;
	lastline = new int [ior.sizex];

	for (int x1 = 0; x1 < ior.sizex; x1++) lastline[x1] = 0;
	for (int x2 = 0; x2 < 338; x2++) palette[x2] = 0;

	if (ior.compression == BZIP2)
	{
		filter = new IO::FilterBZip2();

		out->AddFilter(filter);
	}

	for (int y = 0; y < ior.sizey; y++)
	{
		WriteLine(out, ior, y);
	}

	if (ior.compression == BZIP2)
	{
		out->RemoveFilter(filter);

		delete filter;
	}

	delete lastline;

	return true;
}

bool DecompressPCI(PCIIn in, PCIIO &ior)
{
	IO::FilterBZip2	*filter = NULL;

	palentries = 0;
	bits = 0;
	bitssaa = 0;
	bitsnrle = 0;
	bitssaanrle = 0;
	lastline = new int [ior.sizex];

	for (int x1 = 0; x1 < ior.sizex; x1++) lastline[x1] = 0;
	for (int x2 = 0; x2 < 338; x2++) palette[x2] = 0;

	if (ior.compression == BZIP2)
	{
		filter = new IO::FilterBZip2();

		in->AddFilter(filter);
	}

	for (int y = 0; y < ior.sizey; y++)
	{
		ReadLine(in, ior, y);
	}

	if (ior.compression == BZIP2)
	{
		in->RemoveFilter(filter);

		delete filter;
	}

	delete lastline;

	return true;
}

bool WriteLine(PCIOut out, PCIIO &ior, int y)
{
	int	*line = new int [ior.sizex];
	int	 rle;
	int	 ctsize;
	int	 dorle = 0;
	int	 dosaa = 0;
	int	 lastcol;
	int	 pcirle;
	int	 allinpal = 0;
	int	 gallinpal = 0;
	int	 lbits;
	int	 mod04 = 0;
	int	 mod16 = 0;
	int	 maxpalentries;
	int	 x;

	switch (ior.colorspace)
	{
		case RGBA:
		case YUV:
		case HSV:
		case CMY:
			ctsize = 3;
			maxpalentries = 338;
			break;
		case CMYK:
			ctsize = 4;
			maxpalentries = 338;
			break;
		case GRAY:
			ctsize = 1;
			maxpalentries = 338;
			break;
		default:
			ctsize = 3;
			maxpalentries = 338;
			break;
	}

	for (x = 0; x < ior.sizex; x++)
	{
		line[x] = Color(ior.bmp.GetPixel(x, y)).ConvertTo(ior.colorspace).Downsample(ior.bpcc);
	}

	switch (ior.compression)
	{
		case BZIP2:
		case UNCOMPRESSED:
			for (x = 0; x < ior.sizex; x++)
			{
				out->OutputNumberPBD(line[x], ctsize * ior.bpcc);
			}
			break;
		case RLE:
			for (x = 0; x < ior.sizex; x++)
			{
				out->OutputNumberPBD(line[x], ctsize * ior.bpcc);
				rle = GetRLE(line, ior.sizex, x, ior.rlebits);
				out->OutputNumberPBD(rle, ior.rlebits);
				x += rle;
			}
			break;
		case PCI:
			if (GetSLAL(line, lastline, ior.sizex))
			{
				out->OutputNumberPBD(1, 1);
			}
			else
			{
				out->OutputNumberPBD(0, 1);

				ProbeComp(0, ior.sizex, line, lastline, ior.sizex, ctsize, false, bits, bitssaa);
				ProbeComp(0, ior.sizex, line, lastline, ior.sizex, ctsize, true, bitsnrle, bitssaanrle);

				if (bitssaa < bits)
				{
					out->OutputNumberPBD(1, 1);
					dosaa = 1;
					if (bitssaa < bitssaanrle)
					{
						out->OutputNumberPBD(1, 1);
						dorle = 1;
					}
					else
					{
						out->OutputNumberPBD(0, 1);
						dorle = 0;
					}
				}
				else
				{
					out->OutputNumberPBD(0, 1);
					dosaa = 0;
					if (bits < bitsnrle)
					{
						out->OutputNumberPBD(1, 1);
						dorle = 1;
					}
					else
					{
						out->OutputNumberPBD(0, 1);
						dorle = 0;
					}
				}

				for (x = 0; x < ior.sizex; x++)
				{
					if (x == 0)	lastcol = 0;
					else		lastcol = line[x-1];

					if ((x % 16) == mod16)
					{
						gallinpal = 1;
						for (int x2 = 0; x2 < 16; x2++)
						{
							if ((x+x2) < ior.sizex) if (GetPaletteEntry(line[x+x2]) == -1 )	{ gallinpal = 0; break; }
						}
						if (gallinpal == 1)	out->OutputNumberPBD(1, 1);
						else			out->OutputNumberPBD(0, 1);
					}
					if (((x % 4) == mod04) && !gallinpal)
					{
						allinpal = 1;
						for (int x2 = 0; x2 < 4; x2++)
						{
							if ((x+x2) < ior.sizex) if (GetPaletteEntry(line[x+x2]) == -1 )	{ allinpal = 0; break; }
						}
						if (allinpal == 1)	out->OutputNumberPBD(1, 1);
						else			out->OutputNumberPBD(0, 1);
					}

					if ((GetPaletteEntry(line[x]) == -1))
					{
						out->OutputNumberPBD(0, 1);
						if (GetDelta(line, lastline, x, ctsize))
						{
							out->OutputNumberPBD(1, 1);
							CompressDelta(line, lastline, x, ctsize, out);
						}
						else
						{
							out->OutputNumberPBD(0, 1);
							out->OutputNumberPBD(line[x], ctsize*ior.bpcc);
						}
						if (palentries < maxpalentries)
						{
							palette[palentries] = line[x];
							palentries++;
						}
						else
						{
							RotatePalette(line[x]);
						}
					}
					else
					{
						if (!allinpal && !gallinpal) out->OutputNumberPBD(1, 1);
						if (GetSAA(line, lastline, x) && dosaa)
						{
							out->OutputNumberPBD(1, 1);
						}
						else
						{
							if (dosaa) out->OutputNumberPBD(0, 1);
							if ((palentries-1)-GetPaletteEntry(line[x]) < 2)
							{
								out->OutputNumberPBD(1, 1);
								out->OutputNumberPBD((palentries-1)-GetPaletteEntry(line[x]), 1);
							}
							else
							{
								out->OutputNumberPBD(0, 1);
								if ((palentries-1)-GetPaletteEntry(line[x]) < 18)
								{
									out->OutputNumberPBD(1, 1);
									lbits = GetMinimumBits(palentries-2);
									if (lbits > 4) lbits = 4;
									out->OutputNumberPBD((palentries-1)-GetPaletteEntry(line[x])-2, lbits);
								}
								else
								{
									out->OutputNumberPBD(0, 1);
									if ((palentries-1)-GetPaletteEntry(line[x]) < 82)
									{
										out->OutputNumberPBD(1, 1);
										lbits = GetMinimumBits(palentries-18);
										if (lbits > 6) lbits = 6;
										out->OutputNumberPBD((palentries-1)-GetPaletteEntry(line[x])-18, lbits);
									}
									else
									{
										out->OutputNumberPBD(0, 1);
										out->OutputNumberPBD((palentries-1)-GetPaletteEntry(line[x])-82, GetMinimumBits(palentries-83));
									}
								}
							}
						}
						RotatePaletteEntry(GetPaletteEntry(line[x]));
					}

					if (line[x] == lastcol)
					{
						pcirle = GetRLE(line, ior.sizex, x, 8);
						if (pcirle >= 2)
						{
							out->OutputNumberPBD(1, 1);
							if (pcirle > 9)
							{
								out->OutputNumberPBD(1, 1);
								out->OutputNumberPBD(pcirle - 2, 8);
							}
							else
							{
								out->OutputNumberPBD(0, 1);
								out->OutputNumberPBD(pcirle - 2, 3);
							}
							x = x + pcirle;
							allinpal = 0;
							gallinpal = 0;
							mod04 = (x + 1) %  4;
							mod16 = (x + 1) % 16;
						}
						else
						{
							out->OutputNumberPBD(0, 1);
						}
					}
					else if ((line[x] == lastline[x]) && dorle)
					{
						pcirle = GetVRLE(line, lastline, ior.sizex, x, 8);
						if (pcirle >= 2)
						{
							out->OutputNumberPBD(1, 1);
							if (pcirle > 9)
							{
								out->OutputNumberPBD(1, 1);
								out->OutputNumberPBD(pcirle - 2, 8);
							}
							else
							{
								out->OutputNumberPBD(0, 1);
								out->OutputNumberPBD(pcirle - 2, 3);
							}
							x = x + pcirle;
							allinpal = 0;
							gallinpal = 0;
							mod04 = (x + 1) %  4;
							mod16 = (x + 1) % 16;
						}
						else
						{
							out->OutputNumberPBD(0, 1);
						}
					}
				}
			}
			for (x = 0; x < ior.sizex; x++) lastline[x] = line[x];
			break;
		default:
			break;
	}

	delete [] line;

	return true;
}

bool ReadLine(PCIIn in, PCIIO &ior, int y)
{
	int	*line = new int [ior.sizex];
	int	 rle;
	int	 col;
	int	 ctsize;
	int	 dorle = 0;
	int	 dosaa = 0;
	int	 lastcol;
	int	 pcirle;
	int	 inpal;
	int	 lbits;
	int	 saa;
	int	 allinpal = 0;
	int	 gallinpal = 0;
	int	 mod04 = 0;
	int	 mod16 = 0;
	int	 maxpalentries;
	int	 x;

	switch (ior.colorspace)
	{
		case RGBA:
		case YUV:
		case HSV:
		case CMY:
			ctsize = 3;
			maxpalentries = 338;
			break;
		case CMYK:
			ctsize = 4;
			maxpalentries = 338;
			break;
		case GRAY:
			ctsize = 1;
			maxpalentries = 338;
			break;
		default:
			ctsize = 3;
			maxpalentries = 338;
			break;
	}

	switch (ior.compression)
	{
		case BZIP2:
		case UNCOMPRESSED:
			for (x = 0; x < ior.sizex; x++)
			{
				line[x] = in->InputNumberPBD(ctsize * ior.bpcc);
			}
			break;
		case RLE:
			for (x = 0; x < ior.sizex; x++)
			{
				col = in->InputNumberPBD(ctsize * ior.bpcc);
				rle = in->InputNumberPBD(ior.rlebits);

				for (int i = x; i <= x + rle; i++)
				{
					line[i] = col;
				}

				x += rle;
			}
			break;
		case PCI:
			if (in->InputNumberPBD(1) == 1)
			{
				DecompressSLAL(line, lastline, ior.sizex);
			}
			else
			{
				dosaa = in->InputNumberPBD(1);
				dorle = in->InputNumberPBD(1);

				for (x = 0; x < ior.sizex; x++)
				{
					if (x == 0)	lastcol = 0;
					else		lastcol = line[x-1];

					if ((x % 16) == mod16)
					{
						gallinpal = in->InputNumberPBD(1);
					}
					if (((x % 4) == mod04) && !gallinpal)
					{
						allinpal = in->InputNumberPBD(1);
					}

					if (allinpal || gallinpal)	inpal = 1;
					else
				inpal = in->InputNumberPBD(1);

					if (inpal == 0)
					{
						if (in->InputNumberPBD(1))
						{
							col = DecompressDelta(line, lastline, x, ctsize, in);
						}
						else
						{
							col = in->InputNumberPBD(ctsize*ior.bpcc);
						}

						if (palentries < maxpalentries)
						{
							palette[palentries] = col;
							palentries++;
						}
						else
						{
							RotatePalette(col);
						}
					}
					else
					{
						if (dosaa)	saa = in->InputNumberPBD(1);
						else		saa = 0;

						if (saa)
						{
							col = lastline[x];
						}
						else
						{
							if (in->InputNumberPBD(1) == 1)
							{
								col = palette[(palentries-1)-in->InputNumberPBD(1)];
							}
							else
							{
								if (in->InputNumberPBD(1) == 1)
								{
									lbits = GetMinimumBits(palentries-2);
									if (lbits > 4) lbits = 4;
									col = palette[(palentries-1)-in->InputNumberPBD(lbits)-2];
								}
								else
								{
									if (in->InputNumberPBD(1) == 1)
									{
										lbits = GetMinimumBits(palentries-18);
										if (lbits > 6) lbits = 6;
										col = palette[(palentries-1)-in->InputNumberPBD(lbits)-18];
									}
									else
									{
										col = palette[(palentries-1)-in->InputNumberPBD(GetMinimumBits(palentries-83))-82];
									}
								}
							}
						}
						RotatePaletteEntry(GetPaletteEntry(col));
					}

					line[x] = col;

					if (col == lastcol)
					{
						if (in->InputNumberPBD(1) == 1)
						{
							if (in->InputNumberPBD(1) == 1)
							{
								pcirle = in->InputNumberPBD(8) + 2;
							}
							else
							{
								pcirle = in->InputNumberPBD(3) + 2;
							}
							for (int i = x + 1; i <= x + pcirle; i++)
							{
								line[i] = line[x];
							}
							x = x + pcirle;
							allinpal = 0;
							gallinpal = 0;
							mod04 = (x + 1) %  4;
							mod16 = (x + 1) % 16;
						}
					}
					else if ((col == lastline[x]) && dorle)
					{
						if (in->InputNumberPBD(1) == 1)
						{
							if (in->InputNumberPBD(1) == 1)
							{
								pcirle = in->InputNumberPBD(8) + 2;
							}
							else
							{
								pcirle = in->InputNumberPBD(3) + 2;
							}
							for (int i = x + 1; i <= x + pcirle; i++)
							{
								line[i] = lastline[i];
							}
							x = x + pcirle;
							allinpal = 0;
							gallinpal = 0;
							mod04 = (x + 1) %  4;
							mod16 = (x + 1) % 16;
						}
					}
				}
			}
			for (x = 0; x < ior.sizex; x++) lastline[x] = line[x];
			break;
		default:
			break;
	}

	for (x = 0; x < ior.sizex; x++)
	{
		ior.bmp.SetPixel(x, y, Color(line[x], ior.colorspace).Upsample(ior.bpcc).ConvertTo(RGBA));
	}

	delete [] line;

	return true;
}


int GetSLAL(int line[], int prevline[], int sx)
{
	for (int x = 0; x < sx; x++)
	{
		if (line[x] != prevline[x]) return 0;
	}

	return 1;
}

int DecompressSLAL(int line[], int prevline[], int sx)
{
	for (int x = 0; x < sx; x++)
	{
		line[x] = prevline[x];
	}

	return 1;
}

int GetRLE(int line[], int sx, int x, int maxbits)
{
	if (x == sx - 1) return 0;

	int	 rle = 0;

	for (int i = x + 1; i < x + Math::Pow(2l, maxbits); i++)
	{
		if (line[i] == line[x]) rle++;
		else break;
		if (i == sx - 1) break;
	}

	return rle;
}

int GetVRLE(int line[], int prevline[], int sx, int x, int maxbits)
{
	if (x == sx - 1) return 0;

	int	 rle = 0;

	for (int i = x + 1; i < x + Math::Pow(2l, maxbits); i++)
	{
		if (line[i] == prevline[i]) rle++;
		else break;
		if (i == sx - 1) break;
	}

	return rle;
}

int GetSAA(int line[], int prevline[], int x)
{
	if (line[x] == prevline[x]) return 1;
	return 0;
}

int GetDelta(int line[], int prevline[], int x, int parts)
{
	int pixb1;

	if (x == 0) pixb1 = 0;
	else pixb1 = line[x-1];

	int	 bias;
	int	 redbias;
	int	 greenbias;
	int	 bluebias;
	int	 alphabias;

	switch (parts)
	{
		case 1:
			bias = line[x]-(pixb1+prevline[x])/2;

			if ((bias > -Math::Pow(2l, 3)) && (bias <= Math::Pow(2l, 3)))
			{
				return 1;
			}

			break;
		case 3:
			redbias = Color(line[x]).GetRed()-(Color(pixb1).GetRed()+Color(prevline[x]).GetRed())/2;
			greenbias = Color(line[x]).GetGreen()-(Color(pixb1).GetGreen()+Color(prevline[x]).GetGreen())/2;
			bluebias = Color(line[x]).GetBlue()-(Color(pixb1).GetBlue()+Color(prevline[x]).GetBlue())/2;

			if (((redbias > -Math::Pow(2l, 5)) && (redbias <= Math::Pow(2l, 5))) && ((greenbias > -Math::Pow(2l, 5)) && (greenbias <= Math::Pow(2l, 5))) && ((bluebias > -Math::Pow(2l, 5)) && (bluebias <= Math::Pow(2l, 5))))
			{
				return 1;
			}

			break;
		case 4:
			redbias = Color(line[x]).GetRed()-(Color(pixb1).GetRed()+Color(prevline[x]).GetRed())/2;
			greenbias = Color(line[x]).GetGreen()-(Color(pixb1).GetGreen()+Color(prevline[x]).GetGreen())/2;
			bluebias = Color(line[x]).GetBlue()-(Color(pixb1).GetBlue()+Color(prevline[x]).GetBlue())/2;
			alphabias = Color(line[x]).GetAlpha()-(Color(pixb1).GetAlpha()+Color(prevline[x]).GetAlpha())/2;

			if (((redbias > -Math::Pow(2l, 5)) && (redbias <= Math::Pow(2l, 5))) && ((greenbias > -Math::Pow(2l, 5)) && (greenbias <= Math::Pow(2l, 5))) && ((bluebias > -Math::Pow(2l, 5)) && (bluebias <= Math::Pow(2l, 5))) && ((alphabias > -Math::Pow(2l, 5)) && (alphabias <= Math::Pow(2l, 5))))
			{
				return 1;
			}

			break;
	}

	return 0;
}

int CompressDelta(int line[], int prevline[], int x, int parts, PCIOut out)
{
	int pixb1;

	if (x == 0) pixb1 = 0;
	else pixb1 = line[x-1];

	int	 md = 0;
	int	 bias;
	int	 redbias;
	int	 greenbias;
	int	 bluebias;
	int	 alphabias;

	switch (parts)
	{
		case 1:
			bias = line[x]-(pixb1+prevline[x])/2;

			if (bias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0-bias, 3);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(bias-1, 3);
			}

			break;
		case 3:
			redbias = Color(line[x]).GetRed()-(Color(pixb1).GetRed()+Color(prevline[x]).GetRed())/2;
			greenbias = Color(line[x]).GetGreen()-(Color(pixb1).GetGreen()+Color(prevline[x]).GetGreen())/2;
			bluebias = Color(line[x]).GetBlue()-(Color(pixb1).GetBlue()+Color(prevline[x]).GetBlue())/2;

			if (((redbias > -Math::Pow(2l, 2)) && (redbias <= Math::Pow(2l, 2))) && ((greenbias > -Math::Pow(2l, 2)) && (greenbias <= Math::Pow(2l, 2))) && ((bluebias > -Math::Pow(2l, 2)) && (bluebias <= Math::Pow(2l, 2))))
			{
				out->OutputNumberPBD(0, 2);
				md = 2;
			}
			else if (((redbias > -Math::Pow(2l, 3)) && (redbias <= Math::Pow(2l, 3))) && ((greenbias > -Math::Pow(2l, 3)) && (greenbias <= Math::Pow(2l, 3))) && ((bluebias > -Math::Pow(2l, 3)) && (bluebias <= Math::Pow(2l, 3))))
			{
				out->OutputNumberPBD(1, 2);
				md = 3;
			}
			else if (((redbias > -Math::Pow(2l, 4)) && (redbias <= Math::Pow(2l, 4))) && ((greenbias > -Math::Pow(2l, 4)) && (greenbias <= Math::Pow(2l, 4))) && ((bluebias > -Math::Pow(2l, 4)) && (bluebias <= Math::Pow(2l, 4))))
			{
				out->OutputNumberPBD(2, 2);
				md = 4;
			}
			else if (((redbias > -Math::Pow(2l, 5)) && (redbias <= Math::Pow(2l, 5))) && ((greenbias > -Math::Pow(2l, 5)) && (greenbias <= Math::Pow(2l, 5))) && ((bluebias > -Math::Pow(2l, 5)) && (bluebias <= Math::Pow(2l, 5))))
			{
				out->OutputNumberPBD(3, 2);
				md = 5;
			}

			if (redbias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0-redbias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(redbias-1, md);
			}
			if (greenbias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0-greenbias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(greenbias-1, md);
			}
			if (bluebias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0-bluebias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(bluebias-1, md);
			}

			break;
		case 4:
			redbias = Color(line[x]).GetRed()-(Color(pixb1).GetRed()+Color(prevline[x]).GetRed())/2;
			greenbias = Color(line[x]).GetGreen()-(Color(pixb1).GetGreen()+Color(prevline[x]).GetGreen())/2;
			bluebias = Color(line[x]).GetBlue()-(Color(pixb1).GetBlue()+Color(prevline[x]).GetBlue())/2;
			alphabias = Color(line[x]).GetAlpha()-(Color(pixb1).GetAlpha()+Color(prevline[x]).GetAlpha())/2;

			if (((redbias > -Math::Pow(2l, 2)) && (redbias <= Math::Pow(2l, 2))) && ((greenbias > -Math::Pow(2l, 2)) && (greenbias <= Math::Pow(2l, 2))) && ((bluebias > -Math::Pow(2l, 2)) && (bluebias <= Math::Pow(2l, 2))) && ((alphabias > -Math::Pow(2l, 2)) && (alphabias <= Math::Pow(2l, 2))))
			{
				out->OutputNumberPBD(0, 2);
				md = 2;
			}
			else if (((redbias > -Math::Pow(2l, 3)) && (redbias <= Math::Pow(2l, 3))) && ((greenbias > -Math::Pow(2l, 3)) && (greenbias <= Math::Pow(2l, 3))) && ((bluebias > -Math::Pow(2l, 3)) && (bluebias <= Math::Pow(2l, 3))) && ((alphabias > -Math::Pow(2l, 3)) && (alphabias <= Math::Pow(2l, 3))))
			{
				out->OutputNumberPBD(1, 2);
				md = 3;
			}
			else if (((redbias > -Math::Pow(2l, 4)) && (redbias <= Math::Pow(2l, 4))) && ((greenbias > -Math::Pow(2l, 4)) && (greenbias <= Math::Pow(2l, 4))) && ((bluebias > -Math::Pow(2l, 4)) && (bluebias <= Math::Pow(2l, 4))) && ((alphabias > -Math::Pow(2l, 4)) && (alphabias <= Math::Pow(2l, 4))))
			{
				out->OutputNumberPBD(2, 2);
				md = 4;
			}
			else if (((redbias > -Math::Pow(2l, 5)) && (redbias <= Math::Pow(2l, 5))) && ((greenbias > -Math::Pow(2l, 5)) && (greenbias <= Math::Pow(2l, 5))) && ((bluebias > -Math::Pow(2l, 5)) && (bluebias <= Math::Pow(2l, 5))) && ((alphabias > -Math::Pow(2l, 5)) && (alphabias <= Math::Pow(2l, 5))))
			{
				out->OutputNumberPBD(3, 2);
				md = 5;
			}

			if (redbias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0-redbias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(redbias-1, md);
			}
			if (greenbias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0-greenbias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(greenbias-1, md);
			}
			if (bluebias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0-bluebias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(bluebias-1, md);
			}
			if (alphabias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0-alphabias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(alphabias-1, md);
			}

			break;
	}

	return 1;
}

int DecompressDelta(int line[], int prevline[], int x, int parts, PCIIn in)
{
	int pixb1;

	if (x == 0) pixb1 = 0;
	else pixb1 = line[x-1];

	int	 md;
	int	 bias;
	int	 redbias;
	int	 greenbias;
	int	 bluebias;
	int	 alphabias;
	int	 gray;
	int	 red;
	int	 green;
	int	 blue;
	int	 alpha;

	switch (parts)
	{
		case 1:
			gray = (pixb1+prevline[x])/2;

			if (in->InputNumberPBD(1) == 1)
			{
				bias = in->InputNumberPBD(3)+1;
			}
			else
			{
				bias = 0-in->InputNumberPBD(3);
			}

			return gray+bias;
		case 3:
			red = (Color(pixb1).GetRed()+Color(prevline[x]).GetRed())/2;
			green = (Color(pixb1).GetGreen()+Color(prevline[x]).GetGreen())/2;
			blue = (Color(pixb1).GetBlue()+Color(prevline[x]).GetBlue())/2;

			md = in->InputNumberPBD(2) + 2;

			if (in->InputNumberPBD(1) == 1)
			{
				redbias = in->InputNumberPBD(md)+1;
			}
			else
			{
				redbias = 0-in->InputNumberPBD(md);
			}
			if (in->InputNumberPBD(1) == 1)
			{
				greenbias = in->InputNumberPBD(md)+1;
			}
			else
			{
				greenbias = 0-in->InputNumberPBD(md);
			}
			if (in->InputNumberPBD(1) == 1)
			{
				bluebias = in->InputNumberPBD(md)+1;
			}
			else
			{
				bluebias = 0-in->InputNumberPBD(md);
			}

			return RGB(red+redbias, green+greenbias, blue+bluebias);
		case 4:
			red = (Color(pixb1).GetRed()+Color(prevline[x]).GetRed())/2;
			green = (Color(pixb1).GetGreen()+Color(prevline[x]).GetGreen())/2;
			blue = (Color(pixb1).GetBlue()+Color(prevline[x]).GetBlue())/2;
			alpha = (Color(pixb1).GetAlpha()+Color(prevline[x]).GetAlpha())/2;

			md = in->InputNumberPBD(2) + 2;

			if (in->InputNumberPBD(1) == 1)
			{
				redbias = in->InputNumberPBD(md)+1;
			}
			else
			{
				redbias = 0-in->InputNumberPBD(md);
			}
			if (in->InputNumberPBD(1) == 1)
			{
				greenbias = in->InputNumberPBD(md)+1;
			}
			else
			{
				greenbias = 0-in->InputNumberPBD(md);
			}
			if (in->InputNumberPBD(1) == 1)
			{
				bluebias = in->InputNumberPBD(md)+1;
			}
			else
			{
				bluebias = 0-in->InputNumberPBD(md);
			}
			if (in->InputNumberPBD(1) == 1)
			{
				alphabias = in->InputNumberPBD(md)+1;
			}
			else
			{
				alphabias = 0-in->InputNumberPBD(md);
			}

			return Color(red+redbias, green+greenbias, blue+bluebias, alpha+alphabias);
	}

	return 1;
}

int GetMinimumBits(int number)
{
	for (int i = 0; i <= 32; i++)
	{
		if (number < Math::Pow(2l, i)) return i;
	} 

	return 0;
}

int GetMinimumTriples(int number)
{
	for (int i = 0; i <= 20; i++)
	{
		if (number < Math::Pow(3l, i)) return i;
	}

	return 0;
}

int GetMinimumDecades(int number)
{
	for (int i = 0; i <= 9; i++)
	{
		if (number < Math::Pow(10l, i)) return i;
	}

	return 0;
}

int RotatePalette(int color)
{
	for (int x = 0; x < palentries-1; x++)
	{
		palette[x] = palette[x+1];
	}

	palette[palentries-1] = color;

	return 1;
}

int RotatePaletteEntry(int entry)
{
	int col = palette[entry];

	for (int x = entry; x < palentries-1; x++)
	{
		palette[x] = palette[x+1];
	}

	palette[palentries-1] = col;

	return 1;
}

int GetPaletteEntry(int color)
{
	for (int i = palentries-1; i >= 0; i--)
	{
		if (palette[i] == color) return i;
	}

	return -1;
}

int ProbeComp(int s, int e, int line[], int prevline[], int sx, int ctsize, bool nvRLE, int &rBits, int &rBitsSAA)
{
	int	 lastcol;
	int	 pcirle;
	int	 allinpal = 0;
	int	 gallinpal = 0;
	int	 bits = 0;
	int	 mod04 = 0;
	int	 mod16 = 0;
	int	 comp = 0;
	int	 compsaa = 0;
	int	 saa = 0;

	int	 oldpalette[388];
	int	 oldpalentries = palentries;

	for (int i = 0; i < 388; i++) oldpalette[i] = palette[i];

	for (int x = s; x < e; x++)
	{
		saa = 0;

		if (x == 0)	lastcol = 0;
		else		lastcol = line[x-1];

		if ((x % 16) == mod16)
		{
			gallinpal = 1;
			for (int x2 = 0; x2 < 16; x2++)
			{
				if ((x+x2) < sx) if (GetPaletteEntry(line[x+x2]) == -1 )	{ gallinpal = 0; break; }
			}
			comp++;
		}
		if (((x % 4) == mod04) && !gallinpal)
		{
			allinpal = 1;
			for (int x2 = 0; x2 < 4; x2++)
			{
				if ((x+x2) < sx) if (GetPaletteEntry(line[x+x2]) == -1 )	{ allinpal = 0; break; }
			}
			comp++;
		}

		if ((GetPaletteEntry(line[x]) == -1))
		{
			if (GetDelta(line, prevline, x, ctsize))
			{
				comp += 9;
			}
			else
			{
				comp += 26;
			}
			if (palentries < 338)
			{
				palette[palentries] = line[x];
				palentries++;
			}
			else
			{
				RotatePalette(line[x]);
			}
		}
		else
		{
			if (!allinpal && !gallinpal) comp++;

			compsaa++;

			if (GetSAA(line, prevline, x)) saa = 1;

			if ((palentries-1)-GetPaletteEntry(line[x]) < 2)
			{
				comp += 2;
				if (saa) compsaa -= 2;
			}
			else
			{
				if ((palentries-1)-GetPaletteEntry(line[x]) < 18)
				{
					bits = GetMinimumBits(palentries-2);
					if (bits > 4) bits = 4;
					comp += (bits + 2);
					if (saa) compsaa -= (bits + 2);
				}
				else
				{
					if ((palentries-1)-GetPaletteEntry(line[x]) < 82)
					{
						bits = GetMinimumBits(palentries-18);
						if (bits > 6) bits = 6;
						comp += (bits + 3);
						if (saa) compsaa -= (bits + 3);
					}
					else
					{
						comp += (GetMinimumBits(palentries-83)+3);
						if (saa) compsaa -= (GetMinimumBits(palentries-83)+3);
					}
				}
			}
			RotatePaletteEntry(GetPaletteEntry(line[x]));
		}

		if (line[x] == lastcol)
		{
			pcirle = GetRLE(line, sx, x, 8);
			if (pcirle >= 2)
			{
				if (pcirle > 9)
				{
					comp += 10;
				}
				else
				{
					comp += 5;
				}
				x = x + pcirle;
				allinpal = 0;
				gallinpal = 0;
				mod04 = (x + 1) %  4;
				mod16 = (x + 1) % 16;
			}
			else
			{
				comp++;
			}
		}
		else if (line[x] == prevline[x] && !nvRLE)
		{
			pcirle = GetVRLE(line, prevline, sx, x, 8);
			if (pcirle >= 2)
			{
				if (pcirle > 9)
				{
					comp += 10;
				}
				else
				{
					comp += 5;
				}
				x = x + pcirle;
				allinpal = 0;
				gallinpal = 0;
				mod04 = (x + 1) %  4;
				mod16 = (x + 1) % 16;
			}
			else
			{
				comp++;
			}
		}
	}
	palentries = oldpalentries;

	for (int j = 0; j < 388; j++) palette[j] = oldpalette[j];

	rBits = comp;
	rBitsSAA = comp + compsaa;

	return 0;
}
