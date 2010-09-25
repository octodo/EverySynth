/*
 EverySynth - A multi-timbral AudioUnit for external synthesizers.
 Copyright (C) 2010  Philip Kranz
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include <AudioUnit/AudioUnit.r>
#include <AudioUnit/AudioUnitCarbonView.r>

#include "EverySynthVersion.h"

#define kAudioUnitResID_EverySynth				1000

#define RES_ID			kAudioUnitResID_EverySynth
#define COMP_TYPE		kAudioUnitType_MusicDevice
#define COMP_SUBTYPE	EverySynth_COMP_SUBTYPE
#define COMP_MANUF		EverySynth_COMP_MANF	

#define VERSION			kEverySynthVersion
#define NAME			"PMLK: EverySynth"
#define DESCRIPTION		"EverySynth AU"
#define ENTRY_POINT		"EverySynthEntry"

#include "AUResources.r"

#define kAudioUnitResID_EverySynthCarbonUI     2000

#define RES_ID			kAudioUnitResID_EverySynthCarbonUI
#define COMP_TYPE		'auvw'
#define COMP_SUBTYPE	'ESCV'
#define COMP_MANUF		'PMLK'

#define VERSION			kEverySynthVersion
#define NAME			"PMLK: EverySynthCarbonUI"
#define DESCRIPTION		"EverySynth Carbon UI"
#define ENTRY_POINT		"EverySynthCarbonUIEntry"

#include "AUResources.r"
