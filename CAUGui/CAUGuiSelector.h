/*
 *  CAUGuiSelector.h
 *  CAUGuiDemo
 *
 *  Created by Urs Heckmann on Fri Dec 13 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CAUGUI_SELECTOR__
#define __CAUGUI_SELECTOR__

#include "CAUGuiConfig.h"

#include "CAUGui.h"

class CAUGuiSelector;


/** \brief CAUGuiSelector action callback

	An optional C callback function that gets called when a CAUGuiSelector changes value on user action
	\param value the value of the CarbonControl (not the Parameter!) 
	\param from the actual CAUGuiSelector
	\param userData an optional pointer to a structure / object
*/

typedef void (*selectorUserProcedure)( UInt32 value, CAUGuiSelector* from, void* userData );

/*! \brief a useful helper widget, i.e. to select different layers for a CAUGuiLayeredPane

	There's currently no CAAUParameter attached.
	
	It works much like CAUGuiButton in kRadioMode, but with all states combined in a single widget.
	
	Buttons require AU Parameters, while CAUGuiSelector does not.
	
	The states are evenly spaced inside foreBounds along the orientation axis

	You typically install a callback that takes action on changes. For changing layers, you'd usually pass a pointer to a CAUGuiLayeredPane as userdata and the callback simply sets the layer to the chosen value.

	In future, it might happen that this class will be extended to deal with indexed AU parameters as well.

*/

class CAUGuiSelector : public CAUGuiCtrl
{
public:

	/** \brief constructor for a CAUGuiSelector control

		\param theChief instance of controlling CAUGuiMan
		\param theWhere Control Bounds in pane-relative coordinates
		\param theRange number of different states
		\param theOrientation 0 = vertical, 1 = horizontal
		\param theForeGround foreground CAUGuiGraphic, usually a multiframe CAUGuiGraphic
		\param theBackground background CAUGuiGraphic (can be NULL)
		
		if the foreground graphics is multiframe, it maps the parameter's values accordingly and shows the correct subframe
		
	*/

	CAUGuiSelector (		CAUGuiMan*			theChief,
							eRect*				theWhere,
							UInt32				theRange, 
							UInt32				theOrientation,
							CAUGuiGraphic*		theForeGround, 
							CAUGuiGraphic*		theBackground);
							
	//! destructor
	virtual ~CAUGuiSelector ();

	virtual void draw(CGContextRef context, UInt32 portHeight);
	virtual void mouseDown(Point *P, bool, bool);
	virtual void mouseTrack(Point *P, bool, bool);
	virtual void mouseUp(Point *P, bool, bool);
	
	//! attach a selectorUserProcedure with userdata to the selector
	virtual void setUserProcedure ( selectorUserProcedure theProc, void* theUserData );
		
private:

	UInt32				orientation;
	CAUGuiGraphic*		ForeGround;
	CAUGuiGraphic*		BackGround;
	
	selectorUserProcedure	userProcedure;
	void*				userData;
	
	
};








#endif