/*	Spiked 2.0, by Chuck Groom, 1997.
	
	The Macintosh OS maintains an event queue. Rather than directly feed this queue into the 
	game loop, and let the game loop decode the meaning of the event, we pre-filter events
	with the EventArbitrartor. 
	
	There is a class called a GameEvent that stores information relevant to controlling Spiked.
	The Event_Stack class maintains a large stack of all the GameEvents that need to be used.
	The main things to consider is that this is just a typical stack, with Push, Pop, and Peek
	operations.  */


#include "EventArbitrator.h"


//--------------------------------------
//	Constructor
//--------------------------------------
Event_Stack::Event_Stack(void) 
{
	top = 0;
	MAX_SIZE = 100;
	
	for (int counter = 0; counter < MAX_SIZE; counter++)
		E_Stack[counter] = NULL;
}


//--------------------------------------
//	Copy Constructor
//--------------------------------------
Event_Stack::Event_Stack(Event_Stack &otherStack)
{
	MAX_SIZE = 100;
	//Flush out the stack:
	flush();
	
	//Copy events
	for( int counter = 0; counter <= otherStack.read_top(); counter++)
		push(otherStack.read_Stack()[counter]);
	
	top = otherStack.read_top();
}


//-------------------------------------------------------------------------
//	Flush -- clean it out!
//-------------------------------------------------------------------------
void Event_Stack::flush(void)
{
	for (int counter = 0; counter < MAX_SIZE; counter++)
	{
		delete E_Stack[counter];
		E_Stack[counter] = NULL;
	}
	top = 0;
}	


//-------------------------------------------------------------------------
//	Pop
//-------------------------------------------------------------------------
Game_Event * Event_Stack::pop(void)
{
	Game_Event *theEvent;
	if(top)
	{
		theEvent = E_Stack[top];
		E_Stack[top] = NULL;
		top--;
		return theEvent;
	}
	return NULL;
}	


//--------------------------------------
//	Push adds an event to the stack, if possible.
//--------------------------------------
void Event_Stack::push(Game_Event *theEvent)
{
	if (top > MAX_SIZE - 2)
	{
		flush();		//Very bad: Overflow. Flush the stack
	}

	delete E_Stack[top++];
	E_Stack[top] = theEvent;
	E_Stack[top + 1] = NULL;
}


//--------------------------------------
//	Push
//--------------------------------------
void Event_Stack::push(GEventType e_t)
{
	Game_Event *theEvent = new Game_Event(e_t);
	push(theEvent);
}
