/* Spiked 2.0
	Chuck Groom
	
	The game_arena is the coolest thing since sliced bread. This is the heart of Spiked.
	
	When created, the Game arena grabs the pict resource of the spiffy game background, 
	puts a spike in the middle of screen, and adds the players near the edges. It also
	maintains each player's items and life counters.
	
	The run_frame function uses key_events to control the players, adjusts items, checks
	if sprites had collided, and animates a frame. */
	
	
#include "SAT.h"
#include "arena.h"
#include "sprite.h"
#include "global.h"
#include "sprite_manager.h"
#include "prefs.h"
#include "time.h"

//--------------------------------------
//	Initialize the arena
//--------------------------------------
game_arena::game_arena(Event_Stack * q) : arena(q)
{

	GlobalTool->menus_game_mode();
	
	//Spring cleaning
	green_player = red_player = green_item = spike_sprite = NULL;
	r_life = g_life = NULL;
	
	//Start w/three lives.
	green_lives = red_lives = 3;	
	
	//The mouse will show for a few frames
	mouse_hide_count = 20;
	GetMouse(&last_mouse_point);
	
	//Make sure no sprites are alive -- at all! We don't like memory leaks.
	while (gSAT.sRoot != nil)
		SATKillSprite(gSAT.sRoot);
	theStack->flush();
		
	//Change the background
	GlobalTool->set_background(GAME_PICT);
	SATRedraw();
	
	//For some reason, you have to do it twice
	GlobalTool->set_background(GAME_PICT);
	SATRedraw();
	//Create all the starting sprites 
	fPoint sprite_loc; 
	
	//The icons which represent the players' items
	sprite_loc.h = 271; sprite_loc.v = 2;
	green_item = new p_item(sprite_loc, s_manager, &green_lives);
	s_manager->add_sprite(green_item);
	
	sprite_loc.h = 594; sprite_loc.v = 2;
	red_item = new p_item(sprite_loc, s_manager, &red_lives);
	s_manager->add_sprite(red_item);
	
	//The icons which represent the player's lives
	sprite_loc.h = 58; sprite_loc.v = 4;
	g_life = new p_life(sprite_loc, s_manager, &green_lives);
	s_manager->add_sprite(g_life);
	
	sprite_loc.h = 382; sprite_loc.v = 4;
	r_life = new p_life(sprite_loc, s_manager, &red_lives);
	s_manager->add_sprite(r_life);
	
	//The spike is in the center.
	sprite_loc.h = GAME_WIDTH/2; sprite_loc.v = GAME_HEIGHT/2;
	spike_sprite = new spike(sprite_loc, s_manager);
	s_manager->add_sprite(spike_sprite);
	
	// The two players
	sprite_loc.h = 50; sprite_loc.v = 82;
	green_player = new green_p(sprite_loc, s_manager, (p_item *)green_item);
	s_manager->add_sprite_in_region(green_player);
	
	sprite_loc.h = GAME_WIDTH - 50; sprite_loc.v = GAME_HEIGHT - 50;
	red_player = new red_p(sprite_loc, s_manager, (p_item *)red_item);
	s_manager->add_sprite_in_region(red_player);
	
	populate();			//Fill with rocks (bricks)
	SATRun(TRUE);		//Animate a frame
	GlobalTool->fade_in();
}




//--------------------------------------
//	The populate function, called when the arena is created, fills the screen with
//  a few rocks -- bricks -- which are either floating around randomly, or are 
//  close to the spike.
//--------------------------------------
void game_arena::populate(void)
{
	fPoint sprite_loc;
	
	while (s_manager->num_of_sprites() < NORMAL_NUM_SPRITE - 2)
	{
		if (random(3))		//33%  are put randomly about the screen
		{
			sprite_loc.h = random(GAME_WIDTH - 100) + 50;
			sprite_loc.v = random(GAME_HEIGHT - 125) + 85;	
		}
		else				//66% are put around the spike
		{
			sprite_loc.h = spike_sprite->position.h; 
			sprite_loc.v = spike_sprite->position.v;
		}
		s_manager->add_sprite_in_region(new brick(sprite_loc, s_manager));
	}
}


//--------------------------------------
// Between rounds, we want the spike to stop moving, and each character to exist again if
// they have died.
//--------------------------------------
void game_arena::prepare_round(void)
{
	fPoint sprite_loc;
	
	spike_sprite->velocity.h = spike_sprite->velocity.v = 0;
	if (green_player == NULL)
	{
		sprite_loc.h = 50; sprite_loc.v = 82;
		green_player = new green_p(sprite_loc, s_manager, (p_item *)green_item);
		s_manager->add_sprite_in_region(green_player);
	}
	if (red_player == NULL)
	{
		sprite_loc.h = GAME_WIDTH - 50; sprite_loc.v = GAME_HEIGHT - 50;
		red_player = new red_p(sprite_loc, s_manager, (p_item *)red_item);
		s_manager->add_sprite_in_region(red_player);
	}
}




//--------------------------------------
//	Every frame, there is a chance that a rift in space will open and a new sprite will
// 	pop into being. New sprites are either gifts or rocks -- the rift creates these sprites.
//  The fewer sprites that are on screen, the more likely a rift will open up. Ideally, 
//  the screen will have the const NORMAL_NUM_SPRITE sprites floating about.
//--------------------------------------
void game_arena::rift_create(void)
{
	//An exponential function makes sure that we will have just enough sprites on screen.
	float prob = (pow(1.5, s_manager->num_of_sprites() - NORMAL_NUM_SPRITE));
	prob = prob * FREQ_NEW_SPRITE;
	if (! random((int) prob))
		{
			fPoint rift_location;
			rift_location.v = random(GAME_HEIGHT - 125) + 85;
			rift_location.h = random(GAME_WIDTH - 80) + 40;
			s_manager->add_sprite_in_region(new rift (rift_location, s_manager));
		}
}







/*--------------------------------------
 	The game_arena run_frame is that most interesting loop in Spiked.
 	How does it work? Look at this psuedo-code explaination:
 	
 	if both characters are alive
 		One by one, grab each key_down event and translate its meaning. Feed this to the
 		appropriate sprite, to change its action.
 		
 		Cycle through the sprites, letting each sprite maintain its behaviors.
 		Check to see if any collisions have occured.
 		Check to see if either of the players has died this round
 		Animate -- show whatever happened on screen 
 		See if a rift is created
 	else (someone died)
 		adjust life counters
 		see if the game is over -- is it necessary to return to the intro arena?
 		prepare a new round. Both players are now alive
  --------------------------------------*/
void game_arena::run_frame(void)
{
	long L = TickCount();
	
	if (green_player && red_player)		//If both players are alive
	{
		theEvent = theStack->peek();
		// Loop: grab all the mouse, key events of the round. This way, in a single round
		// the two players can accelerate, turn, and fire at the same time.
		while((theEvent) && ((theEvent->what == GmouseDown) || (theEvent->what == GkeyDown)))
		{
			theEvent = theStack->pop();	
			if(theEvent->what == GkeyDown)
			{
				char the_key = theEvent->key;
				KEY_TYPE command = GlobalData->the_prefs.interpret_key(the_key);
				
				switch (command)
				{
					case GREEN_LEFT:
						green_player->do_sprite_action(TURN_LEFT);
						break;
					case GREEN_RIGHT:
						green_player->do_sprite_action(TURN_RIGHT);
						break;
					case GREEN_THRUST:
						green_player->do_sprite_action(THRUST);
						break;
					case GREEN_ITEM:
						green_player->do_sprite_action(ITEM);
						break;
					case GREEN_CHANGE_ITEM:
						green_player->do_sprite_action(CHANGE_ITEM);
						break;
					case RED_LEFT:
						red_player->do_sprite_action(TURN_LEFT);
						break;
					case RED_RIGHT:
						red_player->do_sprite_action(TURN_RIGHT);
						break;
					case RED_THRUST:
						red_player->do_sprite_action(THRUST);
						break;
					case RED_ITEM:
						red_player->do_sprite_action(ITEM);
						break;
					case RED_CHANGE_ITEM:
						red_player->do_sprite_action(CHANGE_ITEM);
						break;
				}
					
			}
		
		delete theEvent;
		theEvent = theStack->peek();
		}
		
		s_manager->run_behaviors();
		s_manager->collision_check();
		
		if (!(green_player->alive))
		{
			green_player->sprite::do_behavior();
			delete green_player;
			green_player = NULL;
		}
		if (!(red_player->alive))
		{
			red_player->sprite::do_behavior();
			delete red_player;
			red_player = NULL;
		}
		
		//Speed limit
		while (L > (TickCount() - SPEED_LIMIT))
					;
					
		//Hide the mouse if it has been inactive for a while
		maintain_mouse();	
		
		//RUN THE ANIMATION
		SATRun(TRUE);
		
		//Randomly create a rift in space
		rift_create(); 
	}
	else	//Someone died
	{
		if(green_player == NULL)
			green_lives--;
		if(red_player == NULL)
			red_lives--;
		
		if((red_lives < 1) || (green_lives < 1))	//Game over
		{
			//Make a winner!
			if(red_lives < green_lives)
			{
				GlobalData->write_result(GREEN_WON);
			}
			else
			{
				GlobalData->write_result(RED_WON);
			}
			
			//Finish running the explosion, but without runnning collisions
			for (int counter = 0; counter < 20; counter++)
			{
				L = TickCount();
				s_manager->stop_action();
				s_manager->run_behaviors();
				SATRun(TRUE);
				
				while (L > (TickCount() - SPEED_LIMIT))
					;
			}
			
			//De-allocate!
			delete spike_sprite;
			delete red_player;
			delete green_player;
			
			//Create a new event, telling the main game loop that this game is over.
			theStack->push(new Game_Event(Gpost_game));
			
			return;
		}
		
		//Otherwise, we're all set for another round. Make sure everything is clean.
		s_manager->run_behaviors();
		prepare_round();
	}
}


//--------------------------------------------------
// If the mouse isn't moved for more than 20 frames, then hide it. 
// ------------------------------------------------
void game_arena::maintain_mouse(void)
{
	if (mouse_hide_count)
	{
		ShowCursor();
		mouse_hide_count--;
		
		if (mouse_hide_count == 0)
			HideCursor();
	}
	
	Point current_point;
	GetMouse(&current_point);
	
	//Show mouse, reset counter
	if ( !((current_point.h == last_mouse_point.h) && (current_point.v == last_mouse_point.v)))
	{
		ShowCursor();
		mouse_hide_count = 20;
	}
	
	last_mouse_point = current_point;
}




//----------------------------------------------
// As we close this arena, make sure the mouse is visible.
//-----------------------------------------------
game_arena::~game_arena()
{
	ShowCursor();
	ShowCursor();
}
