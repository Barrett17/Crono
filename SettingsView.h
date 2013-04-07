/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef BETICK_SETTINGSVIEW_H
#define BETICK_SETTINGSVIEW_H

class BMenuBar;
class BMenu;
class BButton;
class BSlider;
class BTextControl;

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
