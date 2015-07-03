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

#ifndef CRONO_APP_H
#define CRONO_APP_H

#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>

#include "CronoWindow.h"
#include "Core.h"

#include <stdio.h>
#include <time.h>

/**
 * The Application
 */
class App : public BApplication {
public:
							App();
	virtual void			MessageReceived(BMessage* mesage);
		
private:
			CronoWindow 	*fWindow;
};

#endif
