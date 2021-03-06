/*	Spiked 2.0
	Chuck Groom
	
	Arena class
	Intro_arena derived class
	
	
	Arenas are the where the player dukes it out. Each arena has a list of sprites that it
	animates. Each arena must initialize a new background, set up sprites for animation, and 
	handle events appropriatly. The main call to any arena is the run_frame function, which
	is called once every 20th of a second by the Game loop. The arena must position the sprites,
	change their behavior, and do house-cleaning each iteration. */

#include "SAT.h"
#include "arena.h"
#include "sprite.h"
#include "global.h"
#include "sprite_manager.h"

//--------------------------------------
//	The parent arena class stores the common data used by the derived classes
//--------------------------------------
arena::arena(Event_Stack * q)
{
	theStack = q;
	s_manager = new sprite_manager;
	theEvent = NULL;
}

//--------------------------------------
//	Clean up
//--------------------------------------
arena::~arena()
{
	delete s_manager;
	delete theEvent;
}







/* ----------------------------------------------
	The intro_arena class.
	
	This class presents the user with the splash screen. It maintains three buttons, which
	let the user open the game_arena, the pref_arena, or info about the game. 
-------------------------------------------------*/

//--------------------------------------
//	Constructor: sets up a new background, creates new sprites
//--------------------------------------
intro_arena::intro_arena(Event_Stack * q) : arena(q)
{
	new_button_sprite = pref_button_sprite = about_button_sprite = NULL;
	
	GlobalTool->fade_out();
	
	//Make sure the cursor is showing
	ShowCursor();
	
	
	//Change the Background
	GlobalTool->set_background(INTRO_PICT);
	SATRedraw();
	
	//For some reason, this must be done twice
	GlobalTool->set_background(INTRO_PICT);
	SATRedraw();
	
	
	GlobalData->menu_showing = TRUE;
	GlobalTool->menus_intro_mode();
	GlobalTool->set_offset();

	
	//Create the two sprites
	fPoint sprite_loc; sprite_loc.h = 50; sprite_loc.v = 168;
	new_button_sprite = new new_button(sprite_loc, s_manager, theStack);
	sprite_loc.h = 50; sprite_loc.v = 277;
	pref_button_sprite = new pref_button(sprite_loc, s_manager, theStack);
	sprite_loc.h = 50; sprite_loc.v = 386;
	about_button_sprite = new about_button(sprite_loc, s_manager, theStack);
	
	
	// Add them to the Sprite_Manager
	s_manager->add_sprite(new_button_sprite);
	s_manager->add_sprite(pref_button_sprite);
	s_manager->add_sprite(about_button_sprite);
	
	// Paint the screen with the background, the sprites
	SATRun(TRUE);
	SATRedraw();
	
	GlobalTool->fade_in();
}





//--------------------------------------
//	The heart of the intro_arena. Basically, all this does is animate via a call to SATRun(TRUE)
//  and check to see if the user pushed the a button. The buttons automatically generate the
//  calls that tell the main game loop to terminate the intro_arena.
//--------------------------------------
void intro_arena::run_frame(void)
{
	sprite * theSprite = NULL;

	theEvent = theStack->peek();
	
	while((theEvent) && ((theEvent->what == GmouseDown) || (theEvent->what == GkeyDown)))
	{
		theEvent = theStack->pop();
		switch(theEvent->what)
		{
			case GmouseDown:
				fPoint p; 
				p.h = theEvent->where.h + GlobalData->h_offset;
				p.v = theEvent->where.v + GlobalData->v_offset;
				theSprite = s_manager->sprite_in_range(p, 1);
				if (theSprite)
					theSprite->do_clicked();
				break;
			case GkeyDown:
				char the_key = theEvent->key;
				switch (the_key)
				{
					case 'n':
						new_button_sprite->do_clicked();
						break;
					case 'p':
						pref_button_sprite->do_clicked();
						break;
					case 'a':
						about_button_sprite->do_clicked();
						break;
				}
		}
		delete theEvent;
		theEvent = theStack->peek();
	}
	
	// Let sprites do their business
	s_manager->run_behaviors();
	
	// Animate!
	SATRun(TRUE);
}
