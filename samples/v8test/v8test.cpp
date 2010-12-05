#include <smooth.h>
#include <smooth/main.h>

using namespace smooth;
using namespace smooth::GUI::Dialogs;

#include <v8/v8.h>

using namespace v8;

Int smooth::Main()
{
	// Create a stack-allocated handle scope.
	HandleScope handle_scope;

	// Create a new context.
	Persistent<Context> context = Context::New();

	// Enter the created context for compiling and
	// running the hello world script. 
	Context::Scope context_scope(context);

	// Create a string containing the JavaScript source code.
	Handle<v8::String> source = v8::String::New("'Hello' + ', World!'");
	Handle<v8::String> file = v8::String::New("unnamed");

	// Compile the source code.
	Local<Script> script = Script::Compile(source, file);

	// Run the script to get the result.
	Handle<Value> result = script->Run();

	// Dispose the persistent context.
	context.Dispose();

	// Convert the result to an ASCII string and print it.
	v8::String::AsciiValue ascii(result);

	QuickMessage((char *) *ascii, "V8 Engine Test", MB_OK, IDI_INFORMATION);

	return 0;
}
