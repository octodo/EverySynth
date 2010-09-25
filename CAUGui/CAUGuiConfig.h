/*
 *  CAUGuiConfig.h
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Mon Oct 28 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CAUGUI__CONFIG__ 
#define __CAUGUI__CONFIG__

#define AU_BUNDLE_IDENTIFIER "com.acme.demo.caugui"

#define CAUGUI_FONT "Monaco"

#define CAUGUI_FONT_SIZE 11.0

#define	CAUGUI_FONT_RED 0.0
#define	CAUGUI_FONT_GREEN 0.0
#define	CAUGUI_FONT_BLUE 0.0
#define	CAUGUI_FONT_ALPHA 1.0

#define CAUGUI_DEFAULT_FONT_ALIGN 1 // center 

#define AUTO_POP_UP_MENUES 1

#define CAUGUI_PANE_MAX_LAYERS 4

#ifndef PI
#define PI (3.1415926535897932384626433832795)
#endif

/*****************************************************

	internal parameter scheme
	
	CAUGuiMan references these from the dsp class
	with a pointer
	
	I use normalized values (0.f - 1.f) to
	display them in CAUGuiMeters
	
	they are used in MFM for the "running beat" LEDs 
	of the Step Sequencer

*****************************************************/

enum
{

	kRMS_Left,
	kRMS_Right,
	
	
	kNumInternalParameters

};

#define kInternalParameterProperyID 64000


#endif