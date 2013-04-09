/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */

#include "SettingsWindow.h"

#include <GroupLayout.h>
#include <Window.h>

#include "SettingsView.h"


SettingsWindow::SettingsWindow(BRect frame) 
	:
	BWindow(frame, "Settings", B_TITLED_WINDOW,
		B_NOT_ZOOMABLE)
{	
	SetLayout(new BGroupLayout(B_VERTICAL));

	fSettingsView = new SettingsView();
	GetLayout()->AddView(fSettingsView);
}


void
SettingsWindow::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		default:
			BWindow::MessageReceived(message);
	}
}

/*
void
SettingsWindow::Quit()
{

}*/


bool
SettingsWindow::QuitRequested()
{
	return true;
}
