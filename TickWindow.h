/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef BETICK_WINDOW_H
#define BETICK_WINDOW_H

#include <Application.h>
#include <GroupLayout.h>
#include <Window.h>
#include <View.h>
#include <stdio.h>
#include <time.h>
#include "TickView.h"



/**
 * Finestra del metronomo
 */
class TickWindow : public BWindow {
public:
							TickWindow(BRect frame);
	virtual void			MessageReceived(BMessage* mesage);
	virtual bool    		QuitRequested();


private:
			TickView* 		fTickView;
};



#endif
