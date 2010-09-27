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

void textProc_Volume(UInt32 value, char * text, void * userData)
{
    sprintf(text, "%d", value);
}

void radioArrayProc_ChannelSelect(UInt32 value, CAUGuiRadioArray* from, void* userData)
{
    ((EverySynthCarbonUI*)userData)->activateChannelPane(value);
}

OSStatus	EverySynthCarbonUI::CreateUI(Float32 xoffset, Float32 yoffset)
{
	int XPos = (int) xoffset;
	int YPos = (int) yoffset;
    
    CAAUParameter paramOutputDevice(mEditAudioUnit, kParam_MidiOutputDevice, kAudioUnitScope_Global, 0);
    CAAUParameter ** channelParams = (CAAUParameter**) malloc(sizeof(CAAUParameter*) * kNumberOfChannelParameters * kNumChannels);
    for (int i=0; i<kNumChannels; i++) {
        for (int j=0; j<kNumberOfChannelParameters; j++) {
            CHANNEL_PARAMETER(j, i) = new CAAUParameter(mEditAudioUnit, j, kAudioUnitScope_Part, i);
        }
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
	CAUGuiPane* paneBackground = new CAUGuiPane(theGui, &where, graBackground);
	theGui->addCtrl(paneBackground);
	
    // DeviceType button
    where.set(13, 40, 231, 31);
    CAUGuiLabeledButton * buttonDeviceType = new CAUGuiLabeledButton(theGui, 1, &where, graDeviceType, NULL, kPushButton);
    buttonDeviceType->setText(CFSTR("Roland Fantom X"));
    buttonDeviceType->setOnValue(1);
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
    /*
    CAUGuiDisplay * deviceList = new CAUGuiDisplay(theGui, paramOutputDevice, &where, kCAUGui_res_1, NULL, NULL, graDeviceTypeInactive);
    deviceList->generatePopUpMenue();
    deviceList->text_offset_y = 7;
     */
    CAUGuiList * deviceList = new CAUGuiList(theGui, paramOutputDevice, &where, graDeviceListBackground);
    paneBackground->addCtrl(deviceList);
    
    // Channel select buttons

    /*
    where.set(264, 242, 24, 23);

    channelButtons = new CAUGuiLabeledButton*[kNumChannels];
    for (int i=0; i<kNumChannels; i++) {
        channelButtons[i] = new CAUGuiLabeledButton(theGui, 2, &where, graChannelSelect, NULL, kRadioButton);
        channelButtons[i]->setText(CFStringCreateWithFormat(NULL, NULL, CFSTR("%d"), i+1));
        channelButtons[i]->setUserProcedure(buttonProc_ChannelSelect, (void*)i);
        channelButtons[i]->setOnValue(1);
        paneBackground->addCtrl(channelButtons[i]);
        where.offset(24, 0);
    }
     */
    
    where.set(264, 242, kNumChannels*24, 23);
    channelButtonArray = new CAUGuiRadioArray(theGui, 9, &where, graChannelSelect, graDeviceType);
    channelButtonArray->setUserProc(radioArrayProc_ChannelSelect, this);
    paneBackground->addCtrl(channelButtonArray);

    // kNumChannels Channel panes:
    where.set(0, 268, 655, 300);
    
    channelPane = new CAUGuiLayeredPane(theGui, &where, graChannelPaneBackground);
	paneBackground->addCtrl(channelPane);
	
    for (int channel=0; channel<kNumChannels; channel++) {
        
        channelPane->addBackground(graChannelPaneBackground, channel);
                
        // MSB/LSB
        where.set(48, 38, 105, 15);
        CAUGuiDisplay * bankMsb = new CAUGuiDisplay(theGui, *CHANNEL_PARAMETER(kParam_MidiControlPatch_BankMSB, channel), &where, kCAUGui_res_1, NULL, NULL, graMsbLsbBackground);
        //bankMsb->generatePopUpMenue();
        bankMsb->text_offset_y = 4;
        channelPane->addCtrl(bankMsb, channel);
        where.offset(0, 24);
        CAUGuiDisplay * bankLsb = new CAUGuiDisplay(theGui, *CHANNEL_PARAMETER(kParam_MidiControlPatch_BankLSB, channel), &where, kCAUGui_res_1, NULL, NULL, graMsbLsbBackground);
        //bankLsb->generatePopUpMenue();
        bankLsb->text_offset_y = 4;
        channelPane->addCtrl(bankLsb, channel);
    }
    
    channelPane->setLayer(0);
    
	/*
	// Create the two knobs
	
	where.set ( 8, 90, 80, 80 );
	
	CAUGuiKnob* myKnob = new CAUGuiKnob(myCAUGui, aKnob1, &where, kCAUGui_res_100, round_handle, knob_back );
	myKnob->shrinkForeBounds ( 28, 1, 28, 55 );
	myPane->addCtrl( myKnob );
	
	eRect where2;	
	where2.set ( &where );
	where2.offset ( 0, 8 );
	where2.align ( k_oSC ); // place the eRect below itself (o = outside, S = south, C = center)
	
	myKnob = new CAUGuiKnob(myCAUGui, aKnob2, &where2, kCAUGui_res_100, needle_handle, knob_back );
	myPane->addCtrl( myKnob );
	
	
	
	
	// Build layered Pane with 2 different Control sets
	
	where.align ( k_NEo );
	where.offset ( 8, -12 );
	where.size ( pane_back1->getWidth(),  pane_back1->getHeight());
	
	CAUGuiLayeredPane* myLayeredPane = new CAUGuiLayeredPane(myCAUGui, &where, pane_back1);
	myPane->addCtrl( myLayeredPane );
	
	myLayeredPane->addBackground( pane_back2, 1 );
	
	
	
	// Add a switching Button to the Layered Pane (in all layers)
	
	where.set ( 100, 2, 55, 33 );
	
	CAUGuiButton* myLayerSwitch = new CAUGuiButton( myCAUGui, 1, &where, next_pane, next_pane, kIncButton );
	
	// user procedure to control visible Layer 
	myLayerSwitch->setUserProcedure( switchLayer, myLayeredPane ); 
    
	myLayeredPane->addCtrl( myLayerSwitch );
	
	
	
	
	// Make a horizontal sliders and add it to the layered pane ( layer 0 )
	
	where.set ( 16, 40, 143, 36 ); // always relative to embedding pane
	
	CAUGuiSlider* mySlider = new CAUGuiSlider(myCAUGui, aSlider1, &where, 1, kCAUGui_res_100, round_s_handle, h_slider_back );
	mySlider->shrinkForeBounds ( 20, 5, 23, 7 );
	myLayeredPane->addCtrl( mySlider, 0 );
	
	where.align ( k_oSC );
	
	CAUGuiSlider* mySlider2 = new CAUGuiSlider(myCAUGui, aSlider2, &where, 1, kCAUGui_res_100, round_s_handle, h_slider_back );
	mySlider2->shrinkForeBounds ( 20, 5, 23, 7 );
	myLayeredPane->addCtrl( mySlider2, 0 );
	
	// The generic Display (see NULL for textroutine/foregroundimage)
	
	where.align ( k_oSC );
	where.size ( 96, 39 ); 
	CAUGuiDisplay* myDisplay1 = new CAUGuiDisplay(myCAUGui, aDisplay1, &where, kCAUGui_res_1, NULL, NULL, display_back );
	myDisplay1->shrinkForeBounds ( 17, 5, 23, 19 );
	myLayeredPane->addCtrl( myDisplay1, 0 );
    
	
	// make a clone with different textalignment but same parameter :-)
    
	where.align ( k_oSC );
	myDisplay1 = new CAUGuiDisplay(myCAUGui, aDisplay1, &where, kCAUGui_res_1, NULL, NULL, display_back );
	myDisplay1->shrinkForeBounds ( 17, 5, 23, 19 );
	myDisplay1->setTextAlign ( 2 ); // align right
	myLayeredPane->addCtrl( myDisplay1, 0 );
    
	myDisplay1->generatePopUpMenue ();
	
	
	
	// Make vertical sliders ( layer 1 )
	
	where.set ( 16, 40, 37, 142 ); // always relative to embedding pane
	
	mySlider = new CAUGuiSlider(myCAUGui, aSlider1, &where, 0, kCAUGui_res_100, horizontal_handle, v_slider_back );
	mySlider->shrinkForeBounds ( 3, 18, 10, 24 );
	myLayeredPane->addCtrl( mySlider, 1 );
	
	where.align ( k_MEo );
	
	mySlider2 = new CAUGuiSlider(myCAUGui, aSlider2, &where, 0, kCAUGui_res_100, horizontal_handle, v_slider_back );
	mySlider2->shrinkForeBounds ( 3, 18, 10, 24 );
	myLayeredPane->addCtrl( mySlider2, 1 );
	
	where.align ( k_MEo );
	
	mySlider2 = new CAUGuiSlider(myCAUGui, aSlider3, &where, 0, kCAUGui_res_100, vu_image, v_slider_back );
	mySlider2->shrinkForeBounds ( 13, 19, 17, 23 );
	myLayeredPane->addCtrl( mySlider2, 1 );
    
    
	// VU METER
	where.at (290, 116 );
    
	CAUGuiMeter* myMeter = new CAUGuiMeter(myCAUGui, kRMS_Right, &where, kCAUGui_res_100, vu_image, v_led_back );
	myMeter->shrinkForeBounds ( 13, 19, 17, 23 );
	myPane->addCtrl( myMeter );
	
	where.align ( k_MEo );
	myMeter = new CAUGuiMeter(myCAUGui, kRMS_Left, &where, kCAUGui_res_100, vu_image, v_led_back );
	myMeter->shrinkForeBounds ( 13, 19, 17, 23 );
	myPane->addCtrl( myMeter );
     */
    
    
	
	/******************************************************************************
     
     EZ preset generation.
     
     A small white button appears.
     Press it and your preset setup code jumps outta your console window...
     
     Better don't put this into your release version...
     
     ******************************************************************************/
    
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

