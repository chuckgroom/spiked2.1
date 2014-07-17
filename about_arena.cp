/*	Spiked 2.0
	Chuck Groom
	
	The about arena displays info about how to play the silly game.  */

#include "arena.h"
#include "sprite.h"
#include "global.h"
#include "sprite_manager.h"
#include "time.h"

//------------------------------------
//	When created, the about_arena draws the about pict, and creates a button to
//  return to the main screen. In addition, there are many small sprites that just flit about,
//  animating themselves.
//------------------------------------
about_arena::about_arena(Event_Stack * q) : arena(q)
{
	//Clean up pointers
	go_intro_button_sprite = NULL;
	
	//Make sure the cursor is showing
	ShowCursor();
	
	GlobalTool->fade_out();
	GlobalTool->menus_intro_mode();
	
	//Change the background -- twice, to be safe.
	GlobalTool->set_background(ABOUT_PICT);
	GlobalTool->set_background(ABOUT_PICT);
		
	fPoint sprite_loc;
	
	//Create all the dumbly animating display sprites
	sprite_loc.h = 186; sprite_loc.v = 225;
	s_manager->add_sprite(new gen_all_sprite(sprite_loc, s_manager, EXPLOSION));
	
	sprite_loc.h = 10; sprite_loc.v = 125;
	s_manager->add_sprite(new gen_all_sprite(sprite_loc, s_manager, GREEN_PLAYER));
	
	sprite_loc.h = 240; sprite_loc.v = 125;
	s_manager->add_sprite(new gen_all_sprite(sprite_loc, s_manager, RED_PLAYER));
	
	sprite_loc.h = 330; sprite_loc.v = 370;
	s_manager->add_sprite(new gen_all_sprite(sprite_loc, s_manager, RIFT));
	
	sprite_loc.h = 212; sprite_loc.v = 215;
	s_manager->add_sprite(new gen_all_sprite(sprite_loc, s_manager, SPIKE));
	
	sprite_loc.h = 370; sprite_loc.v = 230;
	s_manager->add_sprite(new gen_all_sprite(sprite_loc, s_manager, TWISTER));
	
	sprite_loc.h = 370; sprite_loc.v = 260;
	s_manager->add_sprite(new gen_all_sprite(sprite_loc, s_manager, GRAVITRON));
	
	sprite_loc.h = 22; sprite_loc.v = 367;
	s_manager->add_sprite(new gen_all_sprite(sprite_loc, s_manager, BRICK));
	
	
	//Make the button which allows the user to return to the intro screen
	sprite_loc.h = 605; sprite_loc.v = 427;
	go_intro_button_sprite = new go_intro_button(sprite_loc, s_manager, theStack);
	s_manager->add_sprite(go_intro_button_sprite);
	
	// Make the screen all nice and ready
	SATRedraw();
	SATRun(TRUE);
	
	GlobalTool->fade_in();
}


// ------------------------------------------------------------------------------
// This is the about loop. It exits when the
// user either chooses a new game or clicks on the "Back to Intro" button.
// -------------------------------------------------------------------------------
void about_arena::run_frame(void)
{
	long L = TickCount();
	
	sprite * theSprite = NULL;
	theEvent = theStack->peek();
	while((theEvent) && ((theEvent->what == GmouseDown) || (theEvent->what == GkeyDown)))
	{
		theEvent = theStack->pop();
			
		// Check to see if the user clicked in the sprite. If so, tell the sprite
		// about it.
		fPoint p; 
		p.h = theEvent->where.h + GlobalData->h_offset;
		p.v = theEvent->where.v + GlobalData->v_offset;
		theSprite = s_manager->sprite_in_range(p, 1);
		if (theSprite)
		{
			theSprite->do_clicked();
		}
		delete theEvent;
		
		theEvent = theStack->peek();
	}
	s_manager->run_behaviors();
	SATRun(TRUE);
	
		//Speed limit
	while (L > (TickCount() - SPEED_LIMIT))
		;
}
