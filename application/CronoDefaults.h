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
	{ 10, 39, "Grave" },
	{ 40, 59, "Largo" },
	{ 60, 65, "Larghetto" },
	{ 66, 75, "Lento/Adagio" },
	{ 76, 107, "Andante" },
	{ 108, 119, "Moderato" },
	{ 120, 139, "Allegro" },
	{ 140, 167, "Vivace" },
	{ 168, 200, "Presto" },
	{ 201, 500, "Prestissimo" },

};

#endif
