/*
 *  CAUGuiMeter.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 27 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */
 
 /**********************************************************************************

	usage:
	
		

**********************************************************************************/

#ifndef __CAUGUI_METER__
#define __CAUGUI_METER__

#include "CAUGuiConfig.h"

#include "CAUGui.h"

/*! \brief CAUGuiMeter - representing private values (internal parameters)

	This class is merely a helper class for general visualizaion purpose like VU meters.
	
	It is currently bound to represent values from normalized float values that are obtained from an Array of floats that's passed from the process class to the GUI via GetProperty() with a PropertyID defined kInternalParameterProperyID (the one that Apple reserves 0-63999 for their own purpose, so this is 64000).

	It could however be extended to use non-propagated AU Parameters as well, as this is the recommended "new way to go" of passing private data from process to GUI. But that would better be handled by Displays where this functionality is already in. One only has to conditionally disable mousing in Displays for that.
	
	If it turns out this class is somewhat useless, it will be deprecated in one of the next updates to CAUGui...
	
	Due to not being bound to AU Parameters, CAUGuiMeters update on timer driven basis when the parameter has changed

*/


class CAUGuiMeter : public CAUGuiCtrl
{
public:

	/** \brief constructor for a meter control

		\param theChief instance of controlling CAUGuiMan
		\param internalIndex index in the Array of internal (private) parameters
		\param theWhere Control Bounds in pane-relative coordinates
		\param theRes CAUGuiResolution
		\param theForeGround foreground CAUGuiGraphic, usually a CAUGuiHandleImage
		\param theBackground background CAUGuiGraphic
		
	*/

	CAUGuiMeter ( CAUGuiMan* theChief, UInt32 internalIndex, eRect* theWhere, CAUGuiResolution theRes, CAUGuiGraphic* theForeGround, CAUGuiGraphic* theBackground );
	
	virtual ~CAUGuiMeter ();

	virtual void draw(CGContextRef context, UInt32 portHeight);
	virtual void mouseDown(Point *P, bool, bool){};
	virtual void mouseTrack(Point *P, bool, bool){};
	virtual void mouseUp(Point *P, bool, bool){};
	
	virtual void idle();
	
private:
		
	CAUGuiGraphic*			ForeGround;
	CAUGuiGraphic*			BackGround;
	
	UInt32					internalParamIndex;
};


#endif