#include <smooth.h>
#include <smooth/main.h>
#include <smooth/args.h>

#include <smooth/pciio.h>
#include <iostream>

#include "ras.h"

using namespace std;
using namespace smooth;

Int smooth::Main(const Array<String> &args)
{
	if (args.Length() < 6)
	{
		cout << "RAS2PCI converter v0.2\n";
		cout << "Copyright (C) 1998-2009 Robert Kausch\n\n";
		cout << "Usage: ras2pci <ras file> <pci file> <image id> <compression> <color> <depth>\n\n";
		cout << "<compression> is one of these compression formats:\n";
		cout << "\t0 = uncompressed\n";
		cout << "\t1 = PCI\n";
		cout << "\t2 = JPEG (not implemented yet)\n";
		cout << "\t3 = RLE\n";
		cout << "\t4 = BZIP2\n\n";
		cout << "<color> is one of these color formats:\n";
		cout << "\t0 = RGB (3 channels)\n";
		cout << "\t1 = HSV (3 channels)\n";
		cout << "\t2 = YUV (3 channels)\n";
		cout << "\t3 = CMY (3 channels)\n";
		cout << "\t4 = CMYK (4 channels)\n";
		cout << "\t5 = Greyscale (1 channel)\n\n";
		cout << "<depth> is the number of bits per channel.\n\n";

		return 0;
	}

	String	 rfn	= args.GetNth(0);
	String	 pfn	= args.GetNth(1);
	Int	 im_id	= args.GetNth(2).ToInt();
	Int	 comp	= args.GetNth(3).ToInt();
	Int	 color	= args.GetNth(4).ToInt();
	Int	 bpc	= args.GetNth(5).ToInt();

	cout << "Loading raster file...";

	HBITMAP	 bmp	= LoadRAS(rfn);

	cout << "done.\n";

	cout << "Writing PCI file...";

	PCIOut	 pci = OpenPCIForOutput(pfn);
	PCIIO	*pio = new PCIIO(bmp);

	pio->SetCompressionType(comp);
	pio->SetColorspace(color);
	pio->SetBitsPerChannel(bpc);
	pio->SetImageID(im_id);

	WritePCI(pci, *pio);

	ClosePCI(pci);

	delete pio;

	cout << "done.\n";

	return 0;
}
