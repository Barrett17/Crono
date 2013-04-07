/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 *		Barrett, barrett666@gmail.com
 */

#include <MediaEventLooper.h>
#include <MediaRoster.h>
#include <TimeSource.h>

#include "TickCore.h"
#include "TickProducer.h"


/*
 * Initialize the class with default values
 */
TickCore::TickCore()
	:
	fSpeed(DEFAULT_SPEED),
	fMeter(0),
	fVolume(DEFAULT_VOLUME),
	fRunning(false)
{
	fTickProducer = new TickProducer();
	Connect();
	
	
	fTickProducer->SetMeter(Meter());
	fTickProducer->SetSpeed(Speed());
}


/*
 * Destroy the class
 */
TickCore::~TickCore()
{
	Disconnect();
	delete fTickProducer;
}


// metodo per controllare eventuali errori interni
status_t
TickCore::InitCheck()
{
	return fErr;
}


// questo dovrebbe occuparsi di connettere i nodi
status_t
TickCore::Connect() 
{
	fMediaRoster = BMediaRoster::Roster(&fErr);
	if ((fErr != B_OK) || !fMediaRoster)) {
		return fErr;
	}

	return fMediaRoster->RegisterNode(fTickProducer);
}


// disconnette i nodi
status_t
TickCore::Disconnect() 
{
	return fMediaRoster->UnregisterNode(fTickProducer);
}


/** Start the metronome */
void 
TickCore::Start()
{
	if (!fRunning) {
		fRunning = 1;	

		fMediaRoster->StartNode(fTickProducer->Node(), 
					fTickProducer->TimeSource()->Now() + 5000LL);
	}
}


/** Stop the metronome */
void 
TickCore::Stop()
{
	if (fRunning) {
		fRunning = 0;
		
		fMediaRoster->StopNode(fTickProducer->Node(), 0);
	}
}


void 
TickCore::SetSpeed(int32 s)
{
	if (s <= 500 && s > 1) {
		fSpeed = s;
		fTickProducer->SetSpeed(s);
	}
}


void 
TickCore::SetMeter(int32 m)
{
	if (m < 10 && m >= 0) {
		fMeter = m;
		fTickProducer->SetMeter(m);
	}
}


void 
TickCore::SetVolume(int32 v)
{
	if (v <= 100 && v >= 0) {
		fVolume = v;
	}
}


int32 
TickCore::Volume()
{
	return fVolume;	
}


int32
TickCore::Speed()
{
	return fSpeed;	
}


int32
TickCore::Meter()
{
	return fMeter;	
}
