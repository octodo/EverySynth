/*
 *  CAUGuiKnob.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 20 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiKnob.h"
#include "CAUGuiMoreImages.h"

CAUGuiKnob::CAUGuiKnob (	CAUGuiMan*			theChief,
							CAAUParameter		&theAuvp, 
							eRect*				theWhere,
							CAUGuiResolution	theRes, 
							CAUGuiGraphic*		theForeGround, 
							CAUGuiGraphic*		theBackground)
						
						:	CAUGuiCtrl ( theChief, theAuvp, theWhere, theRes )
							
{
		
	ForeGround = theForeGround;
	BackGround = theBackground;
	
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
	
	centerX = theWhere->w / 2;
	centerY = theWhere->h / 2;
	
	setType( kCAUGui_Knob );
	
	setTolerance ( (SInt32) theRes );
	
	//printf ("ctor CAUGuiKnob\n");

}

CAUGuiKnob::~CAUGuiKnob ()
{

	ForeGround = NULL;
	BackGround = NULL;


	//printf ("dtor CAUGuiKnob\n");


}

void CAUGuiKnob::draw(CGContextRef context, UInt32 portHeight )
{
	ControlRef carbonControl = getCarbonControl();
	
	UInt32 max = GetControl32BitMaximum(carbonControl);
	UInt32 val = GetControl32BitValue( carbonControl );
	
	CGImageRef theBack = NULL;
	CGImageRef theKnob = NULL;
	
	CGRect bounds;
	
	getBounds()->to( &bounds, portHeight );
	
	if ( BackGround != NULL )
		theBack = BackGround->getImage();
	
	            
	if ( theBack != NULL )
		CGContextDrawImage( context, bounds, theBack );
	
	SInt32 pivotX = centerX - (getForeBounds()->x - getBounds()->x + getForeBounds()->w/2 );
	SInt32 pivotY = centerY - (getForeBounds()->y - getBounds()->y + getForeBounds()->h/2 );
	
	//printf ( "x %d  y %d \n", pivotX, pivotY );
	
	if ( ForeGround != NULL )
	{
		theKnob = ForeGround->getImage();
	
		if ( theKnob != NULL )
		{
			float valNorm = (float) val / (float) max;
			
			if ( ForeGround->getType() == kCAUGui_SpinImage )
				((CAUGuiSpinImage*)ForeGround)->draw ( context, portHeight, getForeBounds(), valNorm, pivotX, pivotY );
			else
				ForeGround->draw ( context, portHeight, getForeBounds(), valNorm );
		}
	}
}

SInt32 cent_X;
SInt32 cent_Y;
SInt32 tmp;
SInt32 val;

void CAUGuiKnob::mouseDown(Point *P, bool with_option, bool with_shift)
{
	ControlRef carbonControl = getCarbonControl();

	if ( !with_option )
		mouseTrack ( P, with_option, with_shift );
	else
	{
		val = tmp = GetControl32BitValue( carbonControl );
		cent_X = P->h;
		cent_Y = 0;
	}

}

void CAUGuiKnob::mouseTrack(Point *P, bool with_option, bool with_shift)
{

	ControlRef carbonControl = getCarbonControl();

	SInt32 max = (SInt32) GetControl32BitMaximum(carbonControl);

	if ( with_option ) // linear mode 
	{				
		if ( cent_Y != (cent_X - P->h) )
		{
			cent_Y = cent_X - P->h;
			if ( with_shift ) val = tmp - (cent_Y/2);
			else val = tmp - (UInt32)getResolution () * (cent_Y/2);
		}
	}
	else
	{
		if ( with_shift ) val = (SInt32)( getValueFromPoint(P) * (float) max );
		else val = (SInt32)getResolution () * (SInt32)( getValueFromPoint(P) * (float) getRange() );
	}
			
	if ( val > max ) val = max;
	if ( val < 0 ) val = 0;
	SetControl32BitValue ( carbonControl, (UInt32) val );
}

void CAUGuiKnob::mouseUp(Point *P, bool with_option, bool with_shift)
{
	mouseTrack ( P, with_option, with_shift );
}

float CAUGuiKnob::getValueFromPoint(Point* P)
{
	
	//printf ( "X= %d - Y= %d\n", P->h, P->v ); 
	
	float deltaX = (float)( P->h - centerX );
	float deltaY = (float)( P->v - centerY ); // bigger if upwards

	//printf ( "X= %f - Y= %f\n", deltaX, deltaY );

	float PIh = PI / 2.0;
    float angle;

    if ( deltaY == 0.f )
    {
        if ( deltaX < 0.f ) angle = -PIh;
        else angle = PIh;
    }
    else
    {
        if ( deltaY > 0.f )
        {
            angle = atan ( deltaX / deltaY );
            if ( angle > 0.f ) angle = PI - angle;
            else angle = -PI - angle;
        }
        else
        {
            angle = - atan ( deltaX / deltaY );
        }
    }

    angle += PI * 3. / 4.;
    return angle / ( PI * 1.5 );


}



