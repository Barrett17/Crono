/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef CRONO_WINDOW_H
#define CRONO_WINDOW_H

#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>
#include <stdio.h>
#include <time.h>
#include "CronoView.h"


/**
 * Finestra del metronomo
 */
class CronoWindow : public BWindow {
public:
							CronoWindow(BRect frame);
	virtual void			MessageReceived(BMessage* mesage);
	virtual bool    		QuitRequested();


private:
			CronoView* 		fCronoView;
};



#endif
