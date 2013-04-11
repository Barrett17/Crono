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
#include <string.h>

BSoundPlayer* kPlayer = NULL;

BMediaFile* kTicFile = NULL;
BMediaFile* kTocFile = NULL;

BMediaTrack* kTic = NULL;
BMediaTrack* kToc = NULL;

size_t kSize = 0;

int sem = 0;

BMallocIO* buf;


void
Core::PlayBuffer(void* cookie, void* buffer, size_t size,
	const media_raw_audio_format& format)
{
	printf("playing\n");

	size_t limit = ((format.frame_rate*format.channel_count) * 60)
		/gCronoSettings.Speed;

	if (kSize > limit) {
		kSize = kSize-limit;
		sem = 0;
	}

	if (sem == 0) {
		printf("read\n");
		ssize_t read = buf->Read(buffer, size);
		if (read < 1) {
			buf->Seek(0, SEEK_SET);
			sem = 1;
		}
		kSize += read;
	} else if (sem == 1) {
			printf("empty\n");
			memset(buffer, 0, size);
			kSize += size;
	}
	printf("%d\n", kSize);
}


void
Core::_PrepareBuffers()
{
}


//Initialize the class with default values
Core::Core()
	:
	fRunning(false)
{
	LoadTicks();
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

	if (kTicFile->InitCheck() != B_OK) {
		printf("Error initializing the first File.");
		return B_ERROR;
	}

	kTocFile = new BMediaFile(
		new BFile(gCronoSettings.TocLocation, B_READ_ONLY));

	if (kTocFile->InitCheck() != B_OK) {
		printf("Error initializing the second File.");
		return B_ERROR;
	}

	kTic = kTicFile->TrackAt(0);
	kToc = kTocFile->TrackAt(0);

	media_format fileFormat;
	fileFormat.type = B_MEDIA_RAW_AUDIO;

	if (kTic != NULL && kTic->EncodedFormat(&fileFormat) == B_OK &&
		kTic->DecodedFormat(&fileFormat) == B_OK
			&& fileFormat.type == B_MEDIA_RAW_AUDIO) {

		if (kPlayer != NULL)
			delete kPlayer;

		kPlayer = new BSoundPlayer(&fileFormat.u.raw_audio, 
			"CronoPlayback", PlayBuffer);

		kPlayer->SetHasData(true);
	} else {
		// Alert
		return B_ERROR;
	}

	buf = new BMallocIO();
	buf->SetSize(kTic->CountFrames()*2);
	void* buffer = new char[fileFormat.u.raw_audio.buffer_size];

	int64 frames;
	status_t ret;
	for (frames = 0; frames < kTic->CountFrames();) {
		int64 count;
		ret = kTic->ReadFrames(buffer, &count);
		frames += count;
		buf->Write(buffer, fileFormat.u.raw_audio.buffer_size);
	}

	delete buffer;
	return B_OK;
}


// Unload sound tracks
status_t
Core::UnloadTicks() 
{
	if (kTicFile != NULL && kTicFile->InitCheck() == B_OK)
		kTicFile->ReleaseAllTracks();

	if (kTocFile != NULL && kTocFile->InitCheck() == B_OK)
		kTocFile->ReleaseAllTracks();

	delete kTicFile;
	delete kTocFile;

	kTicFile = NULL;
	kTocFile = NULL;

	kTic = NULL;
	kToc = NULL;

	delete buf;
	delete kPlayer;

	return B_OK;
}


// Start the metronome 
void 
Core::Start()
{
	if (!fRunning) {
		if (gCronoSettings.LocationsChanged())
			if (LoadTicks() != B_OK) {
				printf("Unable to start!");
				// TODO this method should return an error
				// and the error should be checked by CronoView.
				return;
			}
		fRunning = true;
		kPlayer->SetVolume((float)gCronoSettings.CronoVolume/25);
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
		gCronoSettings.Speed = s;
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
	if (v <= 100 && v >= 0) {
		kPlayer->SetVolume((float)v/25);
		gCronoSettings.CronoVolume = v;
	}
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
