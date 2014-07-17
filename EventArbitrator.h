/*	Spiked 2.0, by Chuck Groom 1997.
	The Event Arbitrator header file.
	
	The Macintosh OS maintains an event queue. Rather than directly feed this queue into the 
	game loop, and let the game loop decode the meaning of the event, we pre-filter events
	with the EventArbitrartor. 
	
	The Game_Event is a class that stores the kinds of events used by Spiked.
	A stack of Game_Events is stored by the Event_Stack, which reprents the current event 
	that we must deal with. */
	
	
#ifndef EVENT_AR
#define EVENT_AR


// The kind of events we will to process. The "G" tag refers to the fact that these are 
// GAME events, not OS events
enum GEventType { GnullEvent = 0, 
				  GkeyDown, 
				  GmouseDown, 
				  Gplay_arena,  
				  GpauseGame,
				  Gquit,
				  Gintro_arena,
				  Gpref_arena,
				  Gabout_arena,
				  Gpost_game};




/*--------------------------------------------------------------
	 The Game_Event class is like the OSEvent struct, but more specialized
 ------------------------------------------------------------------*/
class Game_Event {
	public:
		Game_Event(GEventType w, Point p, char k) 
			{what = w; where.h = p.h; where.v = p.v; key = k;}
		Game_Event(GEventType w) 
			{what = w; where.h = 0; where.v = 0; key = '\0';}
		Game_Event(const Game_Event & otherEvent) 	//Copy Constructor
			{what = otherEvent.what; where.h = otherEvent.where.h; 
			 where.v = otherEvent.where.v; key = otherEvent.key;}
		Game_Event & operator = (Game_Event e)
			{what = e.what; where.h = e.where.h; where.v = e.where.v; key = e.key; return e;}
		
		GEventType what; 	//What type of event is determined by the previous enums.
		Point where;
		char key;	
};




/*--------------------------------------------------------------
	Event_Stack is a stack of semi-processed system events.
 ------------------------------------------------------------------*/
class Event_Stack {
	private:
			Game_Event *E_Stack[100];
			short int top;		 				//The top of the stack
			short int MAX_SIZE;					//The stack has, at most, MAX_SIZE elements.
	public:
			Event_Stack(void); 								
			Event_Stack(Event_Stack & otherStack); 			
			~Event_Stack(void)   				
				{
					for(int counter = 0; counter < 100; counter++)
					delete E_Stack[counter];
				}
			
			void push(Game_Event *);
			void push(GEventType);
			
			Game_Event * pop(void);
			void flush(void);
			Game_Event * peek(void)
				{
					if (top)
						return E_Stack[top];
					return NULL;
				}
			
			Game_Event ** read_Stack(void) {return E_Stack;}
			int read_top(void) {return top;}
};


#endif
