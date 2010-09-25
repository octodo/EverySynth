/*
 *  CAUGuiSlider.cpp
 *  CAUGuiDemo
 *
 *  Created by Urs Heckmann on Fri Dec 13 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiSlider.h"

#include "CAUGuiMoreImages.h"

CAUGuiSlider::CAUGuiSlider (	CAUGuiMan*			theChief,
							CAAUParameter		&theAuvp, 
							eRect*				theWhere,
							UInt32				theOrientation,
							CAUGuiResolution	theRes, 
							CAUGuiGraphic*		theForeGround, 
							CAUGuiGraphic*		theBackground)
						
						:	CAUGuiCtrl ( theChief, theAuvp, theWhere, theRes )
							
{
		
	ForeGround = theForeGround;
	BackGround = theBackground;
	
	orientation = theOrientation;
	
	if ( theWhere->w == 0 )
	{
		if ( BackGround != NULL )
		{
			theWhere->w = BackGround->getWidth();
			setBounds(theWhere); 
		}
		else
		{
			if ( theForeGround != NULL )
			{
				theWhere->w = theForeGround->getWidth();
				setBounds(theWhere); 
			}
		
		}
	
	}
	
	if ( theWhere->h == 0 )
	{
		if ( BackGround != NULL )
		{
			theWhere->h = BackGround->getHeight();
			setBounds(theWhere);
		}
		else
		{
			if ( theForeGround != NULL )
			{
				theWhere->h = theForeGround->getHeight();
				setBounds(theWhere); 
			}
		
		}
	
	}
		
	setType( kCAUGui_Slider );
	
	setTolerance ( (SInt32) theRes );
	
	//printf ("ctor CAUGui_Slider\n");

}

CAUGuiSlider::~CAUGuiSlider ()
{

	ForeGround = NULL;
	BackGround = NULL;


	//printf ("dtor CAUGui_Slider\n");


}

void CAUGuiSlider::draw(CGContextRef context, UInt32 portHeight )
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

SInt32 sldr_X;
SInt32 sldr_Y;

void CAUGuiSlider::mouseDown(Point *P, bool with_option, bool with_shift)
{

	sldr_X = P->h;
	sldr_Y = P->v;

	if ( !with_shift )
		mouseTrack ( P, with_option, with_shift );

}

void CAUGuiSlider::mouseTrack(Point *P, bool with_option, bool with_shift)
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
	
	
	
	//printf ( "fore %d %d %d %d\n", (int)fore.x, (int)fore.y, (int)fore.w, (int)fore.h );

	if ( !with_shift )
	{		
		SInt32 max_X = fore.w + o_X;
		SInt32 max_Y = fore.h + o_Y;
		
		if ( orientation != 0 ) // horizontal mode
		{				
			if ( P->h <= o_X )		val = 0;
			if ( P->h >= max_X )	val = max;
			if ( val == -1 )
			{
				float fract = (float)(P->h - o_X) / (float)fore.w;
				val = (SInt32)((float)max * fract);
			}
		}
		else	// vertical mode
		{
			if ( P->v >= max_Y )	val = 0;
			if ( P->v <= o_Y )		val = max;
			if ( val == -1 )
			{
				float fract = (float)(P->v - o_Y) / (float)fore.h;
				val = (SInt32)((float)max * (1.f - fract));
			}
		}
	}
	else
	{
		val = (SInt32) GetControl32BitValue( carbonControl );
	
		if ( orientation != 0 ) // horizontal mode
		{
		
			SInt32 diff =  P->h - sldr_X;
		
			if ( diff < -1 || diff > 1 )
			{
				diff /= 2;
				val += diff;
				sldr_X += diff * 2;
			}
		}
		else	// vertical mode
		{
			SInt32 diff = sldr_Y - P->v;
		
			if ( diff < -1 || diff > 1 )
			{
				diff /= 2;
				val += diff;
				sldr_Y -= diff * 2;
			}
		}

	
	
	}
	
	if ( val > max ) val = max;
	if ( val < 0 ) val = 0;
	SetControl32BitValue ( carbonControl, (UInt32) val );
	
	//printf ( "Slider says: %d\n", val );
}

void CAUGuiSlider::mouseUp(Point *P, bool with_option, bool with_shift)
{
	mouseTrack ( P, with_option, with_shift );
}



