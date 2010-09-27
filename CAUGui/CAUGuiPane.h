/*
 *  CAUGuiPane.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 27 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

/**********************************************************************************

	usage:
	
**********************************************************************************/

#ifndef __CAUGUI_PANE__
#define __CAUGUI_PANE__

#include "CAUGuiConfig.h"

#include "CAUGui.h"

/*! \brief CAUGuiPane represents a Container into which Controls can be embedded

	The basic function of a Pane is to provide a background image.
	
	Nesting Controls happens in Pane-relative coordinate space to make GUI layout more comfortable.
	
	Technically it is a hack, because they embed two 1x1 pixel sized CarbonControls in mCarbonPane. Since AUCarbonView doesn't provide means to embed Controls in other Controls than mCarbonPane and I didn't want to alter Apple's base classes (or derive/overwrite any of them), I use this hack and it works good so far. So, Control objects nested in Panes are not physically nested on the CarbonControl side of things.
	

*/

class CAUGuiPane : public CAUGuiCtrl
{
public:

	//! constructor
	CAUGuiPane ( CAUGuiMan*, eRect*, CAUGuiGraphic* );
	
	//! destructor
	virtual ~CAUGuiPane ();

	//! traversing its "foreign controls" and nested children
	virtual bool	isControlRef ( ControlRef theControl );
	
	//! getting the requested Control object
	virtual CAUGuiCtrl*	getChild(ControlRef theControl);
	
	//! true, 2 1x1 pixel CarbonControls
	virtual bool	providesForeignControls () { return true; }
	
	//! setting them up
	virtual void	initForeignControls (ControlDefSpec &ControlSpec);

	virtual void draw(CGContextRef context, UInt32 portHeight);
	virtual void mouseDown(Point *P, bool, bool) {};
	virtual void mouseTrack(Point *P, bool, bool) {};
	virtual void mouseUp(Point *P, bool, bool) {};
	
protected:
		
	CAUGuiGraphic*		BackGround;
	ControlRef			upperLeft;
	ControlRef			lowerRight;
	
	
};

#define CAUGUI_PANE_MAX_GROUPS 16

/*! \brief Panes that can dynamically change

	First of all, Layeredpanes work exactly like a CAUGuiPane, but they provide means to dynamically change the GUI
	
	Then, you can also embed controls upon layers. These layers can be switched.
	
	All controls that are added the Pane-way (no layer specified) are available in all layers.
	
	Controls added to a Layer are available (visible) only when that layer is active
	
	Layers can have their own (optional) Background Graphics
	
	<HR>
	
	Furthermore, there is a second mechanism that allows for altering the displayed content during runtime: Groups
	
	Groups work much like layers but don't have their own background. Instead, each group can be switched active (visible) or not, independently from the current layer chosen. Multiple Groups can be visible at once
	
	Groups utilize objects of class CAUGuiLabel to display graphics conditionally, for instance when naming of parameters change under certain circumstances. For convenience, you don't have to create CAUGuiLabels. You just pass the CAUGuiGraphic and the Pane automatically generates a label from it.
	
	<HR>
	
	The common way to handle switching of Layers and Groups is to install callback functions to CAUGuiButton, CAUGuiSelector or CAUGuiDisplay 


*/

class CAUGuiLayeredPane : public CAUGuiPane
{
public:

	//! constructor
	CAUGuiLayeredPane ( CAUGuiMan*, eRect*, CAUGuiGraphic* );
	
	//! destructor
	virtual ~CAUGuiLayeredPane ();

	virtual bool	isControlRef ( ControlRef theControl );
	virtual CAUGuiCtrl*	getChild(ControlRef theControl);
	
	virtual void 	draw(CGContextRef context, UInt32 portHeight);
	virtual void 	mouseDown(Point *P, bool, bool) {};
	virtual void 	mouseTrack(Point *P, bool, bool) {};
	virtual void 	mouseUp(Point *P, bool, bool) {};
	
	virtual void	idle();
	
	virtual void	clip ( bool drawing );
	
	//! setting the current layer
	virtual void 	setLayer ( int l );
	
	//! showing/hiding a group
	virtual void	showGroup ( int l, bool visible );
	
	//! adding a Control to a specified layer
	virtual void 	addCtrl( CAUGuiCtrl* theCtrl, int theLayer );
	
	//! adding a Control to a specified group
	virtual void 	addCtrlToGroup( CAUGuiCtrl* theCtrl, int theGroup );
	
	//! adding a always-seen control
	virtual void 	addCtrl( CAUGuiCtrl* theCtrl ) { CAUGuiPane::addCtrl( theCtrl ); }
	virtual void 	setOffset ( SInt32 x, SInt32 y );
	
	//! adding an optional background image to a layer
	virtual void 	addBackground( CAUGuiGraphic* theBG, int theLayer );
	
	//! adding a CAUGuiGraphic to a group at a specified coordinate
	virtual void	placeGroupLabel ( CAUGuiGraphic* label, int group, int x, int y );
    
    int             getLayer() { return currentLayer; }
	
protected:
		
	CAUGuiGraphic*		layerBackGround[ CAUGUI_PANE_MAX_LAYERS ];
	CAUGuiCtrl*			Layered[ CAUGUI_PANE_MAX_LAYERS ];
	CAUGuiCtrl*			Grouped[ CAUGUI_PANE_MAX_GROUPS ];
	CAUGuiLabel*		GroupLabels[ CAUGUI_PANE_MAX_GROUPS ];
	int					GroupState[ CAUGUI_PANE_MAX_GROUPS ];
	int					currentLayer;
	
	
};


#endif