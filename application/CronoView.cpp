/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Casalinuovo Dario, b.vitruvio@gmail.com
 *		Davide Gessa, dak.linux@gmail.com
 */
#include "CronoView.h"

#include <Alert.h>
#include <Button.h>
#include <Dragger.h>
#include <GroupView.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Roster.h>
#include <Slider.h>
#include <TextControl.h>
#include <View.h>
#include <Box.h>
#include <RadioButton.h>

#include "App.h"
#include "Core.h"
#include "CronoDefaults.h"
#include "SettingsWindow.h"

#include <math.h>
#include <stdlib.h>

struct TempoNames {
	int32 max;
	int32 min;
	const char* name;
};

static TempoNames gTempoNames[] = {
	{ 10, 39, "Grave" },
	{ 40, 59, "Largo" },
	{ 60, 65, "Larghetto" },
	{ 66, 75, "Lento/Adagio" },
	{ 76, 107, "Andante" },
	{ 108, 119, "Moderato" },
	{ 120, 139, "Allegro" },
	{ 140, 167, "Vivace" },
	{ 168, 200, "Presto" },
	{ 201, 500, "Prestissimo" },
	{ 0, 0, NULL }
};


CronoView::CronoView()
	:
	BView("CronoView", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE)
{
    fReplicated = false;

	// Core
	fCore = new Core();

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	rgb_color barColor = { 0, 0, 240 };
	rgb_color fillColor = { 240, 0, 0 };

	// Menu bar
	fMenuBar = new BMenuBar("MenuBar");

	fFileMenu = new BMenu("Metronome");

	fFileMenu->AddItem(new BMenuItem("Quit", new BMessage(MSG_CLOSE), 'Q'));
	fFileMenu->AddItem(new BSeparatorItem);
	fFileMenu->AddItem(new BMenuItem("Start", new BMessage(MSG_START), 'G'));
	fFileMenu->AddItem(new BMenuItem("Stop", new BMessage(MSG_STOP), 'H'));
	fMenuBar->AddItem(fFileMenu);
	
	fEditMenu = new BMenu("Edit");
	fEditMenu->AddItem(new BMenuItem("Settings", new BMessage(MSG_SETTINGS), 'S'));
	fEditMenu->AddItem(new BSeparatorItem);

	fShowMenu = new BMenu("Show");
	fEditMenu->AddItem(fShowMenu);
	
	BMenuItem* item = new BMenuItem("Visual Metronome", NULL, 0);
	item->SetEnabled(false);
	fShowMenu->AddItem(item);
	fShowMenu->AddItem(new BMenuItem("Show accents table", 
		new BMessage(MSG_ACCENT_TABLE), 0));

	fMenuBar->AddItem(fEditMenu);

	fHelpMenu = new BMenu("Help");
	fHelpMenu->AddItem(new BMenuItem("Help", new BMessage(MSG_HELP), 'H'));
	fHelpMenu->AddItem(new BMenuItem("Homepage", new BMessage(MSG_HOMEPAGE), 'P'));
	fHelpMenu->AddItem(new BSeparatorItem);
	fHelpMenu->AddItem(new BMenuItem("About", new BMessage(MSG_ABOUT), 'A'));
	fMenuBar->AddItem(fHelpMenu);

	// Volume slider
	BBox* volBox = new BBox("volbox");
	volBox->SetLabel("Volume");
	BGroupLayout* volLayout = new BGroupLayout(B_VERTICAL);
	volLayout->SetInsets(10, volBox->TopBorderOffset() * 2 + 10, 10, 10);
	volBox->SetLayout(volLayout);
	
	fVolumeSlider = new BSlider("", "", new BMessage(MSG_VOLUME),
		1, 100, B_HORIZONTAL);

	fVolumeSlider->SetLimitLabels("Min", "Max");
	fVolumeSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fVolumeSlider->SetHashMarkCount(10);
	fVolumeSlider->SetBarColor(barColor);
	fVolumeSlider->SetValue((int32)fCore->Volume()*10);
	fVolumeSlider->UseFillColor(true, &fillColor);
	fVolumeSlider->SetPosition(gCronoSettings.CronoVolume);

	volLayout->AddView(fVolumeSlider);

	// Speed Slider & TextControl
	BBox* speedBox = new BBox("speedbox");
	speedBox->SetLabel("BPM");
	BGroupLayout* speedLayout = new BGroupLayout(B_HORIZONTAL);
	speedLayout->SetInsets(10, speedBox->TopBorderOffset() * 2 + 10, 10, 10);
	speedBox->SetLayout(speedLayout);
	
	fSpeedSlider = new BSlider("", "",
		new BMessage(MSG_SPEED_SLIDER), MIN_SPEED, MAX_SPEED, B_HORIZONTAL);

	fSpeedSlider->SetLimitLabels(BString() << MIN_SPEED,
		BString() << MAX_SPEED);

	fSpeedSlider->SetKeyIncrementValue(5);
	fSpeedSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fSpeedSlider->SetHashMarkCount(10);
	fSpeedSlider->SetBarColor(barColor);
	fSpeedSlider->SetValue(fCore->Speed());
	fSpeedSlider->UseFillColor(true, &fillColor);
	
	
	fSpeedEntry = new BTextControl("", "", BString() << gCronoSettings.Speed,
		new BMessage(MSG_SPEED_ENTRY), B_WILL_DRAW);

	fSpeedEntry->SetDivider(70);
	fSpeedEntry->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_RIGHT);
	fSpeedEntry->SetExplicitSize(BSize(35, 20));
	
	speedLayout->AddView(fSpeedSlider);
	speedLayout->AddView(fSpeedEntry);

	// Meter buttons
	BBox* meterBox = new BBox("meterbox");
	meterBox->SetLabel("Meter");
	BGroupLayout* meterLayout = new BGroupLayout(B_HORIZONTAL);
	meterLayout->SetInsets(10, meterBox->TopBorderOffset() * 2 + 10, 10, 10);
	meterBox->SetLayout(meterLayout);

	for(int i = 0; i < 5; i++)
		fMeterRadios[i] = new BRadioButton("", "", new BMessage(MSG_METER_RADIO));

	fMeterRadios[0]->SetLabel("1/4");
	fMeterRadios[1]->SetLabel("2/4");
	fMeterRadios[2]->SetLabel("3/4");
	fMeterRadios[3]->SetLabel("4/4");
	fMeterRadios[3]->SetValue(1);
	fMeterRadios[4]->SetLabel("Other");

	fMeterEntry = new BTextControl("", "", "4",
		new BMessage(MSG_METER_ENTRY), B_WILL_DRAW);

	fMeterEntry->SetDivider(70);
	fMeterEntry->SetEnabled(false);

	for(int i= 0; i < 5; i++)
		meterLayout->AddView(fMeterRadios[i]);
	meterLayout->AddView(fMeterEntry);


	// Start and stop button	
	BGroupView* buttonGroup = new BGroupView(B_HORIZONTAL);

	fStartButton = new BButton("Start", new BMessage(MSG_START));						
	fStartButton->MakeDefault(true);	
	buttonGroup->GroupLayout()->AddView(fStartButton);

	fStopButton = new BButton("Stop", new BMessage(MSG_STOP));							
	buttonGroup->GroupLayout()->AddView(fStopButton);

	// Dragger
	BRect frame(Bounds());
	frame.left = frame.right - 7;
	frame.top = frame.bottom - 7;
	BDragger *dragger = new BDragger(frame, this,
		B_FOLLOW_RIGHT | B_FOLLOW_TOP); 

	// Create view
	BLayoutBuilder::Group<>(this, B_VERTICAL, 5)
		.AddGroup(B_VERTICAL)
			.Add(fMenuBar, 0)
			.Add(volBox, 1)
			.Add(speedBox, 2)
			.Add(meterBox, 3)
			.Add(buttonGroup, 4)
			//.Add(dragger, 5)
		.End();
}


CronoView::CronoView(BMessage* archive)
	:
	BView(archive)
{
    fReplicated = true;

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// Core
	fCore = new Core();

	fMenuBar = new BMenuBar(archive);
	fHelpMenu = new BMenu(archive);
	fFileMenu = new BMenu(archive);
	fEditMenu = new BMenu(archive);
	fStartButton = new BButton(archive);
	fStopButton = new BButton(archive);
	fVolumeSlider = new BSlider(archive);
	
	for(int i = 0; i < 5; i++)
		fMeterRadios[i] = new BRadioButton(archive);
	fMeterEntry = new BTextControl(archive);
	fSpeedEntry = new BTextControl(archive);
	fSpeedSlider = new BSlider(archive);

/*	be_app->Lock();
	be_app->AddHandler(this);
	be_app->Unlock();*/
	fReplicated = true;
}


CronoView::~CronoView()
{
	fCore->Stop();
	delete fCore;
}


void
CronoView::AboutRequested()
{
	BAlert *alert = new BAlert("About Crono", 
	"\nCrono Metronome v1.0 Alpha1\n"
	"Copyright ©2009-2013 Dario Casalinuovo\n\n"
	"Copyright ©2009-2013 Davide Gessa \n\n"
	"Crono is a Software Metronome for Haiku\n"
	"Crono is part of StilNovo - http://www.versut.com/\n\n"
	"Written By:\n"
	"Dario Casalinuovo (GUI, Core)\n"
	"Davide Gessa (GUI)\n\n"
	"Released under the terms of the MIT license.\n\n"
	"Special Thanks :\n Stefano D'Angelo",
	"OK", NULL, NULL, B_WIDTH_FROM_WIDEST, B_EVEN_SPACING, B_INFO_ALERT);
	alert->Go();
}


status_t
CronoView::Archive(BMessage* archive, bool deep) const
{
    BView::Archive(archive, deep);
    archive->AddString("add_on", CRONO_APP_TYPE);
    archive->AddString("class", "CronoView");
    return B_OK;
}


BArchivable*
CronoView::Instantiate(BMessage *data)
{
    return new CronoView(data);
}


void
CronoView::AttachedToWindow()
{
	fStartButton->SetTarget(this);
	fStopButton->SetTarget(this);
	fVolumeSlider->SetTarget(this);

	for(int i = 0; i < 5; i++)
		fMeterRadios[i]->SetTarget(this);
	fMeterEntry->SetTarget(this);
	
	fSpeedEntry->SetTarget(this);
	fSpeedSlider->SetTarget(this);

	fFileMenu->SetTargetForItems(this);
	fEditMenu->SetTargetForItems(this);
	fHelpMenu->SetTargetForItems(this);
	fShowMenu->SetTargetForItems(this);

	if (!fReplicated)
		Window()->CenterOnScreen();
}


void
CronoView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case MSG_CLOSE:
		{
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;
		}

		case MSG_ABOUT:
		{
			AboutRequested();
			break;
		}

		case MSG_HOMEPAGE:
		{
			const char* homepage = CRONO_HOMEPAGE_URL;
			be_roster->Launch("text/html",1, const_cast<char**>(&homepage));
			break;
		}

		case MSG_HELP:
		{
			const char* guide = CRONO_USERGUIDE_URL;
			be_roster->Launch("text/html",1, const_cast<char**>(&guide));
			break;
		}

		case MSG_SETTINGS:
		{
			BRect windowRect(150,150,460,445);
			SettingsWindow *settWindow = new SettingsWindow(windowRect, fCore);
			settWindow->Show();
			break;
		}

		case MSG_START:
		{
			status_t ret = fCore->Start();
			if (ret != B_OK) {
				BString str("\nError starting Crono :\n");
				str << strerror(ret);
				BAlert *alert = new BAlert("Error", 
					str.String(),
					"OK", NULL, NULL, B_WIDTH_FROM_WIDEST, B_EVEN_SPACING,
					B_INFO_ALERT);
					alert->Go();
				break;
			}
			fStartButton->SetEnabled(false);
			fStopButton->SetEnabled(true);
			fStopButton->MakeDefault(true);
			fEditMenu->FindItem(MSG_SETTINGS)->SetEnabled(false);
			fFileMenu->FindItem(MSG_START)->SetEnabled(false);
			fFileMenu->FindItem(MSG_STOP)->SetEnabled(true);
			printf("CronoView: Start\n");
			break;
		}

		case MSG_STOP:
		{
			fCore->Stop();
			fStopButton->SetEnabled(false);
			fStartButton->SetEnabled(true);
			fStartButton->MakeDefault(true);
			fEditMenu->FindItem(MSG_SETTINGS)->SetEnabled(true);
			fFileMenu->FindItem(MSG_START)->SetEnabled(true);
			fFileMenu->FindItem(MSG_STOP)->SetEnabled(false);

			printf("CronoView: Stop\n");
			break;
		}

		case MSG_VOLUME:
		{
			float position = fVolumeSlider->Position();
			fCore->SetVolume(position);
			printf("CronoView: Volume: %f\n", position);
			break;
		}

		case MSG_METER_RADIO:
		{
			int selected = 0;
			// Get the selected radiobutton
			for (int i = 0; i < 5; i++) {
				if (fMeterRadios[i]->Value())
					selected = i;
			}

			// If "Other" is selected, enable the fMeterEntry
			if (selected == 4) {
				fMeterEntry->SetEnabled(true);
			} else {
				fMeterEntry->SetEnabled(false);
				fCore->SetMeter(((int) selected + 1));
			}
			break;
		}

		case MSG_METER_ENTRY:
		{
			unsigned position = abs(atoi(fMeterEntry->Text()));
			fCore->SetMeter(((int) position));
			break;
		}

		case MSG_SPEED_ENTRY:
		{
			unsigned bpm = abs(atoi(fSpeedEntry->Text()));

			if (bpm > MAX_SPEED) {
				fSpeedEntry->SetText(BString() << MAX_SPEED);
				bpm = MAX_SPEED;
			} else if(bpm < MIN_SPEED) {
				fSpeedEntry->SetText(BString() << MIN_SPEED);
				bpm = MIN_SPEED;
			}

			fCore->SetSpeed(((int) bpm));
			fSpeedSlider->SetPosition(((float) bpm / MAX_SPEED));
			printf("Crono Speed: %s %d\n", fSpeedEntry->Text(), bpm);
			_UpdateTempoName(bpm);
			break;
		}

		case MSG_SPEED_SLIDER:
		{
			int v = fSpeedSlider->Value();

			char strv[32];
			sprintf(strv, "%d", v);
			fSpeedEntry->SetText(strv);
			fCore->SetSpeed(v);
			_UpdateTempoName(60);
			break;
		}

		case MSG_ACCENT_TABLE:
		{
			bool val = gCronoSettings.AccentTable;
			gCronoSettings.AccentTable = !val;
			break;
		}

		default:
			BView::MessageReceived(message);
	}
}


void
CronoView::_UpdateTempoName(int32 value)
{
	for (int i = 0; gTempoNames[i].name != NULL; i++) {
		if (value <= gTempoNames[i].max && value >= gTempoNames[i].min) {
			fSpeedSlider->SetLabel(gTempoNames[i].name);
			return;
		}
	}
}
