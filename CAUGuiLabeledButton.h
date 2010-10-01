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

#ifndef __CAUGUI_LABELED_BUTTON__
#define __CAUGUI_LABELED_BUTTON__

#include "CAUGuiConfig.h"

#include "CAUGui.h"
#include "CAUGuiButton.h"

class CAUGuiLabeledButton : public CAUGuiButton
{
public:
	CAUGuiLabeledButton ( CAUGuiMan*, CAAUParameter &, eRect* where, CAUGuiGraphic* fore, CAUGuiGraphic* back, CAUGuiBottonMode );
	
	CAUGuiLabeledButton ( CAUGuiMan*, UInt32 theRange, eRect*, CAUGuiGraphic*, CAUGuiGraphic*, CAUGuiBottonMode );

	virtual void draw(CGContextRef context, UInt32 portHeight);
    
    void setText(char * text);
    
private:
    
    char text[32];
};

#endif
