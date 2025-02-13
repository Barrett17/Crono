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
 
#include "Settings.h"

#include <Entry.h>
#include <FindDirectory.h>
#include <new>

#include <stdio.h>

#include "CronoDefaults.h"

const uint32 kMsgSettings = 'stng';

Settings gCronoSettings;


Settings::Settings()
	:
	BMessage(kMsgSettings)
{
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY, &fSettingsPath);
	if (result == B_OK) {
		fSettingsPath.Append(CRONO_SETTINGS_LOCATION);
		fSettingsFile = new(std :: nothrow) BFile(fSettingsPath.Path(),
			B_READ_WRITE | B_CREATE_FILE);
	} else {
		printf("Error with settings\n");
		// BAlert ERROR!
	}
	OpenSettings();
}


Settings::~Settings()
{
	FlattenSettings();
	delete fSettingsFile;
}


status_t
Settings::OpenSettings()
{
	printf("open\n");
	status_t ret = fSettingsFile->InitCheck();

	if (ret < B_OK) {
		printf("Settings: initchek error %s\n", strerror(ret));
		return ret;	
	}

	ret = Unflatten(fSettingsFile);

	if (ret < B_OK) {
		printf("Settings: unflatten error %s\n", strerror(ret));
		// it seems the file is empty or corrupt, go with default values.
		Speed = DEFAULT_SPEED;
		Meter = DEFAULT_METER;
		CronoVolume = DEFAULT_VOLUME;
		SoundFileLocation = CRONO_SOUNDFILE_LOCATION;
		Engine = CRONO_SINE_ENGINE;
		AccentTable = false;
		return ret;
	}

	_SetTo();
	return B_OK;	
}


status_t
Settings::DeleteSettings()
{
	delete fSettingsFile;
	BEntry* entry = new BEntry(fSettingsPath.Path(), false);
	printf("Settings::DeleteSettings() the entry is %s\n",
		fSettingsPath.Path());

	status_t ret = entry->Remove();
	if (ret < B_OK)
		return ret;

	delete entry;
	return B_OK;
}


status_t
Settings::FlattenSettings()
{
	printf("flatten\n");
	_CheckSettings();

	fSettingsFile->Seek(0, SEEK_SET);
	return Flatten(fSettingsFile);
}


// Used to check if the audio files location has changed.
bool
Settings::LocationsChanged()
{
	if (SoundFileLocation.ICompare(fSoundFileLocationUndo) != 0)
		return true;

	return false;
}


status_t
Settings::ReadSetting(const char* name, BString* string)
{
	const char* setting;
	
	status_t ret = FindString(name, &setting);
	
	if (ret < B_OK) {
		return ret;
	} else {
		string->SetTo(setting);
		return ret;
	}	
}


status_t
Settings::ReadSetting(const char* name, int32* setting)
{
	return FindInt32(name, setting);
}


status_t
Settings::ReadSetting(const char* name, bool* setting)
{
	return FindBool(name, setting);
}


status_t
Settings::ReadSetting(const char* name, float* setting)
{
	return FindFloat(name, setting);
}



status_t
Settings::WriteSetting(const char* name, float setting)
{
	return AddFloat(name, setting);	 
}


status_t
Settings::WriteSetting(const char* name, const char* string)
{
	return AddString(name, string);	 
}


status_t
Settings::WriteSetting(const char* name, int32 setting)
{
	return AddInt32(name, setting);
}


status_t
Settings::WriteSetting(const char* name, bool setting)
{
	return AddBool(name, setting);
}


status_t
Settings::ReplaceSetting(const char* name, const char* string)
{
	return ReplaceString(name, 0, string);
}


status_t
Settings::ReplaceSetting(const char* name, int32 setting)
{
	return ReplaceInt32(name, 0, setting);
}


status_t
Settings::ReplaceSetting(const char* name, bool setting)
{
	return ReplaceBool(name, 0, setting);
}


status_t
Settings::RemoveSetting(const char* name)
{
	return RemoveData(name);
}


void
Settings::_CheckSettings()
{
	MakeEmpty();

	WriteSetting("SOUNDFILE", SoundFileLocation);
	WriteSetting("SPEED", Speed);
	WriteSetting("METER", Meter);
	WriteSetting("VOLUME", CronoVolume);
	WriteSetting("ENGINE", Engine);
	WriteSetting("ACCENT", AccentTable);
}


void
Settings::_SetTo()
{
	fSoundFileLocationUndo = SoundFileLocation.String();

	ReadSetting("SOUNDFILE", &SoundFileLocation);
	ReadSetting("SPEED", &Speed);
	ReadSetting("METER", &Meter);
	ReadSetting("VOLUME", &CronoVolume);
	ReadSetting("ENGINE", &Engine);
	ReadSetting("ACCENT", &AccentTable);
}
