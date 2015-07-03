/*
 * Copyright (c) 2012-2015  Dario Casalinuovo (b.vitruvio@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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

		bool			LocationsChanged();

		BString			SoundFileLocation;

		int32 			Speed;
		int32 			Meter;
		int32			MeterOther;

		float			CronoVolume;
		int32			Engine;
		bool			AccentTable;

protected:

		status_t		ReadSetting(const char* name, BString* string);
		status_t		ReadSetting(const char* name, int32* setting);
		status_t		ReadSetting(const char* name, bool* setting);
		status_t		ReadSetting(const char* name, float* setting);

		status_t		WriteSetting(const char* name, const char* string);
		status_t		WriteSetting(const char* name, int32 setting);
		status_t		WriteSetting(const char* name, bool setting);
		status_t		WriteSetting(const char* name, float setting);
		
		status_t		ReplaceSetting(const char* name, const char* string);
		status_t		ReplaceSetting(const char* name, int32 setting);
		status_t		ReplaceSetting(const char* name, bool setting);

		status_t		RemoveSetting(const char* name);

private:
		const char*		fSoundFileLocationUndo;

		void			_CheckSettings();
		void			_SetTo();
		BFile*			fSettingsFile;
		BPath			fSettingsPath;

		int32			fControl;
}; 

extern Settings gCronoSettings;

#endif /* SETTINGS_H */
