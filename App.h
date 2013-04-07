/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef BETICK_APP_H
#define BETICK_APP_H

#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>
#include <stdio.h>
#include <time.h>
#include "TickWindow.h"
#include "TickCore.h"

/**
 * The Application
 */
class TickApp : public BApplication {
public:
							TickApp();
	virtual void			MessageReceived(BMessage* mesage);
		
private:
			TickWindow 		*fWindow;
};

#endif
