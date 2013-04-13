#ifndef CRONO_DEFAULTS_H
#define CRONO_DEFAULTS_H

#include "Settings.h"

#define MIN_SPEED 30
#define MAX_SPEED 299

const int32	DEFAULT_SPEED	= 60;
const float	DEFAULT_VOLUME	= 8/10;
const int32	DEFAULT_METER	= 1;

#define CRONO_TIC_LOCATION "/boot/apps/Crono/audio/tic.wav"
#define CRONO_TOC_LOCATION "/boot/apps/Crono/audio/toc.wav"
#define CRONO_SETTINGS_LOCATION "CronoSettings"

struct TempoNames {
	int32 max;
	int32 min;
	const char* name;
};

static TempoNames gTempoNames[] = {
	{ 1, 39, "Grave" },
	{ 40, 50, "Largo" },
	{ 50, 59, "Lento/Adagio" },
	{ 60, 79, "Andante" },
	{ 80, 99, "Moderato" },
	{ 100, 126, "Allegro" },
	{ 127, 144, "Vivace" },
	{ 144, 208, "Presto" },
	{ 209, 500, "Prestissimo" },

};

#endif
