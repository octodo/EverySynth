/*
 *  CAUGuiPoof.cpp
 *  RumblenceAU
 *
 *  Created by Urs Heckmann on Mon Oct 28 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiPoof.h"

#define POOF_ICON_WIDTH 			42.0
#define POOF_ICON_HEIGHT 			52.0
#define POOF_ANIMATION_DELAY		(1.0 / 15.0)
#define NUMBER_OF_POOF_ANIM_FRAMES	5


PoofButton::PoofButton (
								CAUGuiMan*			theChief,
								CAAUParameter		&theAuvp, 
								eRect*				theWhere,
								CAUGuiGraphic*		theForeGround, 
								CAUGuiGraphic*		theBackground,
								CAUGuiBottonMode	theMode,
								CAUGuiGraphic*		poof )
								
							:	CAUGuiButton (	theChief,
												theAuvp, 
												theWhere,
												theForeGround, 
												theBackground,
												theMode)
{
	thePoof = poof;
	poofWindow = NULL;
	frame = 0;
	box.origin.x = box.origin.y = 0;
	box.size.width = CGImageGetWidth( poof->getImage() );
	box.size.height = CGImageGetHeight( poof->getImage() );

}


PoofButton::~PoofButton ()
{
	thePoof = NULL;
	poofWindow = NULL;
}
	

void PoofButton::mouseDown(Point *P, bool with_option, bool with_shift)
{
	poofPoint.v = P->v + getBounds()->x;
	poofPoint.h = P->h + getBounds()->y;

	

	CAUGuiButton::mouseDown( P, with_option, with_shift);
	
	if ( poofWindow == NULL )
	{
		frame = 0;
			
		float windowWidth = CGImageGetWidth( thePoof->getImage() ) ;
		float windowHeight = CGImageGetHeight( thePoof->getImage() ) / 5.0;
	
		Rect bounds;

		bounds.top = poofPoint.v - (SInt16)(windowHeight / 2);
		bounds.left = poofPoint.h - (SInt16)(windowWidth / 2);
		bounds.bottom = bounds.top + (SInt16)windowHeight;
		bounds.right = bounds.left + (SInt16)windowWidth;
	
		CreateNewWindow( kOverlayWindowClass, 0, &bounds, &poofWindow );
		ShowWindow( poofWindow );
	}
}


void PoofButton::mouseUp(Point *P, bool with_option, bool with_shift)
{
	CAUGuiButton::mouseUp( P, with_option, with_shift);
	
	
	
}


void PoofButton::idle()
{
	if ( poofWindow != NULL )
	{
		
		CGContextRef context;
		
		float windowHeight = CGImageGetHeight( thePoof->getImage() ) / 5.0;
				
		box.origin.y = windowHeight * ( -4 + frame);
		
		CreateCGContextForPort( GetWindowPort( poofWindow ), &context );
		
		CGContextClearRect( context, box );
		CGContextDrawImage( context, box, thePoof->getImage() );
		CGContextFlush( context );
	
		CGContextRelease( context );
		
		frame++;
		
		if ( frame == 5 )
		{
			frame = 0;
			DisposeWindow( poofWindow );
			poofWindow = NULL;
		}
	}
	
	CAUGuiButton::idle();

}

