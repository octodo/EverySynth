/*
 *  CAUGuiDisplay.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 27 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiMeter.h"

CAUGuiMeter::CAUGuiMeter (	CAUGuiMan*				theChief,
							UInt32 					internalIndex, 
							eRect*					theWhere,
							CAUGuiResolution		theRes, 
							CAUGuiGraphic*			theForeGround, 
							CAUGuiGraphic*			theBackground)
						
						:	CAUGuiCtrl ( theChief, theWhere, theRes )
							
{
		
	ForeGround = theForeGround;
	BackGround = theBackground;
	internalParamIndex = internalIndex;
	setType ( kCAUGui_Meter );

}



CAUGuiMeter::~CAUGuiMeter ()
{
	ForeGround = NULL;
	BackGround = NULL;

}

void CAUGuiMeter::idle()
{
	float fValue = Chief->getInternalParameter ( internalParamIndex );
	
	//printf ( "idling %d - %d - %f\n", (int)getID(), (int)internalParamIndex, fValue );
	
	if ( fValue > 1.f ) fValue = 1.f;
	if ( fValue < 0.f ) fValue = 0.f;
	
	ControlRef carbonControl = getCarbonControl();
	
	if ( carbonControl != NULL )
	{
		UInt32 max = GetControl32BitMaximum(carbonControl);
		UInt32 val = (UInt32)((float)max * fValue );
		SetControl32BitValue ( carbonControl, val );
	}
	
	//printf ( "idling %d\n", (int)getID() );
	
	CAUGuiCtrl::idle();
}

void CAUGuiMeter::draw(CGContextRef context, UInt32 portHeight )
{
	ControlRef carbonControl = getCarbonControl();
	
	UInt32 max = GetControl32BitMaximum(carbonControl);
	UInt32 val = GetControl32BitValue( carbonControl );

	CGImageRef theBack = NULL;
	
	CGRect bounds;
	
	getBounds()->to( &bounds, portHeight );
	
	if ( BackGround != NULL )
		theBack = BackGround->getImage();
		
	if ( theBack != NULL )
		CGContextDrawImage( context, bounds, theBack );
	
	
	if ( ForeGround != NULL )
	{
		float valNorm = (float) val / (float) max;
		
		ForeGround->draw ( context, portHeight, getForeBounds(), valNorm );
		
	}
}

