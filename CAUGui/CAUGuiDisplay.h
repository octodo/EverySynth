/*
 *  CAUGuiDisplay.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 27 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */
 


#ifndef __CAUGUI_DISPLAY__
#define __CAUGUI_DISPLAY__

#include "CAUGuiConfig.h"

#include "CAUGui.h"

class CAUGuiDisplay;

/** \brief text generation callback

	An optional C callback function that produces text output for given values
	\param value the value of the CarbonControl (not the Parameter!) 
	\param text a string where the text is written to
	\param userData an optional pointer to a structure / object
*/
typedef void (*displayTextProcedure)( UInt32 value, char* text, void* userData );

/** \brief display action callback

	An optional C callback function that gets called when a display gets moused
	\param value the value of the CarbonControl (not the Parameter!) 
	\param from the actual Display Object
	\param userData an optional pointer to a structure / object
*/
typedef void (*displayUserProcedure)( UInt32 value, CAUGuiDisplay* from, void* userData );

/*! \brief displays and manipulates values, provides PopUpMenue functionality
	
	This is not just for representing values, it also works for manipulating them
	
	on floating point values, it works like a vertical slider. Dragging up/down on the left side adjusts values in integer steps, dragging on the right side adjusts in fine resolution (determined by the attached CAUGuiResolution
	
	on indexed values, it can be used as a PopUpMenue
	
	a special note: If you have an indexed parameter attached and a simple CAUGuiGraphic as foreground, the display will use subsequently added Graphics to display feedback for each integer value.
	
	<TABLE BGCOLOR="#F0F0C0"><TR><TD><TT>
		
	CAUGuiGraphic* back = new CAUGuiGraphic ( "back.png" );
	
	myCAUGuiMan->addImage( back );
	
	CAUGuiGraphic* fore = new CAUGuiGraphic ( "fore1.png" );
	
	myCAUGuiMan->addImage( fore );
	
	CAUGuiGraphic* anotherfore = new CAUGuiGraphic ( "fore2.png" );
	
	myCAUGuiMan->addImage( anotherfore );

	anotherfore = new CAUGuiGraphic ( "fore3.png" );
	
	myCAUGuiMan->addImage( anotherfore );

	
	CAAUParameter myAUVP(mEditAudioUnit, kIndexedParameter3Values, kAudioUnitScope_Global, 0);
	
	eRect* where = new eRect ( 20, 30, 64, 32 );
	
	CAUGuiDisplay* myDisplay = new CAUGuiDisplay(
											myCAUGuiMan,
											myAUVP,
											where,
											kCAUGui_res_1,
											fore,
											back );
	
	myCAUGuiPane->addCtrl( myDisplay );
	</TT></TD></TR></TABLE>
	
	this will attach the three images to the corresponding three values of myAUVP!

	
*/

class CAUGuiDisplay : public CAUGuiCtrl
{
public:

	/** \brief constructor for graphics driven display

		\param theChief instance of controlling CAUGuiMan
		\param theAuvp the CAAUParameter to be controlled
		\param theWhere Control Bounds in pane-relative coordinates
		\param theRes CAUGuiResolution
		\param theForeGround foreground CAUGuiGraphic
		\param theBackground background CAUGuiGraphic (can be NULL)
		
		if the foreground graphics is multiframe, it maps the parameter's values accordingly and shows the correct subframe
		
		if the foreground is a plain CAUGuiGraphic, hence a single image, the display assumes that it represents an indexed parameter and that the graphic is the first in a series of graphics stored in the same linked list (CAUGuiItem). Commonly, there's only one such list for graphics, observed and maintaines by CAUGuiMan 
	*/
	
	CAUGuiDisplay 		(	CAUGuiMan*				theChief,
							CAAUParameter			&theAuvp, 
							eRect*					theWhere,
							CAUGuiResolution		theRes, 
							CAUGuiGraphic*			theForeGround, 
							CAUGuiGraphic*			theBackground);
	
	/** \brief constructor for text driven display

		\param theChief instance of controlling CAUGuiMan
		\param theAuvp the CAAUParameter to be controlled
		\param theWhere Control Bounds in pane-relative coordinates
		\param theRes CAUGuiResolution
		\param aTexter the displayTextProcedure
		\param userDData userdata to be passed to aTexter
		\param theBackground background CAUGuiGraphic (shouldn't be NULL)
		
		if aTexter is NULL, display values will be generated according to the Parameter's metadata, like integer values on indexed parameters, strings if ParameterValueStrings exist, float values with digits according to the attached CAUGuiResolution
	*/
	
	CAUGuiDisplay 		(	CAUGuiMan*				theChief,
							CAAUParameter			&theAuvp, 
							eRect*					theWhere,
							CAUGuiResolution		theRes, 
							displayTextProcedure	aTexter, 
							void*					userDData,
							CAUGuiGraphic*			theBackground);
	//! destructor
	virtual ~CAUGuiDisplay ();

	virtual void draw(CGContextRef context, UInt32 portHeight);
	virtual void mouseDown(Point *P, bool, bool);
	virtual void mouseTrack(Point *P, bool, bool);
	virtual void mouseUp(Point *P, bool, bool);
	
	//! optional displayUserProcedure that gets called on value changes
	virtual void setUserProcedure ( displayUserProcedure theProc, void* theUserData );
	
	virtual void 	mouseRight(Point *P);
	
	/** \brief use on indexed parameters only!
		
		if AUTO_POP_UP_MENUES in CAUGuiConfig is set to zero, you can use this function to individually set up a PopUpMenue for this display. Otherwise it will be automatically established if applicable (indexed parameter, less than 100 or so values)
		
	*/
	virtual void	generatePopUpMenue();
	
private:
		
	CAUGuiGraphic*			ForeGround;
	CAUGuiGraphic*			BackGround;
	displayTextProcedure	theTexter;
	void*					theUserData;
	bool					hasText;
	
	displayUserProcedure	userProcedure;
	void*					userData;
	
	MenuRef 				popUp;

public:
    
    int                     text_offset_x;
    int                     text_offset_y;
    int                     buffer_size;
};


#endif