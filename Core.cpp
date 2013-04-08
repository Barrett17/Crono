/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#include "Core.h"

#include <SoundPlayer.h>

#include "CronoDefaults.h"

#include <stdio.h>

BSoundPlayer* kPlayer = NULL;

BMediaFile* kTicFile = NULL;
BMediaFile* kTocFile = NULL;

BMediaTrack* kTic = NULL;
BMediaTrack* kToc = NULL;


void
Core::PlayBuffer(void* cookie, void* buffer, size_t size,
const media_raw_audio_format& format)
{
	printf("playing\n");
	int64 frames = 0;
	kTic->ReadFrames(buffer, &frames);

	char* buf = (char*)buffer;

	if (frames <= 0) {
		//kPlayer->SetHasData(false);
		kTic->SeekToTime(0);
	}
}


//Initialize the class with default values
Core::Core()
	:
	fRunning(false)
{
	LoadTicks();

	media_format fileFormat;
	fileFormat.type = B_MEDIA_RAW_AUDIO;

	if (kTic->DecodedFormat(&fileFormat) == B_OK
			&& fileFormat.type == B_MEDIA_RAW_AUDIO) {

		kPlayer = new BSoundPlayer(&fileFormat.u.raw_audio, 
			"CronoPlayback", PlayBuffer);

		kPlayer->SetHasData(true);
	} else {
		// Alert
	}
}


// Destroy the class
Core::~Core()
{
	UnloadTicks();
	delete kPlayer;
}


// Return internal errors
status_t
Core::InitCheck()
{
	return fErr;
}


// Load sound tracks
status_t
Core::LoadTicks() 
{
	if (kTicFile != NULL && kTocFile != NULL)
		UnloadTicks();
 
	kTicFile = new BMediaFile(
		new BFile(gCronoSettings.TicLocation, B_READ_ONLY));

	if (kTicFile->InitCheck() != B_OK)
		printf("Error initializing the first File.");

	kTocFile = new BMediaFile(
		new BFile(gCronoSettings.TocLocation, B_READ_ONLY));

	if (kTocFile->InitCheck() != B_OK)
		printf("Error initializing the second File.");

	kTic = kTicFile->TrackAt(0);
	kToc = kTocFile->TrackAt(0);

	return B_OK;
}


// Unload sound tracks
status_t
Core::UnloadTicks() 
{
	kTicFile->ReleaseAllTracks();
	kTocFile->ReleaseAllTracks();

	delete kTicFile;
	delete kTocFile;

	kTicFile = NULL;
	kTocFile = NULL;

	return B_ERROR;
}


// Start the metronome 
void 
Core::Start()
{
	if (!fRunning) {
		fRunning = true;
		kPlayer->Start();
	}
}


// Stop the metronome 
void 
Core::Stop()
{
	if (fRunning) {
		fRunning = false;
		kPlayer->Stop();
	}
}


void 
Core::SetSpeed(int32 s)
{
	if (s <= 500 && s > 1)
		fSpeed = s;
}


void 
Core::SetMeter(int32 m)
{
	if (m < 10 && m >= 0)
		gCronoSettings.Meter = m;
}


void 
Core::SetVolume(int32 v)
{
	// TODO modify as needed to use BSoundPlayer::SetVolume(float)
	if (v <= 100 && v >= 0)
		gCronoSettings.CronoVolume = v;
}


int32 
Core::Volume()
{
	return gCronoSettings.CronoVolume;	
}


int32
Core::Speed()
{
	return gCronoSettings.Speed;	
}


int32
Core::Meter()
{
	return gCronoSettings.Meter;	
}
