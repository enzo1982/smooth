 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/color.h>
#include <smooth/pciio.h>
#include <smooth/codecs.h>

#include <iolib-cxx.h>
#include <smooth/toolkit.h>
#include <smooth/string.h>

S::String	 cfNames[10];
S::String	 cpNames[5];

PCIIO::PCIIO(HBITMAP b)
{
#ifdef __WIN32__
	HDC	 ddc = GetWindowDC(0);
	HDC	 cdc = CreateCompatibleDC(ddc);
	HBITMAP	 oldbmp;
	RECT	 tgt;
#endif

	cfNames[RGB]		= "RGB";
	cfNames[HSV]		= "HSV";
	cfNames[YUV]		= "YUV";
	cfNames[CMY]		= "CMY";
	cfNames[CMYK]		= "CMK";
	cfNames[CIELAB]		= "LAB";
	cfNames[COLORFORGE]	= "FRG";
	cfNames[GRAY]		= "GRY";
	cfNames[HIFI]		= "HFI";
	cfNames[ACMY]		= "ACM";

	cpNames[RAW]		= "RAW";
	cpNames[PCI]		= "PCI";
	cpNames[RLE]		= "RLE";
	cpNames[JPEG]		= "JPG";
	cpNames[BZIP2]		= "BZ2";

	majorversion = PCIMajorVersion;
	minorversion = PCIMinorVersion;
	compression = RAW;
	colorspace = RGB;
	quality = 100;
	description = NIL;
	sizex = GetBitmapSizeX(b);
	sizey = GetBitmapSizeY(b);
	dpix = 75;
	dpiy = 75;
	bpcc = 8;
	overwrite = true;
	imageid = 0;
	imagename = NIL;
	rlebits = 8;

	pic = new picture(sizex, sizey, 24, true);

#ifdef __WIN32__
	oldbmp = (HBITMAP) SelectObject(cdc, b);

	tgt.left = 0;
	tgt.top = 0;
	tgt.right = tgt.left + sizex;
	tgt.bottom = tgt.top + sizey;

	pic->BlitFromDC(cdc, &tgt);

	b = (HBITMAP) SelectObject(cdc, oldbmp);

	ReleaseDC(0, ddc);
	DeleteDC(cdc);
#endif
}

PCIIO::PCIIO()
{
	cfNames[RGB]		= "RGB";
	cfNames[HSV]		= "HSV";
	cfNames[YUV]		= "YUV";
	cfNames[CMY]		= "CMY";
	cfNames[CMYK]		= "CMK";
	cfNames[CIELAB]		= "LAB";
	cfNames[COLORFORGE]	= "FRG";
	cfNames[GRAY]		= "GRY";
	cfNames[HIFI]		= "HFI";
	cfNames[ACMY]		= "ACM";

	cpNames[RAW]		= "RAW";
	cpNames[PCI]		= "PCI";
	cpNames[RLE]		= "RLE";
	cpNames[JPEG]		= "JPG";
	cpNames[BZIP2]		= "BZ2";

	rlebits = 8;
	quality = 100;

	pic = NULL;
}

PCIIO::~PCIIO()
{
	delete pic;
}

void PCIIO::SetCompressionType(int t)
{
	compression = t;
}

void PCIIO::SetColorspace(int cs)
{
	colorspace = cs;
}

void PCIIO::SetBitsPerChannel(int b)
{
	bpcc = b;
}

void PCIIO::SetQuality(int q)
{
	quality = q;
}

void PCIIO::SetRLEBits(int b)
{
	rlebits = b;
}

void PCIIO::SetDescription(S::String d)
{
	description = d;
}

void PCIIO::SelectImage(S::String name)
{
	imagename = name;
}

void PCIIO::SelectImage(int id)
{
	imageid = id;
}

void PCIIO::SetImageID(int id)
{
	imageid = id;
}

void PCIIO::SetImageName(S::String name)
{
	imagename = name;
}

HBITMAP PCIIO::GetBitmap()
{
#ifdef __WIN32__
	HDC	 ddc = GetWindowDC(0);
	HDC	 cdc = CreateCompatibleDC(ddc);
	HBITMAP	 bmp = CreateBitmap(sizex, sizey, GetDeviceCaps(ddc, PLANES), GetDeviceCaps(ddc, BITSPIXEL), NULL);
	HBITMAP	 oldbmp;

	oldbmp = (HBITMAP) SelectObject(cdc, bmp);

	pic->BlitToDC(cdc);

	bmp = (HBITMAP) SelectObject(cdc, oldbmp);

	ReleaseDC(0, ddc);
	DeleteDC(cdc);

	return bmp;
#else
	return NIL;
#endif
}

PCIOut CreatePCI(S::String filename)
{
	return new OutStream(STREAM_FILE, filename, OS_OVERWRITE);
}

PCIOut OpenPCIForOutput(S::String filename)
{
	return new OutStream(STREAM_FILE, filename, OS_APPEND);
}

PCIIn OpenPCIForInput(S::String filename)
{
	return new InStream(STREAM_FILE, filename);
}

bool ClosePCI(PCIIn instream)
{
	instream->Close();

	delete instream;

	return true;
}

bool ClosePCI(PCIOut outstream)
{
	outstream->Close();

	delete outstream;

	return true;
}

bool WritePCI(PCIOut outstream, PCIIO &ior)
{
	if (outstream->GetLastError() != IOLIB_ERROR_OK) return false;

	if (outstream->GetPos() == 0) WritePCIFTAG(outstream, ior);

	WriteIMAGTAG(outstream, ior);
	WriteRESOTAG(outstream, ior);
	WriteFORMTAG(outstream, ior);

	if (ior.description != NIL) WriteDESCTAG(outstream, ior);
	if (ior.imagename != NIL) WriteNAMETAG(outstream, ior);

	WriteDATATAG(outstream, ior);

	return true;
}

bool ReadPCI(PCIIn instream, PCIIO &ior)
{
	if (instream->GetLastError() != IOLIB_ERROR_OK) return false;

	if (ior.imagename != NIL) FindImageID(instream, ior);

	if (ReadPCIFTAG(instream, ior) == false) return false;
	if (ReadIMAGTAG(instream, ior) == false) return false;
	if (ReadRESOTAG(instream, ior) == false) return false;
	if (ReadFORMTAG(instream, ior) == false) return false;

	ReadDESCTAG(instream, ior);

	if (ReadDATATAG(instream, ior) == false) return false;

	return true;
}

bool WritePCIFTAG(PCIOut out, PCIIO &ior)
{
	out->OutputString("PCIF");
	out->OutputNumber(ior.majorversion, 1);
	out->OutputNumber(ior.minorversion, 1);
	out->OutputNumber(-1, 4);
	out->OutputNumber(0 + 14, 4);

	return true;
}

bool ReadPCIFTAG(PCIIn in, PCIIO &ior)
{
	in->Seek(0);

	do
	{
		char	*tag = in->InputString(4);

		if (S::String(tag).Compare("PCIF") == 0)
		{
			delete [] tag;

			ior.majorversion = in->InputNumber(1);
			ior.minorversion = in->InputNumber(1);

			if (in->InputNumber(4) != -1)	return false;
			if (in->InputNumber(4) != 14)	return false;
			else				return true;
		}
		else
		{
			delete [] tag;

			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	} while (in->Size() > in->GetPos());

	return false;
}

bool WriteIMAGTAG(PCIOut out, PCIIO &ior)
{
	out->OutputString("IMAG");
	out->OutputNumber(ior.majorversion, 1);
	out->OutputNumber(ior.minorversion, 1);
	out->OutputNumber(ior.imageid, 4);
	out->OutputNumber(0 + 14, 4);

	return true;
}

bool ReadIMAGTAG(PCIIn in, PCIIO &ior)
{
	in->Seek(0);

	do
	{
		char	*tag = in->InputString(4);

		if (S::String(tag).Compare("IMAG") == 0)
		{
			delete [] tag;

			in->RelSeek(2);

			if (in->InputNumber(4) == ior.imageid)
			{
				if (in->InputNumber(4) != 14)	return false;
				else				return true;
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			delete [] tag;

			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	} while (in->Size() > in->GetPos());

	return false;
}

bool WriteRESOTAG(PCIOut out, PCIIO &ior)
{
	out->OutputString("RESO");
	out->OutputNumber(ior.majorversion, 1);
	out->OutputNumber(ior.minorversion, 1);
	out->OutputNumber(ior.imageid, 4);
	out->OutputNumber(16 + 14, 4);
	out->OutputNumber(ior.sizex, 4);
	out->OutputNumber(ior.sizey, 4);
	out->OutputNumber(ior.dpix, 4);
	out->OutputNumber(ior.dpiy, 4);

	return true;
}

bool ReadRESOTAG(PCIIn in, PCIIO &ior)
{
	in->Seek(0);

	do
	{
		char	*tag = in->InputString(4);

		if (S::String(tag).Compare("RESO") == 0)
		{
			delete [] tag;

			in->RelSeek(2);

			if (in->InputNumber(4) == ior.imageid)
			{
				if (in->InputNumber(4) != 30)	return false;
				else
				{
					ior.sizex = in->InputNumber(4);
					ior.sizey = in->InputNumber(4);
					ior.dpix = in->InputNumber(4);
					ior.dpiy = in->InputNumber(4);

					return true;
				}
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			delete [] tag;

			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	} while (in->Size() > in->GetPos());

	return false;
}

bool WriteFORMTAG(PCIOut out, PCIIO &ior)
{
	out->OutputString("FORM");
	out->OutputNumber(ior.majorversion, 1);
	out->OutputNumber(ior.minorversion, 1);
	out->OutputNumber(ior.imageid, 4);

	if (ior.compression == RLE)	out->OutputNumber(6 + 14, 4);
	else				out->OutputNumber(5 + 14, 4);

	out->OutputString(cfNames[ior.colorspace]);
	out->OutputNumber(ior.bpcc, 1);
	out->OutputNumber(ior.compression, 1);

	if (ior.compression == RLE)	out->OutputNumber(ior.rlebits, 1);

	return true;
}

bool ReadFORMTAG(PCIIn in, PCIIO &ior)
{
	S::String cfName;

	in->Seek(0);

	do
	{
		char	*tag = in->InputString(4);

		if (S::String(tag).Compare("FORM") == 0)
		{
			delete [] tag;

			in->RelSeek(2);

			if (in->InputNumber(4) == ior.imageid)
			{
				if (in->InputNumber(4) > 20)	return false;
				else
				{
					char	*cfn = in->InputString(3);

					cfName = cfn;

					delete [] cfn;

					ior.bpcc = in->InputNumber(1);
					ior.compression = in->InputNumber(1);

					if (ior.compression == RLE) ior.rlebits = in->InputNumber(1);

					for (int i = 0; i < 8; i++)
					{
						if (cfName.Compare(cfNames[i]) == 0) ior.colorspace = i;
					}

					return true;
				}
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			delete [] tag;

			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	} while (in->Size() > in->GetPos());

	return false;
}

bool WriteDESCTAG(PCIOut out, PCIIO &ior)
{
	out->OutputString("DESC");
	out->OutputNumber(ior.majorversion, 1);
	out->OutputNumber(ior.minorversion, 1);
	out->OutputNumber(ior.imageid, 4);
	out->OutputNumber(ior.description.Length() + 14, 4);
	out->OutputString(ior.description);

	return true;
}

bool ReadDESCTAG(PCIIn in, PCIIO &ior)
{
	in->Seek(0);

	do
	{
		char	*tag = in->InputString(4);

		if (S::String(tag).Compare("DESC") == 0)
		{
			delete [] tag;

			in->RelSeek(2);

			if (in->InputNumber(4) == ior.imageid)
			{
				char	*desc = in->InputString(in->InputNumber(4) - 14);

				ior.description = desc;

				delete [] desc;

				return true;
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			delete [] tag;

			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	} while (in->Size() > in->GetPos());

	return false;
}

bool WriteNAMETAG(PCIOut out, PCIIO &ior)
{
	out->OutputString("NAME");
	out->OutputNumber(ior.majorversion, 1);
	out->OutputNumber(ior.minorversion, 1);
	out->OutputNumber(ior.imageid, 4);
	out->OutputNumber(ior.imagename.Length() + 14, 4);
	out->OutputString(ior.imagename);

	return true;
}

bool ReadNAMETAG(PCIIn in, PCIIO &ior)
{
	in->Seek(0);

	do
	{
		char	*tag = in->InputString(4);

		if (S::String(tag).Compare("NAME") == 0)
		{
			delete [] tag;

			in->RelSeek(2);

			if (in->InputNumber(4) == ior.imageid)
			{
				char	*iname = in->InputString(in->InputNumber(4) - 14);

				ior.imagename = iname;

				delete [] iname;

				return true;
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			delete [] tag;

			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	} while (in->Size() > in->GetPos());

	return false;
}

bool WriteDATATAG(PCIOut out, PCIIO &ior)
{
	int dpos;
	int npos;
	int dsize;

	out->OutputString("DATA");
	out->OutputNumber(ior.majorversion, 1);
	out->OutputNumber(ior.minorversion, 1);
	out->OutputNumber(ior.imageid, 4);

	dpos = out->GetPos();

	out->OutputNumber(0 + 14, 4);

	CompressPCI(out, ior);

	out->RelSeek(0);

	npos = out->GetPos();

	dsize = npos - dpos - 4;

	out->Seek(dpos);
	out->OutputNumber(dsize + 14, 4);
	out->Seek(npos);

	return true;
}

bool ReadDATATAG(PCIIn in, PCIIO &ior)
{
	in->Seek(0);

	do
	{
		char	*tag = in->InputString(4);

		if (S::String(tag).Compare("DATA") == 0)
		{
			delete [] tag;

			in->RelSeek(2);

			if (in->InputNumber(4) == ior.imageid)
			{
				in->RelSeek(4);

				ior.pic = new picture(ior.sizex, ior.sizey, 24, true);

				DecompressPCI(in, ior);

				return true;
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			delete [] tag;

			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	} while (in->Size() > in->GetPos());

	return false;
}

bool FindImageID(PCIIn in, PCIIO &ior)
{
	int	 imageid = 0;

	in->Seek(0);

	do
	{
		char	*tag = in->InputString(4);

		if (S::String(tag).Compare("NAME") == 0)
		{
			delete [] tag;

			in->RelSeek(2);

			imageid = in->InputNumber(4);

			char	*iname = in->InputString(in->InputNumber(4) - 14);

			if (S::String(iname).Compare(ior.imagename) == 0)
			{
				delete [] iname;

				ior.imageid = imageid;

				return true;
			}
			else
			{
				delete [] iname;
			}
		}
		else
		{
			delete [] tag;

			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	} while (in->Size() > in->GetPos());

	return false;
}
