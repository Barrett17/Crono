/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 *		Dario Casalinuovo
 */
#ifndef BETICK_VIEW_H
#define BETICK_VIEW_H

class BMenuBar;
class BMenu;
class BButton;
class BSlider;
class BTextControl;
class BRadioButton;
class TickCore;

const int32 MSG_SETTINGS			= 0x5;
const int32 MSG_EDIT				= 0x6;
const int32 MSG_ABOUT				= 0x7;
const int32 MSG_CLOSE				= 0x8;
const int32 MSG_START				= 0x9;
const int32 MSG_STOP				= 0x10;
const int32 MSG_VOLUME				= 0x11;
const int32 MSG_SPEED_SLIDER		= 0x12;
const int32 MSG_SPEED_ENTRY			= 0x13;
const int32 MSG_METER_ENTRY			= 0x14;
const int32 MSG_METER_RADIO			= 0x15;

/**
 * View del metronomo
 */
class TickView : public BView {
public:
							TickView();
							~TickView();
			void			AttachedToWindow();	
	virtual	void			MessageReceived(BMessage* message);

private:
			BMenuBar		*fMenuBar;
			BMenu			*fFileMenu;
			BMenu			*fEditMenu;
			BButton			*fStartButton;
			BButton			*fStopButton;
			BSlider			*fVolumeSlider;

			BRadioButton	*fMeterRadios[5];
			BTextControl	*fMeterEntry;

			BTextControl	*fSpeedEntry;
			BSlider			*fSpeedSlider;
			TickCore		*fTickCore;
};



#endif
