#include <iolib-cxx.h>
#include "driver_posix.h"

int main()
{
	IOLibDriver	*driver = new IOLibDriverPOSIX("test_posix.out", OS_OVERWRITE);
	OutStream	*out = new OutStream(STREAM_DRIVER, driver);

	out->OutputString("Testing the new IOLib Driver technology.");

	delete out;
	delete driver;

	return 0;
}
