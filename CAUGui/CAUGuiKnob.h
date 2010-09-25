/*
 *  CAUGuiKnob.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 20 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */
 
/*! \brief CAUGuiKnob - Knob control - stirr da mouse
	
	Knobs take either CAUGuiSpinImages or multiframed CAUGuiGraphics
	
	CAUGuiSpinImages can be either rotating or trailing along a circular path.

	a typical rotating knob:
	<TABLE BGCOLOR="#F0F0C0"><TR><TD><TT>
	CAUGuiGraphic* fore = new CAUGuiSpinImage ( "fore.png", -0.375f, 0.375f, 0, 0, false ); 
	
	myCAUGuiMan->addImage( fore );
	
	CAUGuiGraphic* back = new CAUGuiGraphic ( "back.png" );
	
	myCAUGuiMan->addImage( back );
	
	CAAUParameter myAUVP(mEditAudioUnit, kMyParameter, kAudioUnitScope_Global, 0);
	
	eRect* where = new eRect ( 20, 30, 32, 32 );
	
	CAUGuiKnob* myKnob = new CAUGuiKnob(
											myCAUGuiMan,
											myAUVP,
											where,
											kCAUGui_res_100,
											fore,
											back );
	
	myCAUGuiMan->addCtrl( myKnob );
	</TT></TD></TR></TABLE>

	kMyParameter should have a range like 0.0 - 100.0
	
	myCAUGuiMan is the View's CAUGuiMan (you can add controls directly to it, but you'll most likely get along better using CAUGuiPane or CAUGuiLayeredPane to manage controls...
	
	
	now, for orbital behaviour, it get's a little tricky. You have to set the foreBounds in a way that the Pivot of the orbitting image sits at the top most position. It will then orbit around the center of the Knob control. You can displace the center with setCenter().
	
	A bit of experimantation will be necessary, though...
	
	

*/
 
 
 
 

#ifndef __CAUGUI_KNOB__
#define __CAUGUI_KNOB__

#include "CAUGuiConfig.h"

#include "CAUGui.h"


class CAUGuiKnob : public CAUGuiCtrl
{
public:

	/** \brief constructor for a knob control

		\param theChief instance of controlling CAUGuiMan
		\param theAuvp the CAAUParameter to be controlled
		\param theWhere Control Bounds in pane-relative coordinates
		\param theRes CAUGuiResolution
		\param theForeGround foreground CAUGuiGraphic, usually a CAUGuiSpinImage or a multiframe CAUGuiGraphic
		\param theBackground background CAUGuiGraphic (can be NULL)
		
		if the foreground graphics is multiframe, it maps the parameter's values accordingly and shows the correct subframe
		
	*/


	CAUGuiKnob 			(	CAUGuiMan*				theChief,
							CAAUParameter			&theAuvp, 
							eRect*					theWhere,
							CAUGuiResolution		theRes, 
							CAUGuiGraphic*			theForeGround, 
							CAUGuiGraphic*			theBackground);
							
	//! destructor
	virtual ~CAUGuiKnob ();

	virtual void draw(CGContextRef context, UInt32 portHeight);
	virtual void mouseDown(Point *P, bool, bool);
	virtual void mouseTrack(Point *P, bool, bool);
	virtual void mouseUp(Point *P, bool, bool);
	
	//! center of rotation/mouse stirring in relative coordinates
	virtual void setCenter ( SInt32 x, SInt32 y ) { centerX = x; centerY = y; }
	
private:

	float				getValueFromPoint(Point* P);
	
	SInt32				centerX;
	SInt32				centerY;
	
	CAUGuiGraphic*		ForeGround;
	CAUGuiGraphic*		BackGround;
	
	
};


#endif





