/*
 *  CAUGuiSlider.h
 *  CAUGuiDemo
 *
 *  Created by Urs Heckmann on Fri Dec 13 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CAUGUI_SLIDER__
#define __CAUGUI_SLIDER__

#include "CAUGuiConfig.h"

#include "CAUGui.h"

/*! \brief CAUGuiSlider - a one dimensional control for fader like behaviour

	Sliders can be more than just Faders. You can also build switches with 2 or more values upon them.
	
	They are commonly used with a CAUGuiHandleImage to draw the faderhandle above its background. To achieve this in a proper manner, just create your foreBounds accordingly.
	<BR><BR>
	An example:
	
	Assume you have a background image h_slider_back that is 120x20 pixels and a 10x10 pixel handle image h_slider_image  the value should be adjustable in 100 steps (or better 101, taking 0-100 into account) where each pixel of the fader way should represent 1 step. The handle furthermore should be centered in a way to have a 5 pixel border at each side of the fader. You'd have to do this
	
	<TABLE BGCOLOR="#F0F0C0"><TR><TD><TT>
	eRect where;
	
	where.set ( 50, 50, 120, 20 );
	
	CAUGuiSlider* mySlider = new CAUGuiSlider(myCAUGui, AUVParameterForSlider, &where, 1, kCAUGui_res_100, h_slider_image, h_slider_back );
	
	mySlider->shrinkForeBounds ( 10, 5, 10, 5 );
	</TT></TD></TR></TABLE>
	
	This sets the eRect passed to the handle image to a size of 100x10 pixels that perfectly lays centered in our fader's bounds.




*/

class CAUGuiSlider : public CAUGuiCtrl
{
public:

	/** \brief constructor for a slider control (aka fader)

		\param theChief instance of controlling CAUGuiMan
		\param theAuvp the CAAUParameter to be controlled
		\param theWhere Control Bounds in pane-relative coordinates
		\param theOrientation 0 = verical, 1 = horizontal
		\param theRes CAUGuiResolution
		\param theForeGround foreground CAUGuiGraphic, usually a CAUGuiHandleImage
		\param theBackground background CAUGuiGraphic
		
	*/


	CAUGuiSlider 		(	CAUGuiMan*			theChief,
							CAAUParameter		&theAuvp, 
							eRect*				theWhere,
							UInt32				theOrientation,
							CAUGuiResolution	theRes, 
							CAUGuiGraphic*		theForeGround, 
							CAUGuiGraphic*		theBackground);
	//! destructor						
	virtual ~CAUGuiSlider ();

	virtual void draw(CGContextRef context, UInt32 portHeight);
	virtual void mouseDown(Point *P, bool, bool);
	virtual void mouseTrack(Point *P, bool, bool);
	virtual void mouseUp(Point *P, bool, bool);
		
private:

	UInt32				orientation;
	CAUGuiGraphic*		ForeGround;
	CAUGuiGraphic*		BackGround;
	
	
};








#endif