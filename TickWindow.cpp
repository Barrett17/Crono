/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 *		Barrett, barrett666@gmail.com
 */

#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>
#include <Shelf.h>
#include "TickApp.h"
#include "TickCore.h"

TickWindow::TickWindow(BRect frame) 
		:
		BWindow(frame, "BeTick", B_TITLED_WINDOW,
					B_NOT_ZOOMABLE)
{	
	SetLayout(new BGroupLayout(B_VERTICAL));
	fTickView = new TickView();
	
	GetLayout()->AddView(fTickView);
}


void
TickWindow::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		default:
			//fTickView->MessageReceived(message);
			BWindow::MessageReceived(message);
	}

}



bool
TickWindow::QuitRequested()
{
	fTickView->RemoveSelf();
	delete fTickView;

	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
