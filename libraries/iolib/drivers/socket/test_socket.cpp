#include <iolib-cxx.h>
#include "driver_socket.h"
#include "../ansi/driver_ansi.h"

int main()
{
	IOLibDriver	*console = new IOLibDriverANSI(stdout);
	OutStream	*out = new OutStream(STREAM_DRIVER, console);

	IOLibDriver	*driver = new IOLibDriverSocket("localhost", 80);
	InStream	*in = new InStream(STREAM_DRIVER, driver);

	out->OutputLine(in->InputString(100));

	delete in;
	delete driver;

	delete out;
	delete console;

	return 0;
}
