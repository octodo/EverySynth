#ifndef __CAUGUI_LABELED_BUTTON__
#define __CAUGUI_LABELED_BUTTON__

#include "CAUGuiConfig.h"

#include "CAUGui.h"
#include "CAUGuiButton.h"

class CAUGuiLabeledButton : public CAUGuiButton
{
public:
	CAUGuiLabeledButton ( CAUGuiMan*, CAAUParameter &, eRect* where, CAUGuiGraphic* fore, CAUGuiGraphic* back, CAUGuiBottonMode );
	
	CAUGuiLabeledButton ( CAUGuiMan*, UInt32 theRange, eRect*, CAUGuiGraphic*, CAUGuiGraphic*, CAUGuiBottonMode );

	virtual void draw(CGContextRef context, UInt32 portHeight);
    
    void setText(CFStringRef text);
    
private:
    
    char text[32];
};

#endif
