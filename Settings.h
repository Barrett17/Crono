/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Dario Casalinuovo, barrett666@gmail.com
 */
#ifndef BETICK_SETTINGS_H
#define BETICK_SETTINGS_H

#include <File.h>
#include <Message.h>
#include <Path.h>
#include <String.h>

class Settings : public BMessage {
public:
						Settings(BPath path);
virtual					~Settings();

		status_t		OpenSettings();

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

		status_t		FlattenSettings();
		
		status_t		DeleteSettings();
protected:
		BFile*			fSettingsFile;
		BPath			fSettingsPath;
}; 

#endif /* SETTINGS_H */
