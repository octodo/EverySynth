/*
 *  CAUGuiLabeledButton.cpp
 *  EverySynth
 *
 *  Created by Philip Kranz on 26.09.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiLabeledButton.h"

CAUGuiLabeledButton::CAUGuiLabeledButton (	CAUGuiMan*			theChief,
                            CAAUParameter		&theAuvp, 
                            eRect*				theWhere,
                            CAUGuiGraphic*		theForeGround, 
                            CAUGuiGraphic*		theBackground,
                            CAUGuiBottonMode	theMode)

:	CAUGuiButton(theChief, theAuvp, theWhere, theForeGround, theBackground, theMode)
{
}

CAUGuiLabeledButton::CAUGuiLabeledButton (	CAUGuiMan*			theChief,
                            UInt32				theRange, 
                            eRect*				theWhere,
                            CAUGuiGraphic*		theForeGround, 
                            CAUGuiGraphic*		theBackground,
                            CAUGuiBottonMode	theMode)

:	CAUGuiButton(theChief, theRange, theWhere, theForeGround, theBackground, theMode)
{
}

void CAUGuiLabeledButton::draw(CGContextRef context, UInt32 portHeight)
{
    CAUGuiButton::draw(context, portHeight);

    CGRect bounds;
	
	getBounds()->to( &bounds, portHeight );

    CGContextSelectFont(context, font_name, font_size, kCGEncodingMacRoman);    

    CGContextSetTextDrawingMode(context, kCGTextInvisible);
    CGContextShowTextAtPoint(context, 0, 0, text, strlen(text));
    CGPoint pt = CGContextGetTextPosition(context);

    bounds.origin.x += bounds.size.width / 2 - pt.x / 2;
    bounds.origin.y += bounds.size.height / 2 - font_size / 2;
    
    CGContextSetRGBFillColor( context, col_red, col_green, col_blue, col_alpha );
    CGContextSetRGBStrokeColor( context, col_red, col_green, col_blue, col_alpha );
    CGContextSetTextDrawingMode ( context, kCGTextFill );
    CGContextShowTextAtPoint(context, bounds.origin.x, bounds.origin.y, text, strlen(text));
}

void CAUGuiLabeledButton::setText(CFStringRef theText)
{
    CFStringGetBytes(theText, CFRangeMake(0, 30), 0, 0x3F, false, (UInt8*)text, 31, NULL);
}
