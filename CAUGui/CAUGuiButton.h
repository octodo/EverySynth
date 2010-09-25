/*
 *  CAUGuiButton.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 27 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */
 
 
 

#ifndef __CAUGUI_BUTTON__
#define __CAUGUI_BUTTON__

#include "CAUGuiConfig.h"

#include "CAUGui.h"

typedef enum {

	kPushButton	= 0,	/*!< a button that sets MAX value as long as it is pressed, otherwise MIN */
	kIncButton,			/*!< Increments a Parameter value */
	kDecButton,			/*!< Decrements a Parameter value */
	kRadioButton,		/*!< Sets a Parameter to a defined value */
	kOnOffButton		/*!< Switches between MAX and MIN of a Parameter */

} CAUGuiBottonMode;

class CAUGuiButton;

/** \brief Button Callback

	An optional C callback function that gets called when a button is pressed
	\param value the value of the CarbonControl (not the Parameter!) 
	\param from the actual Button Object
	\param userData an optional pointer to a structure / object
*/

typedef void (*buttonUserProcedure)( UInt32 value, CAUGuiButton* from, void* userData );

/*! \brief CAUGuiButton - PushButtons, OnOffButtons, RadioButtons, Inc/DecButtons
	
	Buttons have several uses.
	
	They can either manipulate Parameters or drive GUI specific functions
	
	Incremental/Decremental buttons can be used to step through values within a Parameter's range
	
	RadioButtons work in groups and interact by Carbon's notification scheme, just like they should do.
	
	For any sort of button, callbacks can be installed to customize behaviour.
	
*/
class CAUGuiButton : public CAUGuiCtrl
{
public:

	/** \brief constructor for a button that controls a Parameter

	\param where the region of the button
	\param fore CAUGuiGraphic for foreground
	\param back CAUGuiGraphic for foreground, can be NULL
	
	If width and/or height of where are set to 0, they'll be set from background graphic or - if that is NULL - from foreground...
	
	
*/
	CAUGuiButton ( CAUGuiMan*, CAAUParameter &, eRect* where, CAUGuiGraphic* fore, CAUGuiGraphic* back, CAUGuiBottonMode );
	
	//! constructor for a button with CarbonControl of range theRange, no Parameter but arbitrary purpose
	CAUGuiButton ( CAUGuiMan*, UInt32 theRange, eRect*, CAUGuiGraphic*, CAUGuiGraphic*, CAUGuiBottonMode );
	
	//! destructor
	virtual ~CAUGuiButton ();

	//! drawing
	virtual void draw(CGContextRef context, UInt32 portHeight);
	
	//! mousing
	virtual void mouseDown(Point *P, bool, bool);
	
	//! mousing
	virtual void mouseTrack(Point *P, bool, bool);
	
	//! mousing
	virtual void mouseUp(Point *P, bool, bool);
	
	//! set callback to be called on mouse clicks
	virtual void setUserProcedure ( buttonUserProcedure theProc, void* theUserData );
	
	//! set a value for radioButtons to build switchable groups
	virtual void setOnValue ( UInt32 v ) { onValue = v; }
	
private:
		
	CAUGuiGraphic*		ForeGround;
	CAUGuiGraphic*		BackGround;
	CAUGuiBottonMode	mode;
	
	buttonUserProcedure	userProcedure;
	void*				userData;
	
	UInt32				onValue;
	bool				hilite;
	float				alpha;
	
};


#endif
