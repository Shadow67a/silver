#include "../include/silver.hpp"

struct contentView : silver::View
{
	contentView()
	{
		background(silver::hex(0xff2e54));
	};
};

int main()
{
	silver::App app(800, 600, "windowTest");
	app.mainView(contentView())
	   .run();
	return 0;
};
