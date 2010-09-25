/*
 *  CAUGuiTools.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Mon Oct 28 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CAUGUI_TOOLS__
#define __CAUGUI_TOOLS__

#define kCAUGuiSchemeMaxPatterns 16


#include <Carbon/Carbon.h>

#include "CAUGuiConfig.h"

/** \brief Type identifier for CAUGuiItems

	These identifiers can be used to find out which type of Control or Item
	you have when you pass them by its superclass.
	
	This may be useful if you have to cast the type to a specific class when
	there are more than one possibilities

*/


typedef enum
{
	kCAUGui_noType		=	0,
	kCAUGui_PNG			=	'cPNG',
	kCAUGui_RSRC		=	'cRSC',
	kCAUGui_Graphic		=	'cAgr',
	kCAUGui_SpinImage	=	'cAsi',
	kCAUGui_Pane		=	'cApn',
	kCAUGui_LayeredPane	=	'cAlp',
	kCAUGui_Knob		=	'cAkb',
	kCAUGui_Slider		=	'cAsr',
	kCAUGui_Selector	=	'cAst',
	kCAUGui_Display		=	'cAdy',
	kCAUGui_Meter		=	'cAmt',
	kCAUGui_Label		=	'cAlb'


} CAUGuiType;

/************************************

	eRect alignments
	
	a bit crappy in naming :-(
	
	may need to be altered :-)

************************************/

/** \brief Convenience alignment types for eRects

	These can be used to align an eRect to another or itself

*/

typedef enum
{
	kNone		= 0,
	
	kVOutside	= 1,
	
	kNorth		= 2,
	kSouth		= 4,
	kMiddle		= 6,
	
	kHOutside	= 8,
	
	kWest		= 16,
	kEast		= 32,
	kCenter		= 48,
	
	
	k_NW		= kNorth	+	kWest,
	k_NC		= kNorth	+	kCenter,
	k_NE		= kNorth	+	kEast,
	k_ME		= kMiddle	+ 	kEast,
	k_SE		= kSouth	+	kEast,
	k_SC		= kSouth	+	kCenter,
	k_SW		= kSouth	+	kWest,
	k_MW		= kMiddle	+	kWest,
	
	// Vertically outside
	
	k_oNW		= kNorth	+	kWest	+ kVOutside,
	k_oNC		= kNorth	+	kCenter	+ kVOutside,
	k_oNE		= kNorth	+	kEast	+ kVOutside,
	k_oSE		= kSouth	+	kEast	+ kVOutside,
	k_oSC		= kSouth	+	kCenter	+ kVOutside,
	k_oSW		= kSouth	+	kWest	+ kVOutside,

	// Horizontally outside

	k_NWo		= kNorth	+	kWest	+ kHOutside,
	k_NEo		= kNorth	+	kEast	+ kHOutside,
	k_MEo		= kMiddle	+ 	kEast	+ kHOutside,
	k_SEo		= kSouth	+	kEast	+ kHOutside,
	k_SWo		= kSouth	+	kWest	+ kHOutside,
	k_MWo		= kMiddle	+	kWest	+ kHOutside,

	// Completely outside (with helpful redundancy)

	k_oNWo		= kNorth	+	kWest	+ kVOutside	+ kVOutside,
	k_oNCo		= kNorth	+	kCenter	+ kVOutside	+ kVOutside,
	k_oNEo		= kNorth	+	kEast	+ kVOutside	+ kVOutside,
	k_oMEo		= kMiddle	+ 	kEast	+ kVOutside	+ kVOutside,
	k_oSEo		= kSouth	+	kEast	+ kVOutside	+ kVOutside,
	k_oSCo		= kSouth	+	kCenter	+ kVOutside	+ kVOutside,
	k_oSWo		= kSouth	+	kWest	+ kVOutside	+ kVOutside,
	k_oMWo		= kMiddle	+	kWest	+ kVOutside	+ kVOutside,
	
	// Convenience matrix naming scheme
	
	k_v1		= kNorth + kVOutside,	/*!< above */
	k_v2		= kNorth,				/*!< fit to top (same y) */
	k_v3		= kMiddle,				/*!< middle */
	k_v4		= kSouth,				/*!< fit to bottom */
	k_v5		= kSouth + kVOutside,	/*!< below */

	k_h1		= kWest + kHOutside,	/*!< aligned left next to the other */
	k_h2		= kWest,				/*!< fit to left (same x) */
	k_h3		= kCenter,				/*!< centered */
	k_h4		= kEast,				/*!< aligned left from right border */
	k_h5		= kEast + kHOutside 	/*!< aligned right next to the other */


} CAUGuiAlign;



/***********************************************************************

	eRect
	
	a Rect class with better properties for layout issues...
	
	(sorry for the naming, but I just couldn't resist :-)

***********************************************************************/

/** \brief Rect class

	an x, y, w, h dimension/position object

*/

class eRect
{
public:
	
	//! construct empty eRect
	eRect () { x = y = w = h = 0; }
	
	/** \brief construct eRect
		\param a vertical position (x)
		\param b horizontal position (y)
		\param c width (w)
		\param d height (h)
	
	*/

	eRect ( SInt32 a, SInt32 b, SInt32 c, SInt32 d ) { x = a; y = b; w = c; h = d; }
	
	//! move by
	void offset ( SInt32 a, SInt32 b ) { x += a; y += b; }
	
	//! move to
	void at ( SInt32 a, SInt32 b ) { x = a; y = b; }
	
	//! set height and width
	void size ( SInt32 c, SInt32 d ) { w = c; h = d; }
	
	//! expand border by a, b pixels (width, height respectively)
	void grow ( SInt32 a, SInt32 b ) { x -= a; y -= b; w += a*2; h += b*2; }
	
	//! expand border by a, b, c, d pixels (horizontal position, vertical position, width, height respectively)
	void grow ( SInt32 a, SInt32 b, SInt32 c, SInt32 d) { x -= a; y -= b; w += a + c; h += b + d; }
	
	//! align to self 
	void align ( CAUGuiAlign align );
	
	//! align to another erect
	void align ( eRect* r, CAUGuiAlign align );
	
	//! clone values from another erect 
	void set ( eRect* from ) { x = from->x; y = from->y; w = from->w; h = from->h; }
	
	//! set x, y, w, h directly
	void set ( SInt32 X, SInt32 Y, SInt32 W, SInt32 H ) { x = X; y = Y; w = W; h = H; }
	
	//! transform eRect to common Carbon Rect
	void to ( Rect* r ) { r->left = x; r->top = y; r->right = x + w; r->bottom = y + h; }
	
	//! transform eRect to CoreGraphics CGRect, needs Window height
	void to ( CGRect* r, UInt32 winH )
	{
		r->origin.x = x;
		r->origin.y = winH - y - h;
		r->size.width = w;
		r->size.height = h;
	}

	SInt32	x;	/*!< horizontal position */
	SInt32	y;	/*!< vertical position */
	SInt32	w;	/*!< width */
	SInt32	h;	/*!< height */

};


/***********************************************************************

	CAUGuiItem
	
	base class for Items contained in linked lists

***********************************************************************/

/** \brief Convenience class, mainly a linked list and base class of any object in CAUGui

	every class of Graphics, Controls etc. is derived from CAUGuiItem
	
	They are basically linked lists for some convenience mechanisms:
	
	- CAUGuiItems don't have to become class members of your GUI class
	- They are layed out to be maintained by instantiating objects
	- A single delete() destroys the whole list
	
	you normally don't have to deal with this class, you usually derive from CAUGuiControl or CAUGuiGraphic

*/


class CAUGuiItem
{
public:

	CAUGuiItem()
	{
		this->type 	= kCAUGui_noType;
		this->id 	= 0;
		this->prev 	= NULL;
		this->next 	= NULL;
		
		//printf ("ctor CAUGuiItem\n");
	}
	
	//! destructor WARNING: kills all following Items in the CAUGuiItem linked list!!!
	virtual ~CAUGuiItem()
	{
		//printf ("dtor CAUGuiItem %d\n", (int)id );
		if ( this->next != NULL ) delete ( next );
		
		//printf ("dtor CAUGuiItem\n");
	}
	
	CAUGuiItem* getByID ( UInt32 theID )
	{
		if ( id == theID ) return (this);
		
		if ( next == NULL ) return (NULL);
		else return (next->getByID ( theID ));
	}
	
	/** \brief adding a new Item to the list

		the instantiating object usually cares only about the first item in the list
		
		to append another item, it has just to be passed to the first item and will automatically be inserted at the end of the list, cool, he?

	*/

	
	void append ( CAUGuiItem* nextItem )
	{
		if ( next == NULL )
		{
			next = nextItem;
			nextItem->setPrev( this );
		}
		else
			next->append( nextItem );
	}
	
	CAUGuiItem* 	getNext () 						{ return ( next ); }
	CAUGuiItem* 	getPrev () 						{ return ( prev ); }
	void			setPrev ( CAUGuiItem* thePrev ) { prev = thePrev; }
	UInt32			getID() 						{ return id; }
	void			setID( UInt32 theID ) 			{ id = theID; }
	CAUGuiType		getType() 						{ return type; }
	void			setType( CAUGuiType	theType) 	{ type = theType; }
    
private:
	CAUGuiType			type;
	UInt32 				id;
	CAUGuiItem* 		prev;
	CAUGuiItem*			next;

};

/***********************************************************************

	CAUGuiImage
	
	class for loading and containing images

***********************************************************************/

/** \brief Class for maintaining static references to CGImages [UNIMPLEMENTED]

	currently, every instance of a plugin loads all images again
	
	I wanted to make the images static and shared across plugin instances, but unfortunately never got that baked.

*/

class CAUGuiImage : public CAUGuiItem
{
public:	
	CAUGuiImage ( char* pngFileName );
	
	virtual ~CAUGuiImage ()
	{
		if ( this->Image != NULL )
			CGImageRelease( this->Image );
		this->Image = NULL;
		
		//printf ("dtor CAUGuiGraphic\n");
	}

	virtual CGImageRef		getImage() { return Image; }
	
	virtual bool			isPNG ( char* pngFileName );
	virtual bool			isRSRC ( int res ) { return ( res == resourceID );}
	

private:
	
	void			loadImagePNG ( char* pngFileName );

	char			fileName[ 64 ];
	int				resourceID;

	CGImageRef		Image;

};



/***********************************************************************

	CAUGuiGraphic
	
	class for loading and containing images

***********************************************************************/

/** \brief Graphics, not just Images

	CAUGuiGraphic is the base class of all types of graphical objects in CAUGui
	
	The concept of hiding visual representation from actual control objects offers some possibilities:
	
	- Graphics can be images drawn in various ways (rotated, scaled, stacked animation frames, cropped)

	- Graphics can use Quartz to render lines, polygons, text
*/

class CAUGuiGraphic : public CAUGuiItem
{
public:	
	
	//! trivial graphic made up from .png resource file
	CAUGuiGraphic ( char* pngFileName );
	
	//! stacked image (animation) made up from .png resource file
	CAUGuiGraphic ( char* pngFileName, UInt32 numFrames );
	
	//! destructor WARNING: kills all following Items in the CAUGuiItem linked list!!!
	virtual ~CAUGuiGraphic ()
	{
		if ( this->Image != NULL )
			CGImageRelease( this->Image );
		this->Image = NULL;
		
		//printf ("dtor CAUGuiGraphic\n");
	}

	// passive API (for controls that want to draw images by themselves)


	//! frame based animated Graphics should implement this
	virtual bool			isAnim() { return ( Frames == 1 ? false : true ); }

	//! for relicts in controls that want to draw'em themselves
	virtual CGImageRef		getImage() { return Image; }
	
	//! number of frames
	virtual SInt32			getFrames () { return (SInt32)Frames; }
	
	//! returns Width
	virtual SInt32			getWidth();
	
	//! returns Height
	virtual SInt32			getHeight();
	
	// active API (for more than images...)
	
	/** \brief letting Graphics draw themselves
		\param context is the CGContextRef to render into
		\param portHeight the height of the Window, needed for "upside down" Quartz drawing
		\param rect the eRect where to draw into, does usually scale the image 
		\param value normalized between 0.f and 1.f, needed i.e. from frame based animation 
	
	*/
	virtual void			draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value );

private:
	
	void			loadImagePNG ( char* pngFileName );

	CGImageRef		Image;
	UInt32			Frames;
	SInt32			FrameHeight;

};


/***********************************************************************

	CAUGuiLabel
	
	class for simple images inside panes

***********************************************************************/

/** \brief Labels, made up from Graphics
		
		CAUGuiLabels are currently only used in LayeredPanes
		
		They produce overhead that sometimes is neccessary, for instance when a parameter influences the meaning of another parameter. In those cases, LayeredPanes don't need to change their whole background image but can display little, changing graphics as labels instead
		
		See CAUGuiLayeredPane for details
	
	*/

class CAUGuiLabel : public CAUGuiItem
{
public:	
	CAUGuiLabel ( CAUGuiGraphic* theGraphic, eRect* theBounds );
	
	virtual ~CAUGuiLabel ()
	{
		this->Graphic = NULL;
		
		//printf ("dtor CAUGuiGraphic\n");
	}
	
	virtual SInt32			getWidth() {return Graphic->getWidth();}
	
	virtual SInt32			getHeight() {return Graphic->getHeight();}
		
	virtual void			draw( CGContextRef context, UInt32 portHeight, eRect* rect, float value );

private:
	
	CAUGuiGraphic* 	Graphic;
	
	eRect Bounds;
	
};





/***********************************************************************

	CAUGuiScheme
	
	class for automatically layouting controls

***********************************************************************/

/** \brief layout helper class [NOT WELL TESTED YET]
		
		CAUGuiSchemes hold an array of eRects and alignments which they return subsequently
		
		For layouts, where you have repeating patterns and don't want to scribble down eRect offset orgies, you can set up the scheme once and pass myScheme.get() as an eRect to your Control's constructors...
		
		It's an eRect sequencer...
	
	*/

class CAUGuiScheme
{
public:
	CAUGuiScheme ();

	void			add( eRect* r, CAUGuiAlign align );

	void 			reset();
	
	void			at ( SInt32 x, SInt32 y );
	
	eRect*			get();
	eRect*			getS( SInt32 w, SInt32 h );


private:
	eRect			resulting;
	eRect			rects[ kCAUGuiSchemeMaxPatterns ];
	CAUGuiAlign		aligns[ kCAUGuiSchemeMaxPatterns ];
	UInt32			patternLength;
	UInt32			currentIteration;
	UInt32			currentPattern;
	bool			notAligned;
	SInt32			X;
	SInt32			Y;
	SInt32			startX;
	SInt32			startY;
	SInt32			offsetX;
	SInt32			offsetY;
	
};



#endif
