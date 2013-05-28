/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 *		Dario Casalinuovo
 */
#ifndef CRONO_VIEW_H
#define CRONO_VIEW_H

#include <CheckBox.h>
#include <ObjectList.h>
#include <GroupView.h>
#include <SupportDefs.h>
#include <View.h>

class BBox;
class BMenuBar;
class BMenu;
class BMenuItem;
class BButton;
class BSlider;
class BTextControl;
class BRadioButton;
class Core;

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
const int32 MSG_ACCENT_TABLE		= 0x16;
const int32 MSG_HELP				= 0x17;
const int32 MSG_HOMEPAGE			= 0x18;

class _EXPORT RepliView;


/**
 * View del metronomo
 */
class CronoView : public BView {
public:
							CronoView();
							CronoView(BMessage* archive);

							~CronoView();

			void			AboutRequested();
			void			AttachedToWindow();	
	virtual	void			MessageReceived(BMessage* message);

	static 	BArchivable*	Instantiate(BMessage* archive);
	virtual status_t		Archive(BMessage* data, bool deep = true) const;

private:
			void			_BuildMenu();
			void			_UpdateTempoName(int32 value);
			int				_GetCurrentMeter();
			void			_SetAccentCheckBox(int value);
			void			_ShowTable(bool show);

			BMenuBar*		fMenuBar;
			BMenu*			fHelpMenu;
			BMenu*			fFileMenu;
			BMenu*			fEditMenu;
			BMenu*			fShowMenu;

			BMenuItem*		fAccentTableItem;

			BButton*		fStartButton;
			BButton*		fStopButton;
			BSlider*		fVolumeSlider;

			BRadioButton*	fTempoRadios[5];
			BTextControl*	fTempoEntry;

			BTextControl*	fSpeedEntry;
			BSlider*		fSpeedSlider;
			Core*			fCore;
			bool			fReplicated;

			BGroupView*	fAccentsView;

			BObjectList<BCheckBox> fAccentsList;
};

#endif
