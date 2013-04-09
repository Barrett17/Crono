/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
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
