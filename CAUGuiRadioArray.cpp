/*
 *  CAUGuiRadioArray.cpp
 *  EverySynth
 *
 *  Created by Philip Kranz on 27.09.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiRadioArray.h"

CAUGuiRadioArray::CAUGuiRadioArray(CAUGuiMan * theChief,
                                   UInt32 theRange,
                                   eRect * theWhere,
                                   CAUGuiGraphic * theForeground,
                                   CAUGuiGraphic * theBackground)
:   CAUGuiCtrl(theChief, theWhere, theRange)
{
    foreground = theForeground;
    background = theBackground;
    userProc = NULL;
}

void CAUGuiRadioArray::mouseDown(Point * P, bool, bool)
{
    int item_width = getBounds()->w / getRange();
    int value = P->h / item_width;
    SetControl32BitValue(getCarbonControl(), value);
    
    Draw1Control(getCarbonControl());
    
    if (userProc != NULL) {
        userProc(value, this, userData);
    }
}

void CAUGuiRadioArray::draw(CGContextRef context, UInt32 portHeight)
{
    ControlRef carbonControl = getCarbonControl();
	
    CGRect bounds;
	getBounds()->to( &bounds, portHeight );
	
	UInt32 value = 0;
	
	if ( carbonControl != NULL )
		value = GetControl32BitValue( carbonControl );
    
    if ( background != NULL )
		CGContextDrawImage(context, bounds, background->getImage());
    
    char text[32];
    
    eRect where;
    where.set(getBounds()->x, getBounds()->y, bounds.size.width / getRange(), bounds.size.height);
    
    for (UInt32 i=0; i<getRange(); i++) {
        if (foreground != NULL) {
            foreground->draw(context, portHeight, &where, (value == i ? 1.f : 0.f));
        }
        
        CGRect itemBounds;
        where.to(&itemBounds, portHeight);
        
        sprintf(text, "%d", i+1);
        
        CGContextSelectFont(context, font_name, font_size, kCGEncodingMacRoman);
        
        CGContextSetTextDrawingMode(context, kCGTextInvisible);
        CGContextShowTextAtPoint(context, 0, 0, text, strlen(text));
        CGPoint pt = CGContextGetTextPosition(context);
        
        itemBounds.origin.x += itemBounds.size.width / 2 - pt.x / 2;
        itemBounds.origin.y += itemBounds.size.height / 2 - font_size / 2;
        
        CGContextSetRGBFillColor( context, col_red, col_green, col_blue, col_alpha );
        CGContextSetRGBStrokeColor( context, col_red, col_green, col_blue, col_alpha );
        CGContextSetTextDrawingMode ( context, kCGTextFill );
        CGContextShowTextAtPoint(context, itemBounds.origin.x, itemBounds.origin.y, text, strlen(text));
        
        where.offset(bounds.size.width / getRange(), 0);
    }
}
