/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#include "App.h"

#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>

#include "Core.h"
#include "CronoWindow.h"

App::App() 
	: 
	BApplication("application/x-vnd.Haiku-Crono")
{
	BRect windowRect;
	windowRect.Set(50,50,410,365);
	fWindow = new CronoWindow(windowRect);
	fWindow->Show();
}


void
App::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		default:
			BApplication::MessageReceived(message);
	}

}
