/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#ifndef CRONO_CORE_H
#define CRONO_CORE_H

#include <MediaFile.h>
#include <MediaTrack.h>
#include <SoundPlayer.h>
#include <SupportDefs.h>


/**
 * Il core dell'applicazione
 */
class Core {
public:
						Core();
						~Core();

			status_t	InitCheck();

			status_t	Init();
			status_t	Destroy();

			status_t 	LoadSoundFile();
			status_t 	UnloadSoundFile();

			status_t	LoadGeneratedSounds();
			status_t	UnloadGeneratorSounds();

			status_t	Stop();
			status_t	Start();

			void 		SetSpeed(int32 speed);
			void 		SetMeter(int32 meter);
			void 		SetVolume(float volume);
			void		SetEngine(int32 engine);

			int32		Speed();
			int32 		Meter();
			float		Volume();
			int32		Engine();

	static	void		PlayBuffer(void* cookie, void* buffer, size_t size,
							const media_raw_audio_format& format);
private:
	static	void 		FillFileBuffer(float* data, size_t numFrames);

	static	void 		FillSineBuffer2(float* data, size_t numFrames);

	static	void 		FillSineBuffer(float* data, size_t numFrames,
							bool stereo);

	static	void 		FillTriangleBuffer(float* data,
							size_t numFrames, bool stereo);

	static	void 		FillSawtoothBuffer(float* data,
							size_t numFrames, bool stereo);
	
			status_t	fErr;

			bool 		fRunning;
};

#endif
