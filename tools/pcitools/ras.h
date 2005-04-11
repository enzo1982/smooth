// Load raster images

#include <iolib-cxx.h>
#include <windows.h>

HBITMAP LoadRAS(char *);
int SaveRAS(HBITMAP, char *);

int SaveRAS(HBITMAP hbmp, char *file)
{
	HDC		 dc = GetWindowDC(0);
	BITMAP		 bit;

	GetObject(hbmp, sizeof(bit), &bit);

	int		 sizex = bit.bmWidth;
	int		 sizey = bit.bmHeight;
	OutStream	 Out(STREAM_FILE, file, OS_OVERWRITE);
	HDC		 hdc = CreateCompatibleDC(dc);
	HBITMAP		 oldbmp;

	oldbmp = (HBITMAP) SelectObject(hdc, hbmp);

	Out.OutputNumberRaw(0x59a66a95, 4);
	Out.OutputNumberRaw(sizex, 4);
	Out.OutputNumberRaw(sizey, 4);
	Out.OutputNumberRaw(24, 4);
	Out.OutputNumberRaw(sizex*sizey*3, 4);
	Out.OutputNumberRaw(1, 4);
	Out.OutputNumberRaw(0, 4);
	Out.OutputNumberRaw(0, 4);

	for (int y = 0; y < sizey; y++)
	{
		for (int x = 0; x < sizex; x++)
		{
			Out.OutputNumberRaw(GetPixel(hdc, x, y), 3);
		}
	}

	hbmp = (HBITMAP) SelectObject(hdc, oldbmp);
	DeleteDC(hdc);
	DeleteObject(oldbmp);

	ReleaseDC(0, dc);
	DeleteDC(dc);

	return 1;
}

HBITMAP LoadRAS(char *file)
{
	HDC		 dc;
	InStream	 In(STREAM_FILE, file);
	HDC		 hdc;
	HBITMAP		 hbmp;
	HBITMAP		 oldbmp;
	unsigned int	 rasdepth;
	unsigned int	 raslength;
	unsigned int	 rastype;
	unsigned int	 maptype;
	unsigned int	 maplength;
	int		 sizex;
	int		 sizey;

	if (In.InputNumberRaw(4) == 0x59a66a95)
	{
		sizex		= In.InputNumberRaw(4);
		sizey		= In.InputNumberRaw(4);
		rasdepth	= In.InputNumberRaw(4);
		raslength	= In.InputNumberRaw(4);
		rastype		= In.InputNumberRaw(4);
		maptype		= In.InputNumberRaw(4);
		maplength	= In.InputNumberRaw(4);

		if ((rasdepth == 24) && ((rastype == 0) || (rastype == 1)))
		{
			dc = GetWindowDC(0);

			hdc = CreateCompatibleDC(dc);
			hbmp = CreateBitmap(sizex, sizey, GetDeviceCaps(hdc, PLANES), GetDeviceCaps(hdc, BITSPIXEL), NULL);
			oldbmp = (HBITMAP) SelectObject(hdc, hbmp);

			for (int y = 0; y < sizey; y++)
			{
				for (int x = 0; x < sizex; x++)
				{
					SetPixel(hdc, x, y, In.InputNumberRaw(3));
				}

				if ((3 * sizex) & 1 == 1) In.InputNumber(1);
			}

			hbmp = (HBITMAP) SelectObject(hdc, oldbmp);
			DeleteDC(hdc);
			DeleteObject(oldbmp);

			ReleaseDC(0, dc);
			DeleteDC(dc);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	return hbmp;
}
