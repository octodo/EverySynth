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

#ifndef __CAUGUI_RADIO_ARRAY__
#define __CAUGUI_RADIO_ARRAY__

#include "CAUGuiConfig.h"

#include "CAUGui.h"

class CAUGuiRadioArray;

typedef void (*radioArrayUserProcedure)( UInt32 value, CAUGuiRadioArray* from, void* userData );

class CAUGuiRadioArray : public CAUGuiCtrl
{
public:
    CAUGuiRadioArray(CAUGuiMan * theChief,
                     UInt32 theRange,
                     eRect * theWhere,
                     CAUGuiGraphic * theForeground,
                     CAUGuiGraphic * theBackground);

    virtual void draw(CGContextRef context, UInt32 portHeight);
    
    virtual void mouseDown(Point * P, bool, bool);
    
    void setUserProc(radioArrayUserProcedure theUserProc, void * theUserData) { userProc = theUserProc; userData = theUserData; }

    void setItemNames(CFArrayRef theItemNames) {
        if (itemNames != NULL) {
            CFRelease(itemNames);
        }
        if (theItemNames == NULL) {
            itemNames = NULL;
        }
        else {
            itemNames = CFArrayCreateCopy(NULL, theItemNames);
        }
        //SetControl32BitValue(carbonControl, 0);
        Draw1Control(carbonControl);
    }
    
private:
    CAUGuiGraphic * foreground;
    CAUGuiGraphic * background;
    
    CFArrayRef itemNames;
    
    radioArrayUserProcedure userProc;
    void * userData;
};

#endif

