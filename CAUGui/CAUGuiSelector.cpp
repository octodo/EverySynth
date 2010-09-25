/*
 *  CAUGuiSelector.cpp
 *  CAUGuiDemo
 *
 *  Created by Urs Heckmann on Fri Dec 13 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiSelector.h"

#include "CAUGuiMoreImages.h"

CAUGuiSelector::CAUGuiSelector (	CAUGuiMan*			theChief,
							
							eRect*				theWhere,
							UInt32				theRange, 
							UInt32				theOrientation,
							CAUGuiGraphic*		theForeGround, 
							CAUGuiGraphic*		theBackground)
						
						:	CAUGuiCtrl ( theChief, theWhere, theRange )
							
{
		
	ForeGround = theForeGround;
	BackGround = theBackground;
	
	orientation = theOrientation;
	
	userProcedure = NULL;
	userData = NULL;
	
	if ( theWhere->w == 0 )
	{
		if ( BackGround != NULL )
		{
			theWhere->w = BackGround->getWidth();
			setBounds(theWhere); 
		}
	
	}
	
	if ( theWhere->h == 0 )
	{
		if ( BackGround != NULL )
		{
			theWhere->h = BackGround->getHeight();
			setBounds(theWhere);
		}
	
	}
		
	setType( kCAUGui_Selector );
	
	setTolerance ( 0 );
	
}

CAUGuiSelector::~CAUGuiSelector ()
{

	ForeGround = NULL;
	BackGround = NULL;


}

void CAUGuiSelector::draw(CGContextRef context, UInt32 portHeight )
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
		float valNorm = (float) val / (float)( max - 1 );
		
		if ( valNorm >= 1.f ) valNorm = 1.f;
			
		ForeGround->draw ( context, portHeight, getForeBounds(), valNorm );
	}
}



void CAUGuiSelector::mouseDown(Point *P, bool with_option, bool with_shift)
{

	ControlRef carbonControl = getCarbonControl();

	SInt32 max = (SInt32) GetControl32BitMaximum(carbonControl);

	SInt32 val = -1;

	eRect fore;
	fore.set( getForeBounds() );
	
	eRect back;
	back.set( getBounds() );
	
	SInt32 o_X = fore.x - back.x;
	SInt32 o_Y = fore.y - back.y;


	float s = 0.f;

	val = (SInt32) GetControl32BitValue( carbonControl );

	if ( orientation != 0 ) // horizontal mode
	{
		s = (float)(P->h - o_X) / (float)fore.w;
		val = (SInt32)( s * (float) max );
	}
	else	// vertical mode
	{
		s = (float)(P->v - o_Y) / (float)fore.h;
		val = (SInt32)( s * (float) max );
	}

	
	if ( val > max ) val = max;
	if ( val < 0 ) val = 0;
	SetControl32BitValue ( carbonControl, (UInt32) val );

	//printf ( "selector says: val %d, s %f, max %f\n", (int)val, s, (float) max );
	
	if ( userProcedure != NULL )
	{
		userProcedure ( val, this, userData );
	
	}


}

void CAUGuiSelector::mouseTrack(Point *P, bool with_option, bool with_shift)
{

		
}

void CAUGuiSelector::mouseUp(Point *P, bool with_option, bool with_shift)
{

}


void CAUGuiSelector::setUserProcedure ( selectorUserProcedure theProc, void* theUserData )
{
	userProcedure = theProc;
	userData = theUserData;
}
