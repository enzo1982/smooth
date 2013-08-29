 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/graphics/color.h>
#include <smooth/pciio.h>
#include <smooth/misc/math.h>
#include <smooth/io/filters/filter_bzip2.h>

using namespace smooth;
using namespace smooth::GUI;

Int	*previousLine;
int	 palette[388];
int	 palentries  = 0;
int	 bits	     = 0;
int	 bitssaa     = 0;
int	 bitsnrle    = 0;
int	 bitssaanrle = 0;

int	 GetSLAL(int [], int [], int);
int	 DecompressSLAL(int [], int [], int);
int	 GetRLE(int [], int, int, int);
int	 GetVRLE(int [], int [], int, int, int);
int	 ProbeComp(int, int, int [], int [], int, int, bool, int &, int &);
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

Bool PCIIO::CompressPCI(PCIOut out)
{
	IO::FilterBZip2	*filter = NULL;

	palentries   = 0;
	bits	     = 0;
	bitssaa	     = 0;
	bitsnrle     = 0;
	bitssaanrle  = 0;

	previousLine = new Int [sizex];

	for (Int x = 0; x < sizex; x++) previousLine[x] = 0;
	for (Int x = 0; x < 338;   x++) palette[x]	= 0;

	if (compression == BZIP2)
	{
		filter = new IO::FilterBZip2();

		out->AddFilter(filter);
	}

	for (Int y = 0; y < sizey; y++) WriteLine(out, y);

	if (compression == BZIP2)
	{
		out->RemoveFilter(filter);

		delete filter;
	}

	delete [] previousLine;

	return True;
}

Bool PCIIO::DecompressPCI(PCIIn in)
{
	IO::FilterBZip2	*filter = NULL;

	palentries   = 0;
	bits	     = 0;
	bitssaa	     = 0;
	bitsnrle     = 0;
	bitssaanrle  = 0;

	previousLine = new Int [sizex];

	for (Int x = 0; x < sizex; x++) previousLine[x] = 0;
	for (Int x = 0; x < 338;   x++) palette[x]  = 0;

	if (compression == BZIP2)
	{
		filter = new IO::FilterBZip2();

		in->AddFilter(filter);
	}

	for (Int y = 0; y < sizey; y++) ReadLine(in, y);

	if (compression == BZIP2)
	{
		in->RemoveFilter(filter);

		delete filter;
	}

	delete [] previousLine;

	return True;
}

bool PCIIO::WriteLine(PCIOut out, int y)
{
	/* Check compression type.
	 */
	if (compression != PCI	 &&
	    compression != RLE	 &&
	    compression != BZIP2 &&
	    compression != UNCOMPRESSED) return false;

	/* Write image line.
	 */
	int	*line = new int [sizex];
	int	 ctsize;
	int	 maxpalentries;

	switch (colorspace)
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

	for (int x = 0; x < sizex; x++) line[x] = Color(bmp.GetPixel(Point(x, y))).ConvertTo(colorspace).Downsample(bpcc);

	switch (compression)
	{
		case BZIP2:
		case UNCOMPRESSED:
			for (int x = 0; x < sizex; x++) out->OutputNumberPBD(line[x], ctsize * bpcc);

			break;
		case RLE:
			for (int x = 0; x < sizex; x++)
			{
				int	 rle = GetRLE(line, sizex, x, rlebits);

				out->OutputNumberPBD(line[x], ctsize * bpcc);
				out->OutputNumberPBD(rle, rlebits);

				x += rle;
			}

			break;
		case PCI:
			if (GetSLAL(line, previousLine, sizex))
			{
				out->OutputNumberPBD(1, 1);
			}
			else
			{
				out->OutputNumberPBD(0, 1);

				ProbeComp(0, sizex, line, previousLine, sizex, ctsize, false, bits, bitssaa);
				ProbeComp(0, sizex, line, previousLine, sizex, ctsize, true, bitsnrle, bitssaanrle);

				int	 dorle = 0;
				int	 dosaa = 0;

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

				int	 allinpal = 0;
				int	 gallinpal = 0;
				int	 mod04 = 0;
				int	 mod16 = 0;

				for (int x = 0; x < sizex; x++)
				{
					int	 previousColor = (x > 0 ? line[x - 1] : 0);

					if ((x % 16) == mod16)
					{
						gallinpal = 1;

						for (int x2 = 0; x2 < 16; x2++)
						{
							if ((x + x2) < sizex && GetPaletteEntry(line[x + x2]) == -1) { gallinpal = 0; break; }
						}

						if (gallinpal == 1) out->OutputNumberPBD(1, 1);
						else		    out->OutputNumberPBD(0, 1);
					}

					if (((x % 4) == mod04) && !gallinpal)
					{
						allinpal = 1;

						for (int x2 = 0; x2 < 4; x2++)
						{
							if ((x + x2) < sizex && GetPaletteEntry(line[x + x2]) == -1) { allinpal = 0; break; }
						}

						if (allinpal == 1) out->OutputNumberPBD(1, 1);
						else		   out->OutputNumberPBD(0, 1);
					}

					if ((GetPaletteEntry(line[x]) == -1))
					{
						out->OutputNumberPBD(0, 1);

						if (GetDelta(line, previousLine, x, ctsize))
						{
							out->OutputNumberPBD(1, 1);

							CompressDelta(line, previousLine, x, ctsize, out);
						}
						else
						{
							out->OutputNumberPBD(0, 1);
							out->OutputNumberPBD(line[x], ctsize * bpcc);
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

						if (GetSAA(line, previousLine, x) && dosaa)
						{
							out->OutputNumberPBD(1, 1);
						}
						else
						{
							if (dosaa) out->OutputNumberPBD(0, 1);

							if ((palentries - 1) - GetPaletteEntry(line[x]) < 2)
							{
								out->OutputNumberPBD(1, 1);
								out->OutputNumberPBD((palentries - 1) - GetPaletteEntry(line[x]), 1);
							}
							else
							{
								out->OutputNumberPBD(0, 1);

								if ((palentries - 1)-GetPaletteEntry(line[x]) < 18)
								{
									int	 lbits = Math::Min(GetMinimumBits(palentries - 2), 4);

									out->OutputNumberPBD(1, 1);
									out->OutputNumberPBD((palentries - 1) - GetPaletteEntry(line[x]) - 2, lbits);
								}
								else
								{
									out->OutputNumberPBD(0, 1);

									if ((palentries - 1) - GetPaletteEntry(line[x]) < 82)
									{
										int	 lbits = Math::Min(GetMinimumBits(palentries - 18), 6);

										out->OutputNumberPBD(1, 1);
										out->OutputNumberPBD((palentries - 1)-GetPaletteEntry(line[x]) - 18, lbits);
									}
									else
									{
										out->OutputNumberPBD(0, 1);
										out->OutputNumberPBD((palentries - 1)-GetPaletteEntry(line[x]) - 82, GetMinimumBits(palentries - 83));
									}
								}
							}
						}

						RotatePaletteEntry(GetPaletteEntry(line[x]));
					}

					if (line[x] == previousColor)
					{
						int	 pcirle = GetRLE(line, sizex, x, 8);

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
					else if ((line[x] == previousLine[x]) && dorle)
					{
						int	 pcirle = GetVRLE(line, previousLine, sizex, x, 8);

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

			for (int x = 0; x < sizex; x++) previousLine[x] = line[x];

			break;
	}

	delete [] line;

	return true;
}

bool PCIIO::ReadLine(PCIIn in, int y)
{
	/* Check compression type.
	 */
	if (compression != PCI	 &&
	    compression != RLE	 &&
	    compression != BZIP2 &&
	    compression != UNCOMPRESSED) return false;

	/* Read image line.
	 */
	int	*line = new int [sizex];
	int	 ctsize;
	int	 maxpalentries;

	switch (colorspace)
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

	switch (compression)
	{
		case BZIP2:
		case UNCOMPRESSED:
			for (int x = 0; x < sizex; x++)
			{
				line[x] = in->InputNumberPBD(ctsize * bpcc);
			}

			break;
		case RLE:
			for (int x = 0; x < sizex; x++)
			{
				int	 col = in->InputNumberPBD(ctsize * bpcc);
				int	 rle = in->InputNumberPBD(rlebits);

				for (int i = x; i <= x + rle; i++) line[i] = col;

				x += rle;
			}

			break;
		case PCI:
			if (in->InputNumberPBD(1) == 1)
			{
				DecompressSLAL(line, previousLine, sizex);
			}
			else
			{
				int	 allinpal = 0;
				int	 gallinpal = 0;
				int	 mod04 = 0;
				int	 mod16 = 0;

				int	 dosaa = in->InputNumberPBD(1);
				int	 dorle = in->InputNumberPBD(1);

				for (int x = 0; x < sizex; x++)
				{
					int	 previousColor = (x > 0 ? line[x - 1] : 0);

					if ( (x % 16) == mod16		     ) gallinpal = in->InputNumberPBD(1);
					if (((x %  4) == mod04) && !gallinpal) allinpal	 = in->InputNumberPBD(1);

					int	 col = 0;
					int	 inpal = 1;

					if (!allinpal && !gallinpal) inpal = in->InputNumberPBD(1);

					if (inpal == 0)
					{
						if (in->InputNumberPBD(1)) col = DecompressDelta(line, previousLine, x, ctsize, in);
						else			   col = in->InputNumberPBD(ctsize * bpcc);

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
						int	 saa = 0;

						if (dosaa) saa = in->InputNumberPBD(1);

						if (saa)
						{
							col = previousLine[x];
						}
						else
						{
							if (in->InputNumberPBD(1) == 1)
							{
								col = palette[(palentries - 1) - in->InputNumberPBD(1)];
							}
							else
							{
								if (in->InputNumberPBD(1) == 1)
								{
									int	 lbits = Math::Min(GetMinimumBits(palentries - 2), 4);

									col = palette[(palentries - 1) - in->InputNumberPBD(lbits) - 2];
								}
								else
								{
									if (in->InputNumberPBD(1) == 1)
									{
										int	 lbits = Math::Min(GetMinimumBits(palentries - 18), 6);

										col = palette[(palentries - 1) - in->InputNumberPBD(lbits) - 18];
									}
									else
									{
										col = palette[(palentries - 1) - in->InputNumberPBD(GetMinimumBits(palentries - 83)) - 82];
									}
								}
							}
						}

						int	 paletteEntry = GetPaletteEntry(col);

						if (paletteEntry >= 0) RotatePaletteEntry(paletteEntry);
					}

					line[x] = col;

					if (col == previousColor)
					{
						if (in->InputNumberPBD(1) == 1)
						{
							int	 pcirle;

							if (in->InputNumberPBD(1) == 1) pcirle = in->InputNumberPBD(8) + 2;
							else				pcirle = in->InputNumberPBD(3) + 2;

							for (int i = x + 1; i <= x + pcirle; i++) line[i] = line[x];

							x = x + pcirle;

							allinpal = 0;
							gallinpal = 0;

							mod04 = (x + 1) %  4;
							mod16 = (x + 1) % 16;
						}
					}
					else if ((col == previousLine[x]) && dorle)
					{
						if (in->InputNumberPBD(1) == 1)
						{
							int	 pcirle;

							if (in->InputNumberPBD(1) == 1) pcirle = in->InputNumberPBD(8) + 2;
							else				pcirle = in->InputNumberPBD(3) + 2;

							for (int i = x + 1; i <= x + pcirle; i++) line[i] = previousLine[i];

							x = x + pcirle;

							allinpal = 0;
							gallinpal = 0;

							mod04 = (x + 1) %  4;
							mod16 = (x + 1) % 16;
						}
					}
				}
			}

			for (int x = 0; x < sizex; x++) previousLine[x] = line[x];

			break;
	}

	for (int x = 0; x < sizex; x++) bmp.SetPixel(Point(x, y), Color(line[x], colorspace).Upsample(bpcc).ConvertTo(RGBA));

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
	for (int x = 0; x < sx; x++) line[x] = prevline[x];

	return 1;
}

int GetRLE(int line[], int sx, int x, int maxbits)
{
	if (x == sx - 1) return 0;

	int	 rle = 0;

	for (int i = x + 1; i < x + Math::Pow(2l, maxbits); i++)
	{
		if (line[i] == line[x]) rle++;
		else			break;

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
		else			    break;

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
	int	 pixb1 = (x > 0 ? line[x - 1] : 0);

	int	 bias;
	int	 redbias;
	int	 greenbias;
	int	 bluebias;
	int	 alphabias;

	switch (parts)
	{
		case 1:
			bias = line[x] - (pixb1 + prevline[x]) / 2;

			if ((bias > -Math::Pow(2l, 3)) && (bias <= Math::Pow(2l, 3))) return 1;

			break;
		case 3:
			redbias	  = Color(line[x]).GetRed()   - (Color(pixb1).GetRed()	 + Color(prevline[x]).GetRed())	  / 2;
			greenbias = Color(line[x]).GetGreen() - (Color(pixb1).GetGreen() + Color(prevline[x]).GetGreen()) / 2;
			bluebias  = Color(line[x]).GetBlue()  - (Color(pixb1).GetBlue()	 + Color(prevline[x]).GetBlue())  / 2;

			if (((redbias > -Math::Pow(2l, 5)) && (redbias <= Math::Pow(2l, 5))) && ((greenbias > -Math::Pow(2l, 5)) && (greenbias <= Math::Pow(2l, 5))) && ((bluebias > -Math::Pow(2l, 5)) && (bluebias <= Math::Pow(2l, 5)))) return 1;

			break;
		case 4:
			redbias	  = Color(line[x]).GetRed()   - (Color(pixb1).GetRed()	 + Color(prevline[x]).GetRed())	  / 2;
			greenbias = Color(line[x]).GetGreen() - (Color(pixb1).GetGreen() + Color(prevline[x]).GetGreen()) / 2;
			bluebias  = Color(line[x]).GetBlue()  - (Color(pixb1).GetBlue()	 + Color(prevline[x]).GetBlue())  / 2;
			alphabias = Color(line[x]).GetAlpha() - (Color(pixb1).GetAlpha() + Color(prevline[x]).GetAlpha()) / 2;

			if (((redbias > -Math::Pow(2l, 5)) && (redbias <= Math::Pow(2l, 5))) && ((greenbias > -Math::Pow(2l, 5)) && (greenbias <= Math::Pow(2l, 5))) && ((bluebias > -Math::Pow(2l, 5)) && (bluebias <= Math::Pow(2l, 5))) && ((alphabias > -Math::Pow(2l, 5)) && (alphabias <= Math::Pow(2l, 5)))) return 1;

			break;
	}

	return 0;
}

int CompressDelta(int line[], int prevline[], int x, int parts, PCIOut out)
{
	int	 pixb1 = (x > 0 ? line[x - 1] : 0);

	int	 md = 0;
	int	 bias;
	int	 redbias;
	int	 greenbias;
	int	 bluebias;
	int	 alphabias;

	switch (parts)
	{
		case 1:
			bias = line[x] - (pixb1 + prevline[x]) / 2;

			if (bias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0 - bias, 3);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(bias - 1, 3);
			}

			break;
		case 3:
			redbias	  = Color(line[x]).GetRed()   - (Color(pixb1).GetRed()	 + Color(prevline[x]).GetRed())	  / 2;
			greenbias = Color(line[x]).GetGreen() - (Color(pixb1).GetGreen() + Color(prevline[x]).GetGreen()) / 2;
			bluebias  = Color(line[x]).GetBlue()  - (Color(pixb1).GetBlue()	 + Color(prevline[x]).GetBlue())  / 2;

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
				out->OutputNumberPBD(0 - redbias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(redbias - 1, md);
			}
			if (greenbias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0 - greenbias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(greenbias - 1, md);
			}
			if (bluebias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0 - bluebias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(bluebias - 1, md);
			}

			break;
		case 4:
			redbias	  = Color(line[x]).GetRed()   - (Color(pixb1).GetRed()	 + Color(prevline[x]).GetRed())	  / 2;
			greenbias = Color(line[x]).GetGreen() - (Color(pixb1).GetGreen() + Color(prevline[x]).GetGreen()) / 2;
			bluebias  = Color(line[x]).GetBlue()  - (Color(pixb1).GetBlue()	 + Color(prevline[x]).GetBlue())  / 2;
			alphabias = Color(line[x]).GetAlpha() - (Color(pixb1).GetAlpha() + Color(prevline[x]).GetAlpha()) / 2;

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
				out->OutputNumberPBD(0 - redbias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(redbias - 1, md);
			}
			if (greenbias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0 - greenbias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(greenbias - 1, md);
			}
			if (bluebias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0 - bluebias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(bluebias - 1, md);
			}
			if (alphabias <= 0)
			{
				out->OutputNumberPBD(0, 1);
				out->OutputNumberPBD(0 - alphabias, md);
			}
			else
			{
				out->OutputNumberPBD(1, 1);
				out->OutputNumberPBD(alphabias - 1, md);
			}

			break;
	}

	return 1;
}

int DecompressDelta(int line[], int prevline[], int x, int parts, PCIIn in)
{
	int	 pixb1 = (x > 0 ? line[x - 1] : 0);

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
			gray = (pixb1 + prevline[x]) / 2;

			if (in->InputNumberPBD(1) == 1) bias = in->InputNumberPBD(3) + 1;
			else				bias = 0 - in->InputNumberPBD(3);

			return gray + bias;
		case 3:
			red   = (Color(pixb1).GetRed()	 + Color(prevline[x]).GetRed())	  / 2;
			green = (Color(pixb1).GetGreen() + Color(prevline[x]).GetGreen()) / 2;
			blue  = (Color(pixb1).GetBlue()	 + Color(prevline[x]).GetBlue())  / 2;

			md = in->InputNumberPBD(2) + 2;

			if (in->InputNumberPBD(1) == 1) redbias	  =	in->InputNumberPBD(md) + 1;
			else				redbias	  = 0 - in->InputNumberPBD(md);

			if (in->InputNumberPBD(1) == 1) greenbias =	in->InputNumberPBD(md) + 1;
			else				greenbias = 0 - in->InputNumberPBD(md);

			if (in->InputNumberPBD(1) == 1) bluebias  =	in->InputNumberPBD(md) + 1;
			else				bluebias  = 0 - in->InputNumberPBD(md);

			return Color(red + redbias, green + greenbias, blue + bluebias);
		case 4:
			red   = (Color(pixb1).GetRed()	 + Color(prevline[x]).GetRed())	  / 2;
			green = (Color(pixb1).GetGreen() + Color(prevline[x]).GetGreen()) / 2;
			blue  = (Color(pixb1).GetBlue()	 + Color(prevline[x]).GetBlue())  / 2;
			alpha = (Color(pixb1).GetAlpha() + Color(prevline[x]).GetAlpha()) / 2;

			md = in->InputNumberPBD(2) + 2;

			if (in->InputNumberPBD(1) == 1) redbias	  =	in->InputNumberPBD(md) + 1;
			else				redbias	  = 0 - in->InputNumberPBD(md);

			if (in->InputNumberPBD(1) == 1) greenbias =	in->InputNumberPBD(md) + 1;
			else				greenbias = 0 - in->InputNumberPBD(md);

			if (in->InputNumberPBD(1) == 1) bluebias  =	in->InputNumberPBD(md) + 1;
			else				bluebias  = 0 - in->InputNumberPBD(md);

			if (in->InputNumberPBD(1) == 1) alphabias =	in->InputNumberPBD(md) + 1;
			else				alphabias = 0 - in->InputNumberPBD(md);

			return Color(red + redbias, green + greenbias, blue + bluebias, alpha + alphabias);
	}

	return 1;
}

int GetMinimumBits(int number)
{
	for (Int i = 0; i <= 32; i++)
	{
		if (number < Math::Pow(2l, i)) return i;
	} 

	return 0;
}

int GetMinimumTriples(int number)
{
	for (Int i = 0; i <= 20; i++)
	{
		if (number < Math::Pow(3l, i)) return i;
	}

	return 0;
}

int GetMinimumDecades(int number)
{
	for (Int i = 0; i <= 9; i++)
	{
		if (number < Math::Pow(10l, i)) return i;
	}

	return 0;
}

int RotatePalette(int color)
{
	for (Int x = 0; x < palentries - 1; x++) palette[x] = palette[x + 1];

	palette[palentries - 1] = color;

	return 1;
}

int RotatePaletteEntry(int entry)
{
	Int	 col = palette[entry];

	for (Int x = entry; x < palentries - 1; x++) palette[x] = palette[x + 1];

	palette[palentries - 1] = col;

	return 1;
}

int GetPaletteEntry(int color)
{
	for (Int i = palentries - 1; i >= 0; i--)
	{
		if (palette[i] == color) return i;
	}

	return -1;
}

int ProbeComp(int s, int e, int line[], int prevline[], int sx, int ctsize, bool nvRLE, int &rBits, int &rBitsSAA)
{
	int	 pcirle;
	int	 allinpal = 0;
	int	 gallinpal = 0;
	int	 lbits = 0;
	int	 mod04 = 0;
	int	 mod16 = 0;
	int	 comp = 0;
	int	 compsaa = 0;

	int	 oldpalette[388];
	int	 oldpalentries = palentries;

	for (int i = 0; i < 388; i++) oldpalette[i] = palette[i];

	for (int x = s; x < e; x++)
	{
		int	 previousColor = (x > 0 ? line[x - 1] : 0);

		if ((x % 16) == mod16)
		{
			gallinpal = 1;

			for (int x2 = 0; x2 < 16; x2++)
			{
				if ((x + x2) < sx) if (GetPaletteEntry(line[x + x2]) == -1 ) { gallinpal = 0; break; }
			}

			comp++;
		}

		if (((x % 4) == mod04) && !gallinpal)
		{
			allinpal = 1;

			for (int x2 = 0; x2 < 4; x2++)
			{
				if ((x + x2) < sx) if (GetPaletteEntry(line[x + x2]) == -1 ) { allinpal = 0; break; }
			}

			comp++;
		}

		if ((GetPaletteEntry(line[x]) == -1))
		{
			if (GetDelta(line, prevline, x, ctsize)) comp +=  9;
			else					 comp += 26;

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

			int	 saa = 0;

			if (GetSAA(line, prevline, x)) saa = 1;

			if ((palentries - 1) - GetPaletteEntry(line[x]) < 2)
			{
				comp += 2;

				if (saa) compsaa -= 2;
			}
			else
			{
				if ((palentries - 1) - GetPaletteEntry(line[x]) < 18)
				{
					lbits = Math::Min(GetMinimumBits(palentries - 2), 4);

					comp += (lbits + 2);

					if (saa) compsaa -= (lbits + 2);
				}
				else
				{
					if ((palentries - 1) - GetPaletteEntry(line[x]) < 82)
					{
						lbits = Math::Min(GetMinimumBits(palentries - 18), 6);

						comp += (lbits + 3);

						if (saa) compsaa -= (lbits + 3);
					}
					else
					{
						comp += (GetMinimumBits(palentries - 83) + 3);

						if (saa) compsaa -= (GetMinimumBits(palentries - 83) + 3);
					}
				}
			}


			int	 paletteEntry = GetPaletteEntry(line[x]);

			if (paletteEntry >= 0) RotatePaletteEntry(paletteEntry);
		}

		if (line[x] == previousColor)
		{
			pcirle = GetRLE(line, sx, x, 8);

			if (pcirle >= 2)
			{
				if (pcirle > 9) comp += 10;
				else		comp += 5;

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
				if (pcirle > 9) comp += 10;
				else		comp += 5;

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

	rBits	 = comp;
	rBitsSAA = comp + compsaa;

	return 0;
}
