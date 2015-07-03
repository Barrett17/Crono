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
 *		Davide Gessa, dak.linux@gmail.com
 */

#include "App.h"

#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>

#include "Core.h"
#include "CronoDefaults.h"
#include "CronoWindow.h"

App::App() 
	: 
	BApplication(CRONO_APP_TYPE)
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
