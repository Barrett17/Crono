/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 */
#ifndef BETICK_CORE_H
#define BETICK_CORE_H

class TickProducer;
class BMediaRoster;

const int32		DEFAULT_SPEED	=	60;
const int32		DEFAULT_VOLUME	=	80;

/**
 * Il core dell'applicazione
 */
class TickCore {
public:
						TickCore();
						~TickCore();

	status_t			InitCheck();
	status_t			Connect();
	status_t			Disconnect();
	void 				Stop();
	void 				Start();

	void 				SetSpeed(int32 speed);
	void 				SetMeter(int32 meter);
	void 				SetVolume(int32 volume);
	
	int32 				Speed();
	int32 				Meter();
	int32 				Volume();

private:
	int32 				fSpeed;
	int32 				fMeter;
	int32				fVolume;
	bool 				fRunning;

	status_t			fErr;
	BMediaRoster* 		fMediaRoster;
	TickProducer*		fTickProducer;
};


#endif
