/*	Spiked 2.0:  The Event Processor
	by Chuck Groom, 1997.
	
	OSEvent.cp
	
	The Macintosh OS creates a lot of events, most of the irrelevant. This OSEvent class
	acts as an intermediary between the Macintosh OS and the Event_Stack. 
	
	The core element of this class' function is the ProcessEvent call. Each time this call is
	invoked, we nab the current OS events. If these events have bearing with high-level events
	like windows, menus, etc., this class grabs those events and deals with them. Lower level 
	events, like key presses and mouse clicks, are processed and fed into the Event_Stack.
	
	Note that one call to ProcessEvent can result in many event being added to the stack. */
	

#include <ctype.h>
#include "SAT.h"
#include "Menus.h"
#include "OSEvent.h"
#include "EventArbitrator.h"
#include "KBDFilter.h"


//------------------------------------------------------------------
//ProcessEvent()
//	Poll the system events, process them. If they have bearing on game play, route them to
//  to the Event_Stack.
//------------------------------------------------------------------
Boolean OSEvent::ProcessEvent (Event_Stack *stack)
{
	EventRecord	theEvent;
	Boolean		bsomething_Happened;
	
	//Check the Macintosh System Event queue
	SystemTask();	//Be a good neighbor
	bsomething_Happened = GetNextEvent(everyEvent, &theEvent); 
	

	switch (theEvent.what)
	{
		case mouseDown:
			HandleMouseEvent(&theEvent, stack);
			break;
		case updateEvt:
			HandleUpdateEvent(&theEvent);
			break;
		case keyDown:
			HandleKeyEvent(&theEvent, stack);
			break;
		case osEvt:
			HandleOSEvent(&theEvent);
			break;
		case kHighLevelEvent:
			HandleHighLevelEvent(&theEvent);
			break;
	}
	
	//I wrote my own code to read the keyboard. See KBDFilter.cp
	if(keyboard.key_is_down())
		keyboard.run_filter(stack);
		
	return bsomething_Happened;
}



//-------------------------------------------------------------- 
// HandleMouseEvent() -- if the mouse is chaning a window or selecting a menu, process
// that event. Otherwise, process the event and toss it into the Event_Stack.
//-------------------------------------------------------------- 

void OSEvent::HandleMouseEvent (EventRecord *theEvent, class Event_Stack *stack)
{
	WindowPtr	whichWindow;
	short		thePart;
	thePart = FindWindow(theEvent->where, &whichWindow);
	
	switch (thePart)
	{
		case inSysWindow:
			SystemClick(theEvent, whichWindow);
			break;
		
		case inMenuBar:
				Game_Event * theMenuEvent = menu_system->DoMenuChoice(MenuSelect(theEvent->where));
				if(theMenuEvent)
					stack->push(theMenuEvent);
			break;
		
		case inDrag:
		case inGoAway:
		case inGrow:
		case inZoomIn:
		case inZoomOut:
			break;
		
		case inContent:		//Generate a new game event.
			stack->push (new Game_Event(GmouseDown, theEvent->where, theEvent->message));
			break;
	}
}



//--------------------------------------------------------------  
//HandleUpdateEvent -- redraw window
//--------------------------------------------------------------  
void OSEvent::HandleUpdateEvent (EventRecord *theEvent)
{	
	if ((WindowPtr)theEvent->message == mainWindow)
		SATRedraw();
}


//--------------------------------------------------------------  
// HandleOSEvent
//--------------------------------------------------------------  
void OSEvent::HandleOSEvent (EventRecord *theEvent)
{	
	char eventType;
	eventType = theEvent->message >> 24;	//High bit
	
	if (eventType & suspendResumeMessage)		// suspend or resume event
	{
		if (theEvent->message & resumeFlag)		// resume event
		{
			switchedOut = FALSE;
			SATRedraw();
		}
		else									// suspend event
			switchedOut = TRUE;
	}
}



//--------------------------------------------------------------  
//HandleHighLevelEvent	- unused.
//--------------------------------------------------------------  
void OSEvent::HandleHighLevelEvent (EventRecord *theEvent)
{	
 	// AEProcessAppleEvent(theEvent);
}



 
 
//--------------------------------------------------------------  
// HandleKeyEvent
// If a key + command key is down, treat this as a menu event. We will let the KBD filter deal
// with creating new Events based on keys being pressed.
///--------------------------------------------------------------  

void OSEvent::HandleKeyEvent (EventRecord *theEvent,  Event_Stack *stack)
{
	char		theChar;
	theChar = theEvent->message & charCodeMask;
	
	if ((theEvent->modifiers & cmdKey) != 0)
	{
		Game_Event * theMenuEvent = menu_system->DoMenuChoice(MenuKey(toupper(theChar)));
		if(theMenuEvent)
			stack->push(theMenuEvent);
	}
}









		

