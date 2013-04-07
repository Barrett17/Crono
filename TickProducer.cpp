/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Copyright (c) Davide Gessa. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Barrett
 */
#include <stdio.h>
#include <string.h>

#include <Buffer.h>
#include <BufferGroup.h>
#include <File.h>
#include <MediaFile.h>
#include <MediaTrack.h>

#include "TickProducer.h"

//TODO should be moved away and integrated in settings
#define TIC_AUDIO_FILE_LOCATION "./audio/tic.wav"
#define TOC_AUDIO_FILE_LOCATION "./audio/toc.wav"


TickProducer::TickProducer()
	:
	BMediaNode("BeTick_Metronome_Node"),
	BBufferProducer(B_MEDIA_RAW_AUDIO),
	BMediaEventLooper()
{
	BMediaFile* mediaFile;
	media_format formt;
	
	fBufferGroup = NULL;
	fFormat.type = B_MEDIA_RAW_AUDIO;
	
	fOutput.destination = media_destination::null;
	fOutput.format = fFormat;
	
	
	// Load tic track
	// per ora questo codice rimane qui, sto ancora decidendo se utilizzare
	// un thread secondario (TrackReader)
	mediaFile = new BMediaFile(new BFile(TIC_AUDIO_FILE_LOCATION, B_READ_WRITE));
	fErr = mediaFile->InitCheck();
	
	//TODO: implement errors notification in a rationable method
	if (fErr != B_OK) {
		delete fMediaFile;
		// error notification
		return;
	}

	fTicTrack = mediaFile->TrackAt(0);
	formt.type = B_MEDIA_RAW_AUDIO;

	if(fTicTrack->DecodedFormat(&formt) != B_OK) {
		printf("Bad DecodedFormat()");
		delete fMediaFile;
		return;
	}

	if (formt.type == fFormat.type) {
		printf("format ok %d \n", (int) fTicTrack->Duration());
		memcpy(&fFormat, &formt, sizeof(fFormat));
	}

	if (fFormat.u.raw_audio.format == media_raw_audio_format::B_AUDIO_FLOAT)
		printf("B_AUDIO_FLOAT\n");

	// Load toc track
	mediaFile = new BMediaFile(new BFile(TOC_AUDIO_FILE_LOCATION, B_READ_WRITE));
	fErr = mediaFile->InitCheck();
	
	if (fErr != B_OK) {
		delete fMediaFile;
		return;
	}

	fTocTrack = mediaFile->TrackAt(0);
	formt.type = B_MEDIA_RAW_AUDIO;

	if(fTocTrack->DecodedFormat(&formt) != B_OK) {
		printf("Bad DecodedFormat()");
		delete fMediaFile;
		return;
	}

	if (formt.type == fFormat.type) {
		printf("format ok %d \n", (int) fTocTrack->Duration());
		memcpy(&fFormat, &formt, sizeof(fFormat));
	}
}


TickProducer::~TickProducer()
{
	Quit();
}


// BMediaNode methods
BMediaAddOn *
TickProducer::AddOn(int32 *internal_id) const
{
	return NULL;
}


// BBufferProducer methods
status_t 
TickProducer::GetNextOutput(int32* cookie, media_output* out_output)
{
	if (cookie == 0)
		*out_output = fOutput;
	else
		return B_BAD_INDEX;

	*cookie+=1;

	return B_OK;
}


status_t 
TickProducer::DisposeOutputCookie(int32 cookie)
{
	return B_OK;
}


void 
TickProducer::EnableOutput(const media_source& what, bool enabled, int32* _deprecated_)
{
	if (what != fOutput.source) {
		//error
		return;
	}

	fOutputEnabled = enabled;
}


status_t 
TickProducer::FormatSuggestionRequested(media_type type, int32 /*quality*/, media_format* format)
{
	if (!format)
		return B_BAD_VALUE;

	*format = fFormat;

	if (type == B_MEDIA_UNKNOWN_TYPE) return B_OK;

	return (type != B_MEDIA_RAW_AUDIO) ? B_MEDIA_BAD_FORMAT : B_OK;
}


status_t 
TickProducer::FormatProposal(const media_source& output, media_format* format)
{
	if (output != fOutput.source) return B_MEDIA_BAD_SOURCE;

	*format = fFormat;

	media_type requestedType = format->type;
	if ((requestedType != B_MEDIA_UNKNOWN_TYPE) && (requestedType != B_MEDIA_RAW_AUDIO))
		return B_MEDIA_BAD_FORMAT;

	return B_OK;
}


status_t 
TickProducer::PrepareToConnect(const media_source& what, const media_destination& where, media_format* format, media_source* out_source, char* out_name)
{
	if (what != fOutput.source)
		return B_MEDIA_BAD_SOURCE;

	if (fOutput.destination != media_destination::null)
		return B_MEDIA_ALREADY_CONNECTED;
	else if (format->type != B_MEDIA_RAW_AUDIO)
		return B_MEDIA_BAD_FORMAT;

	if (format->u.raw_audio.channel_count > 2) 
	{
		format->u.raw_audio.channel_count = 2;
		return B_MEDIA_BAD_FORMAT;
	}
	// .. continue
	return B_OK;
}


void 
TickProducer::Connect(status_t error, const media_source& source, const media_destination& destination, const media_format& format, char* io_name)
{
	bigtime_t latency;
	
	GetLatency(&latency);
	
	int32 size = fOutput.format.u.raw_audio.buffer_size;
	int32 count = int32(latency / BufferDuration() + 2);
	fBufferGroup = new BBufferGroup(size, count);
	
	fOutput.destination = destination;
	fOutput.source = source;
	
	Run();
}


void 
TickProducer::Disconnect(const media_source& what, const media_destination& where)
{
	delete fBufferGroup;
}


status_t 
TickProducer::FormatChangeRequested(const media_source& source, const media_destination& destination, media_format* io_format, int32* _deprecated_)
{
	// we don't support format change
	return B_ERROR;
}


status_t 
TickProducer::SetBufferGroup(const media_source& for_source, BBufferGroup* newGroup)
{
	if (fOutput.source != for_source)
		return B_MEDIA_BAD_SOURCE;

	if (newGroup == fBufferGroup)
		return B_OK;

	// delete the last buffer before to use the new one
	delete fBufferGroup;

	//if (newGroup == NULL)
		// fill_the_buffer()
		

	fBufferGroup = newGroup;

	return B_OK;
}


status_t 
TickProducer::GetLatency(bigtime_t* out_latency)
{
	*out_latency = EventLatency() + SchedulingLatency();
	return B_OK;
}


void 
TickProducer::LateNoticeReceived(const media_source& what, bigtime_t how_much, bigtime_t performance_time)
{

}


void 
TickProducer::LatencyChanged(const media_source& source, const media_destination& destination, bigtime_t new_latency, uint32 flags)
{
	if ((source == fOutput.source) && (destination == fOutput.destination)) {
		fLatency = new_latency;
		SetEventLatency(fLatency + fInternalLatency);
	}	
}


status_t 
TickProducer::SetPlayRate(int32 numer, int32 denom)
{
	return B_ERROR;
}


status_t 
TickProducer::HandleMessage(int32 message, const void* data, size_t size)
{
	return B_ERROR;
}


void 
TickProducer::AdditionalBufferRequested(const media_source& source, media_buffer_id prev_buffer, bigtime_t prev_time, const media_seek_tag* prev_tag)
{
	//sendanotherbuffer
	return;
}


// BMediaEventLooper methods
void 
TickProducer::NodeRegistered()
{
	SetPriority(B_URGENT_PRIORITY);
	Run();
}


void 
TickProducer::SetRunMode(run_mode mode)
{
}


void 
TickProducer::HandleEvent(const media_timed_event* event, bigtime_t lateness, bool realTimeEvent)
{
	switch (event->type)
	{
		case BTimedEventQueue::B_START:
			// Schedule first buffer request
			if (RunState() != B_STARTED) 
			{
				media_timed_event startEvent(event->event_time, BTimedEventQueue::B_HANDLE_BUFFER);
				EventQueue()->AddEvent(startEvent);
			}
			break;

		case BTimedEventQueue::B_STOP:
			// Remove all scheduled events
			EventQueue()->FlushEvents(0, BTimedEventQueue::B_ALWAYS, true,
				BTimedEventQueue::B_HANDLE_BUFFER);
			break;

		case BTimedEventQueue::B_HANDLE_BUFFER:
		{
			if (RunState() == B_STARTED) 
			{
				// Create new buffer and send it to the consumer
				BBuffer* buffer = FillNextBuffer(event->event_time);
				
				if (buffer != NULL) 
				{
					status_t err = B_ERROR;
					if (fOutputEnabled) 
					{
						err = SendBuffer(buffer, fOutput.source,
							fOutput.destination);
					}
					if (err != B_OK)
						buffer->Recycle();
				}
				
				// Schedule new buffer request
				media_timed_event nextEvent(event->event_time + BufferDuration(), BTimedEventQueue::B_HANDLE_BUFFER);
				EventQueue()->AddEvent(nextEvent);
			} 
			break;
		}
	}
}


void 
TickProducer::SetMeter(int32 meter)
{
	fMeter = meter;	
}



void
TickProducer::SetSpeed(int32 speed)
{
	fSpeed = speed;
}
	


BBuffer*
TickProducer::FillNextBuffer(bigtime_t eventTime)
{	
	BBuffer *buffer = NULL;
	
	fBufferGroup->WaitForBuffers();
	fBufferGroup->RequestBuffer(fOutput.format.u.raw_audio.buffer_size, BufferDuration());
	

	return buffer;
}


