/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
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

BMediaFile* kSoundFile = NULL;

BMediaTrack* kSound = NULL;

size_t kSize = 0;
size_t kLimit = 0;
size_t kTicLen = 0;

int sem = 0;

BMallocIO* buf;

double mTheta = 2;
media_format fileFormat;
bool mWaveAscending = false;
double mGain = 1;


void
Core::PlayBuffer(void* cookie, void* buffer, size_t size,
	const media_raw_audio_format& format)
{
	//printf("playing\n");

	kLimit = (size_t)((fileFormat.u.raw_audio.frame_rate
		*(fileFormat.u.raw_audio.channel_count+1))*60);

	size_t limit = (kLimit/gCronoSettings.Speed)*10;
	kTicLen = fileFormat.u.raw_audio.frame_rate / 10;

	//printf("%d %d %d\n", limit, kSize, kLimit);
	if (kSize >= limit) {
		kSize = kSize-limit;
		sem = 0;
	}

	 if (sem == 1) {
			//printf("empty\n");
			memset(buffer, 0, size);
			kSize += size;
	} else if (sem == 0) {
		bool stereo = true;

		switch(gCronoSettings.Engine)
		{
			case CRONO_FILE_ENGINE:
				FillFileBuffer((float*)buffer, size);
			break;

			case CRONO_DEFAULT_ENGINE:
			break;

			case CRONO_SINE_ENGINE:
				FillSineBuffer2((float*)buffer, size);
			break;

			case CRONO_TRIANGLE_ENGINE:
				FillTriangleBuffer((float*)buffer, size, stereo);
			break;

			case CRONO_SAWTOOTH_ENGINE:
				FillSawtoothBuffer((float*)buffer, size, stereo);
			break;
		}

		kSize += size;
		if (kSize >= kTicLen)
			sem = 1;
	}
	//printf("%ld\n", kSize);
}


//Initialize the class with default values
Core::Core()
	:
	fRunning(false)
{
	Init();
}


// Destroy the class
Core::~Core()
{
	Destroy();
}


// Return internal errors
status_t
Core::InitCheck()
{
	return fErr;
}


status_t
Core::Init() 
{
	switch(gCronoSettings.Engine)
	{
		case CRONO_FILE_ENGINE:
			LoadSoundFile();
		break;

		case CRONO_DEFAULT_ENGINE:
		case CRONO_SINE_ENGINE:
		case CRONO_TRIANGLE_ENGINE:
		case CRONO_SAWTOOTH_ENGINE:
			LoadGeneratedSounds();
		break;
	}

 	if (fileFormat.type != B_MEDIA_RAW_AUDIO)
 		return B_ERROR;
 
	if (kPlayer != NULL)
		delete kPlayer;

	kPlayer = new BSoundPlayer(&fileFormat.u.raw_audio, 
		"CronoPlayback", PlayBuffer);

	kPlayer->SetHasData(true);

}


status_t
Core::Destroy() 
{
	if (gCronoSettings.Engine == CRONO_FILE_ENGINE)
		UnloadSoundFile();

	delete kPlayer;
	kPlayer = NULL;
}


status_t
Core::LoadGeneratedSounds() 
{
	fileFormat.type = B_MEDIA_RAW_AUDIO;
	fileFormat.u.raw_audio.format = media_raw_audio_format::B_AUDIO_FLOAT;
	fileFormat.u.raw_audio.frame_rate = 44100;
	fileFormat.u.raw_audio.byte_order = 
		(B_HOST_IS_BENDIAN) ? B_MEDIA_BIG_ENDIAN : B_MEDIA_LITTLE_ENDIAN;

	fileFormat.u.raw_audio.buffer_size = media_raw_audio_format::wildcard.buffer_size;
	fileFormat.u.raw_audio.channel_count = media_raw_audio_format::wildcard.channel_count;
}


// Load sound tracks
status_t
Core::LoadSoundFile() 
{
	if (kSoundFile != NULL)
		UnloadSoundFile();

	kSoundFile = new BMediaFile(
		new BFile(gCronoSettings.SoundFileLocation, B_READ_ONLY));

	if (kSoundFile->InitCheck() != B_OK) {
		printf("Error initializing the first File.");
		return B_ERROR;
	}

	kSound = kSoundFile->TrackAt(0);

	fileFormat.type = B_MEDIA_RAW_AUDIO;

	if (kSound != NULL && kSound->EncodedFormat(&fileFormat) != B_OK &&
		kSound->DecodedFormat(&fileFormat) != B_OK) {
		return B_ERROR;
	}

	buf = new BMallocIO();
	buf->SetSize(kSound->CountFrames()*2);
	void* buffer = new char[fileFormat.u.raw_audio.buffer_size];

	int64 frames;
	status_t ret;
	for (frames = 0; frames < kSound->CountFrames();) {
		int64 count;
		ret = kSound->ReadFrames(buffer, &count);
		if (ret != B_OK)
			break;
		frames += count;
		buf->Write(buffer, fileFormat.u.raw_audio.buffer_size);
	}

	delete[] (char*)buffer;

	return B_OK;
}


// Unload sound tracks
status_t
Core::UnloadSoundFile() 
{
	if (fRunning)
		Stop();

	if (kSoundFile != NULL && kSoundFile->InitCheck() == B_OK)
		kSoundFile->ReleaseAllTracks();

	delete kSoundFile;

	kSoundFile = NULL;

	kSound = NULL;

	delete buf;

	return B_OK;
}


// Start the metronome 
status_t
Core::Start()
{
	if (!fRunning) {
		status_t ret;
		/*if (gCronoSettings.LocationsChanged()) {
			ret = LoadSoundFile();
			if (ret != B_OK)
				return ret;
		}*/
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
	kPlayer->SetVolume(v);
	gCronoSettings.CronoVolume = v;
}


void
Core::SetEngine(int32 engine)
{
	if (engine == gCronoSettings.Engine)
		return;

	Destroy();
	gCronoSettings.Engine = engine;
	Init();
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


int32
Core::Engine()
{
	return gCronoSettings.Engine;	
}


void
Core::FillFileBuffer(float* data, size_t numFrames)
{
	size_t read = buf->Read(data, numFrames);
	if (read < numFrames) {
		buf->Seek(0, SEEK_SET);
		sem = 1;
		for (;read <= numFrames; read++) {
			data[read] = 0;
		}
	}	
}


void
Core::FillSineBuffer2(float* data, size_t numFrames)
{
	float* s = data;
	int scale = -SHRT_MIN < SHRT_MAX ? -SHRT_MIN : SHRT_MAX;
	int i;
	double sample;
	double samplefreq = fileFormat.u.raw_audio.frame_rate;
	double freq = 440.0;
	double fadedur = 0.002;
	double sigdur = 0.01;

	for (i = 0; i < numFrames; i++) {
		sample = sin((double)i / samplefreq * freq * 2 * M_PI);
		if (i < samplefreq * fadedur)
			sample *= (-cos((double)i / (samplefreq * sigdur) * 2 * M_PI) + 1) * 0.5;
		if (i > samplefreq * (sigdur - fadedur))
			sample *=
			(-cos((double)(numFrames - i) / (samplefreq * sigdur) * 2 * M_PI) + 1) * 0.5;

		s[i] = sample * scale;
		/* ++s;
		*s = s[i];
		i++;*/
	}
}


// Sine, Triangle and Sawtooth methods comes originally 
// from the ToneProducer example plugin included in Haiku.
void
Core::FillSineBuffer(float* data, size_t numFrames, bool stereo)
{
	// cover 2pi radians in one period
	double dTheta = 2*M_PI * double(440) / fileFormat.u.raw_audio.frame_rate*2;
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
