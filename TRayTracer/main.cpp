#include <iostream>
#include "App.h"

int main()
{
	//Define erro code
	int errc = 0;

	//Initialize App
	errc = rvApp.Initialize(3840, 2160, "Yeah", false);

	//Run App if no error
	if (!errc)
		errc = rvApp.Run();

	//Return result
	return errc;
}