// Chuck Groom. Spiked 2.0. 1997
// The game class header.

#ifndef GAME_H
#define GAME_H


#include "arena.h"
#include "EventArbitrator.h"
#include "OSEvent.h"
#include "Menus.h"

//The kinds of arenas
enum arena_type {intro = 0, pref, game, about, post_game};


class GAME
{
	private:
		OSEvent * OSEventHandler;
		Event_Stack  the_Stack;
		
		Boolean is_running;
		
		arena_type	the_arena_type;		//The type of arena in use
		arena 		*the_arena;			//The current arena
		
		Game_Menu	*the_game_menus;	
			
	public:
		GAME();
		void run();
		void cleanup();
};



#endif
