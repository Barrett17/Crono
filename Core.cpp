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

#include <CronoDefaults.h>

static	BSoundPlayer* kPlayer;
static	BMediaTrack* kTic;
static	BMediaTrack* kTac;

void
Core::PlayBuffer(void* cookie, void* buffer, size_t size,
const media_raw_audio_format& format)
{
	int64 frames = 0;
	//fPlayTrack->ReadFrames(buffer, &frames);

	if (frames <=0)
		kPlayer->SetHasData(false);
}


//Initialize the class with default values
Core::Core()
	:
	fSpeed(DEFAULT_SPEED),
	fMeter(0),
	fVolume(DEFAULT_VOLUME),
	fRunning(false)
{
	media_raw_audio_format format;
	format = media_raw_audio_format::wildcard;
	format.format = media_raw_audio_format::B_AUDIO_FLOAT;
	format.byte_order = B_MEDIA_LITTLE_ENDIAN;

	kPlayer = new BSoundPlayer(&format, 
		"CronoPlayback", PlayBuffer);

	LoadTicks();

	kPlayer->SetHasData(true);
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

	return B_ERROR;
}


// Unload sound tracks
status_t
Core::UnloadTicks() 
{

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
		fMeter = m;
}


void 
Core::SetVolume(int32 v)
{
	if (v <= 100 && v >= 0)
		fVolume = v;
}


int32 
Core::Volume()
{
	return fVolume;	
}


int32
Core::Speed()
{
	return fSpeed;	
}


int32
Core::Meter()
{
	return fMeter;	
}
