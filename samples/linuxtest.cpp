#include <smooth.h>
#include <smooth/main.h>

#include <iostream>

using namespace smooth;
using namespace smooth::GUI;
using namespace std;

class LinuxTest : public Application
{
	private:
		GUI::Window	*mainWnd;
	public:
				 LinuxTest();
				~LinuxTest();
};

Int smooth::Main()
{
	String	 s("This is a smooth String!");

	cout << s.ConvertTo("ISO-8859-1") << "\n";

	LinuxTest	*app = new LinuxTest();

	sleep(5);

	Object::DeleteObject(app);

	return 0;
}

LinuxTest::LinuxTest()
{
	mainWnd	= new GUI::Window();

	mainWnd->SetMetrics(Point(100, 100), Size(400, 250));

	Add(mainWnd);

	mainWnd->Show();
}

LinuxTest::~LinuxTest()
{
	DeleteObject(mainWnd);
}
