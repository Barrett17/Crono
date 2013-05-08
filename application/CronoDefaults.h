#ifndef CRONO_DEFAULTS_H
#define CRONO_DEFAULTS_H

#include "Settings.h"

#define CRONO_HOMEPAGE_URL "https://github.com/Barrett17/Crono"
#define CRONO_USERGUIDE_URL "http://www.versut.com/"

#define MIN_SPEED 30
#define MAX_SPEED 299

const int32	DEFAULT_SPEED	= 60;
const float	DEFAULT_VOLUME	= 0.5;
const int32	DEFAULT_METER	= 3;

#define CRONO_SOUNDFILE_LOCATION "/boot/apps/Crono/audio/tic.wav"
#define CRONO_SETTINGS_LOCATION "CronoSettings"

#define CRONO_SINE_ENGINE		0
#define CRONO_TRIANGLE_ENGINE	1
#define CRONO_SAWTOOTH_ENGINE	2
#define CRONO_FILE_ENGINE		3

// TODO This should be an hardcoded sound based engine
#define CRONO_DEFAULT_ENGINE	4
// TODO implement it to provide a MIDI metronome
#define CRONO_MIDI_ENGINE		5

// Currently not working as well.
// TODO finish it.
//#define CRONO_REPLICANT_ACTIVE 1

#define CRONO_APP_TYPE "application/x-vnd.Crono-Metronome"

#endif
