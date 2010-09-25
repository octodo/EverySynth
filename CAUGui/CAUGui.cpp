/*
 *  CAUGui.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sat Oct 19 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGui.h"



/***********************************************************************

	CAUGuiMan
	
	the manager class that does all the admin for you

***********************************************************************/

static UInt32 numConstructions = 0;

CAUGuiMan::CAUGuiMan (AUCarbonViewBase* theAUCarbonViewBase, UInt32 x, UInt32 y )
{
	itemCount = 0;
	Images = NULL;
	Controls = NULL;
	
	relaxed	= false;
	
	myAUCarbonViewBase = theAUCarbonViewBase;
	X = x;
	Y = y;
	
	
	internalParameters = NULL;
	
	ControlClass			= NULL;
	ControlSpec.defType		= kControlDefObjectClass;
	ControlSpec.u.classRef	= NULL;
	
	
	
	numConstructions++;
	
	//printf ("ctor CAUGuiMan %d\n", (int) numConstructions);

}

CAUGuiMan::~CAUGuiMan()
{

	//printf ("dtor CAUGuiMan\n");
	
	RemoveEventLoopTimer(timer);
	
	if ( Controls != NULL ) delete Controls;
	Controls = NULL;

	if ( Images != NULL ) delete Images;
	Images = NULL;
	
	
	
	//UnregisterToolboxObjectClass(ControlClass);
	
	

}
	
void CAUGuiMan::initialize()
{
	
	UInt32 myAddress= (UInt32)this;
	
	bool found = false;
	
	
	EventTypeSpec CEventList[] = {{kEventClassControl, kEventControlDraw},
                                   {kEventClassControl, kEventControlHit},
                                    {kEventClassControl, kEventControlTrack},
                                    {kEventClassControl, kEventControlClick}};

	//CFStringCreateWithCString(CFAllocatorRef alloc, const char *cStr, CFStringEncoding encoding)


	while ( !found )
	{

		sprintf ( defString, "com.CAUGui.zoyd%d.cntrl", (int)myAddress );

		if ( noErr == RegisterToolboxObjectClass (	CFStringCreateWithCString(NULL, defString, kCFStringEncodingISOLatin1 ),
												NULL,
												4,
												CEventList,
												NewEventHandlerUPP (CAUGuiControlHandler),
												this,
												&ControlClass)) found = true;
		myAddress++;
	}
	
	ControlSpec.u.classRef	= ControlClass;

	InstallEventLoopTimer(
        GetCurrentEventLoop(),
        0,
        kEventDurationMillisecond * 50,
        NewEventLoopTimerUPP( TimerProc ),
        this,
        &timer );
}

void CAUGuiMan::requestInternalParameters()
{
/*
	EXTERN_API( ComponentResult )
AudioUnitGetProperty(
  AudioUnit             ci,
  AudioUnitPropertyID   inID,
  AudioUnitScope        inScope,
  AudioUnitElement      inElement,
  void *                outData,
  UInt32 *              ioDataSize)                           FIVEWORDINLINE(0x2F3C, 0x0014, 0x0004, 0x7000, 0xA82A);
*/

	UInt32 numParamsPassed = kNumInternalParameters * sizeof( float* );
	ComponentResult result;
	
	result = AudioUnitGetProperty( myAUCarbonViewBase->GetEditAudioUnit(), kInternalParameterProperyID, kAudioUnitScope_Global, 1, &internalParameters, &numParamsPassed );

	//printf ( "result: %d\n", (int)result );

}

float CAUGuiMan::getInternalParameter ( int index )
{

	if ( internalParameters == NULL )
		requestInternalParameters();
		
	if ( internalParameters != NULL )	
		return internalParameters[ index ];
	
	return 0.f;
}


void CAUGuiMan::idle()
{
/*
	if ( kNumInternalParameters != 0 )
	{
		requestInternalParameters();
	}
*/	
	setRelaxed ( true ); // Dunno if this makes sense (Draw1Control interrupts the thread)...
	
	if ( Controls != NULL )
		Controls->idle();
		
	setRelaxed ( false );

}

void TimerProc ( EventLoopTimerRef inTimer, void *inUserData )
{
	if ( inUserData != NULL )
	{

		if ( ((CAUGuiMan*)inUserData) != NULL )
			((CAUGuiMan*)inUserData)->idle();
	}
	
}

void CAUGuiMan::addImage ( CAUGuiGraphic* theImage )
{
	UInt32 nextID = requestID();
	
	theImage->setID ( nextID );
		
	if ( Images == NULL )
		Images = theImage;
	else
		Images->append( theImage );
		
}

CAUGuiGraphic* CAUGuiMan::getImageByID ( UInt32 theID )
{
	if ( Images == NULL ) return NULL;
	return (CAUGuiGraphic*)Images->getByID( theID );

}



void CAUGuiMan::addCtrl( CAUGuiCtrl* theCtrl )
{
	UInt32 nextID = requestID();
	
	theCtrl->setID ( nextID );
	
	if ( theCtrl->getDaddy() == NULL )
	{
		if ( Controls == NULL )
			Controls = theCtrl;
		else
			Controls->append( theCtrl );
			
		theCtrl->setOffset ( X, Y );
	}
	
	
	if ( theCtrl->providesForeignControls () )
	{
		theCtrl->initForeignControls (ControlSpec);
	}
	else
	{
		ControlRef newControl;
	
		Rect r;
	
		theCtrl->getBounds ( &r );
	
		verify_noerr(CreateCustomControl( myAUCarbonViewBase->GetCarbonWindow(), &r, &ControlSpec, NULL, &newControl));
	
		float factor = (float) theCtrl->getResolution();
		
		UInt32 theMax = (UInt32)( theCtrl->getRange() * factor );
		
		SetControl32BitMaximum( newControl, theMax);
		
		SetControl32BitMinimum( newControl, 0);
        
		if ( theCtrl->isAUVPattached() )
		{
		
			CAAUParameter myParam = theCtrl->getAUVP();
			
			float value = myParam.GetValue();
			
			float maxf = myParam.ParamInfo().maxValue;
			
			float minf = myParam.ParamInfo().minValue;
			
			value = (value - minf) / (maxf - minf );
			
			SInt32 initValue = (SInt32)( value * (float) theMax );
			
			SetControl32BitValue( newControl, initValue);
			
		
			myAUCarbonViewBase->AddCarbonControl(AUCarbonViewControl::kTypeContinuous, myParam, newControl);
			
			
		}
		else
		{
			myAUCarbonViewBase->EmbedControl( newControl );
		}
		
		
		theCtrl->setCarbonControl( newControl );
	}
}

CAUGuiCtrl* CAUGuiMan::getCtrlByID( UInt32 theID )
{

	if ( Controls == NULL ) return NULL;
	return (CAUGuiCtrl*)Controls->getByID( theID );

}

CAUGuiCtrl* CAUGuiMan::getCtrlByControlRef ( ControlRef theControl )
{
	
	CAUGuiCtrl* current = Controls;
	
	while ( current != NULL )
	{
		if ( current->isControlRef( theControl ) )
		{
			current = current->getChild(theControl);
			break;
		}
		current = (CAUGuiCtrl*)current->getNext();
	
	}
	
	return current;

}

UInt32 CAUGuiMan::requestID ( void )
{
	itemCount++;
	return ( itemCount );

}








