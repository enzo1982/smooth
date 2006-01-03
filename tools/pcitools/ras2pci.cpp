#include "ras.h"
#include <iostream>

#include <smooth/pciio.h>

using namespace std;

int main()
{
	char rfn[1024];
	char pfn[1024];
	HBITMAP bmp;
	int im_id = 0;
	int compression = 1;
	int bpc = 8;
	int color = 0;

	cout << "Raster filename: ";
	cin >> rfn;
	cout << "Loading file...";
	bmp = LoadRAS(rfn);
	cout << "done.\n";
	cout << "PCI filename: ";
	cin >> pfn;
	cout << "Image ID:";
	cin >> im_id;
	cout << "Available compression formats:\n0 = uncompressed\n1 = PCI\n2 = JPEG (not implemented yet)\n3 = RLE\n4 = BZIP2\n\nCompression format:";
	cin >> compression;
	cout << "Available color formats:\n0 = RGB (3 channels)\n1 = HSV (3 channels)\n2 = YUV (3 channels)\n3 = CMY (3 channels)\n4 = CMYK (4 channels)\n5 = Greyscale (1 channel)\n\nColor format:";
	cin >> color;
	cout << "Bits per channel:";
	cin >> bpc;
	cout << "Writing file...";

	PCIOut	 pci = OpenPCIForOutput(pfn);
	PCIIO	*pio = new PCIIO(bmp);

	pio->SetCompressionType(compression);
	pio->SetColorspace(color);
	pio->SetBitsPerChannel(bpc);
	pio->SetImageID(im_id);

	WritePCI(pci, *pio);

	ClosePCI(pci);

	delete pio;

	cout << "done.\n";

	return 0;
}
