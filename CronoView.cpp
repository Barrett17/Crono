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
#include <GroupView.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Slider.h>
#include <TextControl.h>
#include <View.h>
#include <Box.h>
#include <RadioButton.h>

#include "App.h"
#include "Core.h"
#include "SettingsWindow.h"

#include <math.h>
#include <stdlib.h>


CronoView::CronoView()
	:
	BView("CronoView", B_WILL_DRAW, 0)
{
	rgb_color barColor = { 0, 0, 240 };
	rgb_color fillColor = { 240, 0, 0 };
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// Main layout
	BGroupLayout* rootLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(rootLayout);

	// Core
	fCore = new Core();
	fCore->SetVolume(80);
	fCore->SetSpeed(60);


	// Menu bar
	fMenuBar = new BMenuBar("MenuBar");
	rootLayout->AddView(fMenuBar);
	
	fFileMenu = new BMenu("File");
	fFileMenu->AddItem(new BMenuItem("Quit", new BMessage(MSG_CLOSE), 'Q'));
	fFileMenu->AddItem(new BMenuItem("About", new BMessage(MSG_ABOUT), 'A'));
	fMenuBar->AddItem(fFileMenu);
	
	fEditMenu = new BMenu("Edit");
	fEditMenu->AddItem(new BMenuItem("Settings", new BMessage(MSG_SETTINGS), 'S'));
	fEditMenu->AddItem(new BSeparatorItem);
	fEditMenu->AddItem(new BMenuItem("Start", new BMessage(MSG_START), 'G'));
	fEditMenu->AddItem(new BMenuItem("Stop", new BMessage(MSG_STOP), 'H'));
	fMenuBar->AddItem(fEditMenu);

	// Volume slider
	BBox* volBox = new BBox("volbox");
	volBox->SetLabel("Volume");
	BGroupLayout* volLayout = new BGroupLayout(B_VERTICAL);
	volLayout->SetInsets(10, volBox->TopBorderOffset() * 2 + 10, 10, 10);
	volBox->SetLayout(volLayout);
	
	fVolumeSlider = new BSlider("", "", new BMessage(MSG_VOLUME),
		0, 100, B_HORIZONTAL);

	fVolumeSlider->SetLimitLabels("Min", "Max");
	fVolumeSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fVolumeSlider->SetHashMarkCount(10);
	fVolumeSlider->SetBarColor(barColor);
	fVolumeSlider->SetValue(fCore->Volume());
	//fVolumeSlider->SetPosition(((float) DEFAULT_VOLUME) / 100.0);
	fVolumeSlider->UseFillColor(true, &fillColor);
	
	volLayout->AddView(fVolumeSlider);
	rootLayout->AddView(volBox);


	// Speed Slider & TextControl
	BBox* speedBox = new BBox("speedbox");
	speedBox->SetLabel("BPM");
	BGroupLayout* speedLayout = new BGroupLayout(B_HORIZONTAL);
	speedLayout->SetInsets(10, speedBox->TopBorderOffset() * 2 + 10, 10, 10);
	speedBox->SetLayout(speedLayout);
	
	fSpeedSlider = new BSlider("", "",
		new BMessage(MSG_SPEED_SLIDER), 1, 300, B_HORIZONTAL);

	fSpeedSlider->SetLimitLabels("1", "300");
	fSpeedSlider->SetKeyIncrementValue(5);
	fSpeedSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fSpeedSlider->SetHashMarkCount(10);
	fSpeedSlider->SetBarColor(barColor);
	fSpeedSlider->SetValue(fCore->Speed());
	fSpeedSlider->UseFillColor(true, &fillColor);
	
	
	fSpeedEntry = new BTextControl("", "", "60",
		new BMessage(MSG_SPEED_ENTRY), B_WILL_DRAW);

	fSpeedEntry->SetDivider(70);
	fSpeedEntry->SetAlignment(B_ALIGN_CENTER, B_ALIGN_CENTER);
//	fSpeedEntry->SetExplicitMaxSize(BSize(1, 1));
	
	speedLayout->AddView(fSpeedSlider);
	speedLayout->AddView(fSpeedEntry);
	rootLayout->AddView(speedBox);


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
	rootLayout->AddView(meterBox);
	

	// Start and stop button	
	BGroupView* buttonGroup = new BGroupView(B_HORIZONTAL);

	fStartButton = new BButton("Start", new BMessage(MSG_START));						
	fStartButton->MakeDefault(true);	
	buttonGroup->GroupLayout()->AddView(fStartButton);

	fStopButton = new BButton("Stop", new BMessage(MSG_STOP));							
	buttonGroup->GroupLayout()->AddView(fStopButton);

	rootLayout->AddView(buttonGroup);
}


CronoView::~CronoView()
{
	fCore->Stop();
	delete fCore;
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
	
	Window()->CenterOnScreen();
}


void
CronoView::MessageReceived(BMessage *message)
{
	float position;
	
	switch(message->what)
	{

		case MSG_CLOSE:
		{
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;
		}

		case MSG_ABOUT:
		{
			BAlert *alert = new BAlert("About Crono", 
			"\nCrono Metronome V0.0.3\n\n"
			"Copyright ©2009 - 2013 Dario Casalinuovo\n\n"
			"Copyright ©2009 - 2013 Davide Gessa \n\n"
			"Crono is a Software Metronome for Haiku "
			"Released under MIT license for non commercial use.\n\n"
			"WARNING - Crono is unstable and unfinished, this is not\n"
			"the final release! Please submit us bugs and improvements ideas.\n\n"
			"Project Homepage:\n https://github.com/Barrett17/Crono\n",
			"OK", NULL, NULL, B_WIDTH_FROM_WIDEST, B_EVEN_SPACING, B_INFO_ALERT);
			alert->Go();

			break;
		}

		case MSG_SETTINGS:
		{
			SettingsWindow *settWindow;
			BRect windowRect;
			
			windowRect.Set(150,150,340,325);
	
			settWindow = new SettingsWindow(windowRect);
			settWindow->Show();
			//delete settWindow;
			
			// CHIARO? @_@!!! Chiarissimo v_v
			break;
		}

		case MSG_START:
		{
			printf("Start\n");
			fCore->Start();
			fStopButton->MakeDefault(true);
			break;
		}
	
		case MSG_STOP:
		{
			printf("Stop\n");
			fCore->Stop();
			fStartButton->MakeDefault(true);
			break;
		}

		case MSG_VOLUME:
		{
			position = fVolumeSlider->Position();
			position = 100 * position;
			printf("Volume: %d\n", (int) position);
			fCore->SetVolume(((int) position));
			break;
		}

		case MSG_METER_RADIO:
		{
			int selected = 0;
			
			// Get the selected radiobutton
			for (int i = 0; i < 5; i++)
				if (fMeterRadios[i]->Value())
					selected = i;
			
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
			
			if (bpm > 500) {
				fSpeedEntry->SetText("500");
				bpm = 500;
			} else if(bpm == 0) {
				fSpeedEntry->SetText("1");
				bpm = 1;
			}
				
			fCore->SetSpeed(((int) bpm));
			
			fSpeedSlider->SetPosition(((float) bpm / 500));
			printf("Speed: %s %d\n", fSpeedEntry->Text(), bpm);
			break;
		}

		case MSG_SPEED_SLIDER:
		{
			float v = fSpeedSlider->Value();
			
			if (v == 0)
				v = 1;
				
			char strv[32];
			
			sprintf(strv, "%d", (int) v);
			fSpeedEntry->SetText(strv);

			fCore->SetSpeed((int) v);
			
			break;
		}

		default:
			BView::MessageReceived(message);
	}			
}
