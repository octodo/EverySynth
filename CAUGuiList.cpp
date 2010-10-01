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
    userProc = NULL;
    userData = NULL;
    currentPage = 0;
    pageButtons = NULL;
}

CAUGuiList::CAUGuiList(CAUGuiMan * theChief,
                       UInt32 theRange,
                       eRect * theWhere,
                       CAUGuiGraphic * theBackground,
                       int col)
:	CAUGuiCtrl (theChief, theWhere, theRange)
{
    background = theBackground;
    itemNames = NULL;
    numColumns = col;
    itemPadding = 5;
    userProc = NULL;
    userData = NULL;
    currentPage = 0;
    pageButtons = NULL;
}

void CAUGuiList::selectPage(int page) {
	ControlRef carbonControl = getCarbonControl();
    
	UInt32 value = 0;
	
	if ( carbonControl != NULL )
		value = GetControl32BitValue( carbonControl );

    int item_height = (int)font_size + 2 * itemPadding;
    int num_items_per_column = where.h / item_height;
    int num_items_per_page = num_items_per_column * numColumns;

    if (page == -1) {
        currentPage = value / num_items_per_page; 
    }
    else {
        if (page > (getRange() / num_items_per_page)) {
            return;
        }
        else {
            currentPage = page;
        }
    }
    
    Draw1Control(carbonControl);
}

void CAUGuiList::mouseDown(Point *P, bool, bool)
{
	ControlRef carbonControl = getCarbonControl();

	UInt32 value = 0;
	
	if ( carbonControl != NULL )
		value = GetControl32BitValue( carbonControl );
    
    if (pageButtons != NULL) {
        CGPoint pt = CGPointMake(P->h, P->v);
        eRect * bounds = getBounds();
        CGRect upperRect = CGRectMake(bounds->w - itemPadding * 2 - pageButtons->getWidth(), 0, itemPadding * 2 + pageButtons->getWidth(), itemPadding * 2 + pageButtons->getHeight());
        CGRect lowerRect = CGRectMake(upperRect.origin.x, bounds->h - (pageButtons->getHeight() + itemPadding * 2), upperRect.size.width, upperRect.size.height);
        if (CGRectContainsPoint(upperRect, pt)) {
            selectPage(currentPage - 1);
            return;
        }
        else if (CGRectContainsPoint(lowerRect, pt)) {
            selectPage(currentPage + 1);
            return;
        }
    }
    
    int item_height = (int)font_size + 2 * itemPadding;
    int item_width = where.w / numColumns;
    int num_items_per_column = where.h / item_height;
    int num_items_per_page = num_items_per_column * numColumns;

    int item = num_items_per_page * currentPage + P->v / item_height + (P->h / item_width) * num_items_per_column;
    
    SetControl32BitValue(carbonControl, item);

	Draw1Control(carbonControl);

    if (userProc != NULL) {
        userProc(item, this, userData);
    }
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

    CAAUParameter myParam;
    
    if (isAUVPattached()) {
        myParam = getAUVP();
    
        // Only indexed parameters!
        if (!myParam.IsIndexedParam())
            return;
    }

    CGContextSelectFont(context, font_name, font_size, kCGEncodingMacRoman);
    
    getForeBounds()->to(&bounds, portHeight);

    // Determine dimensions
    int item_height = (int)font_size + 2 * itemPadding;
    int item_width = bounds.size.width / numColumns;
    int approx_font_height = (int)font_size;
    
    int num_items_per_column = bounds.size.height / item_height;
    int num_items_per_page = num_items_per_column * numColumns;
    
    CGContextSetRGBFillColor( context, col_red, col_green, col_blue, col_alpha );
    CGContextSetRGBStrokeColor( context, col_red, col_green, col_blue, col_alpha );

    CGContextSetTextDrawingMode ( context, kCGTextFill );
    
    for (int i=0; (i < num_items_per_page) && (currentPage * num_items_per_page + i <= getRange()); i++) {
        char text[32];
        int nItem = currentPage * num_items_per_page + i;
        CFIndex usedBufLen;
        
        CGRect itemRect;
        itemRect.origin.x = bounds.origin.x + (i / num_items_per_column) * item_width;
        itemRect.origin.y = bounds.origin.y + bounds.size.height - (i % num_items_per_column + 1) * item_height;
        itemRect.size.width = item_width;
        itemRect.size.height = item_height;
        
        if (itemNames != NULL && CFArrayGetCount(itemNames) > nItem) {
            CFStringGetBytes((CFStringRef)CFArrayGetValueAtIndex(itemNames, nItem), CFRangeMake(0, 31), 0, (UInt8)'?', false, (UInt8*)text, 32, &usedBufLen);
            text[usedBufLen] = '\0';
        }
        else if (isAUVPattached() && myParam.HasNamedParams()) {
            CFStringGetBytes(myParam.GetParamName(nItem), CFRangeMake(0, 31), 0, (UInt8)'?', false, (UInt8*)text, 32, &usedBufLen);
            text[usedBufLen] = '\0';
        }
        else {
            sprintf(text, "%d", nItem);
        }
                
        if ((int)value == nItem) {
            CGContextSetRGBFillColor(context, 0,0,0,1);
            CGContextFillRect(context, itemRect);
            CGContextSetRGBFillColor(context, col_red, col_green, col_blue, col_alpha);
        }
        
		CGContextShowTextAtPoint(context, itemRect.origin.x + 10, itemRect.origin.y + (itemRect.size.height / 2 - approx_font_height / 2), text, strlen(text));
    }
    
    if (pageButtons != NULL && num_items_per_page < getRange()) {
        //CGRect rect1 = CGRectMake(bounds.origin.x + bounds.size.width - pageButtons->getWidth() - itemPadding, bounds.origin.y + bounds.size.height - pageButtons->getHeight() - itemPadding, pageButtons->getWidth(), pageButtons->getHeight());
        //CGRect rect2 = CGRectMake(bounds.origin.x + bounds.size.width - pageButtons->getWidth() - itemPadding, bounds.origin.y + itemPadding, pageButtons->getWidth(), pageButtons->getHeight());
        eRect where;
        eRect * myBounds = getBounds();
        where.set(myBounds->x + myBounds->w - pageButtons->getWidth() - itemPadding, myBounds->y + itemPadding, pageButtons->getWidth(), pageButtons->getHeight());
        pageButtons->draw(context, portHeight, &where, 0.f);
        where.offset(0, myBounds->h - 2 * itemPadding - pageButtons->getHeight());
        pageButtons->draw(context, portHeight, &where, 1.f);
        //CGContextDrawImage(context, rect1, pageButtons->getImage());
        //CGContextDrawImage(context, rect1, pageButtons->getImage());
    }
}