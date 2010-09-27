/*
 *  CAUGuiRadioArray.h
 *  EverySynth
 *
 *  Created by Philip Kranz on 27.09.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
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
    
private:
    CAUGuiGraphic * foreground;
    CAUGuiGraphic * background;
    
    radioArrayUserProcedure userProc;
    void * userData;
};

#endif

