/*
 *  CAUGuiButton.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 27 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiButton.h"

CAUGuiButton::CAUGuiButton (	CAUGuiMan*			theChief,
								CAAUParameter		&theAuvp, 
								eRect*				theWhere,
								CAUGuiGraphic*		theForeGround, 
								CAUGuiGraphic*		theBackground,
								CAUGuiBottonMode	theMode)
						
							:	CAUGuiCtrl ( theChief, theAuvp, theWhere, kCAUGui_res_1 )
							
{
		
	ForeGround = theForeGround;
	BackGround = theBackground;
	mode = theMode;
	
	userProcedure = NULL;
	userData = NULL;
	
	onValue = 0;
	hilite = false;
	alpha = 0.7f;
	//printf ("ctor CAUGuiButton\n");

}

CAUGuiButton::CAUGuiButton (	CAUGuiMan*			theChief,
								UInt32				theRange, 
								eRect*				theWhere,
								CAUGuiGraphic*		theForeGround, 
								CAUGuiGraphic*		theBackground,
								CAUGuiBottonMode	theMode)
						
							:	CAUGuiCtrl ( theChief, theWhere, theRange )
							
{
		
	ForeGround = theForeGround;
	BackGround = theBackground;
	mode = theMode;
	
	userProcedure = NULL;
	userData = NULL;
	
	onValue = 0;
	hilite = false;
	alpha = 0.7f;
	//printf ("ctor CAUGuiButton\n");

}


CAUGuiButton::~CAUGuiButton ()
{

	ForeGround = NULL;
	BackGround = NULL;


	//printf ("dtor CAUGuiButton\n");


}

void CAUGuiButton::draw(CGContextRef context, UInt32 portHeight )
{
		
	CGImageRef theBack = NULL;
	CGImageRef theButton = NULL;
	CGRect bounds;
	
	getBounds()->to( &bounds, portHeight );
	
	if ( BackGround != NULL )
		theBack = BackGround->getImage();
		
	if ( ForeGround != NULL )
		theButton = ForeGround->getImage();
	
	if ( theBack != NULL )
		CGContextDrawImage( context, bounds, theBack );
	
	if ( theButton != NULL )
	{
		switch ( mode )
		{
		
			case kPushButton:
			case kRadioButton:
			
						if ( GetControl32BitValue( getCarbonControl() ) == (SInt32) onValue )
							ForeGround->draw( context, portHeight, getBounds(), 1.f );
						else
							ForeGround->draw( context, portHeight, getBounds(), 0.f );
						break;
						
			case kOnOffButton:
						if ( GetControl32BitValue( getCarbonControl() ) == 0 )
							ForeGround->draw( context, portHeight, getBounds(), 0.f );
						else
							ForeGround->draw( context, portHeight, getBounds(), 1.f );
						break;
				
		
			default:
	
						if ( hilite )
							CGContextDrawImage( context, bounds, theButton );
						else
						{
							if ( alpha != 0.0f )
							{
								CGContextSetAlpha( context, alpha);
								CGContextDrawImage( context, bounds, theButton );
							}
						}
						break;
		}
	}

}


void CAUGuiButton::mouseDown(Point *P, bool with_option, bool with_shift)
{
	ControlRef carbonControl = getCarbonControl();
	
	UInt32 value = GetControl32BitValue( carbonControl );
	UInt32 max = GetControl32BitMaximum( carbonControl );
	
	hilite = true;
	
	switch ( mode )
	{
		case kPushButton:
			SetControl32BitValue ( carbonControl, max );
			break;
	
		case kIncButton:
			if ( value == max ) value = 0;
			else value++;
			SetControl32BitValue ( carbonControl, value );
			break;
			
		case kDecButton:
			if ( value == 0 ) value = max;
			else value--;
			SetControl32BitValue ( carbonControl, value );
			break;
		
		case kRadioButton:
			SetControl32BitValue ( carbonControl, onValue );
			value = onValue;
			break;
		
		case kOnOffButton:
			if ( value != 0 )
				value = 0;
			else
				value = max;
			
			SetControl32BitValue ( carbonControl, value );
			break;
			
	
	}
	
	if ( userProcedure != NULL && mode != kPushButton )
	{
		userProcedure ( value, this, userData );
	
	}
    
    Draw1Control(getCarbonControl());
	
}

void CAUGuiButton::mouseTrack(Point *P, bool with_option, bool with_shift)
{
	;
}

void CAUGuiButton::mouseUp(Point *P, bool with_option, bool with_shift)
{
	ControlRef carbonControl = getCarbonControl();
	
	hilite = false;
	
	switch ( mode )
	{
		case kPushButton:
			SetControl32BitValue ( carbonControl, 0 );
			break;
	
		case kIncButton:
			Draw1Control ( carbonControl );
			break;
			
		case kDecButton:
			Draw1Control ( carbonControl );
			break;
		
		case kRadioButton:
			SetControl32BitValue ( carbonControl, onValue );
			break;
		
		default:
			Draw1Control ( carbonControl );
			break;
	}

	if ( userProcedure != NULL && mode == kPushButton )
	{
		userProcedure ( 0, this, userData );
        
	}
    
    Draw1Control(getCarbonControl());
}

void CAUGuiButton::setUserProcedure ( buttonUserProcedure theProc, void* theUserData )
{
	userProcedure = theProc;
	userData = theUserData;
}




