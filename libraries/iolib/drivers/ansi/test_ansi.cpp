#include <iolib-cxx.h>
#include "driver_ansi.h"

int main()
{
	IOLibDriver	*driver = new IOLibDriverANSI("test_ansi.out", OS_OVERWRITE);
	OutStream	*out = new OutStream(STREAM_DRIVER, driver);

	out->OutputString("Testing the new IOLib Driver technology.");

	delete out;
	delete driver;

	return 0;
}
