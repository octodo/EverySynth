/*
 *  CAUGuiList.cpp
 *  EverySynth
 *
 *  Created by Philip Kranz on 26.09.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiList.h"

CAUGuiList::CAUGuiList(CAUGuiMan * theChief,
                       CAAUParameter &theAuvp,
                       eRect * theWhere,
                       CAUGuiGraphic * theBackground,
                       int col)
:	CAUGuiCtrl (theChief, theAuvp, theWhere, kCAUGui_res_1)
{
    background = theBackground;
    itemNames = NULL;
    numColumns = col;
    itemPadding = 5;
}

void CAUGuiList::mouseDown(Point *P, bool, bool)
{
	ControlRef carbonControl = getCarbonControl();

	UInt32 value = 0;
	
	if ( carbonControl != NULL )
		value = GetControl32BitValue( carbonControl );
    
    int item_height = (int)font_size + 2 * itemPadding;
    int item_width = where.w / numColumns;
    int num_items_per_column = where.h / item_height;
    int num_items_per_page = num_items_per_column * numColumns;
    int current_page = value / num_items_per_page;

    int item = num_items_per_page * current_page + P->v / item_height + (P->h / item_width) * num_items_per_column;
    
    SetControl32BitValue(carbonControl, item);

	Draw1Control(carbonControl);
}

void CAUGuiList::draw(CGContextRef context, UInt32 portHeight)
{
	ControlRef carbonControl = getCarbonControl();
	
    CGRect bounds;
	getBounds()->to( &bounds, portHeight );
	
	UInt32 value = 0;
	
	if ( carbonControl != NULL )
		value = GetControl32BitValue( carbonControl );

    if ( background != NULL )
		CGContextDrawImage( context, bounds, background->getImage() );

    CAAUParameter myParam = getAUVP();
    
    // Only indexed parameters!
    if (!myParam.IsIndexedParam())
        return;

    CGContextSelectFont(context, font_name, font_size, kCGEncodingMacRoman);
    
    getForeBounds()->to(&bounds, portHeight);
    // Determine dimensions

    //CGContextSetTextDrawingMode(context , kCGTextInvisible);
    //CGContextShowTextAtPoint(context, 0, 0, "TEST\n", 5);
    //CGPoint pt = CGContextGetTextPosition(context);
    //int item_height = pt.y;
    int item_height = (int)font_size + 2 * itemPadding;
    int item_width = bounds.size.width / numColumns;
    int approx_font_height = (int)font_size;
    
    int num_items_per_column = bounds.size.height / item_height;
    int num_items_per_page = num_items_per_column * numColumns;
    int current_page = value / num_items_per_page;
    
    CGContextSetRGBFillColor( context, col_red, col_green, col_blue, col_alpha );
    CGContextSetRGBStrokeColor( context, col_red, col_green, col_blue, col_alpha );

    CGContextSetTextDrawingMode ( context, kCGTextFill );
    
    for (int i=0; (i < num_items_per_page) && (current_page*num_items_per_page+i < myParam.GetNumIndexedParams()); i++) {
        char text[32];
        int nItem = current_page * num_items_per_page + i;
        
        CGRect itemRect;
        itemRect.origin.x = bounds.origin.x + (i / num_items_per_column) * item_width;
        itemRect.origin.y = bounds.origin.y + bounds.size.height - (i % num_items_per_column + 1) * item_height;
        itemRect.size.width = item_width;
        itemRect.size.height = item_height;
        
        if (itemNames != NULL) {
            CFStringGetBytes(itemNames[nItem], CFRangeMake(0, 30), 0, 0x3F, false, (UInt8*)text, 31, NULL);
        }
        else if (myParam.HasNamedParams()) {
            CFStringGetBytes(myParam.GetParamName(nItem), CFRangeMake(0, 30), 0, 0x3F, false, (UInt8*)text, 31, NULL);
        }
        else {
            sprintf(text, "%d", nItem);
        }
        
        if (value == nItem) {
            CGContextSetRGBFillColor(context, 0,0,0,1);
            CGContextFillRect(context, itemRect);
            CGContextSetRGBFillColor(context, col_red, col_green, col_blue, col_alpha);
        }
        
		CGContextShowTextAtPoint(context, itemRect.origin.x + 10, itemRect.origin.y + (itemRect.size.height / 2 - approx_font_height / 2), text, strlen(text));
    }
}