/*
 EverySynth - A multi-timbral AudioUnit for external synthesizers.
 Copyright (C) 2010  Philip Kranz
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include "EverySynthCarbonUI.h"


COMPONENT_ENTRY(EverySynthCarbonUI)

OSStatus	EverySynthCarbonUI::CreateUI(Float32 xoffset, Float32 yoffset)
{
	int XPos = (int) xoffset;
	int YPos = (int) yoffset;

	/*
    CAAUParameter aKnob1		(mEditAudioUnit, kParam_Knob_1,		kAudioUnitScope_Global, 0);
	CAAUParameter aKnob2		(mEditAudioUnit, kParam_Knob_2, 	kAudioUnitScope_Global, 0);
	CAAUParameter aButtons1	(mEditAudioUnit, kParam_Buttons_1, 	kAudioUnitScope_Global, 0);
	CAAUParameter aButtons2	(mEditAudioUnit, kParam_Buttons_2, 	kAudioUnitScope_Global, 0);
	CAAUParameter aSlider1	(mEditAudioUnit, kParam_Slider_1, 	kAudioUnitScope_Global, 0);
	CAAUParameter aSlider2	(mEditAudioUnit, kParam_Slider_2, 	kAudioUnitScope_Global, 0);
	CAAUParameter aSlider3	(mEditAudioUnit, kParam_Slider_3, 	kAudioUnitScope_Global, 0);
	CAAUParameter aDisplay1	(mEditAudioUnit, kParam_Display_1, 	kAudioUnitScope_Global, 0);
     */
	
	theGui = new CAUGuiMan(this, XPos, YPos);
	theGui->initialize();
	
	// Background image
	CAUGuiGraphic* graBackground = new CAUGuiGraphic("background.png");
    theGui->addImage(graBackground);
	
    /*
	// Backgrounds for Layered Pane
	CAUGuiGraphic* pane_back1 = new CAUGuiGraphic ( "pane1_back.png" ); myCAUGui->addImage( pane_back1 );
	CAUGuiGraphic* pane_back2 = new CAUGuiGraphic ( "pane2_back.png" ); myCAUGui->addImage( pane_back2 );
	
	// Special Images for Control Foregrounds
	
	// this one orbits a center point
	CAUGuiGraphic* round_handle = new CAUGuiSpinImage ( "round_handle.png", -0.375f, 0.375f, 0, 0, true ); myCAUGui->addImage( round_handle );
	
	// this one rotates around a center
	CAUGuiGraphic* needle_handle = new CAUGuiSpinImage ( "knob_needle.png", -0.375f, 0.375f, 0, 0, false ); myCAUGui->addImage( needle_handle );
	
	// these shift within the drawing rectangle
	CAUGuiGraphic* round_s_handle = new CAUGuiHandleImage ( "round_handle2.png", -1, 1 ); myCAUGui->addImage( round_s_handle );
	
	CAUGuiGraphic* horizontal_handle = new CAUGuiHandleImage ( "horizontal_handle.png", -1, 1 ); myCAUGui->addImage( horizontal_handle );
	
	// this crops by value within the drawing rectangle
	CAUGuiGraphic* vu_image = new CAUGuiCroppingImage ( "ledchain.png", 0, 50, 2, 0 ); myCAUGui->addImage( vu_image );
	
	
	// this just sits there and does nothing
	CAUGuiGraphic* next_pane = new CAUGuiGraphic ( "next_pane.png" ); myCAUGui->addImage( next_pane );
	
	// Backgrounds for Controls
	
	CAUGuiGraphic* knob_back = new CAUGuiGraphic ( "knob_background.png" ); myCAUGui->addImage( knob_back );
    
	CAUGuiGraphic* h_slider_back = new CAUGuiGraphic ( "horizontal_slider_back.png" ); myCAUGui->addImage( h_slider_back );
	
	CAUGuiGraphic* v_slider_back = new CAUGuiGraphic ( "vertical_slider_back.png" ); myCAUGui->addImage( v_slider_back );
	
	CAUGuiGraphic* v_led_back = new CAUGuiGraphic ( "led_back.png" ); myCAUGui->addImage( v_led_back );
	
	CAUGuiGraphic* display_back = new CAUGuiGraphic ( "display_back.png" ); myCAUGui->addImage( display_back );
    */
    
	/***************************************
     
     create controls
     
     ***************************************/
    
	
	// Get size of GUI from Background Image and create Background Pane
	
	int width = CGImageGetWidth(graBackground->getImage());
	int height = CGImageGetHeight(graBackground->getImage());
	
	eRect where;
	where.set ( 0, 0, width, height );
	
	CAUGuiPane* paneBackground = new CAUGuiPane(theGui, &where, graBackground);
	theGui->addCtrl(paneBackground);
	
	/*
	// Create the two knobs
	
	where.set ( 8, 90, 80, 80 );
	
	CAUGuiKnob* myKnob = new CAUGuiKnob(myCAUGui, aKnob1, &where, kCAUGui_res_100, round_handle, knob_back );
	myKnob->shrinkForeBounds ( 28, 1, 28, 55 );
	myPane->addCtrl( myKnob );
	
	eRect where2;	
	where2.set ( &where );
	where2.offset ( 0, 8 );
	where2.align ( k_oSC ); // place the eRect below itself (o = outside, S = south, C = center)
	
	myKnob = new CAUGuiKnob(myCAUGui, aKnob2, &where2, kCAUGui_res_100, needle_handle, knob_back );
	myPane->addCtrl( myKnob );
	
	
	
	
	// Build layered Pane with 2 different Control sets
	
	where.align ( k_NEo );
	where.offset ( 8, -12 );
	where.size ( pane_back1->getWidth(),  pane_back1->getHeight());
	
	CAUGuiLayeredPane* myLayeredPane = new CAUGuiLayeredPane(myCAUGui, &where, pane_back1);
	myPane->addCtrl( myLayeredPane );
	
	myLayeredPane->addBackground( pane_back2, 1 );
	
	
	
	// Add a switching Button to the Layered Pane (in all layers)
	
	where.set ( 100, 2, 55, 33 );
	
	CAUGuiButton* myLayerSwitch = new CAUGuiButton( myCAUGui, 1, &where, next_pane, next_pane, kIncButton );
	
	// user procedure to control visible Layer 
	myLayerSwitch->setUserProcedure( switchLayer, myLayeredPane ); 
    
	myLayeredPane->addCtrl( myLayerSwitch );
	
	
	
	
	// Make a horizontal sliders and add it to the layered pane ( layer 0 )
	
	where.set ( 16, 40, 143, 36 ); // always relative to embedding pane
	
	CAUGuiSlider* mySlider = new CAUGuiSlider(myCAUGui, aSlider1, &where, 1, kCAUGui_res_100, round_s_handle, h_slider_back );
	mySlider->shrinkForeBounds ( 20, 5, 23, 7 );
	myLayeredPane->addCtrl( mySlider, 0 );
	
	where.align ( k_oSC );
	
	CAUGuiSlider* mySlider2 = new CAUGuiSlider(myCAUGui, aSlider2, &where, 1, kCAUGui_res_100, round_s_handle, h_slider_back );
	mySlider2->shrinkForeBounds ( 20, 5, 23, 7 );
	myLayeredPane->addCtrl( mySlider2, 0 );
	
	// The generic Display (see NULL for textroutine/foregroundimage)
	
	where.align ( k_oSC );
	where.size ( 96, 39 ); 
	CAUGuiDisplay* myDisplay1 = new CAUGuiDisplay(myCAUGui, aDisplay1, &where, kCAUGui_res_1, NULL, NULL, display_back );
	myDisplay1->shrinkForeBounds ( 17, 5, 23, 19 );
	myLayeredPane->addCtrl( myDisplay1, 0 );
    
	
	// make a clone with different textalignment but same parameter :-)
    
	where.align ( k_oSC );
	myDisplay1 = new CAUGuiDisplay(myCAUGui, aDisplay1, &where, kCAUGui_res_1, NULL, NULL, display_back );
	myDisplay1->shrinkForeBounds ( 17, 5, 23, 19 );
	myDisplay1->setTextAlign ( 2 ); // align right
	myLayeredPane->addCtrl( myDisplay1, 0 );
    
	myDisplay1->generatePopUpMenue ();
	
	
	
	// Make vertical sliders ( layer 1 )
	
	where.set ( 16, 40, 37, 142 ); // always relative to embedding pane
	
	mySlider = new CAUGuiSlider(myCAUGui, aSlider1, &where, 0, kCAUGui_res_100, horizontal_handle, v_slider_back );
	mySlider->shrinkForeBounds ( 3, 18, 10, 24 );
	myLayeredPane->addCtrl( mySlider, 1 );
	
	where.align ( k_MEo );
	
	mySlider2 = new CAUGuiSlider(myCAUGui, aSlider2, &where, 0, kCAUGui_res_100, horizontal_handle, v_slider_back );
	mySlider2->shrinkForeBounds ( 3, 18, 10, 24 );
	myLayeredPane->addCtrl( mySlider2, 1 );
	
	where.align ( k_MEo );
	
	mySlider2 = new CAUGuiSlider(myCAUGui, aSlider3, &where, 0, kCAUGui_res_100, vu_image, v_slider_back );
	mySlider2->shrinkForeBounds ( 13, 19, 17, 23 );
	myLayeredPane->addCtrl( mySlider2, 1 );
    
    
	// VU METER
	where.at (290, 116 );
    
	CAUGuiMeter* myMeter = new CAUGuiMeter(myCAUGui, kRMS_Right, &where, kCAUGui_res_100, vu_image, v_led_back );
	myMeter->shrinkForeBounds ( 13, 19, 17, 23 );
	myPane->addCtrl( myMeter );
	
	where.align ( k_MEo );
	myMeter = new CAUGuiMeter(myCAUGui, kRMS_Left, &where, kCAUGui_res_100, vu_image, v_led_back );
	myMeter->shrinkForeBounds ( 13, 19, 17, 23 );
	myPane->addCtrl( myMeter );
     */
    
    
	
	/******************************************************************************
     
     EZ preset generation.
     
     A small white button appears.
     Press it and your preset setup code jumps outta your console window...
     
     Better don't put this into your release version...
     
     ******************************************************************************/
    
    XPos = (int) xoffset;
	YPos = (int) yoffset;
	
	// set size of overall pane
	SizeControl(mCarbonPane, width + XPos, height + YPos);
	
	return noErr;
}
