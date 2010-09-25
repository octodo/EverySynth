/*
 *  CAUGuiPoof.h
 *  RumblenceAU
 *
 *  Created by Urs Heckmann on Mon Oct 28 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __CAUGuiPoof__
#define __CAUGuiPoof__

#include "CAUGuiButton.h"



class PoofButton : public CAUGuiButton
{
public:
	PoofButton ( CAUGuiMan*, CAAUParameter &, eRect*, CAUGuiGraphic*, CAUGuiGraphic*, CAUGuiBottonMode, CAUGuiGraphic* poof );
	
	virtual ~PoofButton ();
	
	virtual void mouseDown(Point *P, bool, bool);
	virtual void mouseUp(Point *P, bool, bool);
	void idle();
	

private:
		
	CAUGuiGraphic*		thePoof;
	WindowRef			poofWindow;
	SInt32				frame;
	CGRect				box;
	Point 				poofPoint;
};

#endif