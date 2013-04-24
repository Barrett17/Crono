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
size_t kLimit = 0;

int sem = 0;

BMallocIO* buf;

double mTheta = 0;
media_format fileFormat;
bool mWaveAscending = false;
double mGain = 1;


void
Core::PlayBuffer(void* cookie, void* buffer, size_t size,
	const media_raw_audio_format& format)
{
	//printf("playing\n");

	size_t limit = kLimit/gCronoSettings.Speed;

	if (kSize >= limit) {
		kSize = kSize-limit;
		sem = 0;
	}

	if (sem == 0) {
		FillFileBuffer((float*)buffer, size);
		kSize += size;
		sem = 1;
	} else if (sem == 1) {
			//printf("empty\n");
			memset(buffer, 0, size);
			kSize += size;
	}
	printf("%ld\n", kSize);
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
		if (ret != B_OK)
			break;
		frames += count;
		buf->Write(buffer, fileFormat.u.raw_audio.buffer_size);
	}

	delete[] (char*)buffer;

	kLimit = (size_t)(fileFormat.u.raw_audio.frame_rate
		*(float)(fileFormat.u.raw_audio.channel_count+1))*60;

	return B_OK;
}


// Unload sound tracks
status_t
Core::UnloadTicks() 
{
	if (fRunning)
		Stop();

	delete kPlayer;

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

	return B_OK;
}


// Start the metronome 
status_t
Core::Start()
{
	if (!fRunning) {
		status_t ret;
		if (gCronoSettings.LocationsChanged()) {
			ret = LoadTicks();
			if (ret != B_OK)
				return ret;
		}
		fRunning = true;
		kPlayer->SetVolume(gCronoSettings.CronoVolume);
		return kPlayer->Start();
	}
	return B_OK;
}


// Stop the metronome 
status_t
Core::Stop()
{
	if (fRunning) {
		fRunning = false;
		kPlayer->Stop();
		buf->Seek(0, SEEK_SET);
	}
	return B_OK;
}


void 
Core::SetSpeed(int32 s)
{
	if (s <= MAX_SPEED && s > MIN_SPEED)
		gCronoSettings.Speed = s;
}


void 
Core::SetMeter(int32 m)
{
	if (m < 10 && m >= 0)
		gCronoSettings.Meter = m;
}


void 
Core::SetVolume(float v)
{
	if (v <= 1.0 || v >= 0.0) {
		kPlayer->SetVolume(v);
		gCronoSettings.CronoVolume = v;
	}
}


float
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


void
Core::FillFileBuffer(float* data, size_t numFrames)
{
		//printf("read\n");
	size_t read = buf->Read(data, numFrames);
	if (read < numFrames) {
		buf->Seek(0, SEEK_SET);
		sem = 1;
		for (;read <= numFrames; read++) {
			data[read] = 0;
		}
	}	
}


// Sine, Triangle and Sawtooth methods comes originally 
// from the ToneProducer example plugin included in Haiku.
void
Core::FillSineBuffer(float* data, size_t numFrames, bool stereo)
{
	// cover 2pi radians in one period
	double dTheta = 2*M_PI * double(50) / fileFormat.u.raw_audio.frame_rate;

	// Fill the buffer!
	for (size_t i = 0; i < numFrames; i++, data++) {
		float val = mGain * float(sin(mTheta));
		*data = val;
		if (stereo) {
			++data;
			*data = val;
		}

		mTheta += dTheta;
		if (mTheta > 2*M_PI)
			mTheta -= 2*M_PI;
	}
}


void
Core::FillTriangleBuffer(float* data, size_t numFrames, bool stereo)
{
	// ramp from -1 to 1 and back in one period
	double dTheta = 4.0 * double(50) / fileFormat.u.raw_audio.frame_rate;
	if (!mWaveAscending)
		dTheta = -dTheta;

	// fill the buffer!
	for (size_t i = 0; i < numFrames; i++, data++) {
		float val = mGain * mTheta;
		*data = val;
		if (stereo) {
			++data;
			*data = val;
		}
		
		mTheta += dTheta;
		if (mTheta >= 1) {
			mTheta = 2 - mTheta; // reflect across the mTheta=1 line to preserve drift
			mWaveAscending = false;
			dTheta = -dTheta;
		} else if (mTheta <= -1) {
			mTheta = -2 - mTheta; // reflect across mTheta=-1
			mWaveAscending = true;
			dTheta = -dTheta;
		}
	}
}


void
Core::FillSawtoothBuffer(float* data, size_t numFrames, bool stereo)
{
	// ramp from -1 to 1 in one period
	double dTheta = 2 * double(50) / fileFormat.u.raw_audio.frame_rate;
	mWaveAscending = true;

	// fill the buffer!
	for (size_t i = 0; i < numFrames; i++, data++) {
		float val = mGain * mTheta;
		*data = val;
		if(stereo) {
			++data;
			*data = val;
		}

		mTheta += dTheta;
		if (mTheta > 1)
			mTheta -= 2; // back to the base of the sawtooth, including cumulative drift
	}
}
