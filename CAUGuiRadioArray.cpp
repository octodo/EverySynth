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
    itemNames = NULL;
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
        
        if (itemNames != NULL && CFArrayGetCount(itemNames) > i) {
            CFStringGetBytes((CFStringRef)CFArrayGetValueAtIndex(itemNames, i), CFRangeMake(0, 30), 0, 0x3F, false, (UInt8*)text, 31, NULL);
        }
        else {
            sprintf(text, "%d", i+1);
        }
        
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
