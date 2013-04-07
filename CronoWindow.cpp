/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 *		Barrett, b.vitruvio@gmail.com
 */
#include "CronoWindow.h"

#include <GroupLayout.h>
#include <Window.h>

#include "App.h"
#include "Core.h"


CronoWindow::CronoWindow(BRect frame) 
		:
		BWindow(frame, "Crono", B_TITLED_WINDOW,
					B_NOT_ZOOMABLE)
{	
	SetLayout(new BGroupLayout(B_VERTICAL));
	fCronoView = new CronoView();
	
	GetLayout()->AddView(fCronoView);
}


void
CronoWindow::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		default:
			BWindow::MessageReceived(message);
	}
}



bool
CronoWindow::QuitRequested()
{
	fCronoView->RemoveSelf();
	delete fCronoView;

	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
