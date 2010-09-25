/*
 *  CAUGuiDisplay.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 27 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiDisplay.h"

CAUGuiDisplay::CAUGuiDisplay (	CAUGuiMan*			theChief,
							CAAUParameter			&theAuvp, 
							eRect*					theWhere,
							CAUGuiResolution		theRes, 
							CAUGuiGraphic*			theForeGround, 
							CAUGuiGraphic*			theBackground)
						
						:	CAUGuiCtrl ( theChief, theAuvp, theWhere, theRes )
							
{
		
	ForeGround = theForeGround;
	BackGround = theBackground;
	theTexter = NULL;
	theUserData = NULL;
	hasText = false;
	
	userProcedure = NULL;
	userData = NULL;
	
	textalign = CAUGUI_DEFAULT_FONT_ALIGN; 
	
	//printf ("ctor CAUGuiDisplay\n");
	
	setType ( kCAUGui_Display );
	
	popUp = NULL;

}

CAUGuiDisplay::CAUGuiDisplay (	CAUGuiMan*			theChief,
							CAAUParameter			&theAuvp, 
							eRect*					theWhere,
							CAUGuiResolution		theRes, 
							displayTextProcedure	aTexter, 
							void*					userDData,
							CAUGuiGraphic*			theBackground)
						
						:	CAUGuiCtrl ( theChief, theAuvp, theWhere, theRes )
							
{
		
	ForeGround = NULL;
	BackGround = theBackground;
	
	userProcedure = NULL;
	userData = NULL;
	
	theTexter = aTexter;
	
	if ( theTexter != NULL )
		hasText = true;
	else
		hasText = false;
		
	popUp = NULL;
	
	if ( userData != NULL )
		theUserData = userDData;
	else
		theUserData = this;
	
	textalign = CAUGUI_DEFAULT_FONT_ALIGN;
		
	//printf ("ctor CAUGuiDisplay\n");
	
	setType ( kCAUGui_Display );

#if AUTO_POP_UP_MENUES

	if ( (int)auvp.ParamInfo().unit == kAudioUnitParameterUnit_Indexed )
	{
		if ( (int)auvp.ParamInfo().maxValue < 50 )
		{
			generatePopUpMenue ();
		}
	}

#endif

}


CAUGuiDisplay::~CAUGuiDisplay ()
{
	ForeGround = NULL;
	BackGround = NULL;
	theTexter = NULL;
	theUserData = NULL;

	//printf ("dtor CAUGuiDisplay\n");

	if ( popUp != NULL )
		ReleaseMenu( popUp );
}


void CAUGuiDisplay::generatePopUpMenue ()
{

	MenuItemIndex outNewItem;
	
	//ControlRef carbonControl = getCarbonControl();

	//SInt32 max = (SInt32) GetControl32BitMaximum(carbonControl);
	
	CreateNewMenu ( 16383, 0, &popUp );

	SetMenuWidth( popUp, 0 );
	SetMenuHeight( popUp, 0 );
	
	if ( AUVPattached )
	{
		if ( auvp.HasNamedParams() )
		{
			for ( int i = 0; i <= Range; i++ )
			{
				AppendMenuItemTextWithCFString( popUp, auvp.GetParamName( i ), 0, i+1, &outNewItem );		
			}
		}
		else
		{
			char text[24];
		
			if ( theTexter != NULL )
			{
				for ( int i = 0; i <= Range; i++ )
				{
					theTexter( i, text, theUserData );
					AppendMenuItemTextWithCFString( popUp,
						CFStringCreateWithCString(NULL, text, kCFStringEncodingISOLatin1 ),
						0, i+1, &outNewItem );
					
				}
			
			}
		
		}
	}
	wantsTrackingFlag = false;


}



SInt32 last_X;
SInt32 last_Y;


void CAUGuiDisplay::mouseDown(Point *P, bool, bool)
{
	ControlRef carbonControl = getCarbonControl();

	SInt32 val = GetControl32BitValue( carbonControl );

	if ( carbonControl != NULL )
	{
		last_X = P->v;
		last_Y = P->h;
	}
	
	if ( popUp != NULL )
	{
		GetGlobalMouse ( P );
	
		long result;
	
		result = PopUpMenuSelect( popUp,  P->v, P->h, val+1 );
		
		result &= 0xFFFF;
				
		if ( result != 0 )
		{
			SetControl32BitValue ( carbonControl, (UInt32) result - 1 );
			if ( userProcedure != NULL )
			{
				userProcedure ( val, this, userData );
			}

		}
	}

	
}

void CAUGuiDisplay::mouseTrack(Point *P, bool with_option, bool with_shift)
{

	ControlRef carbonControl = getCarbonControl();

	SInt32 max = (SInt32) GetControl32BitMaximum(carbonControl);
	SInt32 val = GetControl32BitValue( carbonControl );

	eRect* bounds = getBounds();
	
	SInt32 precision = 4;
	
	SInt32 subtle = (SInt32) getResolution ();
	
	if ( with_shift ) precision = 10;

	SInt32 dx = last_X - P->v;

	if ( carbonControl != NULL )
	{	
		if ( getResolution () == kCAUGui_res_1 )
		{
			
			precision *= 3;
			
			if ( dx > precision || dx < -precision )
			{
			
				if ( dx < 0 )
				{
					val--;
					if ( val < 0 ) val = max;
				}
				else
				{
					val++;
					if ( val > max ) val = 0;
				}
				
				SetControl32BitValue ( carbonControl, (UInt32) val );
				
				last_X -= dx;
				
			}
		
		
		}
		else
		{
			
			if ( P->h > ( bounds->w / 2 ) ) subtle = 1;
						
			if ( dx > precision || dx < -precision )
			{
				dx /= precision;
				
				val += dx * subtle;
				
				if ( val > max ) val = max;
				if ( val < 0 ) val = 0;
				
				SetControl32BitValue ( carbonControl, (UInt32) val );
				
				last_X -= dx * precision;
			}
		
		
		}
	}

}

void CAUGuiDisplay::mouseUp(Point *P, bool, bool)
{
	// nothing to do here


}

void CAUGuiDisplay::mouseRight(Point *P)
{

	ControlRef carbonControl = getCarbonControl();
	
	MenuRef ctmenu;
	MenuItemIndex outNewItem;
	
	SetMenuID( ctmenu, 16383 );
	SetMenuWidth( ctmenu, 300 );
	SetMenuHeight( ctmenu, 300 );
	
	AppendMenuItemTextWithCFString( ctmenu, CFSTR ("Hello World"), 0, 5, &outNewItem );
	/*
 		MenuRef              inMenu,
		CFStringRef          inString,
		MenuItemAttributes   inAttributes,
		MenuCommand          inCommandID,
		MenuItemIndex *      outNewItem)
	*/
	
	long result;
	
	result = PopUpMenuSelect( ctmenu, 0, 0, 0 );
/*
		MenuRef   menu,
		short     top,
		short     left,
		short     popUpItem)
*/

	printf ( "selected %d\n", (int) result );

}

void CAUGuiDisplay::draw(CGContextRef context, UInt32 portHeight )
{

	ControlRef carbonControl = getCarbonControl();
	
	UInt32 value = 0;
	
	if ( carbonControl != NULL )
		value = GetControl32BitValue( carbonControl );
	
	CGImageRef theBack = NULL;
	CGImageRef theDisplay = NULL;
	char text[32];
	text[0] = 0;
	CGRect bounds;
	
	getBounds()->to( &bounds, portHeight );
		
	if ( BackGround != NULL )
		theBack = BackGround->getImage();
		
	if ( ForeGround != NULL )
	{
		CAUGuiGraphic* current = ForeGround;
	
		for ( int i = 1; i <= (int)value; i++ ) // maximum inclusive + only when not current
		{
			if ( current == NULL ) break;
			current = (CAUGuiGraphic*)current->getNext();
		}
		
		if ( current != NULL )
			theDisplay = current->getImage();
		else
			printf ( "Shiiet, no Image there!\n" );
	}
            
	if ( theBack != NULL )
		CGContextDrawImage( context, bounds, theBack );
	else
		printf ( "Shiiet, no background Image there!\n" );
	
	getForeBounds()->to ( &bounds, portHeight );
		
	if ( theDisplay != NULL )
	{
		CGContextDrawImage( context, bounds, theDisplay );
	}
	else
	{
		if ( hasText )
		{
			theTexter( value, text, theUserData );
		}
		else
		{		
			// generic display
			CAAUParameter myParam = getAUVP();
						
			switch ( (int)myParam.ParamInfo().unit )
			{
				case kAudioUnitParameterUnit_Indexed:
					if ( myParam.HasNamedParams() )
					{
						CFStringGetCString(myParam.GetParamName(value), text, 24, 0);
					}
					else
					{
						sprintf ( text, "%d", (int) myParam.GetValue() );
					}
					break;
				
				case kAudioUnitParameterUnit_Boolean:
					if ( value == 0.f )
						strcpy ( text, "no" );
					else
						strcpy ( text, "yes" );
					break;
					
				default:
					sprintf ( text,  "%.2f", (float) myParam.GetValue() );
					break;
			}

		
		
		}
		
		CGContextSelectFont ( context, font_name, font_size, kCGEncodingMacRoman );
		CGContextSetRGBFillColor( context, col_red, col_green, col_blue, col_alpha );
		
		
		if ( textalign != 0 )
		{
			CGContextSetTextDrawingMode( context , kCGTextInvisible);
			CGContextShowTextAtPoint(context, 0, 0, text, strlen(text));
			CGPoint pt = CGContextGetTextPosition(context);
			
			if ( textalign == 1 )
			{
				// center
				bounds.origin.x += bounds.size.width / 2 - pt.x / 2;
			}
			else
			{
				// right
				bounds.origin.x += bounds.size.width - pt.x;
			
			}
		}
		CGContextSetTextDrawingMode ( context, kCGTextFill );
		CGContextShowTextAtPoint(context, bounds.origin.x, bounds.origin.y, text, strlen (text) );
	
	}
	
}



void CAUGuiDisplay::setUserProcedure ( displayUserProcedure theProc, void* theUserData )
{
	userProcedure = theProc;
	userData = theUserData;
}





