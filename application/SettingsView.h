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
 *		Casalinuovo Dario, b.vitruvio@gmail.com
 *		Davide Gessa, dak.linux@gmail.com
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

			Core*			fCore;
};

#endif
