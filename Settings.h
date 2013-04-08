/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
#ifndef CRONO_SETTINGS_H
#define CRONO_SETTINGS_H

#include <File.h>
#include <Message.h>
#include <Path.h>
#include <String.h>


class Settings : public BMessage {
public:
						Settings();
virtual					~Settings();

		status_t		OpenSettings();

		status_t		FlattenSettings();
		
		status_t		DeleteSettings();

		BString			TicLocation;
		BString			TocLocation;
		int32 			Speed;
		int32 			Meter;
		int32			CronoVolume;

protected:

		status_t		ReadSetting(const char* name, BString* string);
		status_t		ReadSetting(const char* name, int32* setting);
		status_t		ReadSetting(const char* name, bool* setting);

		status_t		WriteSetting(const char* name, const char* string);
		status_t		WriteSetting(const char* name, int32 setting);
		status_t		WriteSetting(const char* name, bool setting);
		
		status_t		ReplaceSetting(const char* name, const char* string);
		status_t		ReplaceSetting(const char* name, int32 setting);
		status_t		ReplaceSetting(const char* name, bool setting);

		status_t		RemoveSetting(const char* name);

private:
		void			_CheckSettings();
		void			_SetTo();
		BFile*			fSettingsFile;
		BPath			fSettingsPath;

		int32			fControl;
}; 

extern Settings gCronoSettings;

#endif /* SETTINGS_H */
