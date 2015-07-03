/*
 * Copyright (c) 2012-2015  Dario Casalinuovo (b.vitruvio@gmail.com)
 * Copyright (c) 2012-2015  Davide Gessa (gessadavide@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Authors:
 *		Casalinuovo Dario, b.vitruvio@gmail.com
 *		Davide Gessa, dak.linux@gmail.com
 */
#include "CronoWindow.h"

#include <GroupLayout.h>
#include <Window.h>

#include "App.h"


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
