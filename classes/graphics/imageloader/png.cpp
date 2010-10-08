 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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

#include <malloc.h>

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

	/* Create input stream for file or buffer.
	 */
	InStream	*in = NIL;

	if (gotFileName)
	{
		in = new InStream(STREAM_FILE, fileName, IS_READ);
	}
	else if (gotBuffer)
	{
		in = new InStream(STREAM_BUFFER, buffer, buffer.Size());
	}

	if (in->GetLastError() != IO_ERROR_OK)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

		delete in;

		bitmap = NIL;

		return bitmap;
	}

	/* If you are using replacement read functions, instead of calling
	 * png_init_io() here you would call:
	 */
	png_set_read_fn(png_ptr, (void *) in, &my_png_read);

	/* The call to png_read_info() gives us all of the information from the
	 * PNG file before the first IDAT (image data chunk).
	 */
	png_read_info(png_ptr, info_ptr);

	/* Tell libpng to strip 16 bit/color files down to 8 bits/color
	 */
	png_set_strip_16(png_ptr);

	/* Set the background color to draw transparent and alpha images over.
	 * It is possible to set the red, green, and blue components directly
	 * for paletted images instead of supplying a palette index.
	 */
	png_color_16	 my_background;

	my_background.red	= Setup::BackgroundColor.GetRed();
	my_background.green	= Setup::BackgroundColor.GetGreen();
	my_background.blue	= Setup::BackgroundColor.GetBlue();

	png_set_background(png_ptr, &my_background, PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);

	/* The easiest way to read the image
	 */
	png_bytep *row_pointers = (png_bytep *) malloc(info_ptr->height * sizeof(png_bytep));

	for (unsigned int row = 0; row < info_ptr->height; row++) row_pointers[row] = (png_bytep) malloc(png_get_rowbytes(png_ptr, info_ptr));

	/* Now it's time to read the image.
	 */
	png_read_image(png_ptr, row_pointers);

	bitmap.CreateBitmap(info_ptr->width, info_ptr->height, 24);

	for (UnsignedInt y = 0; y < info_ptr->height; y++)
	{
		for (UnsignedInt x = 0; x < info_ptr->width; x++)
		{
			if	((info_ptr->color_type == PNG_COLOR_TYPE_GRAY || info_ptr->color_type == PNG_COLOR_TYPE_GRAY_ALPHA) && info_ptr->bit_depth == 8) bitmap.SetPixel(Point(x, y), Color(row_pointers[y][x], row_pointers[y][x], row_pointers[y][x]));
			else if	((info_ptr->color_type == PNG_COLOR_TYPE_RGB  || info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)  && info_ptr->bit_depth == 8) bitmap.SetPixel(Point(x, y), Color(row_pointers[y][3 * x], row_pointers[y][3 * x + 1], row_pointers[y][3 * x + 2]));
			else if ( info_ptr->color_type == PNG_COLOR_TYPE_PALETTE						    && info_ptr->bit_depth == 8) bitmap.SetPixel(Point(x, y), Color(info_ptr->palette[row_pointers[y][x]].red, info_ptr->palette[row_pointers[y][x]].green, info_ptr->palette[row_pointers[y][x]].blue));
		}
	}

	/* Free PNG rows.
	 */
	for (unsigned int row = 0; row < info_ptr->height; row++) free(row_pointers[row]);

	free(row_pointers);

	/* Read rest of file, and get additional chunks in info_ptr */
	png_read_end(png_ptr, info_ptr);

	/* Clean up after the read, and free any memory allocated
	 */
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	delete in;

	/* That's it
	 */
	return bitmap;
}
