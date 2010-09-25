/*
 *  CAUGuiTools.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Mon Oct 28 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 *	Thanks to Franck Stauffer for the multifame code example
 *	(you won't recognize it anymore, but it's here, somewhere ;-)
 *
 */

#include "CAUGuiTools.h"

/***********************************************************************

	eRect
	
	a Rect class with better properties for layout issues...
	
	(sorry for the naming, but I just couldn't resist :-)

***********************************************************************/

void eRect::align ( CAUGuiAlign align )
{
	eRect r;
	
	r.set ( this );
	
	this->align ( &r, align );

}



void eRect::align ( eRect* r, CAUGuiAlign align )
{

	int V = ((int) align & 7 );
	
	switch ( V )
	{
		case 2:	// North
			
				y = r->y;
				break;
		
		case 4: // South
		
				y = r->y + r->h - h;
				break;
				
		case 7:
		case 6: // Middle
		
				y = r->y + ( r->h / 2 ) - ( h / 2 );
				break;
		
		case 3: // Above
		
				y = r->y - h;
				break;
		
		case 5: // Below
				
				y = r->y + r->h;
				break;
	
		default:
				break;
	
	}
	
	int H = ((int) align & 56 ) / 8;
	
	switch ( H )
	{
		case 2:	// West
			
				x = r->x;
				break;
		
		case 4: // East
		
				x = r->x + r->w - w;
				break;
				
		case 7:
		case 6: // Center
		
				x = r->x + ( r->w / 2 ) - ( w / 2 );
				break;
		
		case 3: // Before
		
				x = r->x - w;
				break;
		
		case 5: // Behind
				
				x = r->x + r->w;
				break;
	
		default:
				break;	
	
	}



}

/***********************************************************************

	CAUGuiImage
	
	class for loading and containing images

***********************************************************************/

CAUGuiImage::CAUGuiImage ( char* pngFileName )
{
	
	loadImagePNG ( pngFileName );
	
	strcpy ( fileName, pngFileName );
	
	resourceID = 0;
	
	setType ( kCAUGui_PNG );

}

bool CAUGuiImage::isPNG ( char* pngFileName )
{

	if ( strcmp ( fileName, pngFileName ) ) return false;
	
	return true;

}

void CAUGuiImage::loadImagePNG ( char* pngFileName )
{
	
	CFBundleRef bundleRef = CFBundleGetBundleWithIdentifier(CFSTR( AU_BUNDLE_IDENTIFIER ));
	CGDataProviderRef	provider;
	
	if ( bundleRef )
	{
		SInt16 tmp = CurResFile();
	
		CFStringRef fileName = ::CFStringCreateWithCString( NULL, pngFileName, kCFStringEncodingASCII );
		if ( fileName != NULL )
		{
			CFURLRef url = ::CFBundleCopyResourceURL( bundleRef, fileName, NULL, NULL );
			
			provider = CGDataProviderCreateWithURL( url );

			Image = CGImageCreateWithPNGDataProvider( provider, NULL, false,  kCGRenderingIntentDefault );
			
			CGDataProviderRelease( provider );
			CFRelease( url );
			CFRelease( fileName );
		}
		else
		{
			Image = NULL;
		}
		
		 UseResFile(tmp);
	}
	
}

/*

void CAUGuiImage::loadImageRSRC ( char* pngFileName )
{
	
	CFBundleRef bundleRef = CFBundleGetBundleWithIdentifier(CFSTR( AU_BUNDLE_IDENTIFIER ));
	CGDataProviderRef	provider;
	
	if ( bundleRef )
	{
		SInt16 tmp = CurResFile();

		PicHandle thePicture;
	
		thePicture = GetPicture( 134 );
		DetachResource( (Handle) thePicture );
	
			
	// reminder: open QuartzPrimer.pdf in Acrobat and Copy/Paste that damn code!!! 			
							
		
		if (thePicture)
		ReleaseResource( (Handle) thePicture );

	
		
		 UseResFile(tmp);
	}
	
}

*/

/***********************************************************************

	CAUGuiGraphic
	
	class for loading and containing images

***********************************************************************/

CAUGuiGraphic::CAUGuiGraphic ( char* pngFileName )
{
	
	loadImagePNG ( pngFileName );	
	Frames = 1;
	FrameHeight = 1;
	
	if ( Image != NULL )
		FrameHeight = CGImageGetHeight (Image);
		
	setType ( kCAUGui_Graphic );

}

CAUGuiGraphic::CAUGuiGraphic ( char* pngFileName, UInt32 numFrames )
{
	
	loadImagePNG ( pngFileName );
	if ( numFrames > 1 )	Frames = numFrames;
	else	Frames = 1;
	
	int height = Frames;
	
	if ( Image != NULL )
		height = CGImageGetHeight (Image);
	
	FrameHeight = height / numFrames;
	
	setType ( kCAUGui_Graphic );

}

SInt32 CAUGuiGraphic::getWidth()
{
	SInt32 width = 0;

	if ( Image != NULL )
		width = CGImageGetWidth (Image); 

	return width;
}
	
SInt32 CAUGuiGraphic::getHeight()
{
	return FrameHeight;
}

void CAUGuiGraphic::draw( CGContextRef context, UInt32 portHeight, eRect* r, float value )
{
	// if Image has frames, the frame will be drawn
	// otherwise, the plain image will get out
	
	CGRect bounds;
	
	if ( Frames > 1 )
	{
     
		r->to( &bounds, portHeight );
		
		SInt32 offset = (SInt32)((float)(Frames-1) * value) * r->h + r->h;
		
		SInt32 height = Frames * r->h;
		
		bounds.size.height = height;
		
		bounds.origin.y += offset-height;
	
	}
	else
	{
		r->to( &bounds, portHeight );
	}

	if ( Image != NULL )
			CGContextDrawImage( context, bounds, Image );


}


void CAUGuiGraphic::loadImagePNG ( char* pngFileName )
{
	
	CFBundleRef bundleRef = CFBundleGetBundleWithIdentifier(CFSTR( AU_BUNDLE_IDENTIFIER ));
	CGDataProviderRef	provider;
	
	if ( bundleRef )
	{
		SInt16 tmp = CurResFile();
	
		CFStringRef fileName = ::CFStringCreateWithCString( NULL, pngFileName, kCFStringEncodingASCII );
		if ( fileName != NULL )
		{
			CFURLRef url = ::CFBundleCopyResourceURL( bundleRef, fileName, NULL, NULL );
			
			provider = CGDataProviderCreateWithURL( url );

			Image = CGImageCreateWithPNGDataProvider( provider, NULL, false,  kCGRenderingIntentDefault );
			
			CGDataProviderRelease( provider );
			CFRelease( url );
			CFRelease( fileName );
		}
		else
		{
			Image = NULL;
		}
		
		 UseResFile(tmp);
	}
	
}


/***********************************************************************

	CAUGuiLabel
	
	class for loading and containing images

***********************************************************************/

CAUGuiLabel::CAUGuiLabel ( CAUGuiGraphic* theGraphic, eRect* theBounds )
{
	
	Graphic = theGraphic;
	
	Bounds.set( theBounds );
	
	if ( Bounds.w == 0 )
		Bounds.w = theGraphic->getWidth();
		
	if ( Bounds.h == 0 )
		Bounds.h = theGraphic->getHeight();
		
	setType ( kCAUGui_Label );

}

void CAUGuiLabel::draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value )
{

	eRect* theBounds;
	
	theBounds->set( rect );
	
	theBounds->x += Bounds.x;
	theBounds->y += Bounds.y;
	theBounds->h = Bounds.h;
	theBounds->w = Bounds.w;
	
	Graphic->draw( context, portHeight, theBounds, value );
	
	
}

/***********************************************************************

	CAUGuiScheme
	
	class for automatically layouting controls

***********************************************************************/

CAUGuiScheme::CAUGuiScheme ()
{
	patternLength = 0;
	currentIteration = 0;
	currentPattern = 0;
	X = 0;
	Y = 0;
	startX = 0;
	startY = 0;
	offsetX = 0;
	offsetY = 0;
	notAligned = true;

}

void CAUGuiScheme::add( eRect* r, CAUGuiAlign align )
{
	if ( patternLength < kCAUGuiSchemeMaxPatterns )
	{
		rects[ patternLength ].set( r );
		aligns[ patternLength ] = align;
		patternLength++;
	}
}
	
void CAUGuiScheme::at ( SInt32 x, SInt32 y )
{
	X = x; Y = y;
	notAligned = true;
}
	
eRect* CAUGuiScheme::get()
{
	currentPattern++;
	if ( currentPattern >= patternLength )
	{
		currentPattern = 0;
		X = startX + offsetX * currentIteration;
		Y = startY + offsetY * currentIteration;
		notAligned = true;
		currentIteration++;
	}
	
	
	if ( notAligned )
	{
		resulting.set( &rects[ currentPattern ] );
		resulting.offset( X, Y );
		notAligned = false;
	}
	else
	{
		resulting.align ( &rects[ currentPattern ], aligns[ currentPattern ] );
	
	}
	
	return &resulting;

}

eRect* CAUGuiScheme::getS( SInt32 w, SInt32 h )
{
	eRect* ref = get ();
	
	ref->w = w;
	ref->h = h;

	return ref;
}


void CAUGuiScheme::reset()
{
	currentPattern = 0;
	currentIteration = 0;
	notAligned = true;
	X = startX;
	Y = startY;
	
}