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

#include "EverySynthCarbonUI.h"

#define CHANNEL_PARAMETER(param, chan) channelParams[chan * kNumberOfChannelParameters + param]

COMPONENT_ENTRY(EverySynthCarbonUI)

static const CFStringRef kPageLabels[] = { CFSTR("1-64"), CFSTR("65-128") };

void textProc_Volume(UInt32 value, char * text, void * userData)
{
    sprintf(text, "%d", value);
}

void radioArrayProc_ChannelSelect(UInt32 value, CAUGuiRadioArray* from, void* userData)
{
    ((EverySynthCarbonUI*)userData)->activateChannelPane(value);
}

void buttonProc_DeviceType(UInt32 value, CAUGuiButton* from, void* userData)
{
    ((EverySynthCarbonUI*)userData)->selectDevicePopup();
}

void listProc_BankSelect(UInt32 value, CAUGuiList* from, void * userData)
{
    ((EverySynthCarbonUI*)userData)->bankSelect(value);
}

void radioArrayProc_PageSelect(UInt32 value, CAUGuiRadioArray* from, void * userData)
{
    ((CAUGuiList*)userData)->selectPage((int)value);
}

OSStatus	EverySynthCarbonUI::CreateUI(Float32 xoffset, Float32 yoffset)
{
	int XPos = (int) xoffset;
	int YPos = (int) yoffset;
    
    CAAUParameter paramOutputDevice(mEditAudioUnit, kParam_MidiOutputDevice, kAudioUnitScope_Global, 0);
    channelParams = (CAAUParameter**) malloc(sizeof(CAAUParameter*) * kNumberOfChannelParameters * kNumChannels);
    for (int i=0; i<kNumChannels; i++) {
        for (int j=0; j<kNumberOfChannelParameters; j++) {
            CHANNEL_PARAMETER(j, i) = new CAAUParameter(mEditAudioUnit, j, kAudioUnitScope_Part, i);
        }
    }
    
    devDb = new DeviceDB(CFSTR("nothing"));
    deviceList = devDb->getDeviceList();
    CreateNewMenu(17492, 0, &deviceTypePopup);
    for (int i=0; i<CFArrayGetCount(deviceList); i++) {
        AppendMenuItemTextWithCFString(deviceTypePopup, (CFStringRef)CFArrayGetValueAtIndex(deviceList, i), 0, 0, NULL);
    }
	
	theGui = new CAUGuiMan(this, XPos, YPos);
	theGui->initialize();
	
	// Background image
	CAUGuiGraphic * graBackground = new CAUGuiGraphic("background.png");
    theGui->addImage(graBackground);
	CAUGuiGraphic * graChannelPaneBackground = new CAUGuiGraphic("channel_pane_background.png");
    theGui->addImage(graChannelPaneBackground);

    CAUGuiGraphic * graDeviceListBackground = new CAUGuiGraphic("device_list_background.png");
    theGui->addImage(graDeviceListBackground);
    CAUGuiGraphic * graMsbLsbBackground = new CAUGuiGraphic("msb_lsb_background.png");
    theGui->addImage(graMsbLsbBackground);
    CAUGuiGraphic * graPatchListBackground = new CAUGuiGraphic("patch_list_background.png");
    theGui->addImage(graPatchListBackground);
    CAUGuiGraphic * graBankListBackground = new CAUGuiGraphic("bank_list_background.png");
    theGui->addImage(graBankListBackground);

    // Buttons
    CAUGuiGraphic * graChannelMute = new CAUGuiGraphic("channel_mute.png", 2);
    theGui->addImage(graChannelMute);
    CAUGuiGraphic * graDeviceType = new CAUGuiGraphic("device_type.png", 2);
    theGui->addImage(graDeviceType);
    CAUGuiGraphic * graPage = new CAUGuiGraphic("page.png", 2);
    theGui->addImage(graPage);
    CAUGuiGraphic * graChannelSelect = new CAUGuiGraphic("channel_select.png", 2);
    theGui->addImage(graChannelSelect);
    
    // Mixer channel strip
    CAUGuiGraphic * graMixerVolumeBackground = new CAUGuiGraphic("mixer_volume_background.png");
    theGui->addImage(graMixerVolumeBackground);
    CAUGuiGraphic * graMixerVolumeHandle = new CAUGuiCroppingImage("mixer_volume_handle.png", 0, 83, 2, 0);
    theGui->addImage(graMixerVolumeHandle);
    CAUGuiGraphic * graMixerValueBackground = new CAUGuiGraphic("mixer_value_background.png");
    theGui->addImage(graMixerValueBackground);
    
	/***************************************
     
     create controls
     
     ***************************************/
    
	
	// Get size of GUI from Background Image and create Background Pane
	
	int width = CGImageGetWidth(graBackground->getImage());
	int height = CGImageGetHeight(graBackground->getImage());
	
	eRect where;
	where.set(0, 0, width, height);
	
    // Background pane
	paneBackground = new CAUGuiPane(theGui, &where, graBackground);
	theGui->addCtrl(paneBackground);
	
    // DeviceType button
    where.set(13, 40, 231, 31);
    
    buttonDeviceType = new CAUGuiLabeledButton(theGui, 1, &where, graDeviceType, NULL, kPushButton);
    buttonDeviceType->setText("Unknown Device");
    buttonDeviceType->setOnValue(1);
    buttonDeviceType->setUserProcedure(buttonProc_DeviceType, this);
    paneBackground->addCtrl(buttonDeviceType);
    
    // Mixer
    eRect where_value;
    eRect where_mute;
    where.set(265, 53, 22, 181);
    where_value.set(267, 221, 18, 11);
    where_mute.set(265, 33, 22, 19);
    for (int i=0; i<kNumChannels; i++) {
        CAUGuiSlider * channelStrip = new CAUGuiSlider(theGui, *CHANNEL_PARAMETER(kParam_Volume, i), &where, 0, kCAUGui_res_100, graMixerVolumeHandle, graMixerVolumeBackground);
        channelStrip->shrinkForeBounds(2, 2, 2, 13);
        paneBackground->addCtrl(channelStrip);
        where.offset(24, 0);
        CAUGuiDisplay * channelVolumeDisplay = new CAUGuiDisplay(theGui, *CHANNEL_PARAMETER(kParam_Volume, i), &where_value, kCAUGui_res_1, textProc_Volume, NULL, graMixerValueBackground);
        channelVolumeDisplay->setFont("Copperplate");
        channelVolumeDisplay->setFontSize(9.0);
        channelVolumeDisplay->text_offset_y = 3;
        channelVolumeDisplay->text_offset_x = -1;
        paneBackground->addCtrl(channelVolumeDisplay);
        where_value.offset(24, 0);
        CAUGuiButton * channelMuteButton = new CAUGuiButton(theGui, *CHANNEL_PARAMETER(kParam_Active, i), &where_mute, graChannelMute, NULL, kOnOffButton);
        paneBackground->addCtrl(channelMuteButton);
        where_mute.offset(24, 0);
    }
    
    // Output Device
    where.set(14, 83, 229, 135);

    listDevices = new CAUGuiList(theGui, paramOutputDevice, &where, graDeviceListBackground);
    paneBackground->addCtrl(listDevices);
    
    // Channel select buttons    
    where.set(264, 242, kNumChannels*24, 23);
    channelButtonArray = new CAUGuiRadioArray(theGui, 9, &where, graChannelSelect, NULL);
    channelButtonArray->setUserProc(radioArrayProc_ChannelSelect, this);
    paneBackground->addCtrl(channelButtonArray);

    // kNumChannels Channel panes:
    where.set(0, 268, 655, 300);
    
    channelPane = new CAUGuiLayeredPane(theGui, &where, graChannelPaneBackground);
	paneBackground->addCtrl(channelPane);
	
    bankLists = new CAUGuiList*[kNumChannels];
    patchLists = new CAUGuiList*[kNumChannels];
    bankMSBs = new CAUGuiDisplay*[kNumChannels];
    bankLSBs = new CAUGuiDisplay*[kNumChannels];
    pageButtons = new CAUGuiRadioArray*[kNumChannels];
    
    for (int channel=0; channel<kNumChannels; channel++) {
        
        channelPane->addBackground(graChannelPaneBackground, channel);
                
        // MSB/LSB
        where.set(48, 38, 105, 15);
        bankMSBs[channel] = new CAUGuiDisplay(theGui, *CHANNEL_PARAMETER(kParam_MidiControlPatch_BankMSB, channel), &where, kCAUGui_res_1, NULL, NULL, graMsbLsbBackground);
        //bankMsb->generatePopUpMenue();
        bankMSBs[channel]->text_offset_y = 4;
        channelPane->addCtrl(bankMSBs[channel], channel);
        where.offset(0, 24);
        bankLSBs[channel] = new CAUGuiDisplay(theGui, *CHANNEL_PARAMETER(kParam_MidiControlPatch_BankLSB, channel), &where, kCAUGui_res_1, NULL, NULL, graMsbLsbBackground);
        //bankLsb->generatePopUpMenue();
        bankLSBs[channel]->text_offset_y = 4;
        channelPane->addCtrl(bankLSBs[channel], channel);
        
        // Patch list
        where.set(180, 34, 461, 250);
        patchLists[channel] = new CAUGuiList(theGui, *CHANNEL_PARAMETER(kParam_MidiControlPatch_Patch, channel), &where, graPatchListBackground, 4);
        patchLists[channel]->setFontSize(11.0);
        patchLists[channel]->setItemPadding(2);
        channelPane->addCtrl(patchLists[channel], channel);

        // Page buttons
        where.set(547, 6, 96, 19);
        pageButtons[channel] = new CAUGuiRadioArray(theGui, 2, &where, graPage, NULL);
        pageButtons[channel]->setUserProc(radioArrayProc_PageSelect, patchLists[channel]);
        pageButtons[channel]->setItemNames(CFArrayCreate(NULL, (const void**)kPageLabels, 2, NULL));
        channelPane->addCtrl(pageButtons[channel], channel);
                
        // Bank list
        where.set(15, 90, 140, 195);
        bankLists[channel] = new CAUGuiList(theGui, 2, &where, graBankListBackground);
        bankLists[channel]->setItemNames(devDb->getBankNames());
        bankLists[channel]->setUserProc(listProc_BankSelect, this);
        channelPane->addCtrl(bankLists[channel], channel);
    }
    
    channelPane->setLayer(0);
    
    XPos = (int) xoffset;
	YPos = (int) yoffset;
	
	// set size of overall pane
	SizeControl(mCarbonPane, width + XPos, height + YPos);
	
	return noErr;
}
                                                                 
void EverySynthCarbonUI::activateChannelPane(int number)
{
    channelPane->setLayer(number);
}

void EverySynthCarbonUI::selectDevicePopup()
{
    char curDevice[kPropertySize_MidiDeviceType];
    
    AudioUnitGetProperty(mEditAudioUnit, kProperty_MidiDeviceType, kAudioUnitScope_Global, 0, curDevice, NULL);
    
    Point pt;
    GetGlobalMouse(&pt);

    int result = PopUpMenuSelect(deviceTypePopup, pt.v, pt.h, CFArrayGetFirstIndexOfValue(deviceList, CFRangeMake(0, CFArrayGetCount(deviceList)), CFStringCreateWithCStringNoCopy(NULL, curDevice, 0, NULL)));
    result &= 0xFFFF;
    
    if (result == 0)
        return;
    
    char * newDevice = (char*)CFStringGetCStringPtr((CFStringRef)CFArrayGetValueAtIndex(deviceList, result-1), 0);
    AudioUnitSetProperty(mEditAudioUnit, kProperty_MidiDeviceType, kAudioUnitScope_Global, 0, newDevice, strlen(newDevice));
    
    buttonDeviceType->setText(newDevice);
    devDb->loadDevice((CFStringRef)CFArrayGetValueAtIndex(deviceList, result-1));
    
    CFArrayRef bankNames = devDb->getBankNames();
    
    for (int i=0; i<kNumChannels; i++) {
        bankLists[i]->setItemNames(bankNames);
        bankLists[i]->setRange(CFArrayGetCount(bankNames));
        Draw1Control(bankLists[i]->getCarbonControl());
        bankSelect(0, i);
    }
}

void EverySynthCarbonUI::bankSelect(int value, int channel)
{
    int curChannel;
    if (channel == -1)
        curChannel = channelPane->getLayer();
    else
        curChannel = channel;

    patchLists[curChannel]->setItemNames(devDb->getPatchNames(value));
    
    BankSelectInfo bs = devDb->getBankSelectInfo(value);
    
    SetControl32BitValue(bankMSBs[curChannel]->getCarbonControl(), bs.msb);
    SetControl32BitValue(bankLSBs[curChannel]->getCarbonControl(), bs.lsb);
    
    Draw1Control(bankMSBs[curChannel]->getCarbonControl());
    Draw1Control(bankLSBs[curChannel]->getCarbonControl());
}
