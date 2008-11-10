 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/imageloader/png.h>
#include <smooth/io/instream.h>

#include <libpng/png.h>

using namespace smooth::IO;

S::GUI::ImageLoaderPNG::ImageLoaderPNG(const String &iFileName) : ImageLoader(iFileName)
{
}

S::GUI::ImageLoaderPNG::ImageLoaderPNG(const Buffer<UnsignedByte> &iBuffer) : ImageLoader(iBuffer)
{
}

S::GUI::ImageLoaderPNG::~ImageLoaderPNG()
{
}

void my_png_error(png_structp png_ptr, png_const_charp error)
{
}

void my_png_warning(png_structp png_ptr, png_const_charp warning)
{
}

void my_png_read(png_structp png_ptr, png_bytep buffer, png_size_t size)
{
	InStream	*in = (InStream *) png_ptr->io_ptr;

	in->InputData(buffer, size);
}

const S::GUI::Bitmap &S::GUI::ImageLoaderPNG::Load()
{
	InStream	*in = NIL;

	if (gotFileName)
	{
		in = new InStream(STREAM_FILE, fileName, IS_READONLY);
	}
	else if (gotBuffer)
	{
		in = new InStream(STREAM_BUFFER, buffer, buffer.Size());
	}

	/* Create and initialize the png_struct with the desired error handler
	 * functions.  If you want to use the default stderr and longjump method,
	 * you can supply NULL for the last three parameters.  We also supply the
	 * the compiler header file version, so that we know if the application
	 * was compiled with a compatible version of the library.
	 */
	png_structp	 png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NIL, &my_png_error, &my_png_warning);

	if (png_ptr == NULL)
	{
		bitmap = NIL;

		return bitmap;
	}

	/* Allocate/initialize the memory for image information.
	 */
	png_infop	 info_ptr = png_create_info_struct(png_ptr);

	if (info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);

		bitmap = NIL;

		return bitmap;
	}

	/* If you are using replacement read functions, instead of calling
	 * png_init_io() here you would call:
	 */
	png_set_read_fn(png_ptr, (void *) in, &my_png_read);

	/* If you have enough memory to read in the entire image at once,
	 * and you need to specify only transforms that can be controlled
	 * with one of the PNG_TRANSFORM_* bits (this presently excludes
	 * dithering, filling, setting background, and doing gamma
	 * adjustment), then you can read the entire image (including
	 * pixels) into the info structure with this call:
	 */
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_STRIP_ALPHA | PNG_TRANSFORM_PACKING, png_voidp_NULL);

	/* Get PNG row data and copy it to an actual bitmap
	 *
	 * TODO: The copying step needs lots of optimization
	 */
	png_bytep	*row_pointers = png_get_rows(png_ptr, info_ptr);

	bitmap.CreateBitmap(info_ptr->width, info_ptr->height, 24);

	for (UnsignedInt y = 0; y < info_ptr->height; y++)
	{
		for (UnsignedInt x = 0; x < info_ptr->width; x++)
		{
			bitmap.SetPixel(Point(x, y), Color(row_pointers[y][3 * x], row_pointers[y][3 * x + 1], row_pointers[y][3 * x + 2]));
		}
	}

	/* Clean up after the read, and free any memory allocated
	 */
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	delete in;

	/* That's it
	 */
	return bitmap;
}
