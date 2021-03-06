/* 	Spiked 2.0, 1997
    Chuck Groom
    
	OSEvents.h allows for access to the important ProcessEvent function which is the backbone
	of Spiked events. This class processes events, catches OS events and fills the Event_Stack
	with events relevant to game play. */
	
#ifndef OSEVENT_HEADER
#define OSEVENT_HEADER

#include "Menus.h"
#include "EventArbitrator.h"
#include "KBDFilter.h"


class OSEvent
{
	private: 
		 Rect		mainWindowRect;
 		 WindowPtr	mainWindow;
 		 Boolean	switchedOut;
 		 
 		 Game_Menu	*menu_system;	
		
		void HandleMouseEvent (EventRecord *, Event_Stack *);
		void HandleKeyEvent (EventRecord *, Event_Stack *);
		void HandleUpdateEvent (EventRecord *);
		void HandleOSEvent (EventRecord *);
		void HandleHighLevelEvent (EventRecord *);
		
		 keyboard_filter keyboard;
	public:
		OSEvent(Game_Menu * gm) {menu_system = gm; switchedOut = FALSE; }
		Boolean ProcessEvent (Event_Stack *stack);
		Boolean IsSwitchedOut(void) 	{return switchedOut;}
};



#endif
