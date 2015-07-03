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

#include "SettingsView.h"

#include <GroupLayout.h>
#include <GroupView.h>
#include <Box.h>
#include <CheckBox.h>
#include <LayoutBuilder.h>

#include "CronoDefaults.h"

#include <stdio.h>

// TODO should save settings on window close.

SettingsView::SettingsView(Core* core)
	:
	BView("SettingsView", B_WILL_DRAW, 0),
	fCore(core)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// Engines entry
	BBox* audioBox = new BBox("audiobox");
	audioBox->SetLabel("Engines");

	BGroupLayout* audioLayout = new BGroupLayout(B_VERTICAL);
	audioLayout->SetInsets(10, audioBox->TopBorderOffset() * 2 + 10, 10, 10);
	audioBox->SetLayout(audioLayout);

	fEngines[0] = new BRadioButton("sine", "Sine",
		_ButtonMsg(CRONO_SINE_ENGINE));
	audioLayout->AddView(fEngines[0]);

	fEngines[1] = new BRadioButton("triangle", "Triangle",
		_ButtonMsg(CRONO_TRIANGLE_ENGINE));
	audioLayout->AddView(fEngines[1]);

	fEngines[2] = new BRadioButton("sawtooth", "Sawtooth",
		_ButtonMsg(CRONO_SAWTOOTH_ENGINE));
	audioLayout->AddView(fEngines[2]);

	fEngines[3] = new BRadioButton("file", "File Engine",
		_ButtonMsg(CRONO_FILE_ENGINE));
	audioLayout->AddView(fEngines[3]);

	fSoundEntry = new BTextControl("Soundfile", "Soundfile",
		gCronoSettings.SoundFileLocation, new BMessage(MSG_SET), B_WILL_DRAW);
	fSoundEntry->SetDivider(70);
	fSoundEntry->SetAlignment(B_ALIGN_CENTER, B_ALIGN_CENTER);
	audioLayout->AddView(fSoundEntry);
	fSoundEntry->SetEnabled(false);

	fDefaultsButton = new BButton("Defaults", new BMessage(MSG_DEFAULTS));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 5)
		.AddGroup(B_VERTICAL)
			.Add(audioBox, 0)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fDefaultsButton, 0)
		.End();

	_SetEngine(fCore->Engine());
}


SettingsView::~SettingsView()
{
	// Save the data
	 _UpdateData();
}


void
SettingsView::AttachedToWindow()
{
	fDefaultsButton->SetTarget(this);
	fSoundEntry->SetTarget(this);	

	for (int i = 0; i < 4; i++)
		fEngines[i]->SetTarget(this);

	Window()->CenterOnScreen();
}


void
SettingsView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case MSG_DEFAULTS:
		{
			fSoundEntry->SetText(CRONO_SOUNDFILE_LOCATION);
			_SetEngine(CRONO_SINE_ENGINE);
		}
		break;

		case MSG_SET_ENGINE:
		{
			int32 engine = 0;
			message->FindInt32("engineval", &engine);
			_SetEngine(engine);
		}
		break;

		case MSG_SET:
			_UpdateData();
		break;

		default:
			BView::MessageReceived(message);
	}			
}


void
SettingsView::_UpdateData()
{
	gCronoSettings.SoundFileLocation.SetTo(
		fSoundEntry->Text());
}


BMessage*
SettingsView::_ButtonMsg(int32 engineval)
{
	BMessage* msg = new BMessage(MSG_SET_ENGINE);
	msg->AddInt32("engineval", engineval);
	return msg;
}


void
SettingsView::_SetEngine(int32 engine) {
	if (engine != fCore->Engine())
		fCore->SetEngine(engine);

	if (engine == CRONO_FILE_ENGINE)
		fSoundEntry->SetEnabled(true);
	else
		fSoundEntry->SetEnabled(false);

	for (int i = 0; i < 4; i++) {
		if (i != engine) {
			fEngines[i]->SetEnabled(true);
			fEngines[i]->SetValue(0);
			continue;
		}
		fEngines[i]->SetValue(1);
		fEngines[i]->SetEnabled(false);
	}
}
