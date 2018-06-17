#include <iostream>
#include "App.h"
#include "RayFileLoader.h"

int main()
{
	//Define erro code
	int errc = 0;
	
	//Initialize App
	errc = rvApp.Initialize(1920, 1080, "TRayTracer", false, false);

	//Run App if no error
	if (!errc)
		errc = rvApp.Run();

	//Return result
	return errc;
}