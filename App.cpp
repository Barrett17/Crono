/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>

#include "TickApp.h"
#include "TickCore.h"
#include "TickWindow.h"

TickApp::TickApp() 
	: 
	BApplication("application/x-vnd.Haiku-BeTick")
{
	BRect windowRect;
	windowRect.Set(50,50,410,365);
	fWindow = new TickWindow(windowRect);
	fWindow->Show();
}



void
TickApp::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		default:
			BApplication::MessageReceived(message);
	}

}

