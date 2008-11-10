 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/imageloader/jpeg.h>
#include <smooth/system/system.h>
#include <smooth/io/outstream.h>
#include <smooth/files/file.h>

#include <stdio.h>
#include <time.h>
#include <libjpeg/jpeglib.h>

using namespace smooth::IO;

S::GUI::ImageLoaderJPEG::ImageLoaderJPEG(const String &iFileName) : ImageLoader(iFileName)
{
}

S::GUI::ImageLoaderJPEG::ImageLoaderJPEG(const Buffer<UnsignedByte> &iBuffer) : ImageLoader(iBuffer)
{
}

S::GUI::ImageLoaderJPEG::~ImageLoaderJPEG()
{
}

void my_error_exit(j_common_ptr cinfo)
{
}

const S::GUI::Bitmap &S::GUI::ImageLoaderJPEG::Load()
{
	/* In this example we want to open the input file before doing anything else.
	 * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	 * requires it in order to read binary files.
	 */
	if (gotBuffer)
	{
		/* Write buffer contents to a temporary file.
		 */
		fileName = System::System::GetTempDirectory().Append("tmpJpeg-").Append(String::FromInt(clock())).Append(".jpg");

		OutStream	 out(STREAM_FILE, fileName, OS_OVERWRITE);

		out.OutputData(buffer, buffer.Size());
	}

	FILE	 *stream = NIL;

	if (Setup::enableUnicode) stream = _wfopen(fileName, String("r+b"));
	else			  stream = fopen(fileName, "r+b");

	if (stream == NIL)
	{
		return bitmap;
	}

	/* This struct contains the JPEG decompression parameters and pointers to
	 * working space (which is allocated as needed by the JPEG library).
	 */
	jpeg_decompress_struct	 cinfo;

	/* We set up the normal JPEG error routines, then override error_exit.
	 */
	jpeg_error_mgr	 jerr;

	cinfo.err = jpeg_std_error(&jerr);
	cinfo.err->error_exit = my_error_exit;

	/* Now we can initialize the JPEG decompression object.
	 */
	jpeg_create_decompress(&cinfo);

	/* Specify data source (eg, a file)
	 */
	jpeg_stdio_src(&cinfo, stream);

	/* Read file parameters with jpeg_read_header()
	 */
	jpeg_read_header(&cinfo, TRUE);

	/* We can ignore the return value from jpeg_read_header since
	 *   (a) suspension is not possible with the stdio data source, and
	 *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	 * See libjpeg.doc for more info.
	 */

	/* Start decompressor
	 */
	jpeg_start_decompress(&cinfo);

	/* We can ignore the return value since suspension is not possible
	 * with the stdio data source.
	 */

	/* We may need to do some setup of our own at this point before reading
	 * the data.  After jpeg_start_decompress() we have the correct scaled
	 * output image dimensions available, as well as the output colormap
	 * if we asked for color quantization.
	 * In this example, we need to make an output work buffer of the right size.
	 */

	/* JSAMPLEs per row in output buffer
	 */
	int		 row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image
	 */
	JSAMPARRAY	 buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	bitmap.CreateBitmap(cinfo.output_width, cinfo.output_height, 24);

	/* Here we use the library's state variable cinfo.output_scanline as the
	 * loop counter, so that we don't have to keep track ourselves.
	 */
	while (cinfo.output_scanline < cinfo.output_height)
	{
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could ask for
		 * more than one scanline at a time if that's more convenient.
		 */
		jpeg_read_scanlines(&cinfo, buffer, 1);

		/* TODO: The copying step needs lots of optimization
		 */
		for (Int x = 0; x < (signed) cinfo.output_width; x++)
		{
			bitmap.SetPixel(Point(x, cinfo.output_scanline - 1), Color(buffer[0][3 * x], buffer[0][3 * x + 1], buffer[0][3 * x + 2]));
		}
	}

	/* We can ignore the return value since suspension is not possible
	 * with the stdio data source.
	 */
	jpeg_finish_decompress(&cinfo);

	/* This is an important step since it will release a good deal of memory.
	 */
	jpeg_destroy_decompress(&cinfo);

	/* After finish_decompress, we can close the input file.
	 * Here we postpone it until after no more JPEG errors are possible,
	 * so as to simplify the setjmp error logic above.  (Actually, I don't
	 * think that jpeg_destroy can do an error exit, but why assume anything...)
	 */
	fclose(stream);

	if (gotBuffer)
	{
		/* Delete our temporary file.
		 */
		File(fileName).Delete();
	}

	/* At this point you may want to check to see whether any corrupt-data
	 * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	 */

	/* And we're done!
	 */
	return bitmap;
}
