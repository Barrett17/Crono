/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
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

// TODO should save settings on window close.

SettingsView::SettingsView()
	:
	BView("SettingsView", B_WILL_DRAW, 0)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// Engines entry
	BBox* audioBox = new BBox("audiobox");
	audioBox->SetLabel("Engines");

	BGroupLayout* audioLayout = new BGroupLayout(B_VERTICAL);
	audioLayout->SetInsets(10, audioBox->TopBorderOffset() * 2 + 10, 10, 10);
	audioBox->SetLayout(audioLayout);

	fSineEngine = new BRadioButton("sine", "Sine",
		new BMessage(MSG_SET_ENGINE));
	audioLayout->AddView(fSineEngine);

	fTriangleEngine = new BRadioButton("triangle", "Triangle",
		new BMessage(MSG_SET_ENGINE));
	audioLayout->AddView(fTriangleEngine);

	fSawtoothEngine = new BRadioButton("sawtooth", "Sawtooth",
		new BMessage(MSG_SET_ENGINE));
	audioLayout->AddView(fSawtoothEngine);

	fFileEngine = new BRadioButton("file", "File Engine",
		new BMessage(MSG_SET_ENGINE));
	audioLayout->AddView(fFileEngine);

	fTocSoundEntry = new BTextControl("Soundfile", "Soundfile",
		gCronoSettings.TocLocation, new BMessage(MSG_SET), B_WILL_DRAW);
	fTocSoundEntry->SetDivider(70);
	fTocSoundEntry->SetAlignment(B_ALIGN_CENTER, B_ALIGN_CENTER);
	audioLayout->AddView(fTocSoundEntry);

	fDefaultsButton = new BButton("Defaults", new BMessage(MSG_DEFAULTS));
	fRevertButton = new BButton("Revert", new BMessage(MSG_REVERT));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 5)
		.AddGroup(B_VERTICAL)
			.Add(audioBox, 0)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fDefaultsButton, 0)
			.Add(fRevertButton, 1)
		.End();
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
	fRevertButton->SetTarget(this);
	fTocSoundEntry->SetTarget(this);	
	
	Window()->CenterOnScreen();
}


void
SettingsView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case MSG_REVERT:
		case MSG_DEFAULTS:
		{
			fTocSoundEntry->SetText(CRONO_TOC_LOCATION);
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
	gCronoSettings.TocLocation.SetTo(
		fTocSoundEntry->Text());
}
