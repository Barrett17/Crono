/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef CRONO_SETTINGSVIEW_H
#define CRONO_SETTINGSVIEW_H

#include <Button.h>
#include <Menu.h>
#include <MenuBar.h>
#include <SupportDefs.h>
#include <TextControl.h>
#include <View.h>

const int32 MSG_DEFAULTS = 0xA1;
const int32 MSG_REVERT	 = 0xA2;


class SettingsView : public BView {
public:
							SettingsView();
							~SettingsView();
			void			AttachedToWindow();	
	virtual	void			MessageReceived(BMessage* message);

private:
			BTextControl*	fTicSoundEntry;
			BTextControl*	fTocSoundEntry;
			BButton*		fDefaultsButton;
			BButton*		fRevertButton;
};

#endif
