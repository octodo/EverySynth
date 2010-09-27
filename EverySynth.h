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

#include <CoreMIDI/CoreMIDI.h>

#include "MusicDeviceBase.h"

#include "EverySynthVersion.h"
 
#if AU_DEBUG_DISPATCHER
#include "AUDebugDispatcher.h"
#endif

#ifndef __EverySynth_h__
#define __EverySynth_h__

// Linked list for devices.
struct MIDIDeviceList {
	CFStringRef name;
	MIDIEndpointRef endpoint;
	MIDIDeviceList * next;
};

#include "Parameters.h"

class EverySynth : public MusicDeviceBase
{
public:
    EverySynth(ComponentInstance inComponentInstance);
	virtual						~EverySynth() {}
    
	virtual ComponentResult		Initialize();
	virtual void				Cleanup() {}
	virtual ComponentResult		Version() { return kEverySynthVersion; }
    
	virtual ComponentResult		GetParameterInfo(AudioUnitScope					inScope,
                                                 AudioUnitParameterID			inParameterID,
                                                 AudioUnitParameterInfo &		outParameterInfo);
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &                outDataSize,
												Boolean	&               outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 	inElement,
											void *			outData);
    

    virtual OSStatus			SetProperty(AudioUnitPropertyID 			inID,
                                            AudioUnitScope 					inScope,
                                            AudioUnitElement 				inElement,
                                            const void *					inData,
                                            UInt32 							inDataSize);

    virtual ComponentResult		GetParameterValueStrings (AudioUnitScope          inScope,
                                                          AudioUnitParameterID    inParameterID,
                                                          CFArrayRef              *outStrings);
    
	virtual ComponentResult 	SetParameter(AudioUnitParameterID			inID,
                                             AudioUnitScope 				inScope,
                                             AudioUnitElement 				inElement,
                                             Float32						inValue,
                                             UInt32							inBufferOffsetInFrames);
    
	virtual OSStatus			StartNote(MusicDeviceInstrumentID 	inInstrument, 
                                          MusicDeviceGroupID 			inGroupID, 
                                          NoteInstanceID 				*outNoteInstanceID, 
                                          UInt32 						inOffsetSampleFrame, 
                                          const MusicDeviceNoteParams &inParams) { return noErr; }
    
	virtual ComponentResult		StopNote(MusicDeviceGroupID 			inGroupID, 
                                         NoteInstanceID 				inNoteInstanceID, 
                                         UInt32 						inOffsetSampleFrame) { return noErr; }
    
	virtual ComponentResult		Reset(AudioUnitScope 				inScope,
                                      AudioUnitElement 				inElement);
    
	virtual ComponentResult		Render(AudioUnitRenderActionFlags &     ioActionFlags,
                                       const AudioTimeStamp &			inTimeStamp,
                                       UInt32							inNumberFrames);
    
	virtual bool				StreamFormatWritable(AudioUnitScope             scope,
                                                     AudioUnitElement           element) { return true; }

    virtual int		GetNumCustomUIComponents () { return 1; }
	
    virtual void 	GetUIComponentDescs (ComponentDescription* inDescArray);

/*
	virtual OSStatus			GetInstrumentCount (	UInt32 &outInstCount) const;

	virtual OSStatus			PrepareInstrument(MusicDeviceInstrumentID inInstrument);
    
	virtual OSStatus			ReleaseInstrument(MusicDeviceInstrumentID inInstrument);
*/
 
protected:
	virtual OSStatus	HandleMidiEvent(UInt8 	inStatus,
                                        UInt8 	inChannel,
                                        UInt8 	inData1,
                                        UInt8 	inData2,
                                        UInt32 	inStartFrame);
    
private:
	void UpdateMidiDevices();
	void InitHardware();
	
	MIDIDeviceList * midiDevices;
	int midiDeviceCount;
	
	MIDIPortRef midiPort;
	MIDIClientRef midiClient;
	MIDIEndpointRef midiEndpoint;
	
	bool hardwareDidInit;
	bool deviceListDidInit;
    
    char * property_MidiDeviceType;
};


#endif
