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

#include "AUCarbonViewBase.h"
#include "AUControlGroup.h"

#include "CAUGui.h"
#include "CAUGuiKnob.h"
#include "CAUGuiSlider.h"
#include "CAUGuiPane.h"
#include "CAUGuiDisplay.h"
#include "CAUGuiButton.h"
#include "CAUGuiPoof.h"
#include "CAUGuiMoreImages.h"
#include "CAUGuiMeter.h"
#include "CAUGuiSelector.h"

#include "CAUGuiList.h"
#include "CAUGuiLabeledButton.h"
#include "CAUGuiRadioArray.h"

#include "DeviceDB.h"

#include "Parameters.h"


class EverySynthCarbonUI : public AUCarbonViewBase {
public:
	EverySynthCarbonUI(AudioUnitCarbonView auv) : AUCarbonViewBase(auv)
    {
        theGui = NULL;
    }
    
    ~EverySynthCarbonUI()
    {
	    
        if (theGui != NULL)
        {
            delete(theGui);
        }
    }
	
	virtual OSStatus CreateUI(Float32 xoffset, Float32 yoffset);
    
    void activateChannelPane(int number);
    void selectDevicePopup();
    void bankSelect(int value, int channel = -1);
    
private:
    CAAUParameter ** channelParams;
    CAUGuiMan * theGui;
    
    // Controls
    CAUGuiPane * paneBackground;
    
    CAUGuiList * listDevices;
    
    CAUGuiLayeredPane * channelPane;
    CAUGuiLabeledButton * buttonDeviceType;
    CAUGuiRadioArray * channelButtonArray;
    
    CAUGuiList ** bankLists;
    CAUGuiList ** patchLists;
    CAUGuiDisplay ** bankMSBs;
    CAUGuiDisplay ** bankLSBs;
    CAUGuiRadioArray ** pageButtons;
    
    DeviceDB * devDb;
    MenuRef deviceTypePopup;
    CFArrayRef deviceList;
};
