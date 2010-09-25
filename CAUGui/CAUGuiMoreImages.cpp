/*
 *  CAUGuiMoreImages.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Nov 03 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiMoreImages.h"

CAUGuiSpinImage::CAUGuiSpinImage (
									char* pngFileName,
									float min,
									float max,
									SInt32 pivotOffsetX,
									SInt32 pivotOffsetY,
									bool fixed
								)
								:	CAUGuiGraphic ( pngFileName )
{

	this->startRad = -2.0 * PI * min;
	this->allRad = 2.0 * PI * max + startRad;
	
	
	
	pivotX = 0.5 + (float)pivotOffsetX / (float)getWidth();
	pivotY = 0.5 + (float)pivotOffsetY / (float)getHeight();

	//printf ( "x = %f  y = %f \n", pivotX, pivotY );

	imageOrientationFixed = fixed;
	
	setType ( kCAUGui_SpinImage );


}
	
void CAUGuiSpinImage::draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value )
{

	CGRect bounds;
	
	rect->to( &bounds, portHeight );
	
	
	CGContextTranslateCTM( context, bounds.origin.x + bounds.size.width * pivotX, bounds.origin.y + bounds.size.height * pivotY);
		
	bounds.origin.x = -bounds.size.width/2;
	bounds.origin.y = -bounds.size.height/2;
		
	float angle;
		
	angle = value * allRad - startRad;
				
	CGContextRotateCTM( context, -angle );
		
	CGContextTranslateCTM( context, bounds.size.width * ( 0.5f - pivotX ), bounds.size.height * ( pivotY - 0.5f ));
		
	if ( imageOrientationFixed )
		CGContextRotateCTM( context, angle );
		
	if ( getImage() != NULL )
	{
			CGContextDrawImage( context, bounds, getImage() );
	}

	
}

void CAUGuiSpinImage::draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value, SInt32 pivotOffsetX, SInt32 pivotOffsetY )
{

	CGRect bounds;
	
	rect->to( &bounds, portHeight );
	
	
	CGContextTranslateCTM( context, bounds.origin.x + bounds.size.width/2 + pivotOffsetX, bounds.origin.y + bounds.size.height/2 - pivotOffsetY);
		
	bounds.origin.x = -bounds.size.width/2;
	bounds.origin.y = -bounds.size.height/2;
		
	float angle;
		
	angle = value * allRad - startRad;
				
	CGContextRotateCTM( context, -angle );
		
	CGContextTranslateCTM( context, -pivotOffsetX, pivotOffsetY);
		
	if ( imageOrientationFixed )
		CGContextRotateCTM( context, angle );
		
	if ( getImage() != NULL )
	{
			CGContextDrawImage( context, bounds, getImage() );
	}


}




CAUGuiHandleImage::CAUGuiHandleImage (
									char* pngFileName,
									SInt32 pivotOffsetX, 
									SInt32 pivotOffsetY 
								)
								:	CAUGuiGraphic ( pngFileName )
{

	pivotX = pivotOffsetX;
	pivotY = pivotOffsetY;


}
	
void CAUGuiHandleImage::draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value )
{

	CGRect bounds;
	SInt32 offset;
	
	eRect fore;
	
	fore.set( rect );
	
	float scaleY = (float)fore.w  / (float)getWidth();
	float scaleX = (float)fore.h  / (float)getHeight();
	
	if ( getImage() != NULL )
	{

		if ( scaleX > scaleY )
		{
		
			SInt32 height = (SInt32)((float)getHeight() * scaleY );
		
			offset = (SInt32)((float)fore.h * (1.f - value)) - height/2 - pivotY;
			
			fore.offset ( -pivotX, offset );
			
			fore.h = height;
						
		}
		else
		{
			SInt32 width = (SInt32)((float)getWidth() * scaleX );
		
			offset = (SInt32)((float)fore.w * value) - width/2 + pivotX;
			
			fore.offset ( offset, -pivotY );
			
			fore.w = width;
	
		}
		
		fore.to( &bounds, portHeight );
			
		CGContextDrawImage( context, bounds, getImage() );
	}
	
}

CAUGuiCroppingImage::CAUGuiCroppingImage (
									char* pngFileName,
									SInt32 pivotOffsetX, 
									SInt32 pivotOffsetY,
									SInt32 theOrientation,
									UInt32 theMode
								)
								:	CAUGuiGraphic ( pngFileName )
{

	if ( theOrientation == 0 )
		theOrientation = 2; // default: vertical from top
		
	pivotX = pivotOffsetX;
	pivotY = pivotOffsetY;
	orientation = theOrientation;
	mode = theMode;


}
	
void CAUGuiCroppingImage::draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value )
{

	CGRect bounds;
	
	eRect fore;
	
	fore.set( rect );
	
	float sPivotX = ((float)fore.w  / (float)getWidth()) * (float)(getWidth()/2 + pivotX);
	float sPivotY = ((float)fore.h  / (float)getHeight()) * (float)(getHeight()/2 + pivotY);
	
	if ( getImage() != NULL )
	{
	
		switch ( mode )
		{
			case 1: // Shifting
			{
				fore.to( &bounds, portHeight );
				
				CGContextSaveGState( context );
				
				CGContextClipToRect( context, bounds);
			
			
				value = 1.f - value;
		
				SInt32 shift_h = 0;
				SInt32 shift_v = 0;
		
				switch ( orientation & 5 )
				{
					case 1:
							shift_h = (SInt32)(-value * (float)fore.w);
							break;
					case 4:
							shift_h = (SInt32)(value * (float)fore.w);
							break;
				}
				
				switch ( orientation & 10 )
				{
					case 2:
							shift_v = (SInt32)(value * (float)fore.h);
							break;
					case 8:
							shift_v = (SInt32)(-value * (float)fore.h);
							break;
				}
		
				fore.offset ( shift_h, shift_v );
								
				fore.to( &bounds, portHeight );
			
				CGContextDrawImage( context, bounds, getImage() );
				
				CGContextRestoreGState( context );
				
				break;
			}
			
			case 2: // Scaling
			{
				value = 1.f - value;
		
				if ( (orientation & 1) == 1 && sPivotX > 0.f )
				{
					fore.grow ( -(int)(sPivotX * value), 0, 0, 0 );
				}
			
				if ( (orientation & 2) == 2  && sPivotY > 0.f )
				{
					fore.grow ( 0, -(int)(sPivotY * value), 0, 0 );
				}
			
				if ( (orientation & 4) == 4  && sPivotX < (float)fore.w )
				{
					fore.grow ( 0, 0, -(int)((sPivotX - (float)fore.w ) * value), 0 );
				}
			
				if ( (orientation & 8) == 8  && sPivotY < (float)fore.h )
				{
					fore.grow ( 0, 0, 0, -(int)((sPivotY - (float)fore.h) * value) );
				}
				fore.to( &bounds, portHeight );
			
				CGContextDrawImage( context, bounds, getImage() );
				
				break;
			}
			
			default: // Cropping
			{
				value = 1.f - value;
		
				if ( (orientation & 1) == 1 && sPivotX > 0.f )
				{
					fore.grow ( -(int)(sPivotX * value), 0, 0, 0 );
				}
			
				if ( (orientation & 2) == 2  && sPivotY > 0.f )
				{
					fore.grow ( 0, -(int)(sPivotY * value), 0, 0 );
				}
			
				if ( (orientation & 4) == 4  && sPivotX < (float)fore.w )
				{
					fore.grow ( 0, 0, -(int)((sPivotX - (float)fore.w ) * value), 0 );
				}
			
				if ( (orientation & 8) == 8  && sPivotY < (float)fore.h )
				{
					fore.grow ( 0, 0, 0, -(int)((sPivotY - (float)fore.h) * value) );
				}
				
				fore.to( &bounds, portHeight );
				
				CGContextSaveGState( context );
				
				CGContextClipToRect( context, bounds);
								
				rect->to( &bounds, portHeight );
			
				CGContextDrawImage( context, bounds, getImage() );
				
				CGContextRestoreGState( context );
				
				break;
			}
		}		
	}
}




