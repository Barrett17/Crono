/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#include "App.h"

int 
main()
{
	App *cronoApp = new App();
	
	cronoApp->Run();
	
	delete cronoApp;	
}
