/* Chuck Groom
	Spiked 2.0
	1997
	
	The Sprite Manager
	
	This is the backbone of sprite manangement. It is a giant array of sprite pointers, that
	keeps track of every sprite that exists. Every iteration of the game, the sprite manager
	is told to run_behaviors() -- that is, cycle through every sprite in the array and order
	it to invoke its do_behavior() function. In this way, each sprite it responsible for its
	own behavior, but is prompted by the sprite manager.
	
	The sprite manager also preforms collision detection. If two sprites can and do collide,
	their do_collison() calls are invoked on both sprites. 
	
	You can add a sprite, delete a sprite, and check to see if a point is in any sprite in
	the list. */



#include "sprite.h"
#include "sprite_manager.h"
#include "global.h"
#include "stdlib.h"


//-------------------------------------------------
//Constructor: create the list of sprite pointers
//-------------------------------------------------
sprite_manager::sprite_manager()
{
	sprite_list = new sprite*[MAX_SPRITES];		//an array of MAX_SPRITES sprite pointers
	top = 0;
	
	for(int kk = 0; kk < MAX_SPRITES; kk++)
	{
		sprite_list[kk] = NULL;					//Clean out the array, to be safe.
	}
}

//-------------------------------------------------
//Destructor: Destroy the all sprites
//-------------------------------------------------
sprite_manager::~sprite_manager()
{
	for (int counter = 0; counter < top; counter++)
		delete sprite_list[counter]; 				//Delete my data
	
	delete sprite_list;	
		
	//Make sure that the Sprite Animation Toolkit is also empty
	while (gSAT.sRoot != nil)
		SATKillSprite(gSAT.sRoot);
}



//-------------------------------------------------
//run_behaviors calls do_behavior for all sprites. It also culls out dead sprites.
//-------------------------------------------------
void sprite_manager::run_behaviors(void)
{
	for (int counter = 0; counter < top; counter++)
	{
		sprite_list[counter]->do_behavior();
		if(!sprite_list[counter]->alive)	//If this sprite died
		{
			delete_sprite(sprite_list[counter]);	//Eliminate it
		}
	}
}




//-------------------------------------------------
//collision_check checks for collisions between all sprites in the list. It invokes both
//sprites' collision routines in the case where the sprites collide. It is an o(n^2) algorithm.
//-------------------------------------------------
void sprite_manager::collision_check(void)
{
	if (top < 2) //if there aren't two sprites to collide
		return;
		
	for (int kk = 0; kk < top - 1; kk++)
	{
		for (int mm = kk + 1; mm < top; mm++)
		{
			if(sprite_list[kk]->collide(sprite_list[mm]))
			{
				sprite_list[kk]->do_collision(sprite_list[mm]);
				sprite_list[mm]->do_collision(sprite_list[kk]);
			}
		}
	}
}



//-----------------------------------------------
// Stop action is used at the end of the game to stop everyone so that we can run the animation
// frames of the last explosion without funny things happening on screen.
//------------------------------------------------
void sprite_manager::stop_action(void)
{
	for (int kk = 0; kk < top; kk++)
	{
		sprite_list[kk]->velocity.h = 0;
		sprite_list[kk]->velocity.v = 0;
	}
}




//-------------------------------------------------
//Add a new sprite at the top of the list
//-------------------------------------------------
void sprite_manager::add_sprite(sprite * new_sprite)
{
	if(!new_sprite)	//if the sprite doesn't exits
		return;
		
	if (top > MAX_SPRITES - 1)	//If the list of sprites is full to overflowing -- this should never be true
		exit(1);
	sprite_list[top] = new_sprite;
	top++;
	sprite_list[top] = NULL;
}


//-------------------------------------------------
// Often times, we want to add a sprite at a given point, but aren't sure if another sprite is
// there. This lets you say that you want a sprite in a region, but let the sprite manager
// shuffle it around to an open area.
//-------------------------------------------------
void sprite_manager::add_sprite_in_region(sprite * new_sprite)
{
	if (!new_sprite)
		return;
		
	for (int mm = 0; mm < top; mm++)
	{
		if(sprite_list[mm]->collide(new_sprite))
		{
			//If there would be a collision, move the new sprite. I jiggle around randomly until
			//it finds a home. 
			do {
				new_sprite->position.h += random(30) - 20;
				new_sprite->position.v += random(30) - 20;
				
				//Check screen boundary
				if ((new_sprite->position.h - new_sprite->draw_radius) < 0)
					new_sprite->position.h = new_sprite->draw_radius;
				if ((new_sprite->position.h + new_sprite->draw_radius) > GAME_WIDTH)
					new_sprite->position.h = GAME_WIDTH - new_sprite->draw_radius;
				if ((new_sprite->position.v - new_sprite->draw_radius) < 52)
					new_sprite->position.v = new_sprite->draw_radius + 52;
				if ((new_sprite->position.v + new_sprite->draw_radius) > GAME_HEIGHT)
					new_sprite->position.v = GAME_HEIGHT - new_sprite->draw_radius;
			} while (sprite_list[mm]->collide(new_sprite));
	
			//Start over
			mm = 0;
		}
	}
	
	//Now, add this sprite to the game.
	add_sprite(new_sprite);
}



//-------------------------------------------------
//Return the index of a sprite: -1 if not found.
//-------------------------------------------------
int sprite_manager::sprite_index(sprite * search_sprite)
{
	if(!search_sprite)
		return -1;
		
	for (int counter = 0; counter < top; counter++)
	{
		if (sprite_list[counter] == search_sprite)
			return counter;
	}
	return -1;
}
	


//-------------------------------------------------
//Delete a sprite from the array, and fill in the gap
//-------------------------------------------------
void sprite_manager::delete_sprite(sprite * doomed_sprite)
{
	int index = sprite_index(doomed_sprite);
	
	if (index == -1) //sprite isn't in the list
	 	return;	
	
	//Kill that sprite.
	delete sprite_list[index]; 						//Delete my data 
	sprite_list[index] = NULL;

	//Next, fill in the gap in the array appropraitly.
	if (index == top - 1)	//If the sprite was at the top of the list, life is simple.
	{
		top--;
		return;
	}
	else
	{
		sprite_list[index] = sprite_list[top - 1];		//move the top item to fill gap.
		sprite_list[top-1] = NULL;
		top--;
	}
}





//-------------------------------------------------
// To check if a point (and radius) fall within any sprites on screen, invoke the circle_in_sprite
// function of all sprites in the list.
//-------------------------------------------------
sprite * sprite_manager::sprite_in_range(fPoint p, int r)
{
	for (int kk = 0; kk < top; kk++)
	{
		if(sprite_list[kk]->circle_in_sprite(p, r))
		{
			return sprite_list[kk];
		}
	}
	return NULL;
}
