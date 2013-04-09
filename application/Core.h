/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
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

			status_t 	LoadTicks();
			status_t 	UnloadTicks();

			void 		Stop();
			void 		Start();

			void 		SetSpeed(int32 speed);
			void 		SetMeter(int32 meter);
			void 		SetVolume(int32 volume);
	
			int32		Speed();
			int32 		Meter();
			int32 		Volume();

	static	void		PlayBuffer(void* cookie, void* buffer, size_t size,
							const media_raw_audio_format& format);
private:
	status_t			fErr;

	bool 				fRunning;
};

#endif
