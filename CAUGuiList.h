/*
 *  CAUGuiList.h
 *  EverySynth
 *
 *  Created by Philip Kranz on 26.09.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CAUGUI_LIST__
#define __CAUGUI_LIST__

#include "CAUGuiConfig.h"

#include "CAUGui.h"

class CAUGuiList : public CAUGuiCtrl
{
public:
	
    CAUGuiList(CAUGuiMan * theChief,
               CAAUParameter &theAuvp,
               eRect * theWhere,
               CAUGuiGraphic * theBackground,
               int col = 1);
	
	virtual ~CAUGuiList() { }
    
	virtual void draw(CGContextRef context, UInt32 portHeight);
	virtual void mouseDown(Point *P, bool, bool);
    //virtual void mouseUp(Point *P, bool, bool);
    
    //void setItemNames(CFStringRef * theItemNames);
    
    void setItemPadding(int padding) { itemPadding = padding; }
    
private:
    
	CAUGuiGraphic *	background;
    CFStringRef * itemNames;
    int numColumns;
    int itemPadding;
};


#endif