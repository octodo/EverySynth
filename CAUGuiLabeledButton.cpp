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

void CAUGuiLabeledButton::setText(char * theText)
{
    strncpy(text, theText, 31);
}
