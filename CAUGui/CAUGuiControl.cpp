/*
 *  CAUGuiControl.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Mon Oct 28 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiControl.h"

CAUGuiCtrl::CAUGuiCtrl (
					CAUGuiMan* theChief,
					CAAUParameter &theAuvp,
					eRect* theWhere,
					CAUGuiResolution theRes
				)
{	

	Chief = theChief;
	where.set( theWhere );
	vizArea.set( theWhere );
	
	setDefaults();
	
	auvp = theAuvp;
	
	AUVPattached = true;
		
	where.set( theWhere );
	vizArea.set( theWhere );
	
	Resolution = theRes;
	
	Range = theAuvp.ParamInfo().maxValue - theAuvp.ParamInfo().minValue;

	//printf ("ctor CAUGuiCtrl\n");
}

CAUGuiCtrl::CAUGuiCtrl (
					CAUGuiMan* theChief,
					eRect* theWhere,
					UInt32 theRange
				)
{
	Chief = theChief;
	where.set( theWhere );
	vizArea.set( theWhere );

	setDefaults();
	
	
	Resolution = kCAUGui_res_1;
	
	auvp = CAAUParameter(); // a NULLy auvp
	AUVPattached = false;
	
	Range = theRange;

	//printf ("ctor CAUGuiCtrl2\n");
}

	
CAUGuiCtrl::CAUGuiCtrl (
					CAUGuiMan* theChief,
					eRect* theWhere
				)
{
	Chief = theChief;
	where.set( theWhere );
	vizArea.set( theWhere );
	
	setDefaults();
		
	Resolution = kCAUGui_res_0;
	
	auvp = CAAUParameter(); // a NULLy auvp
	AUVPattached = false;
	
	Range = 0;
	
	//printf ("ctor CAUGuiCtrl2\n");
}
	
CAUGuiCtrl::~CAUGuiCtrl()
{
	if (carbonControl) DisposeControl(carbonControl);
	if ( children != NULL ) delete children;
	//printf ("dtor CAUGuiCtrl\n");
}

void CAUGuiCtrl::setDefaults()
{
	
	
	Daddy = NULL;
	
	
	
	id = 0;
	
	carbonControl = NULL;
	
	opaque = true;
	
	children = NULL;
	
	lastUpdatedValue = 0;
	
	tolerance = 0;
	
	becameVisible = false;
	
	wantsTrackingFlag = true;
	
	col_red = CAUGUI_FONT_RED;
	col_green = CAUGUI_FONT_GREEN;
	col_blue = CAUGUI_FONT_BLUE;
	col_alpha = CAUGUI_FONT_ALPHA;
	font_size = CAUGUI_FONT_SIZE;
	textalign = CAUGUI_DEFAULT_FONT_ALIGN;
	strcpy ( font_name, CAUGUI_FONT );



}


void CAUGuiCtrl::addCtrl( CAUGuiCtrl* theCtrl )
{

	//printf ( "Huh, adding...\n" );

	if ( children == NULL )
			children = theCtrl;
		else
			children->append( theCtrl );
			
	theCtrl->setDaddy ( this );
	theCtrl->setOffset ( where.x, where.y );
	
	Chief->addCtrl( theCtrl );
}

void CAUGuiCtrl::setOffset ( SInt32 x, SInt32 y )
{
	this->where.offset ( x, y );
	this->vizArea.offset ( x, y );

	if ( this->children != NULL )
	{
		CAUGuiCtrl* current = children;
		while ( current != NULL )
		{
			current->setOffset ( x, y );
			current = (CAUGuiCtrl*)(current->getNext());
		}
	}
}
	
void CAUGuiCtrl::clip ( bool drawing )
{

/*	
	if ( drawing )
		printf ( "drawing id%d type%d\n", (int)getID(), (int) getType() );
	else
		printf ( "clipping id%d type%d\n", (int)getID(), (int) getType() );
*/

	if ( opaque )
	{
		Rect r;
		where.to ( &r );
		FrameRect( &r );
		//printf ( "clipping opaque %d %d %d %d\n", r.left, r.top, r.right, r.bottom );
	}
	
	if ( !opaque || drawing )
	{		
		CAUGuiCtrl* current = children;
		while( current != NULL )
		{
			current->clip( false );
			current = (CAUGuiCtrl*)current->getNext();
		}
	}
}

void CAUGuiCtrl::setVisible ( bool viz )
{
	if ( carbonControl != NULL )
	{
		if ( viz )
		{
			ShowControl ( carbonControl );
			becameVisible = true;
			Draw1Control ( carbonControl );
			
		}
		else
			HideControl ( carbonControl );
	}
	
	if ( children != NULL )
	{
		CAUGuiCtrl* current = children;
		while( current != NULL )
		{
			current->setVisible ( viz );
			current = (CAUGuiCtrl*)current->getNext();
		}
	}
}

void CAUGuiCtrl::setDefault()
{
	if ( isAUVPattached() )
	{
		float fDefault = auvp.ParamInfo().defaultValue - auvp.ParamInfo().minValue;
	
		fDefault /= Range;
	
		ControlRef carbonControl = getCarbonControl();
	
		UInt32 max = GetControl32BitMaximum(carbonControl);
		
		UInt32 defaultValue = (UInt32)((float)max * fDefault);
		
		SetControl32BitValue ( carbonControl, defaultValue );
	}

}


void CAUGuiCtrl::idle()
{
	if ( children != NULL )
		children->idle();
	if ( getNext() != NULL )
		((CAUGuiCtrl*)getNext())->idle();
}
	
bool CAUGuiCtrl::isControlRef ( ControlRef theControl )
{
	if (carbonControl == theControl) return true;
	if ( children != NULL )
	{
		CAUGuiCtrl* current = children;
		while( current != NULL )
		{
			if ( current->isControlRef ( theControl ) ) return true;
			current = (CAUGuiCtrl*)current->getNext();
		}
	}
	return false;
}

CAUGuiCtrl* CAUGuiCtrl::getChild(ControlRef theControl)
{
	if ( carbonControl == theControl ) return this;
	if ( children != NULL )
	{
		CAUGuiCtrl* current = children;
		while( current != NULL )
		{
			if ( current->isControlRef ( theControl ) )
				return current->getChild( theControl );
		
			current = (CAUGuiCtrl*)current->getNext();
		}
	}
	return NULL;
}

void CAUGuiCtrl::setForeBounds ( SInt32 x, SInt32 y, SInt32 w, SInt32 h )
{
	vizArea.set ( x, y, w, h );

}

void CAUGuiCtrl::shrinkForeBounds ( SInt32 x, SInt32 y, SInt32 w, SInt32 h )
{
	vizArea.grow ( -x, -y, -w, -h );
}

bool CAUGuiCtrl::mustUpdate ()
{
	
	if ( carbonControl == NULL ) return true; // probably no User interaction
	
	SInt32 v = (SInt32 ) GetControl32BitValue( carbonControl );

	SInt32 diff = v - lastUpdatedValue;
	
	if ( Chief->isRelaxed() == false || becameVisible == true )
	{
		becameVisible = false;
		
		//printf ( "CAUGuiCtrl::mustUpdate () no relax!\n" );
		
		lastUpdatedValue = diff;
	
		return true;
	}
	
	
	if ( diff <= -tolerance || diff >= tolerance || Chief->isRelaxed() == false )
	{
		lastUpdatedValue = diff;
	
		return true;
	
	}
	
	printf ( "CAUGuiCtrl::mustUpdate () draw prevented!\n" );
	
	return false;

}


pascal OSStatus CAUGuiControlHandler ( EventHandlerCallRef myHandler, EventRef theEvent, void* userData )
{
    #pragma unused (myHandler )
        
    OSStatus result = eventNotHandledErr;
    UInt32 whatHappened;
    
    GrafPtr thePort;
	CGrafPtr windowPort;
    WindowRef theWindow;
    ControlRef theControl;
    
	Rect controlBounds;
    Rect portBounds;
	Rect globalBounds; // Window Content Region...
    
    CGContextRef context;
   
    
    Point P;
	int doDaTrack = 1;
	UInt32 modifiers;
	bool with_option, with_shift;
         
    whatHappened = GetEventKind ( theEvent );
    
    
    GetEventParameter ( theEvent, kEventParamDirectObject, typeControlRef, NULL, sizeof( ControlRef ), NULL, &theControl );
        
    GetControlBounds ( theControl, &controlBounds );
    
	theWindow = GetControlOwner ( theControl );
		
	GetWindowBounds( theWindow, kWindowGlobalPortRgn, &globalBounds);
	
	CAUGuiMan* theChief = (CAUGuiMan*)userData;
	CAUGuiCtrl* theCtrl = NULL;
		
	if ( theChief != NULL )
	{
		theCtrl = theChief->getCtrlByControlRef ( theControl );	
	}
		
	if ( theCtrl != NULL )
	{
		//printf ("Control exists!\n");
	
		switch ( whatHappened )
		{
			case kEventControlApplyBackground:
				break; 
			case kEventControlDraw:
        
				if ( theCtrl->mustUpdate () == false )
				{
					result = noErr;
					break;

				}
				
		
				GetPort ( &thePort );
				windowPort = GetWindowPort(  theWindow );
				SetPort(windowPort);
				GetPortBounds ( windowPort, &portBounds );
				
				// C L I P P I N G
				
				RgnHandle clipRgn;
				
				clipRgn = NewRgn();
				
				OpenRgn();
			
				//FrameRect( &portBounds );
				
				theCtrl->clip( true );
				
				CloseRgn( clipRgn );
				
				SetClip( clipRgn );
			
				clipRgn = GetPortClipRegion( windowPort, clipRgn );
				
				// D R A W I N G
            
				QDBeginCGContext ( windowPort, &context );            
			
				ClipCGContextToRegion( context, &portBounds, clipRgn );
				
				SyncCGContextOriginWithPort( context, windowPort );
				
				CGContextSaveGState( context );
            				
				theCtrl->draw( context, portBounds.bottom );
								
				CGContextRestoreGState( context );
				
				CGContextSynchronize ( context );
				
				QDEndCGContext ( windowPort, &context );
				
				DisposeRgn ( clipRgn );
				
				SetPort ( thePort );
                                    
				result = noErr;
				break;
            
			case kEventControlHitTest:
            
				result = noErr;
				break;
    
			case kEventControlTrack:
			case kEventControlClick:
				
					
				modifiers = GetCurrentEventKeyModifiers();
				if ( modifiers & controlKey )
				{
				
					GetGlobalMouse ( &P );
								
					printf ("with control\n");
					theCtrl->mouseRight( &P ); 
				
					result = noErr;
					break;
				
				}
				
				if ( modifiers & cmdKey )
				{
					theCtrl->setDefault(); 
				
					result = noErr;
					break;
				
				}
				
				
				with_option = modifiers & optionKey ? true : false;
				with_shift = modifiers & shiftKey ? true : false;
			
				//GetMouse ( &P );
				GetGlobalMouse ( &P );
				P.h -= controlBounds.left + globalBounds.left;
				P.v -= controlBounds.top + globalBounds.top;
				
				
				// prevent unnecessary draws
				
				theChief->setRelaxed ( true );
				
				
				theCtrl->mouseDown( &P, with_option, with_shift );
				
				if ( theCtrl->wantsTracking() )
				{
					MouseTrackingResult outResult;
            			
					while ( doDaTrack )
					{
						TrackMouseLocation (NULL, &P, &outResult);
						
						GetGlobalMouse ( &P );
						P.h -= controlBounds.left + globalBounds.left;
						P.v -= controlBounds.top + globalBounds.top;
	
						if ( outResult == kMouseTrackingMouseUp )
						{
							theCtrl->mouseUp( &P, with_option, with_shift );
							doDaTrack = 0;
						}
						else
							theCtrl->mouseTrack( &P, with_option, with_shift );
						
					}
				}
				
				theChief->setRelaxed ( false );
				
				result = noErr;
				break;
		}
    }
	
	return ( result );
}






