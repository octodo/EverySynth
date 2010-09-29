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

class CAUGuiList;

typedef void (*listUserProcedure)( UInt32 value, CAUGuiList* from, void* userData );

class CAUGuiList : public CAUGuiCtrl
{
public:
	
    CAUGuiList(CAUGuiMan * theChief,
               CAAUParameter &theAuvp,
               eRect * theWhere,
               CAUGuiGraphic * theBackground,
               int col = 1);
    CAUGuiList(CAUGuiMan * theChief,
               UInt32 theRange,
               eRect * theWhere,
               CAUGuiGraphic * theBackground,
               int col = 1);
	
	virtual ~CAUGuiList() { }
    
	virtual void draw(CGContextRef context, UInt32 portHeight);
	virtual void mouseDown(Point *P, bool, bool);
    //virtual void mouseUp(Point *P, bool, bool);
    
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
    
    void setItemPadding(int padding) { itemPadding = padding; }
    
    void setPageButtons(CAUGuiGraphic * thePageButtons) { pageButtons = thePageButtons; }
    
    void setUserProc(listUserProcedure theUserProc, void * theUserData) { userProc = theUserProc; userData = theUserData; }
    
    void selectPage(int page);
    
private:
    
	CAUGuiGraphic *	background;
    CFArrayRef itemNames;
    int numColumns;
    int itemPadding;
    CAUGuiGraphic * pageButtons;
    
    int currentPage;
    
    listUserProcedure userProc;
    void * userData;
};


#endif