#include <smooth.h>
#include <smooth/main.h>

using namespace smooth;
using namespace smooth::GUI::Dialogs;

#include <smooth-js/v8.h>

using namespace v8;

Int smooth::Main()
{
	// Create a new isolate.
	Isolate			*isolate = v8::Isolate::New();

	// Enter the created isolate. 
	{
		Isolate::Scope		 isolateScope(isolate);

		// Create a stack-allocated handle scope.
		HandleScope		 handleScope(isolate);

		// Create a new context.
		Local<Context>		 context = Context::New(isolate);

		// Enter the created context for compiling and
		// running the hello world script. 
		Context::Scope		 contextScope(context);

		// Create a string containing the JavaScript source code.
		Handle<v8::String>	 source = v8::String::New("'Hello' + ', World!'");
		Handle<v8::String>	 file = v8::String::New("unnamed");

		// Compile the source code.
		Local<Script>		 script = Script::Compile(source, file);

		// Run the script to get the result.
		Handle<Value>		 result = script->Run();

		// Convert the result to an ASCII string and print it.
		v8::String::AsciiValue ascii(result);

		QuickMessage((char *) *ascii, "V8 Engine Test", GUI::Dialogs::Message::Buttons::Ok, GUI::Dialogs::Message::Icon::Information);
	}

	// Dispose the isolate.
	isolate->Dispose();

	return 0;
}
