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

#include "EverySynth.h"

/* Macro to get an entry from a linked list */
#define GET_CLIST_ENTRY(list,nr) 	for (;nr>0;nr--) { if (list) { list = list->next; } }


COMPONENT_ENTRY(EverySynth)

int kNumChannels = 9;
// int kNumChannels = 16        /* 10.6 only, 10.5 is buggy! */

#pragma mark FRAMEWORK METHODS

EverySynth::EverySynth(AudioUnit component)
: MusicDeviceBase(component, 0, 1, kNumChannels, kNumChannels)
{
	DEBUG_OUT("EverySynth::EverySynth")
    
	midiDevices = NULL;
	
	hardwareDidInit = false;
    
#if AU_DEBUG_DISPATCHER
    FILE * f = fopen("/Users/philipkranz/everysynth.log", "w");
	mDebugDispatcher = new AUDebugDispatcher (this, f);
#endif
	
}

ComponentResult EverySynth::Initialize()
{	
	DEBUG_OUT("EverySynth::Initialize")
    
	MusicDeviceBase::Initialize();

	Globals()->UseIndexedParameters(kNumberOfGlobalParameters);
	Globals()->SetParameter(kParam_MidiOutputDevice, kDefaultValue_MidiOutputDevice );

    for (int i=0; i<kNumChannels; i++) {
        Parts().GetElement(i)->UseIndexedParameters(kNumberOfChannelParameters);
        Parts().GetElement(i)->SetParameter(kParam_Active, kDefaultValue_Active);
        Parts().GetElement(i)->SetParameter(kParam_MidiControlType, kDefaultValue_MidiControlType);
        Parts().GetElement(i)->SetParameter(kParam_MidiControlPatch_BankMSB, kDefaultValue_MidiControlPatch_BankMSB);
        Parts().GetElement(i)->SetParameter(kParam_MidiControlPatch_BankLSB, kDefaultValue_MidiControlPatch_BankLSB);
        Parts().GetElement(i)->SetParameter(kParam_MidiControlPatch_Patch, kDefaultValue_MidiControlPatch_Patch);
        Parts().GetElement(i)->SetParameter(kParam_Volume, kDefaultValue_Volume);
    }
    
	UpdateMidiDevices();
	
	MIDIClientCreate(CFSTR("EverySynth"),NULL,NULL,&midiClient);
	MIDIOutputPortCreate(midiClient,CFSTR("EverySynth Out Port"),&midiPort);	
	
	DEBUG_OUT("Done Initializing")
	
	return noErr;
}

/*
 * I have no idea how and if these methods are called...
 * Anyway, EverySynth::Reset() seems to be sufficient for
 * that purpose.
 
OSStatus EverySynth::PrepareInstrument(MusicDeviceInstrumentID inInstrument) {
    DEBUG_OUT1("EverySynth::PrepareInstrument(%d)", (int)inInstrument)
    return noErr;
}

OSStatus EverySynth::ReleaseInstrument(MusicDeviceInstrumentID inInstrument) {
    DEBUG_OUT1("EverySynth::ReleaseInstrument(%d)", (int)inInstrument)
    return noErr;
}
 */

/*
 * This is probably useless, too.
 
OSStatus EverySynth::GetInstrumentCount (UInt32 &outInstCount) const
{
	outInstCount = kNumChannels;
	return noErr;
}
 */

ComponentResult	EverySynth::GetParameterInfo(AudioUnitScope inScope,
                                             AudioUnitParameterID inParameterID,
                                             AudioUnitParameterInfo &outParameterInfo)
{
	DEBUG_OUT("EverySynth::GetParameterInfo")
    
	ComponentResult result = noErr;
    
	outParameterInfo.flags = kAudioUnitParameterFlag_IsWritable
                           | kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_MidiOutputDevice:
                AUBase::FillInParameterName (outParameterInfo, kParamName_MidiOutputDevice, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = (midiDeviceCount>0) ? midiDeviceCount-1 : 0; // dito
                outParameterInfo.defaultValue = kDefaultValue_MidiOutputDevice;
                break;
            default:
                result = kAudioUnitErr_InvalidParameter;
                break;
		}
	}
	else if (inScope == kAudioUnitScope_Part) {
		switch (inParameterID) {
            case kParam_Active:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Active, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 1;
                outParameterInfo.defaultValue = kDefaultValue_Active;
                break;
			case kParam_MidiControlType:
                AUBase::FillInParameterName (outParameterInfo, kParamName_MidiControlType, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = midiControlType_LastEntry-1;
                outParameterInfo.defaultValue = kDefaultValue_MidiControlType;
                break;
            case kParam_MidiControlPatch_BankMSB:
                AUBase::FillInParameterName (outParameterInfo, kParamName_MidiControlPatch_BankMSB, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 127;
                outParameterInfo.defaultValue = kDefaultValue_MidiControlPatch_BankMSB;
                break;
            case kParam_MidiControlPatch_BankLSB:
                AUBase::FillInParameterName (outParameterInfo, kParamName_MidiControlPatch_BankLSB, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 127;
                outParameterInfo.defaultValue = kDefaultValue_MidiControlPatch_BankLSB;
                break;
            case kParam_MidiControlPatch_Patch:
                AUBase::FillInParameterName (outParameterInfo, kParamName_MidiControlPatch_Patch, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 127;
                outParameterInfo.defaultValue = kDefaultValue_MidiControlPatch_Patch;
                break;
            case kParam_Volume:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Volume, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 127;
                outParameterInfo.defaultValue = kDefaultValue_Volume;
                break;
            default:
                result = kAudioUnitErr_InvalidParameter;
                break;
        }
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
    
	return result;
}

ComponentResult EverySynth::SetParameter(AudioUnitParameterID inID,
                                         AudioUnitScope inScope,
                                         AudioUnitElement inElement,
                                         Float32 inValue,
                                         UInt32 inBufferOffsetInFrames)
{
	DEBUG_OUT("EverySynth::SetParameter")
    
	if (inScope == kAudioUnitScope_Global) {
		if (inID == kParam_MidiOutputDevice) {
			if (inValue >= midiDeviceCount)
				inValue = kDefaultValue_MidiOutputDevice;
		}
	}
	
	hardwareDidInit = false;
	
	return MusicDeviceBase::SetParameter(inID, inScope, inElement, inValue, inBufferOffsetInFrames);
}

ComponentResult EverySynth::GetParameterValueStrings(AudioUnitScope inScope,
                                                     AudioUnitParameterID inParameterID,
                                                     CFArrayRef *outStrings)
{
	DEBUG_OUT("EverySynth::GetParameterValueStrings")

    if (inScope == kAudioUnitScope_Global) {
        if (inParameterID == kParam_MidiOutputDevice) {
            
			if (outStrings == NULL) return noErr;
			CFMutableArrayRef arr = CFArrayCreateMutable(CFAllocatorGetDefault(),20,NULL);
			MIDIDeviceList * curDevice = midiDevices;
			if (curDevice) {
				while (curDevice) {
					CFArrayAppendValue(arr,curDevice->name);
					curDevice = curDevice->next;
				}
			}
			else {
				CFArrayAppendValue(arr, CFSTR("no device"));
			}
            
			*outStrings = CFArrayCreateCopy(CFAllocatorGetDefault(),arr);
            
			return noErr;
		}
	}
	else if (inScope == kAudioUnitScope_Part) {
		if (inParameterID == kParam_Active) {
			if (outStrings == NULL) return noErr;
			
			CFStringRef typeNames[2] = {
				CFSTR("No"),
				CFSTR("Yes")
			};
			
			*outStrings = CFArrayCreate(CFAllocatorGetDefault(), (const void**)typeNames, 2, NULL);
			
			return noErr;
		}
/*
		else if (inParameterID == kParam_MidiControlPatch_BankMSB || inParameterID == kParam_MidiControlPatch_BankLSB) {
			if (outStrings == NULL) return noErr;
			
			CFStringRef bankNames[7] = {
                CFSTR("User Bank"),
                CFSTR("Bank A"),
                CFSTR("Bank B"),
                CFSTR("Bank C"),
                CFSTR("Bank D"),
                CFSTR("Bank E"),
                CFSTR("Bank F")
			};
			
			*outStrings = CFArrayCreate(CFAllocatorGetDefault(), (const void**)bankNames, 7, NULL);
			
			return noErr;
		}
*/
		else if (inParameterID == kParam_MidiControlType) {
			if (outStrings == NULL) return noErr;
			
			CFStringRef typeNames[2] = {
				CFSTR("Program Change"),
				CFSTR("Patch Dump (not yet supported)")
			};
			
			*outStrings = CFArrayCreate(CFAllocatorGetDefault(), (const void**)typeNames, 2, NULL);
			
			return noErr;
		}
		else if (inParameterID == kParam_MidiControlPatch_Patch || inParameterID == kParam_MidiControlPatch_BankMSB || inParameterID == kParam_MidiControlPatch_BankLSB) {
			if (outStrings == NULL) return noErr;
			
			CFStringRef patchNames[128];
			for (int i=0;i<128;i++) {
				patchNames[i] = CFStringCreateWithFormat(CFAllocatorGetDefault(),NULL,CFSTR("%d"),i);
			}
			
			*outStrings = CFArrayCreate(CFAllocatorGetDefault(), (const void**)patchNames, 128, NULL);
			
			return noErr;
		}
    }
    return kAudioUnitErr_InvalidParameter;
}

ComponentResult EverySynth::GetPropertyInfo (AudioUnitPropertyID inID,
                                             AudioUnitScope inScope,
                                             AudioUnitElement	inElement,
                                             UInt32 &outDataSize,
                                             Boolean &outWritable)
{
	DEBUG_OUT("EverySynth::GetPropertyInfo")
    
    if (inScope == kAudioUnitScope_Part) {
        switch (inID) {
            case kMusicDeviceProperty_PartGroup:
                outWritable = false;
                outDataSize = sizeof(AudioUnitElement);
                return noErr;
        }
    }

        // Cocoa UI stuff.
      
    else if (inScope == kAudioUnitScope_Global) {
        switch (inID) {
            case kAudioUnitProperty_CocoaUI:
                outWritable = false;
                outDataSize = sizeof (AudioUnitCocoaViewInfo);
                return noErr;
        }
    }

    
    return MusicDeviceBase::GetPropertyInfo(inID, inScope, inElement, outDataSize, outWritable);
}

ComponentResult EverySynth::GetProperty(AudioUnitPropertyID inID,
                                        AudioUnitScope inScope,
                                        AudioUnitElement inElement,
                                        void *outData)
{   
	DEBUG_OUT("EverySynth::GetProperty")
    
    if (inScope == kAudioUnitScope_Part) {
        switch (inID) {
            case kMusicDeviceProperty_PartGroup:
                *((AudioUnitElement*)outData) = inElement;
                return noErr;
        }
    }

        // More Cocoa UI stuff.

    else if (inScope == kAudioUnitScope_Global) {
        switch (inID) {
            case kAudioUnitProperty_CocoaUI:
                // Look for a resource in the main bundle by name and type.
                CFBundleRef bundle = CFBundleGetBundleWithIdentifier(CFSTR("net.pmlk.audiounit.EverySynth.CocoaUI"));
     
                if (bundle == NULL) return fnfErr;
     
                CFURLRef bundleURL = CFBundleCopyResourceURL(bundle, 
                                                             CFSTR("CocoaUI"), 
                                                             CFSTR("bundle"), 
                                                             NULL);
     
                if (bundleURL == NULL) return fnfErr;
     
                CFStringRef className = CFSTR("ESyn_CocoaViewFactory");
                AudioUnitCocoaViewInfo cocoaInfo = { bundleURL, className };
                *((AudioUnitCocoaViewInfo *)outData) = cocoaInfo;
     
                return noErr;
        }
    }

    
	return MusicDeviceBase::GetProperty (inID, inScope, inElement, outData);
}

OSStatus EverySynth::HandleMidiEvent(UInt8 inStatus,
                                     UInt8 inChannel,
                                     UInt8 inData1,
                                     UInt8 inData2,
                                     UInt32 inStartFrame)
{	
	DEBUG_OUT("EverySynth::HandleMidiEvent")
    
    // Don't know if that's needed anymore.
	if (!hardwareDidInit)
		InitHardware();
	if (!deviceListDidInit)
		UpdateMidiDevices();
    
	int devNr = (int)Globals()->GetParameter(kParam_MidiOutputDevice);
	MIDIDeviceList * curDevice = midiDevices;
    
	GET_CLIST_ENTRY(curDevice,devNr)
	
	if (!curDevice)
		return noErr;
    
	MIDIPacketList pktlist;
	MIDIPacket * packet = MIDIPacketListInit(&pktlist);
    
	for (int i=0; i<kNumChannels; i++) {
		if (Parts().GetElement(i)->GetParameter(kParam_Active) == 0)
			continue;
		
		UInt8 data[] = {
			inStatus | (UInt8)i,
			inData1,
			inData2
		};
		
		packet = MIDIPacketListAdd(&pktlist,sizeof(pktlist),packet,0,3,data);
	}
	
	MIDISend(midiPort,curDevice->endpoint,&pktlist);
    
	
	return noErr;
}


ComponentResult	EverySynth::Reset(AudioUnitScope inScope,
                                  AudioUnitElement inElement)
{
	DEBUG_OUT("EverySynth::Reset")

	UpdateMidiDevices();
	InitHardware();
	
	return noErr;
}

ComponentResult	EverySynth::Render(AudioUnitRenderActionFlags &ioActionFlags,
                                   const AudioTimeStamp &inTimeStamp,
                                   UInt32 inNumberFrames)
{
	UInt32 numOutputs = Outputs().GetNumberOfElements();
	for (UInt32 j = 0; j < numOutputs; ++j)
	{
		AudioBufferList& bufferList = GetOutput(j)->GetBufferList();
		for (UInt32 k = 0; k < bufferList.mNumberBuffers; ++k)
		{
			memset(bufferList.mBuffers[k].mData, 0, bufferList.mBuffers[k].mDataByteSize);
		}
	}
	
	return noErr;
}

#pragma mark PRIVATE METHODS

/*
 * Update the list of midi devices.
 */
void EverySynth::UpdateMidiDevices()
{
	MIDIDeviceList * nextDevice = NULL;
	MIDIDeviceList * curDevice = midiDevices;
	while (curDevice) {
		nextDevice = curDevice->next;
		CFRelease(curDevice->name);
		delete curDevice;
		curDevice = nextDevice;
	}
	midiDevices = new MIDIDeviceList;
	curDevice = midiDevices;
	MIDIDeviceList * lastDevice = NULL;
	
	midiDeviceCount = 0;
	
	int midiDevCount = MIDIGetNumberOfDestinations();
	int j;
	for (j=0;j<midiDevCount;j++) {
		MIDIEndpointRef device = MIDIGetDestination(j);
        
		CFStringRef deviceName;
		MIDIObjectGetStringProperty(device,kMIDIPropertyName,&deviceName);
		
		SInt32 offline;
		MIDIObjectGetIntegerProperty(device,kMIDIPropertyOffline,&offline);
		
		if (offline) continue;
		
		CFStringRef model;
		MIDIObjectGetStringProperty(device,kMIDIPropertyModel,&model);
		
		curDevice->name = CFStringCreateWithFormat(CFAllocatorGetDefault(),NULL,CFSTR("%@ - %@"),model,deviceName);
		curDevice->endpoint = device;
		curDevice->next = new MIDIDeviceList;
		lastDevice = curDevice;
		curDevice = curDevice->next;
		curDevice->next = NULL;
		midiDeviceCount++;
	}
	if (!lastDevice) {
		midiDevices = NULL;
		return;
	}
	
	lastDevice->next = NULL;
	
	deviceListDidInit = true;
}

/*
 * Send the appropriate Bank/Patch select and volume messages.
 */
void EverySynth::InitHardware()
{
	Float32 bank_msb, bank_lsb, patch, volume;
	UInt8 channel;
	int controlType;
	AUElement * element;
	
	int devNr = (int)Globals()->GetParameter(kParam_MidiOutputDevice);
	MIDIDeviceList * curDevice = midiDevices;
	
	GET_CLIST_ENTRY(curDevice,devNr)
	
	if (!curDevice)
		return;
	
	for (int i=0; i<kNumChannels; i++) {
		element = Parts().GetElement(i);
		if (element->GetParameter(kParam_Active) == 0)
			continue;
		
		channel = (UInt8)i;
		controlType = (int)(element->GetParameter(kParam_MidiControlType));
		
		switch (controlType) {
			case midiControlType_PatchSelect:
				bank_msb = element->GetParameter(kParam_MidiControlPatch_BankMSB);
				bank_lsb = element->GetParameter(kParam_MidiControlPatch_BankLSB);				
                patch = element->GetParameter(kParam_MidiControlPatch_Patch);
				volume = element->GetParameter(kParam_Volume);
                
				MIDIPacketList pktlist;
				MIDIPacket * packet = MIDIPacketListInit(&pktlist);
				UInt8 data[3];
				
				// Bank select MSB
				data[0] = 0xB0 | channel;
				data[1] = 0x00;
				data[2] = (UInt8)bank_msb;
				packet = MIDIPacketListAdd(&pktlist,sizeof(pktlist),packet,0,3,data);

				// Bank select LSB
				data[0] = 0xB0 | channel;
				data[1] = 0x20;
				data[2] = (UInt8)bank_lsb;
				packet = MIDIPacketListAdd(&pktlist,sizeof(pktlist),packet,0,3,data);
                
				// Patch select
				data[0] = 0xC0 | channel;
				data[1] = (UInt8)patch;
				packet = MIDIPacketListAdd(&pktlist,sizeof(pktlist),packet,0,2,data);
				
				// Volume
				data[0] = 0xB0 | channel;
				data[1] = 0x07;
				data[2] = (UInt8)volume;
				packet = MIDIPacketListAdd(&pktlist,sizeof(pktlist),packet,0,3,data);
				
				MIDISend(midiPort,curDevice->endpoint,&pktlist);
		}
	}
    
	
	hardwareDidInit = true;
}
