/*	Spiked 2.0
	Chuck Groom
	
	The pref_arena shows the user a brief manual for game play, and lets them change the pref
	keys. To do this, the user can click in one of the eight buttons floating around the screen.
	This actives that button -- it starts flashing -- and de-activates all other buttons.
	The user can then press a key to change the corresponding preference of what action that
	key represents.
	
	How is this possible? Well, each of the eight buttons is linked to the type of control 
	it represents. When a key is pressed, we grab the type of control it represents, ask the
	prefs. to change that key, and draw this key on the screen. */

#include "SAT.h"
#include "arena.h"
#include "sprite.h"
#include "global.h"
#include "sprite_manager.h"

//------------------------------------
//	When created, the pref_arena draws the pref pict, and creates nine buttons: 8 are
//  buttons corresponding to the prefs, and the other button lets the user return to the
//  intro screen.
//------------------------------------
pref_arena::pref_arena(Event_Stack * q) : arena(q)
{
	//Clean up pointers
	go_intro_button_sprite = NULL;
	for (int ii = 0; ii < 10; ii++)
		 key_buttons[ii] = NULL;
	selected_key_button = NULL;

	//Make sure the cursor is showing
	ShowCursor();
	
		
	GlobalTool->fade_out();
	GlobalTool->menus_intro_mode();
	
	
	//Change the background
	GlobalTool->set_background(PREF_PICT);
	GlobalTool->set_background(PREF_PICT);
		
	fPoint sprite_loc;
	//Make all eight of the key_button sprites for setting the prefs
	// The green pref buttons
	sprite_loc.h = 432; sprite_loc.v = 235;
	key_buttons[0] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, GREEN_LEFT, GlobalData->the_prefs.command_is_key(GREEN_LEFT));
	sprite_loc.h = 401; sprite_loc.v = 438;
	key_buttons[1] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, GREEN_RIGHT, GlobalData->the_prefs.command_is_key(GREEN_RIGHT));
	sprite_loc.h = 522; sprite_loc.v = 338;
	key_buttons[2] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, GREEN_THRUST, GlobalData->the_prefs.command_is_key(GREEN_THRUST));
	sprite_loc.h = 509; sprite_loc.v = 249;
	key_buttons[3] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, GREEN_ITEM, GlobalData->the_prefs.command_is_key(GREEN_ITEM));
	sprite_loc.h = 584; sprite_loc.v = 249;
	key_buttons[4] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, GREEN_CHANGE_ITEM, GlobalData->the_prefs.command_is_key(GREEN_CHANGE_ITEM));

	// The red pref buttons
	sprite_loc.h = 182; sprite_loc.v = 119;
	key_buttons[5] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, RED_LEFT, GlobalData->the_prefs.command_is_key(RED_LEFT));
	sprite_loc.h = 173; sprite_loc.v = 243;
	key_buttons[6] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, RED_RIGHT, GlobalData->the_prefs.command_is_key(RED_RIGHT));
	sprite_loc.h = 243; sprite_loc.v = 201;
	key_buttons[7] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, RED_THRUST, GlobalData->the_prefs.command_is_key(RED_THRUST));
	sprite_loc.h = 63; sprite_loc.v = 286;
	key_buttons[8] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, RED_ITEM, GlobalData->the_prefs.command_is_key(RED_ITEM));
	sprite_loc.h = 138; sprite_loc.v = 286;
	key_buttons[9] = new game_key_button(sprite_loc, s_manager, theStack, key_buttons, RED_CHANGE_ITEM, GlobalData->the_prefs.command_is_key(RED_CHANGE_ITEM));

	
	//Make the button which allows the user to return to the intro screen
	sprite_loc.h = 70; sprite_loc.v = 390;
	go_intro_button_sprite = new go_intro_button(sprite_loc, s_manager, theStack);
	
	//Add all these sprites to the sprite stack
	for (int kk = 0; kk < 10; kk++)
	{
		s_manager->add_sprite((sprite *)key_buttons[kk]);
	}
	s_manager->add_sprite(go_intro_button_sprite);
	
	// Make the screen all nice and ready
	SATRedraw();
	SATRun(TRUE);
	
	GlobalTool->fade_in();
}


// ------------------------------------------------------------------------------
// This is the prefs loop. It lets the user change the prefs settings, and it exits when the
// user either chooses a new game or clicks on the "Back to Intro" button.
// -------------------------------------------------------------------------------
void pref_arena::run_frame(void)
{
	sprite * theSprite = NULL;
	theEvent = theStack->peek();
	while((theEvent) && ((theEvent->what == GmouseDown) || (theEvent->what == GkeyDown)))
	{
		theEvent = theStack->pop();
		switch(theEvent->what)
		{
			case GmouseDown:
				// Check to see if the user clicked in a button sprite. If so, tell the sprite
				// about it.
				fPoint p; 
				p.h = theEvent->where.h + GlobalData->h_offset;
				p.v = theEvent->where.v + GlobalData->v_offset;
				theSprite = s_manager->sprite_in_range(p, 2);
				if (theSprite)
				{
					if(theSprite->myType == GAME_KEY_BUTTON)
						selected_key_button = (game_key_button *) theSprite;
					theSprite->do_clicked();
				}
				break;
			case GkeyDown:
				// If the user has selected a button that indicates that they want to change 
				// a prefs setting, then change that setting if possible and write the change
				// to screen.
				if (selected_key_button)	
				{
					char the_char = theEvent->key;
					KEY_TYPE key_type = selected_key_button->get_key_type();
					GlobalData->the_prefs.set_command(key_type, the_char);
					the_char = GlobalData->the_prefs.command_is_key(key_type);
					selected_key_button->set_char(the_char);
					
				}
				break;
		}
		delete theEvent;
		theEvent = theStack->peek();
	}
	s_manager->run_behaviors();
	SATRun(TRUE);
}
