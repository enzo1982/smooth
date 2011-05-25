 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/misc/string.h>

using namespace smooth;
using namespace smooth::GUI;

S::String	 cfNames[10];
S::String	 cpNames[5];

PCIIO::PCIIO(const S::GUI::Bitmap &b)
{
	cfNames[RGBA]	= "RGB";
	cfNames[HSV]	= "HSV";
	cfNames[YUV]	= "YUV";
	cfNames[CMY]	= "CMY";
	cfNames[CMYK]	= "CMK";
	cfNames[GRAY]	= "GRY";

	cpNames[RAW]	= "RAW";
	cpNames[PCI]	= "PCI";
	cpNames[RLE]	= "RLE";
	cpNames[JPEG]	= "JPG";
	cpNames[BZIP2]	= "BZ2";

	majorversion = PCIMajorVersion;
	minorversion = PCIMinorVersion;
	compression = RAW;
	colorspace = RGBA;
	quality = 100;
	description = NIL;

	dpix = 75;
	dpiy = 75;
	bpcc = 8;
	overwrite = true;
	imageid = 0;
	imagename = NIL;
	rlebits = 8;

	bmp = b;

	sizex = bmp.GetSize().cx;
	sizey = bmp.GetSize().cy;
}

PCIIO::PCIIO()
{
	cfNames[RGBA]	= "RGB";
	cfNames[HSV]	= "HSV";
	cfNames[YUV]	= "YUV";
	cfNames[CMY]	= "CMY";
	cfNames[CMYK]	= "CMK";
	cfNames[GRAY]	= "GRY";

	cpNames[RAW]	= "RAW";
	cpNames[PCI]	= "PCI";
	cpNames[RLE]	= "RLE";
	cpNames[JPEG]	= "JPG";
	cpNames[BZIP2]	= "BZ2";

	rlebits = 8;
	quality = 100;
}

PCIIO::~PCIIO()
{
	bmp.DeleteBitmap();
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

void PCIIO::SetDescription(const S::String &d)
{
	description = d;
}

void PCIIO::SelectImage(const S::String &name)
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

void PCIIO::SetImageName(const S::String &name)
{
	imagename = name;
}

S::GUI::Bitmap &PCIIO::GetBitmap()
{
	return bmp;
}

PCIOut CreatePCI(const S::String &filename)
{
	return new IO::OutStream(IO::STREAM_FILE, filename, IO::OS_REPLACE);
}

PCIOut OpenPCIForOutput(const S::String &filename)
{
	return new IO::OutStream(IO::STREAM_FILE, filename, IO::OS_APPEND);
}

PCIIn OpenPCIForInput(const S::String &filename)
{
	return new IO::InStream(IO::STREAM_FILE, filename, IO::IS_READ);
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

bool PCIIO::WritePCI(PCIOut outstream)
{
	if (outstream->GetLastError() != IO::IO_ERROR_OK) return false;

	if (outstream->GetPos() == 0) WritePCIFTAG(outstream);

	WriteIMAGTAG(outstream);
	WriteRESOTAG(outstream);
	WriteFORMTAG(outstream);

	if (description != NIL) WriteDESCTAG(outstream);
	if (imagename	!= NIL) WriteNAMETAG(outstream);

	WriteDATATAG(outstream);

	return true;
}

bool PCIIO::ReadPCI(PCIIn instream)
{
	if (instream->GetLastError() != IO::IO_ERROR_OK) return false;

	if (imagename != NIL) FindImageID(instream);

	if (ReadPCIFTAG(instream) == false) return false;
	if (ReadIMAGTAG(instream) == false) return false;
	if (ReadRESOTAG(instream) == false) return false;
	if (ReadFORMTAG(instream) == false) return false;

	ReadDESCTAG(instream);

	if (ReadDATATAG(instream) == false) return false;

	return true;
}

bool PCIIO::WritePCIFTAG(PCIOut out)
{
	out->OutputString("PCIF");
	out->OutputNumber(majorversion, 1);
	out->OutputNumber(minorversion, 1);
	out->OutputNumber(-1, 4);
	out->OutputNumber(0 + 14, 4);

	return true;
}

bool PCIIO::ReadPCIFTAG(PCIIn in)
{
	in->Seek(0);

	do
	{
		static String	 pcifTag = "PCIF";

		if (in->InputString(4).Compare(pcifTag) == 0)
		{
			majorversion = in->InputNumber(1);
			minorversion = in->InputNumber(1);

			if (in->InputNumber(4) != -1)	return false;
			if (in->InputNumber(4) != 14)	return false;
			else				return true;
		}
		else
		{
			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	}
	while (in->Size() > in->GetPos());

	return false;
}

bool PCIIO::WriteIMAGTAG(PCIOut out)
{
	out->OutputString("IMAG");
	out->OutputNumber(majorversion, 1);
	out->OutputNumber(minorversion, 1);
	out->OutputNumber(imageid, 4);
	out->OutputNumber(0 + 14, 4);

	return true;
}

bool PCIIO::ReadIMAGTAG(PCIIn in)
{
	in->Seek(0);

	do
	{
		static String	 imagTag = "IMAG";

		if (in->InputString(4).Compare(imagTag) == 0)
		{
			in->RelSeek(2);

			if (in->InputNumber(4) == imageid)
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
			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	}
	while (in->Size() > in->GetPos());

	return false;
}

bool PCIIO::WriteRESOTAG(PCIOut out)
{
	out->OutputString("RESO");
	out->OutputNumber(majorversion, 1);
	out->OutputNumber(minorversion, 1);
	out->OutputNumber(imageid, 4);
	out->OutputNumber(16 + 14, 4);
	out->OutputNumber(sizex, 4);
	out->OutputNumber(sizey, 4);
	out->OutputNumber(dpix, 4);
	out->OutputNumber(dpiy, 4);

	return true;
}

bool PCIIO::ReadRESOTAG(PCIIn in)
{
	in->Seek(0);

	do
	{
		static String	 resoTag = "RESO";

		if (in->InputString(4).Compare(resoTag) == 0)
		{
			in->RelSeek(2);

			if (in->InputNumber(4) == imageid)
			{
				if (in->InputNumber(4) != 30)	return false;
				else
				{
					sizex = in->InputNumber(4);
					sizey = in->InputNumber(4);
					dpix = in->InputNumber(4);
					dpiy = in->InputNumber(4);

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
			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	}
	while (in->Size() > in->GetPos());

	return false;
}

bool PCIIO::WriteFORMTAG(PCIOut out)
{
	out->OutputString("FORM");
	out->OutputNumber(majorversion, 1);
	out->OutputNumber(minorversion, 1);
	out->OutputNumber(imageid, 4);

	if (compression == RLE)	out->OutputNumber(6 + 14, 4);
	else			out->OutputNumber(5 + 14, 4);

	out->OutputString(cfNames[colorspace]);
	out->OutputNumber(bpcc, 1);
	out->OutputNumber(compression, 1);

	if (compression == RLE)	out->OutputNumber(rlebits, 1);

	return true;
}

bool PCIIO::ReadFORMTAG(PCIIn in)
{
	S::String cfName;

	in->Seek(0);

	do
	{
		static String	 formTag = "FORM";

		if (in->InputString(4).Compare(formTag) == 0)
		{
			in->RelSeek(2);

			if (in->InputNumber(4) == imageid)
			{
				if (in->InputNumber(4) > 20)	return false;
				else
				{
					cfName = in->InputString(3);

					bpcc = in->InputNumber(1);
					compression = in->InputNumber(1);

					if (compression == RLE) rlebits = in->InputNumber(1);

					for (int i = 0; i < 8; i++)
					{
						if (cfName.Compare(cfNames[i]) == 0) colorspace = i;
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
			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	}
	while (in->Size() > in->GetPos());

	return false;
}

bool PCIIO::WriteDESCTAG(PCIOut out)
{
	out->OutputString("DESC");
	out->OutputNumber(majorversion, 1);
	out->OutputNumber(minorversion, 1);
	out->OutputNumber(imageid, 4);
	out->OutputNumber(description.Length() + 14, 4);
	out->OutputString(description);

	return true;
}

bool PCIIO::ReadDESCTAG(PCIIn in)
{
	in->Seek(0);

	do
	{
		static String	 descTag = "DESC";

		if (in->InputString(4).Compare(descTag) == 0)
		{
			in->RelSeek(2);

			if (in->InputNumber(4) == imageid)
			{
				description = in->InputString(in->InputNumber(4) - 14);

				return true;
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	}
	while (in->Size() > in->GetPos());

	return false;
}

bool PCIIO::WriteNAMETAG(PCIOut out)
{
	out->OutputString("NAME");
	out->OutputNumber(majorversion, 1);
	out->OutputNumber(minorversion, 1);
	out->OutputNumber(imageid, 4);
	out->OutputNumber(imagename.Length() + 14, 4);
	out->OutputString(imagename);

	return true;
}

bool PCIIO::ReadNAMETAG(PCIIn in)
{
	in->Seek(0);

	do
	{
		static String	 nameTag = "NAME";

		if (in->InputString(4).Compare(nameTag) == 0)
		{
			in->RelSeek(2);

			if (in->InputNumber(4) == imageid)
			{
				imagename = in->InputString(in->InputNumber(4) - 14);

				return true;
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	}
	while (in->Size() > in->GetPos());

	return false;
}

bool PCIIO::WriteDATATAG(PCIOut out)
{
	out->OutputString("DATA");
	out->OutputNumber(majorversion, 1);
	out->OutputNumber(minorversion, 1);
	out->OutputNumber(imageid, 4);

	int	 dpos = out->GetPos();

	out->OutputNumber(0 + 14, 4);

	CompressPCI(out);

	out->RelSeek(0);

	int	 npos = out->GetPos();
	int	 dsize = npos - dpos - 4;

	out->Seek(dpos);
	out->OutputNumber(dsize + 14, 4);
	out->Seek(npos);

	return true;
}

bool PCIIO::ReadDATATAG(PCIIn in)
{
	in->Seek(0);

	do
	{
		static String	 dataTag = "DATA";

		if (in->InputString(4).Compare(dataTag) == 0)
		{
			in->RelSeek(2);

			if (in->InputNumber(4) == imageid)
			{
				in->RelSeek(4);

				bmp.CreateBitmap(sizex, sizey);

				DecompressPCI(in);

				return true;
			}
			else
			{
				in->RelSeek(in->InputNumber(4) - 14);
			}
		}
		else
		{
			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	}
	while (in->Size() > in->GetPos());

	return false;
}

bool PCIIO::FindImageID(PCIIn in)
{
	int	 imageid = 0;

	in->Seek(0);

	do
	{
		static String	 nameTag = "NAME";

		if (in->InputString(4).Compare(nameTag) == 0)
		{
			in->RelSeek(2);

			imageid = in->InputNumber(4);

			if (in->InputString(in->InputNumber(4) - 14).Compare(imagename) == 0)
			{
				imageid = imageid;

				return true;
			}
		}
		else
		{
			in->RelSeek(6);

			in->RelSeek(in->InputNumber(4) - 14);
		}
	}
	while (in->Size() > in->GetPos());

	return false;
}
