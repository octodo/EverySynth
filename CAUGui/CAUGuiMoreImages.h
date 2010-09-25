/*
 *  CAUGuiMoreImages.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Nov 03 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiTools.h"


/** \brief CAUGuiSpinImage handles rotating images

	This CAUGuiGraphic obviously handles an image.
	
	It does this in two ways:
	
	- a) dynamically rotates it or
	
	- b) traverses it on an orbital path
	
	For both ways it has a so called Pivot point that by default sits in the center of the image. For both cases, this may be in most cases sufficient, but for orbital movement the there's another Pivot to be passed from the control object that should commonly be shifted _below_ the image by half the diameter of the circle.
	
	The image passed is assumed to point to the top.
	
	The amount rotation is scaled and mapped to a definable angle. Commonly it is a 270 degree rotation which means minimum value is 135 degree to the left (counter clockwise) and maximum value is 135 degree to the right (clockwise)
	
	The scaling itself isn't done in degree or radians but in normalized measure of the unit circle, so the values look like this:
	
	min = -0.375f // 135 degree counter clockwise
	
	max = 0.375f // 135 degree clockwise
	
	example:
	
	the typical rotating knob:
	<TABLE BGCOLOR="#F0F0C0"><TR><TD><TT>
	CAUGuiGraphic* needle_handle = new CAUGuiSpinImage ( "knob_needle.png", -0.375f, 0.375f, 0, 0, false );
	</TT></TD></TR></TABLE>
	
	an orbiting handle with an Pivot offset to compensate for a little shadow on bottom right:
	<TABLE BGCOLOR="#F0F0C0"><TR><TD><TT>
	CAUGuiGraphic* round_handle = new CAUGuiSpinImage ( "round_handle.png", -0.375f, 0.375f, -2, 2, true );
	</TT></TD></TR></TABLE>
	
*/


class CAUGuiSpinImage : public CAUGuiGraphic
{
public:

	/** \brief constructor for a CAUGuiSpinImage

		\param pngFileName the filename of the PNG resource
		\param min rotation amout to the left
		\param max rotation amount to the right
		\param pivotOffsetX horizontal offset of center of rotation from center of image
		\param pivotOffsetY vertical offset of center of rotation from center of image
		\param fixed false = rotation, true = orbital behaviour 
		
	*/

	CAUGuiSpinImage ( char* pngFileName, float min, float max, SInt32 pivotOffsetX, SInt32 pivotOffsetY, bool fixed );
	
	//! destructor
	virtual ~CAUGuiSpinImage () {};
	
	virtual void			draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value );
	
	//! special case when used in a knob and the knob's internal pivot isn't in the center
	virtual void			draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value, SInt32 pivotOffsetX, SInt32 pivotOffsetY );

private:

	float		startRad;
	float		allRad;
	float		pivotX;
	float		pivotY;
	bool		imageOrientationFixed; // for non-rotating handle Images
	

};


/** \brief CAUGuiHandleImage performs value dependent shifting (i.e. for faders)

	This type of image can be used for faders, switches, peak meters etc.
	
	It scales itself proportional to fit in the foreBounds of the Control object it is attached to. Naturally, when doing this, there's still room inside the area in either horizontal or vertical direction. The image will then traverse along that direction according to the _normalized_ value passed.
	
	Note that the position is determined from the Pivot (default: center of image), so on maximom values this image will draw itself with one half out of the bounds of the foreground area passed.
	
	
	
	Example given: 
	
	Your image is 30x20 pixels
	
	Your control passes an eRect (foreground Bounds, member "vizArea") that is 100 pixels wide and 10 pixels high
	
	The CAUGuiHandleImage object will the scale the image to 15x10 pixels.
	
	On value 0.0f it will be drawn with its center at x = 0, y = 5 of your foreBounds, hence exceed the bounds on the left side about 7 pixels!
	
	Hence, if your Parameter has 100 values, each pixel represents an integer step.
	
	So, take care how you shrink your Slider's foreBounds. Once you get that, it's easy.
			
*/

class CAUGuiHandleImage : public CAUGuiGraphic
{
public:

	/** \brief constructor for a CAUGuiHandleImage

		\param pngFileName the filename of the PNG resource
		\param pivotOffsetX horizontal offset of center of rotation from center of image
		\param pivotOffsetY vertical offset of center of rotation from center of image
		
	*/

	CAUGuiHandleImage ( char* pngFileName, SInt32 pivotOffsetX, SInt32 pivotOffsetY );
	
	//! destructor
	virtual ~CAUGuiHandleImage () {};
	
	virtual void			draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value );
	

private:

	SInt32		pivotX;
	SInt32		pivotY;
	

};


/** \brief CAUGuiCroppingImage performs value dependent shifting, cropping or scaling

	This type of image can be used for faders, meters etc. It generally hides a part of itself according to value...
	
	You have three different modes:
	
	- 0 - Cropping: Image draw inside foreBounds, but displays only a part of it according to value
	
	- 1 - Shifting: Image scales to foreBounds but shifts from the outside into these according to value
	
	- 2 - Scaling: Image draws inside foreBounds, but scales according to value
	
	
	Again, a Pivot determines the center of action
	
	An Orientation parameter determines either horizontal or vertical action, it is set up in binary flags:
	
	- bit 0 (1) = bottom-pivotY, lower values scale the action from bottommost position up to the Pivot
	
	- bit 1 (2) = top-pivotY, lower values scale the action from topmost position down to the Pivot
	
	- bit 2 (4) = left-pivotX, lower values scale the action from leftmost position right to the Pivot
	
	- bit 3 (8) = right-pivotX, lower values scale the action from rightmost position left to the Pivot
	
	
	Note: on scaling and cropping, the Orientation flags can be combined, but only one vertical and one horizontal make sense on shifting images.
	
	
	example:
	
	a simple image that works good for a vertical LED (meter) or a vertical Slider
	
	assume ledchain.png to be an image of 10x100 pixels
	
	<TABLE BGCOLOR="#F0F0C0"><TR><TD><TT>
	CAUGuiGraphic* horizontal_handle = new CAUGuiCroppingImage ( "ledchain.png", 0, 50, 2, 0 ); 
	</TT></TD></TR></TABLE>
	
	Mode is 0, so this is a cropping image
	
	Orientation is 2, so on lower values it displays less of it above the pivot
	
	The pivot is set to 50 pixels down from center of the image, so at its very bottom. Hence, normalized values from 0.0f to 1.0f make the Image more and more visible from its bottom to its top.
	
	
	If we now set Mode to 2 (scaling), the whole area of the image would be drawn, but it would be shrinked on lower values.
	
	Accordingly, setting Mode to 1 (shifting), the image would behave like a drawbar and on lower values shift out of the bottom of the control
	
	Tip: Not setting the Pivot to image borders lets some of it visible even on extreme values. This may be good for darwbars (organ visuals, made up from CAUGuiSliders), VU Meters etc.
	
*/

class CAUGuiCroppingImage : public CAUGuiGraphic
{
public:

	/** \brief constructor for a CAUGuiCroppingImage

		\param pngFileName the filename of the PNG resource
		\param pivotOffsetX horizontal offset of center of rotation from center of image
		\param pivotOffsetY vertical offset of center of rotation from center of image
		\param theOrientation 1 = bottom-pivotY, 2 = top-pivotY, 3 = left-pivotX, 4 = right-pivotX
		\param theMode 0 = cropping, 1 = shifting, 2 = scaling
		
	*/

	CAUGuiCroppingImage ( char* pngFileName, SInt32 pivotOffsetX, SInt32 pivotOffsetY, SInt32 theOrientation, UInt32 theMode );
	
	//! destructor
	virtual ~CAUGuiCroppingImage () {};
	
	virtual void			draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value );
	

private:

	SInt32		pivotX;
	SInt32		pivotY;
	SInt32		orientation;
	UInt32		mode;
	

};