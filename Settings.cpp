/*
 * Copyright (c) Casalinuovo Dario. All rights reserved.
 * Distributed under the terms of the MIT License for non commercial use.
 *
 * Authors:
 *		Dario Casalinuovo, barrett666@gmail.com
 */
#include "Settings.h"

#include <stdio.h>

#include <Entry.h>
#include <new>

const uint32 kMsgSettings = 'stng';

Settings::Settings(BPath path)
	:
	BMessage(kMsgSettings), fSettingsPath(path)
{
		fSettingsFile = new(std :: nothrow) BFile(path.Path(),
			B_READ_WRITE | B_CREATE_FILE);
}


Settings::~Settings()
{

}


status_t
Settings::OpenSettings()
{
	status_t ret = fSettingsFile->InitCheck();

	if (ret < B_OK) {
		printf("Settings: initchek error %s\n", strerror(ret));
		return ret;	
	}

	ret = Unflatten(fSettingsFile);

	if (ret < B_OK) {
		printf("Settings: unflatten error %s\n", strerror(ret));
		return ret;
	}

	return B_OK;	
}


status_t
Settings::ReadSetting(const char* name, BString* string)
{
	const char* setting;
	
	status_t ret = FindString(name, &setting);
	
	if(ret < B_OK) {
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



status_t
Settings::DeleteSettings()
{
	delete fSettingsFile;
	BEntry* entry = new BEntry(fSettingsPath.Path(), false);
	printf("Settings::DeleteSettings() the entry is %s\n", fSettingsPath.Path());

	status_t ret = entry->Remove();
	if (ret < B_OK)
		return ret;

	delete entry;
	return B_OK;
}


status_t
Settings::FlattenSettings()
{
	fSettingsFile->Seek(0, SEEK_SET);
	return Flatten(fSettingsFile);
}
