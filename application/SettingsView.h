/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 *		Dario Casalinuovo
 */
#ifndef CRONO_SETTINGSVIEW_H
#define CRONO_SETTINGSVIEW_H

#include <Button.h>
#include <Menu.h>
#include <MenuBar.h>
#include <RadioButton.h>
#include <SupportDefs.h>
#include <TextControl.h>
#include <View.h>

#include "Core.h"

const int32 MSG_DEFAULTS 	= 0xA1;
const int32 MSG_REVERT	 	= 0xA2;
const int32 MSG_SET		 	= 0xA3;
const int32 MSG_SET_ENGINE	= 0xA4;


class SettingsView : public BView {
public:
							SettingsView(Core* core);
							~SettingsView();
			void			AttachedToWindow();
	virtual	void			MessageReceived(BMessage* message);

private:
			void			_UpdateData();
			BMessage*		_ButtonMsg(int32 engineval);
			void			_SetEngine(int32 engine);

			BRadioButton*	fEngines[4];

			BTextControl*	fSoundEntry;
			BButton*		fDefaultsButton;
			BButton*		fRevertButton;

			Core*			fCore;
};

#endif
