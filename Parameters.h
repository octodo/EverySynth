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

/*
 Currently we support the following parameters:
    
    GLOBAL
    - MidiOutputDevice          (Indexed) The device where our external synth is
                                connected.
 
    PART
    - Active                    (Indexed) Use this part (0 or 1)
    - MidiControlType           (Indexed) How to switch patches. Currently only
                                0 = Bank/Patch select.
    - MidiControlPatch_Bank_MSB (MIDIController) Bank MSB for Patch select.
    - MidiControlPatch_Bank_LSB (MIDIController) Bank LSB for Patch select.
    - MidiControlPatch_Patch    (Indexed) Patch for Patch select.
    - Volume                    (Float) Channel volume.
 */

#define kNumChannels 9

/*** Default Values ***/

// global parameters
static const int kDefaultValue_MidiOutputDevice = 0;

// channel parameters
static const int kDefaultValue_Active = 0;
static const int kDefaultValue_MidiControlType = 0;
static const int kDefaultValue_MidiControlPatch_BankMSB = 0;
static const int kDefaultValue_MidiControlPatch_BankLSB = 0;
static const int kDefaultValue_MidiControlPatch_Patch = 0;
static const int kDefaultValue_Volume = 100;

/*** Parameter Names ***/

// global parameters
static CFStringRef kParamName_MidiOutputDevice = CFSTR("Output Device");

// channel parameters
static CFStringRef kParamName_Active = CFSTR("Active");
static CFStringRef kParamName_MidiControlType = CFSTR("Control Type");
static CFStringRef kParamName_MidiControlPatch_BankMSB = CFSTR("Bank MSB");
static CFStringRef kParamName_MidiControlPatch_BankLSB = CFSTR("Bank LSB");
static CFStringRef kParamName_MidiControlPatch_Patch = CFSTR("Patch");
static CFStringRef kParamName_Volume = CFSTR("Volume");

/*** Parameter IDs ***/

// channel parameters
enum {
	kParam_Active,
	kParam_MidiControlType,
	kParam_MidiControlPatch_BankMSB,
	kParam_MidiControlPatch_BankLSB,
	kParam_MidiControlPatch_Patch,
	kParam_Volume,
	
	kNumberOfChannelParameters
};

// global parameters
enum {
	kParam_MidiOutputDevice,
	
	kNumberOfGlobalParameters
};

/***************** other enums ********************************/

// Control Types
enum {
	midiControlType_PatchSelect,
	midiControlType_PatchDump,
	midiControlType_LastEntry
};
