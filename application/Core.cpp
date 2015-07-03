/*
 * Copyright (c) 2012-2015  Dario Casalinuovo (b.vitruvio@gmail.com)
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
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */

#include "Core.h"

#include <SoundPlayer.h>

#include "CronoDefaults.h"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

BSoundPlayer* kPlayer = NULL;

BMediaFile* kSoundFile = NULL;

BMediaTrack* kSound = NULL;

size_t kSize = 0;
size_t kLimit = 0;
size_t kTicLen = 0;

int kSem = 0;
BMallocIO* kBuf;
double kTheta = 2;
media_format kFileFormat;
bool kWaveAscending = false;
double kGain = 0.5;
double kDuration = 0.01;
double kFreq = 660;
double kScale = 1;

//timeval prev;


void
Core::PlayBuffer(void* cookie, void* buffer, size_t size,
	const media_raw_audio_format& format)
{
	size_t limit = (kLimit/gCronoSettings.Speed);
	bool stereo = format.channel_count == 2;

	printf("%f\n", limit / kFileFormat.u.raw_audio.frame_rate*2);

	kTicLen = kFileFormat.u.raw_audio.frame_rate/7;
	limit = limit*10;

	if (kSize >= limit) { 
		//timeval start;
		//prev = start;
		//gettimeofday(&start, NULL);
		//printf("took %lu\n", start.tv_usec - prev.tv_usec);
		kSize -= limit;
		kSem = 0;
	}

	size_t remaining = 0;
	if (kSize+size > limit) {
		remaining = kSize+size - limit;
		size -= remaining;
	}

	if (kSem == 1) {
		memset(buffer, 0, size);
		kSize += size;
		//if (fillSize < kSize) {
		//	fillSize = kSize - fillSize;
		//	kSem = 0;
		//}
		if (remaining >= 0) {
			kSem = 0;
			size = remaining;
		}
	}

	if (kSem == 0) {

		switch(gCronoSettings.Engine)
		{
			case CRONO_SINE_ENGINE:
				FillSineBuffer((float*)buffer, size, stereo);
			break;

			case CRONO_TRIANGLE_ENGINE:
				FillTriangleBuffer((float*)buffer, size, true);
			break;

			case CRONO_SAWTOOTH_ENGINE:
				FillSawtoothBuffer((float*)buffer, size, stereo);
			break;
		}

		kSize += size;
		if (remaining > 0) {
			printf("defect\n");
			memset(buffer, size-remaining, remaining);
			remaining = 0;
		}
			
		if (kSize >= kTicLen) {
			kSem = 1;
		}
	}
}


void
Core::PlayMathBuffer(void* cookie, void* buffer, size_t size,
	const media_raw_audio_format& format)
{
	//size_t limit = (kLimit/gCronoSettings.Speed);
	size_t limit = (gCronoSettings.Speed / 60)*(kFileFormat.u.raw_audio.frame_rate
		*(kFileFormat.u.raw_audio.channel_count+1));
	bool stereo = format.channel_count == 2;

	kTicLen = (gCronoSettings.Speed/60*60)/ 5;
	limit = limit*10;

	if (kSize >= limit) { 
/*		timeval start;
		prev = start;
		gettimeofday(&start, NULL);
		printf("took %lu\n", start.tv_usec - prev.tv_usec);*/
		kSize -= limit;
		kSem = 0;
	}

	size_t fillSize = size;

	if (kSize + size > limit) {
		fillSize = size + kSize - limit;
	}

	if (kSem == 1) {
		memset(buffer, 0, fillSize);
		kSize += fillSize;
		/*if (fillSize < kSize) {
			fillSize = kSize - fillSize;
			kSem = 0;
		}*/
	}

	if (kSem == 0) {

		switch(gCronoSettings.Engine)
		{
			case CRONO_SINE_ENGINE:
				FillSineBuffer((float*)buffer, fillSize, stereo);
			break;

			case CRONO_TRIANGLE_ENGINE:
				FillTriangleBuffer((float*)buffer, fillSize, stereo);
			break;

			case CRONO_SAWTOOTH_ENGINE:
				FillSawtoothBuffer((float*)buffer, fillSize, stereo);
			break;
		}

		kSize += fillSize;
		if (kSize >= kTicLen) {
			kSem = 1;
		}
	}
}


void
Core::PlayFileBuffer(void* cookie, void* buffer, size_t size,
	const media_raw_audio_format& format)
{
	size_t limit = (kLimit/gCronoSettings.Speed);
	bool stereo = format.channel_count == 2;

	off_t s;
	kBuf->GetSize(&s);
	kTicLen = s;

	if (kSize + size > limit)
		size -= kSize + size - limit;

	if (kSize >= limit) { 
		kSize -= limit;
		kSem = 0;
	}

	 if (kSem == 1) {
		memset(buffer, 0, size);
		kSize += size;
	} else if (kSem == 0) {
		FillFileBuffer((float*)buffer, size);

		if (size <  kFileFormat.u.raw_audio.buffer_size)
			size -= kSize + size - limit;

		kSize += size;
		if (kSize >= kTicLen)
			kSem = 1;
	}
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

 	if (kFileFormat.type != B_MEDIA_RAW_AUDIO)
 		return B_ERROR;
 
	if (kPlayer != NULL)
		delete kPlayer;

	kPlayer = new BSoundPlayer(&kFileFormat.u.raw_audio, 
		"CronoPlayback", PlayBuffer);


	kLimit = (size_t)((kFileFormat.u.raw_audio.frame_rate
		*(kFileFormat.u.raw_audio.channel_count+1))*60);
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
	kFileFormat.type = B_MEDIA_RAW_AUDIO;
	kFileFormat.u.raw_audio.format = media_raw_audio_format::B_AUDIO_FLOAT;
	kFileFormat.u.raw_audio.frame_rate = 44100;
	kFileFormat.u.raw_audio.byte_order = 
		(B_HOST_IS_BENDIAN) ? B_MEDIA_BIG_ENDIAN : B_MEDIA_LITTLE_ENDIAN;

	kFileFormat.u.raw_audio.buffer_size 
		= media_raw_audio_format::wildcard.buffer_size;

	kFileFormat.u.raw_audio.channel_count
		= media_raw_audio_format::wildcard.channel_count;
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

	kFileFormat.type = B_MEDIA_RAW_AUDIO;

	kSound->EncodedFormat(&kFileFormat);
	kSound->DecodedFormat(&kFileFormat);

	kBuf = new BMallocIO();
	void* buffer = new char[kFileFormat.u.raw_audio.buffer_size];

	int64 frames;
	status_t ret;
	for (frames = 0; kSound->CurrentFrame() != kSound->CountFrames();) {
		int64 count;
		ret = kSound->ReadFrames(buffer, &count);
		if (ret != B_OK)
			break;
		frames += count;
		kBuf->Write(buffer, count);
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

	delete kBuf;

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
		kPlayer->SetHasData(true);
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
		kPlayer->SetHasData(false);
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
	size_t read = kBuf->Read(data, numFrames);
	if (read < numFrames) {
		kSem = 1;
		kBuf->Seek(0, SEEK_SET);
	}	
}


void
Core::FillSineBuffer(float* data, size_t numFrames, bool stereo)
{
	double framerate = kFileFormat.u.raw_audio.frame_rate;
	for (size_t i = 0; i < numFrames; i++, data++) {
		double kTheta = (2 * M_PI * double(kFreq) / (framerate * kDuration) * 0.5);
		float val = float(cos(i / kTheta));

		if (i > kFileFormat.u.raw_audio.frame_rate * (kDuration - 0.002)) {
			val *=
				(cos(2 * M_PI * (double)(numFrames - i) 
					/ (framerate * kDuration))) * 0.5;
		}

		if (i < kFileFormat.u.raw_audio.frame_rate * kDuration) {
			val *=
				(-cos(2 * M_PI * (double)(numFrames - i) 
					/ (framerate * kDuration)) * 0.5);
		}

		*data = val;

		if (stereo) {
			++data;
			*data = val;
		}
	}
}


// Triangle and Sawtooth are a custom version 
// of the proccess methods from the ToneProducer example plugin included in Haiku.
void
Core::FillTriangleBuffer(float* data, size_t numFrames, bool stereo)
{
	// ramp from -1 to 1 and back in one period
	double dTheta = 4.0 * double(kFreq) / kFileFormat.u.raw_audio.frame_rate;
	if (!kWaveAscending)
		dTheta = -dTheta;

	// fill the buffer!
	for (size_t i = 0; i < numFrames; i++, data++) {
		float val = kTheta;
		*data = val;
		if (stereo) {
			++data;
			*data = val;
		}
		
		kTheta += dTheta;
		if (kTheta >= 1) {
			kTheta = 2 - kTheta; // reflect across the kTheta=1 line to preserve drift
			kWaveAscending = false;
			dTheta = -dTheta;
		} else if (kTheta <= -1) {
			kTheta = -2 - kTheta; // reflect across kTheta=-1
			kWaveAscending = true;
			dTheta = -dTheta;
		}
	}
}


void
Core::FillSawtoothBuffer(float* data, size_t numFrames, bool stereo)
{
	// ramp from -1 to 1 in one period
	double dTheta = 2 * double(kFreq) / kFileFormat.u.raw_audio.frame_rate;
	kWaveAscending = true;

	// fill the buffer!
	for (size_t i = 0; i < numFrames; i++, data++) {
		float val = kTheta;
		*data = val;
		if(stereo) {
			++data;
			*data = val;
		}

		kTheta += dTheta;
		if (kTheta > 1)
			kTheta -= 2; // back to the base of the sawtooth, including cumulative drift
	}
}
