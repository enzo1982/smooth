 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/imageloader/webp.h>
#include <smooth/io/instream.h>

#include <webp/decode.h>

using namespace smooth::IO;

S::GUI::ImageLoaderWebP::ImageLoaderWebP(const String &iFileName) : ImageLoader(iFileName)
{
}

S::GUI::ImageLoaderWebP::ImageLoaderWebP(const Buffer<UnsignedByte> &iBuffer) : ImageLoader(iBuffer)
{
}

S::GUI::ImageLoaderWebP::~ImageLoaderWebP()
{
}

const S::GUI::Bitmap &S::GUI::ImageLoaderWebP::Load()
{
	/* Make sure bitmap is initialized.
	 */
	bitmap = NIL;

	/* Check if we either have a file or a buffer to load.
	 */
	if (!gotFileName && !gotBuffer) return bitmap;

	/* Load file into buffer.
	 */
	if (gotFileName)
	{
		InStream	 in(STREAM_FILE, fileName, IS_READ);

		buffer.Resize(in.Size());
		in.InputData(buffer, buffer.Size());
	}

	/* Init WebP decoder.
	 */
	WebPDecoderConfig config;
	WebPInitDecoderConfig(&config);

	if (WebPGetFeatures(buffer, buffer.Size(), &config.input) != VP8_STATUS_OK) return bitmap;

	/* Decode image in RGBA format.
	 */
	config.output.colorspace = MODE_RGBA;

	if (WebPDecode(buffer, buffer.Size(), &config) != VP8_STATUS_OK) return bitmap;

	/* Copy decoded image to bitmap.
	 */
	bitmap.CreateBitmap(Size(config.input.width, config.input.height));

	for (Int y = 0; y < config.input.height; y++)
	{
		Int	 lineOffset = 4 * y * config.input.width;

		for (Int x = 0; x < config.input.width; x++)
		{
			bitmap.SetPixel(Point(x, y), Color(config.output.u.RGBA.rgba[lineOffset + 4 * x + 3] << 24 | config.output.u.RGBA.rgba[lineOffset + 4 * x + 2] << 16 | config.output.u.RGBA.rgba[lineOffset + 4 * x + 1] << 8 | config.output.u.RGBA.rgba[lineOffset + 4 * x], Color::RGBA));
		}
	}

	WebPFreeDecBuffer(&config.output);

	return bitmap;
}
