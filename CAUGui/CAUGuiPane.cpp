/*
 *  CAUGuiPane.cpp
 *  CoreGraphics AudioUnit GUI framework
 *
 *  Created by Urs Heckmann on Sun Oct 27 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "CAUGuiPane.h"

CAUGuiPane::CAUGuiPane (	CAUGuiMan*			theChief,
							eRect*				theWhere,
							CAUGuiGraphic*		theBackground)
						
						:	CAUGuiCtrl ( theChief, theWhere )
							
{
	
	BackGround = theBackground;
	
	upperLeft = NULL;
	lowerRight = NULL;
	
	//printf ("ctor CAUGuiPane\n");
	
	setType ( kCAUGui_Pane );
	
	setTolerance ( 0 );

}

CAUGuiPane::~CAUGuiPane ()
{
	//printf ("dtor CAUGuiPane\n");


}

bool CAUGuiPane::isControlRef ( ControlRef theControl ) 
{

	if (upperLeft == theControl || lowerRight == theControl)
		return true;
		
	return CAUGuiCtrl::isControlRef (theControl);
	
}

CAUGuiCtrl*	CAUGuiPane::getChild(ControlRef theControl)
{
	if (upperLeft == theControl || lowerRight == theControl)
		return this;

	return CAUGuiCtrl::getChild (theControl);

}

void CAUGuiPane::initForeignControls ( ControlDefSpec &ControlSpec )
{
	// if the pane has a Background image, it gets two Carbon controls to receive events
		
	if ( BackGround != NULL )
	{
		
		ControlRef newControl;
		
		Rect r;
	
		getBounds ( &r );
	
		r.right = r.left + 1;
		r.bottom = r.top + 1;
		
		verify_noerr(CreateCustomControl( getCAUGuiMan()->getAUCarbonViewBase()->GetCarbonWindow(), &r, &ControlSpec, NULL, &newControl));
		
		upperLeft = newControl;
		
		getBounds ( &r );
	
		r.left = r.right - 1;
		r.top = r.bottom - 1;
		
		verify_noerr(CreateCustomControl( getCAUGuiMan()->getAUCarbonViewBase()->GetCarbonWindow(), &r, &ControlSpec, NULL, &newControl));
		
		lowerRight = newControl;
	
		getCAUGuiMan()->getAUCarbonViewBase()->EmbedControl ( upperLeft );
		getCAUGuiMan()->getAUCarbonViewBase()->EmbedControl ( lowerRight );
	
	}
	else
		setOpaque( false );
}


void CAUGuiPane::draw(CGContextRef context, UInt32 portHeight )
{
	//printf ( "drawing this pane!\n");
	
	CGImageRef theBack = NULL;
	
	if ( BackGround != NULL )
		theBack = BackGround->getImage();
		
	if ( theBack != NULL )
		CGContextDrawImage( context, CGRectMake( getBounds()->x, portHeight - getBounds()->h - getBounds()->y, getBounds()->w, getBounds()->h), theBack );


}


CAUGuiLayeredPane::CAUGuiLayeredPane (	CAUGuiMan*			theChief,
										eRect*				theWhere,
										CAUGuiGraphic*		theBackground)
						
						:	CAUGuiPane ( theChief, theWhere, theBackground )
							
{
	
	for ( int i = 0; i < CAUGUI_PANE_MAX_LAYERS; i++ )
	{
		layerBackGround[ i ] = NULL;
		Layered[ i ] = NULL;
	}
	
	for ( int i = 0; i < CAUGUI_PANE_MAX_GROUPS; i++ )
	{
		Grouped[ i ] = NULL;
		GroupLabels[ i ] = NULL;
		GroupState[ i ] = 0;
	}
	
	currentLayer = 0;
	
	//printf ("ctor CAUGuiPane\n");
	
	setType ( kCAUGui_LayeredPane );

}

CAUGuiLayeredPane::~CAUGuiLayeredPane ()
{
	for ( int i = 0; i < CAUGUI_PANE_MAX_LAYERS; i++ )
	{
		if ( Layered[ i ] != NULL ) delete Layered[ i ];
		if ( layerBackGround[ i ] != NULL ) layerBackGround[ i ] = NULL;
	}
	
	for ( int i = 0; i < CAUGUI_PANE_MAX_GROUPS; i++ )
	{
		if ( Grouped[ i ] != NULL ) delete Grouped[ i ];
		if ( GroupLabels[ i ] != NULL ) delete GroupLabels[ i ];
		
	}
	
	//printf ("dtor CAUGuiLayeredPane\n");


}


CAUGuiCtrl*	CAUGuiLayeredPane::getChild(ControlRef theControl)
{		
	if ( Layered[ currentLayer ] != NULL )
	{
		CAUGuiCtrl* current = Layered[ currentLayer ];
		while( current != NULL )
		{
			if ( current->isControlRef ( theControl ) )
				return current->getChild( theControl );
		
			current = (CAUGuiCtrl*)current->getNext();
		}
	}

	for ( int i = 0; i < CAUGUI_PANE_MAX_GROUPS; i++ )
	{
		if ( GroupState[ i ] != 0 )
		{
			CAUGuiCtrl* current = Grouped[ i ];
			while( current != NULL )
			{
				if ( current->isControlRef ( theControl ) )
					return current->getChild( theControl );
			
				current = (CAUGuiCtrl*)current->getNext();
			}
		
		
		}
		
	}

	return CAUGuiPane::getChild (theControl);

}

bool CAUGuiLayeredPane::isControlRef ( ControlRef theControl )
{
	if ( Layered[ currentLayer ] != NULL )
	{
		CAUGuiCtrl* current = Layered[ currentLayer ];
		while( current != NULL )
		{
			if ( current->isControlRef ( theControl ) ) return true;
			current = (CAUGuiCtrl*)current->getNext();
		}
	}
	
	for ( int i = 0; i < CAUGUI_PANE_MAX_GROUPS; i++ )
	{
		if ( GroupState[ i ] != 0 )
		{
			CAUGuiCtrl* current = Grouped[ i ];
			while( current != NULL )
			{
				if ( current->isControlRef ( theControl ) )
					return true;
			
				current = (CAUGuiCtrl*)current->getNext();
			}
		
		
		}
		
	}

	
	
	return CAUGuiPane::isControlRef (theControl);
}



void CAUGuiLayeredPane::draw(CGContextRef context, UInt32 portHeight )
{
	//printf ( "drawing this pane!\n");
	
	CGImageRef theBack = NULL;
	
	if ( layerBackGround[ currentLayer ] != NULL )
		theBack = layerBackGround[ currentLayer ]->getImage();
	else
	{
		if ( BackGround != NULL )
		theBack = BackGround->getImage();
	
	}
		
	if ( theBack != NULL )
		CGContextDrawImage( context, CGRectMake( getBounds()->x, portHeight - getBounds()->h - getBounds()->y, getBounds()->w, getBounds()->h), theBack );

	eRect where;
	where.set (getBounds());
	
	for ( int i = 0; i < CAUGUI_PANE_MAX_GROUPS; i++ )
	{
		if ( GroupState[ i ] != 0 )
		{
			CAUGuiLabel* current = GroupLabels[ i ];
			while( current != NULL )
			{
				current->draw ( context, portHeight, &where, 1.f );
			
				current = (CAUGuiLabel*)current->getNext();
			}
		
		
		}
		
	}


}

void	CAUGuiLayeredPane::clip ( bool drawing )
{
	if ( getOpaque() )
	{
		Rect r;
		getBounds()->to ( &r );
		FrameRect( &r );
		//printf ( "clipping opaque %d %d %d %d\n", r.left, r.top, r.right, r.bottom );
	}
	
	if ( !getOpaque() || drawing )
	{		
		CAUGuiCtrl* current = children;
		while( current != NULL )
		{
			//printf ( "clipping child %d\n", (int) current->getID() );
			current->clip( false );
			current = (CAUGuiCtrl*)current->getNext();
		}
		
		current = Layered[ currentLayer ];
		while( current != NULL )
		{
			//printf ( "clipping layered %d %d\n", currentLayer, (int) current->getID() );
			current->clip( false );
			current = (CAUGuiCtrl*)current->getNext();
		}
		
		for ( int i = 0; i < CAUGUI_PANE_MAX_GROUPS; i++ )
		{
			if ( GroupState[ i ] != 0 )
			{
				current = Grouped[ i ];
				while( current != NULL )
				{
					current->clip( false );
				
					current = (CAUGuiCtrl*)current->getNext();
				}
			
			
			}
			
		}
	}


}

void 	CAUGuiLayeredPane::idle ( )
{
	CAUGuiPane::idle();

	if ( Layered[ currentLayer ] != NULL )
		Layered[ currentLayer ]->idle();
		
	for ( int i = 0; i < CAUGUI_PANE_MAX_GROUPS; i++ )
	{
		if ( GroupState[ i ] == 1 )
		{
			if ( Grouped[ i ] != NULL )
			{
				Grouped[ i ]->idle();
			}
		}
	}
}

void 	CAUGuiLayeredPane::setLayer ( int l )
{
	if ( Layered[ currentLayer ] != NULL )
	{
		CAUGuiCtrl* current = Layered[ currentLayer ];
		while( current != NULL )
		{
			current->setVisible( false );
			current = (CAUGuiCtrl*)current->getNext();
		}
	}
	
	if ( l >= CAUGUI_PANE_MAX_LAYERS || l < 0 ) l = 0;
	
	currentLayer = l;
	
	if ( Layered[ currentLayer ] != NULL )
	{
		CAUGuiCtrl* current = Layered[ currentLayer ];
		while( current != NULL )
		{
			current->setVisible( true );
			current = (CAUGuiCtrl*)current->getNext();
		}
	}
	
	if ( upperLeft != NULL )
	{
		becameVisible = true; // too much drawing while mousing prevention workaround
		Draw1Control ( upperLeft );
	}
}


void	CAUGuiLayeredPane::showGroup ( int l, bool visible )
{
	if ( Grouped[ l ] != NULL )
	{
		if ( visible )
		{
			if ( GroupState[ l ] == 0 )
			{
				GroupState[ l ] = 1;
			
				CAUGuiCtrl* current = Grouped[ l ];
				while( current != NULL )
				{
					current->setVisible( true );
					current = (CAUGuiCtrl*)current->getNext();
				}
			
			}
		
		
		}
		else
		{
			if ( GroupState[ l ] != 0 )
			{
				GroupState[ l ] = 0;
			
				CAUGuiCtrl* current = Grouped[ l ];
				while( current != NULL )
				{
					current->setVisible( false );
					current = (CAUGuiCtrl*)current->getNext();
				}
			
			}
		
		
		}
	}

}



void 	CAUGuiLayeredPane::addCtrl( CAUGuiCtrl* theCtrl, int theLayer )
{

	if ( theLayer >= CAUGUI_PANE_MAX_LAYERS || theLayer < 0 ) theLayer = 0;

	if ( Layered[ theLayer ] == NULL )
			Layered[ theLayer ] = theCtrl;
		else
			Layered[ theLayer ]->append( theCtrl );
			
	theCtrl->setDaddy ( this );
	theCtrl->setOffset ( where.x, where.y );
	
	Chief->addCtrl( theCtrl );

	if ( theLayer != currentLayer )
		theCtrl->setVisible( false );


}

void 	CAUGuiLayeredPane::addCtrlToGroup( CAUGuiCtrl* theCtrl, int theGroup )
{

	if ( theGroup >= CAUGUI_PANE_MAX_GROUPS || theGroup < 0 ) theGroup = 0;

	if ( Grouped[ theGroup ] == NULL )
			Grouped[ theGroup ] = theCtrl;
		else
			Grouped[ theGroup ]->append( theCtrl );
			
	theCtrl->setDaddy ( this );
	theCtrl->setOffset ( where.x, where.y );
	
	Chief->addCtrl( theCtrl );

	theCtrl->setVisible( false );


}


void CAUGuiLayeredPane::setOffset ( SInt32 x, SInt32 y )
{
	CAUGuiPane::setOffset ( x, y );


	for ( int i = 0; i < CAUGUI_PANE_MAX_LAYERS; i++ )
	{
		if ( Layered[ i ] != NULL )
		{
			CAUGuiCtrl* current = Layered[ i ];
			while ( current != NULL )
			{
				current->setOffset ( x, y );
				current = (CAUGuiCtrl*)(current->getNext());
			}
		}
	}
}

void 	CAUGuiLayeredPane::addBackground( CAUGuiGraphic* theBG, int theLayer )
{

	if ( theLayer >= CAUGUI_PANE_MAX_LAYERS || theLayer < 0 ) theLayer = 0;
	
	layerBackGround[ theLayer ] = theBG;


}

void	CAUGuiLayeredPane::placeGroupLabel ( CAUGuiGraphic* label, int group, int x, int y )
{
	eRect where ( x, y, 0, 0 );
	
	CAUGuiLabel* aLabel = new CAUGuiLabel( label, &where );

	if ( GroupLabels[ group ] == NULL )
		GroupLabels[ group ] = aLabel;
	else
		GroupLabels[ group ]->append( aLabel );


}






