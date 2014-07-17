/*  Spiked 2.0, 1997 Chuck Groom
	The GAME is the main control structure behind Spiked. When created, it creates the two major
	data storage classes -- GlobalTool and GlobalData -- as well as the Event handlers. Then, 
	it creates one of the three arenas, and enters a loop where is grabs the top event on the 
	stack, processes it if it is a high-level event, then tells the current arena to cycle through
	a frame of action. */


#include "SAT.h"
#include <stdlib.h>
#include "EventArbitrator.h"
#include "OSEvent.h"
#include "arena.h"
#include "GAME.h"
#include "global.h"
#include "fades.h"
#include "time.h"

//Constructor: Initialize the sprite animation toolkit, create a window, test the depth, 
// and set up data structures.
GAME::GAME()
{
	is_running = TRUE;
	
	//Initialize everything
	SATInitToolbox();
	FlushEvents(everyEvent, 0);
	
	//Set up sound system
	SATSoundInitChannels (2);
	
	//Set up menus
	the_game_menus = new Game_Menu;
	
	//Open a new screen with the words "Loading Data"
	SATInit(DEF_PICT, DEF_PICT, GAME_WIDTH, GAME_HEIGHT); 
	SATConfigure(FALSE, kLayerSort, kNoCollision, 50);
	
	//Check the screen depth. If it isn't 256 colors, make like a banana and split.
	if (gSAT.initDepth != 8)
	{
		SATReportStr(C2PStr("Spiked 2.0 requires 256 colors. Please change your monitor's depth."));
		exit(10);
	}
		
	srand(clock());
	
	// Create the event handler
	OSEventHandler = new OSEvent(the_game_menus);
	
	
	// The two primary data structures. GlobalData takes about 15 seconds to load because
	// it reads a lot of stuff from disc. As the GlobalTool loads, it draws a nice loading bar on the screen.
	GlobalTool = new GTool(the_game_menus); 
	GlobalData = new GData;

	// Fade the screen. 
	GlobalTool->fade_out();
	
	// Finally, set up the first scene -- arena -- that has the splash screen. 
	the_arena = new intro_arena(&the_Stack);
	the_arena_type = intro;
	
	// Play the intro sound
	GlobalData->PlaySound(SPIKED_INTRO);
}






// The main loop. Exits when is_running = false, which occurs when the user chooses "quit" from the menu.
void GAME::run()
{
	Game_Event * topEvent;
	long L = TickCount();

	//Iterate until the current event is to quit.
	while (is_running)
	{
		
		OSEventHandler->ProcessEvent(&the_Stack);	//Grab events
		
		the_arena->run_frame();			//Pass stack to the current arena
		
		topEvent = the_Stack.peek(); 	
		if (topEvent)								//see if we need to create a new arena
		{
			switch(topEvent->what)
			{
				case Gplay_arena:
					if(the_arena_type != game)
					{
						topEvent = the_Stack.pop();
						delete the_arena;
						the_arena = new game_arena(&the_Stack);
						the_arena_type = game;
					}
					break;
				case Gintro_arena:
					if(the_arena_type != intro)
					{
						topEvent = the_Stack.pop();
						delete the_arena;
						the_arena = new intro_arena(&the_Stack);
						the_arena_type = intro;
					}
					break;
				case Gpref_arena:
					if(the_arena_type != pref)
					{
						topEvent = the_Stack.pop();
						delete the_arena;
						the_arena = new pref_arena(&the_Stack);
						the_arena_type = pref;
					}
					break;
				case Gabout_arena:
					if(the_arena_type != about)
					{
						topEvent = the_Stack.pop();
						delete the_arena;
						the_arena = new about_arena(&the_Stack);
						the_arena_type = about;
					}
					break;
				case Gpost_game:
					if(the_arena_type != post_game)
					{
						topEvent = the_Stack.pop();
						delete the_arena;
						the_arena = new post_game_arena(&the_Stack);
						the_arena_type = post_game;
					}
					break;
				case Gquit:
					is_running = FALSE;
					break;
			}	
		}
		
		//Speed limit: Slow the animation down to 60 frames per second.
		while (L > (TickCount() - SPEED_LIMIT))
			;
	}
}




//De-allocate like a madman.
void GAME::cleanup()
{
	SATSoundShutup();
	GlobalTool->fade_in();	//just to be sure
	delete the_arena;
	delete GlobalTool;
	delete GlobalData;
}
