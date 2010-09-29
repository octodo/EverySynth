/*
 *  CAUGui.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sat Oct 19 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */


/**********************************************************************************

	usage:
	
		
**********************************************************************************/



#ifndef __COREGRAPHICS_AUDIOUNIT_GUI__
#define __COREGRAPHICS_AUDIOUNIT_GUI__




#include <Carbon/Carbon.h>
#include "AUCarbonViewBase.h"
#include "AUCarbonViewControl.h"

#include "CAUGuiConfig.h"

#ifndef __CAUGUI_TOOLS__
#include "CAUGuiTools.h"
#endif

#ifndef __CAUGUI_CONTROL__
#include "CAUGuiControl.h"
#endif



typedef enum
{
	kCAUGui_res_0	=	0,		/*!< no values */
	kCAUGui_res_1	=	1,		/*!< indexed / integer only */
	kCAUGui_res_10	=	10,		/*!< one decimal, i.e. 33.4 */
	kCAUGui_res_100	=	100,	/*!< two decimals, i.e. 41.22 */
	kCAUGui_res_50	=	50		/*!< fifty steps, i.e. for Cent tuning */
	

} CAUGuiResolution;


/** \brief The main control handler callback

	This function gets called whenever one of our custom CarbonControls get a mouse or draw event.
	
	The userdata field is the CAUGuiMan who then determines which Control object has to take action and passes the reference back to the handler who distributes the work.
			
*/
extern pascal OSStatus CAUGuiControlHandler ( EventHandlerCallRef, EventRef, void*  );

/** \brief The timer callback

	this function gets called by an CarbonRunLoopTimer. It gets passed the CAUGuiMan who then calls each Control object's idle() function subsequently.
			
*/
extern void TimerProc ( EventLoopTimerRef inTimer, void *inUserData );

class CAUGuiCtrl;


/***********************************************************************

	CAUGuiMan
	
	CoreGraphics AudioUnit Graphical User Interface Manager
	-            -    -    -         -    -         ---
	
	the manager class that does all the admin for you

***********************************************************************/

/** \brief CAUGui Manager object

	This object is your basic assistant for everything related to CAUGui
	
	You won't be much in touch with it. Everything you do is having an instance of it in your GUI class. The other thing is, you have to pass it the topmost Panes of your user interface and all your graphics. It then deals with their lifetime and connects control objects to corresponding Carbon APIs and CoreAudio base classes.
	
	 It also cares about idle timers and stuff. 
			
*/

class CAUGuiMan
{
public:
	
	/** \brief construct CAUGui Manager object
	
		in AUCarbonViewBase::CreateUI you pass it this (your GUI class derived form AUCarbonViewBase) and the x, y parameter of CreateUI
	
	*/
	
	CAUGuiMan (AUCarbonViewBase*, UInt32 x, UInt32 y );
	
	/** \brief destroy CAUGui Manager object
	
		destroy the CAUGuiMan in your GUI class's destructor and it cleans up all your GUI objects. If you have instances of Objects in your GUI class that are also passed to CAUGuiMan (either by directly passing it via add methods or indirectly by those of Controls), just set them to NULL.
		
		DON'T DELETE OBJECTS THAT ARE OBSERVED BY CAUGuiMan!!!
	*/

	~CAUGuiMan();
	
	//! post constructor
	void		initialize();

	// Images
	
	//! adding a Graphics object
	void					addImage ( CAUGuiGraphic* theImage );
	
	//! getting a Graphics object from it's ID (see CAUGuiItem)
	CAUGuiGraphic*			getImageByID ( UInt32 theID );

	// Controls
	
	//! adding a Control object
	void					addCtrl( CAUGuiCtrl* theCtrl );
	
	//! getting a Control object from it's ID (see CAUGuiItem)
	CAUGuiCtrl*				getCtrlByID( UInt32 theID );
	
	//! getting a Control object from it's ControlRef (used by event handler)
	CAUGuiCtrl*				getCtrlByControlRef ( ControlRef theControl );
	
	//! getting the linked list (CAUGuiItem) of controls
	CAUGuiCtrl*				getControls (){ return Controls; }
	
	// Relaxed drawing occurs when User adjusts parameters.
	
	//! set this to true if you're in User thread to prevent unnecessary redraws
	void					setRelaxed ( bool r ) { relaxed = r; }
	
	//! how urgent is a redraw?
	bool					isRelaxed ( void ) { return relaxed; }
	
	//! sometimes you have to deal with the AUCarbonViewBase, for instance in C-callbacks
	AUCarbonViewBase* 		getAUCarbonViewBase() { return myAUCarbonViewBase; }
	
	//! set up internal Parameter Scheme (not elegant, direct pointers, but works so far)
	void					requestInternalParameters(); // Fetch internal Parameters from AU
	
	//! get value from an internal Parameter
	float					getInternalParameter ( int index );

	//! the timer based idle function, triggers  those of control objects
	void					idle();

	//! CAUGuiItems ask for unique ids. Not much use yet, but who knows...
	UInt32					requestID ( void );
	
	UInt32					X;
	UInt32					Y;

private:

	UInt32					itemCount;

	CAUGuiGraphic*			Images;
	CAUGuiCtrl*				Controls;
	
	bool					relaxed;
	
	EventLoopTimerRef		timer;
	
	AUCarbonViewBase* 		myAUCarbonViewBase;
	
	
	ControlDefSpec 			ControlSpec;
	ToolboxObjectClassRef	ControlClass;
	char					defString[ 64 ];
	
	
	// Mirrors the internal Parameters of the AudioUnit class
	
	float*					internalParameters;
	

};



/***********************************************************************

	CAUGuiCtrl
	
	base class for all control objects

***********************************************************************/

/** \brief CAUGuiCtrl - The Control objects that define mousing behaviour
	
			
	*/

class CAUGuiCtrl : public CAUGuiItem
{
public:

	// Parameter driven control
	
	/** \brief constructor for an AudioUnit Parameter related Control object
	
		\param theCAUGuiMan the instance of CAUGuiMan that observes everything
		\param theAUVP the Parameter to be controlled
		\param bounds the eRect that declare the region of the control object
		\param resolution defines the fine resolution between integer steps of the parameter value 
			
	*/

	
	CAUGuiCtrl ( CAUGuiMan* theCAUGuiMan, CAAUParameter& theAUVP, eRect* bounds, CAUGuiResolution resolution );
	
	// Control with no actual parameter attached
	
	/** \brief constructor for a CarbonControl object that has general purpose
	
		\param theCAUGuiMan the instance of CAUGuiMan that observes everything
		\param bounds the eRect that declare the region of the control object
		\param Range how many values have to be represented 
			
	*/
	CAUGuiCtrl ( CAUGuiMan* theCAUGuiMan, eRect* bounds, UInt32 Range );
	
	/** \brief constructor for a "virtual" Control object doesn't even has to have a CarbonControl
	
		\param theCAUGuiMan the instance of CAUGuiMan that observes everything
		\param bounds the eRect that declare the region of the control object
		
		the Panes are actually such Controls. They provide their own 1x1 Pixel CarbonControls in order to work properly, because the current status of Apple's base classes doesn't allow for true Control inheritance and I didn't want to change their code. Future revisions will probably work different and more elegant.
			
	*/
	CAUGuiCtrl ( CAUGuiMan* theCAUGuiMan, eRect* bounds );
	
	//! destructor WARNING! - Kills all subsequent and inherited Control objects (linked list behaviour of CAUGuiItem...) 
	virtual ~CAUGuiCtrl();
	
	//! sets default values, called from constructor
	void setDefaults(); 
		
	// ControlRefs will be implemented by Manager Class
	
	//! CAUGuiMan will allocate the CarbonControl and pass it here
	virtual void		setCarbonControl(ControlRef	theCarbonControl) {carbonControl = theCarbonControl;}
	
	//! get the CarbonControl
	virtual ControlRef	getCarbonControl() { return carbonControl; }
	
	// Called by EventHandler Callback, draws a clipping region if opaque == true
	// otherwise asks embedded CAUGuiControls for their clipping regions
	
	//! draw a clipping region (either to prepare drawing or to not being overdrawn)
	virtual void	clip ( bool drawing );
	
	//! hide / unhide a Control
	virtual void	setVisible ( bool viz );
	
	// The methods you should implement in derived controls
	
	//! receives draws from CAUGuiControlHandler
	virtual void	draw(CGContextRef context, UInt32 portHeight) {};
	
	/** \brief receives Mouse clicks from CAUGuiControlHandler
	
		\param P the point where the mouse is, relative to control bounds
		\param with_option is option key pressed?
		\param with_shift is shift key pressed?
		
		(Currently the status of modifiers is only tested on first click and then maintained over the whole interaction)
			
	*/
	virtual void	mouseDown(Point *P, bool with_option, bool with_shift) {};	// P is relative to controlBounds, so no hassle
	
	//! receives MouseTracking from CAUGuiControlHandler
	virtual void 	mouseTrack(Point *P, bool with_option, bool with_shift) {};
	
	//! receives Mouse ups from CAUGuiControlHandler
	virtual void 	mouseUp(Point *P, bool with_option, bool with_shift) {};
	
	//! derive this to react on mouse clicks /w CTRL for contextual stuff, but you need to install special event handlers. I never managed to get this working
	virtual void 	mouseRight(Point *P) {};
	
	// NEW this is called when you [CMD]-click the control. It resets the value to its default
	
	//! on command click, the default value is restored
	virtual void	setDefault(); 
	
	// Timer driven function.
	// If you derive this, always call this superclass CAUGuiCtrl::idle()
	
	//! the idle timer...
	virtual void	idle();
	
	//! checks if this or an embedded control is inside
	virtual bool	isControlRef ( ControlRef theControl );
	
	// if you do not want parameter attached controls
	
	//! true if it has CarbonControls that havn't been assigned by CAUGuiMan
	virtual bool	providesForeignControls () { return false; }
	
	//! if it has its own CarbonControls, then we initialize them here
	virtual void	initForeignControls (ControlDefSpec &ControlSpec) {};
	
	//! true if the Control is attached to an AudioUnit Parameter
	virtual bool	isAUVPattached() { return AUVPattached; }
	
	// called by CAUGuiMan 
	
	//! only used from panes since true embedding/inheritance doesn't work
	virtual void	embedCAUGuiCtrl() {};
	
	//! only used from panes
	virtual void 	addCtrl( CAUGuiCtrl* theCtrl );
	
	// for the clipping region
	
	//! is the control object a visible object (CarbonControl) or "virtual" (pane)
	bool	getOpaque() { return opaque; }
	
	//! setter for customization by derived objects 
	void	setOpaque(bool o) { opaque = o; }
	
	// accessor functions to be called by derived controls
	
	//! sets the passed eRect to the control's bounds on window coordinates
	virtual void				getBounds ( Rect* r ) { where.to ( r ); }
	
	//! sets the passed eRect to the control's inner bounds on window coordinates
	virtual void				getForeBounds ( Rect* r ) { vizArea.to ( r ); }
	
	//! ...
	virtual CAUGuiResolution	getResolution () { return Resolution; }
	
	//! ...
	virtual CAAUParameter& 		getAUVP() { return auvp; }
	
	//! ...
	virtual float				getRange() { return Range; }
	
	//! returns the control's bounds on window coordinates
	virtual eRect*				getBounds() { return &where; }
	
	//! returns the control's inner bounds on window coordinates
	virtual eRect*				getForeBounds() { return &vizArea; }
	
	//! ...
	virtual CAUGuiMan*			getCAUGuiMan() { return Chief; }
	
	//! ...
	virtual CAUGuiCtrl*			getDaddy() { return Daddy; }
	
	//! ...
	virtual CAUGuiCtrl*			getChild(ControlRef theControl);
	
	//! ...
	virtual void				setDaddy( CAUGuiCtrl* theDaddy ) { this->Daddy = theDaddy; }
	
	//! this is used toconvert from relative coordinate space to window coordinate space while embedding the control
	virtual void				setOffset ( SInt32 x, SInt32 y );
	
	//! sets the Bounds - use ONLY before embedding
	virtual void				setBounds ( eRect* r ) { where.set ( r ); }
	
	//! sets the Bounds of Foreground from erect - use ONLY before embedding
	virtual void				setForeBounds ( eRect* r ) { vizArea.set ( r ); }
	
	//! sets the Bounds of Foreground form x,y,w,h values - use ONLY before embedding
	virtual void				setForeBounds ( SInt32 x, SInt32 y, SInt32 w, SInt32 h );
	
	//! use this if the background is bigger than the foreground
	virtual void				shrinkForeBounds ( SInt32 x, SInt32 y, SInt32 w, SInt32 h );
	
	//! ...
	virtual void				setLastUpdated ( SInt32 v ) { lastUpdatedValue = v; }
	
	//! ...
	virtual SInt32				getLastUpdated (void ) { return lastUpdatedValue; }
	
	//! ...
	virtual void				setTolerance ( SInt32 t ) { tolerance = t; }
	
	//! ...
	virtual bool				mustUpdate ( void );
	
	//! ...
	virtual bool				wantsTracking ( void ) { return wantsTrackingFlag;}
	
	
	//! set Colour for foreground drawing
	virtual void				setColour ( float r, float g, float b, float a )
								{
									col_red = r;
									col_green = g;
									col_blue = b;
									col_alpha = a;
								}
	
	//! set fontface, i.e. "Monaco"
	virtual void				setFont ( char *theFont ) { strcpy ( font_name, theFont ); }

    virtual void				setFontSize ( float theFontSize ) { font_size = theFontSize; }

	//! 0 = left, 1 = center, 2 = right
	virtual void 				setTextAlign ( int a ) { textalign = a; }

	void                        setRange(UInt32 r) { Range = r; SetControl32BitMaximum(carbonControl, r); }
	
	
protected:

	CAUGuiMan*			Chief;
	CAAUParameter 		auvp;
	CAUGuiCtrl*			Daddy;
	CAUGuiCtrl*			children;
	UInt32				id;
	CAUGuiResolution	Resolution;
	float				Range;			//
	
	bool				AUVPattached;
	
	bool				opaque;			// is it a drawing control or just a virtual region?
	
	ControlRef			carbonControl;	// the physical control, if any
	eRect				where; 			// the bounds...
	eRect				vizArea; 		// where the foreground displays
	
	SInt32				lastUpdatedValue;
	SInt32				tolerance;
	bool				becameVisible;	// mustUpdate after setVisible(true)
	
	bool				wantsTrackingFlag;
	
	float				col_red;
	float				col_green;
	float				col_blue;
	float				col_alpha;
	
	float				font_size;
	int					textalign;
	char				font_name[ 32 ];

};







#endif
