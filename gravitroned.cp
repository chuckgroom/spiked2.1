/* Spiked 2.0
	Chuck Groom, 1997
	
	A sprite hit by a gravitron has a nimbus floating around it.*/
	
#include "sprite.h"
#include "math.h"
#include "global.h"

gravitroned::gravitroned(fPoint p, sprite_manager * s_m, player *the_player, mobile_sprite * other) : sprite(p, s_m)
{
	myType = GRAVITRONED;
	load_faces();
	cur_face = 0;
	draw_radius = 22;
	
	the_owner = the_player;
	afflicted_sprite = other;
	life_time = GRAVITRONED_FRAMES; //How many frames the gravitron will last. 
}


void gravitroned::do_behavior(void)
{
	life_time--;
	cur_face++;
	cur_face = cur_face % num_of_faces;
	
	//Conditions for this puppy to be alive: the owner and afflicted sprites are alive, and the
	//life_time is valid
	
	if (life_time < 1)
		alive = FALSE;
		
	if((!afflicted_sprite) || (afflicted_sprite->alive == FALSE))
		alive = FALSE;
	if((!the_owner) || (the_owner->alive == FALSE))
		alive = FALSE;
	
	if((afflicted_sprite->position.h < 0) || (afflicted_sprite->position.v < 0)
		|| (afflicted_sprite->position.h > GAME_WIDTH) || (afflicted_sprite->position.v > GAME_HEIGHT))
		alive = FALSE;
	
	position.h = afflicted_sprite->position.h;
	position.v = afflicted_sprite->position.v;

	//Pull the afflicted sprite towards the player. To do this, we need an angle and the 
	//distance between the two.
	
	float dx = the_owner->position.h - afflicted_sprite->position.h;
	float dy = the_owner->position.v - afflicted_sprite->position.v;
	
	float theta;
	if(dx)
		theta = atan(dy/dx);
	else
		theta = pi/2;
		
	//Decide which direction the arctan points us
	if((dx < 0) && (dy < 0))
		theta += pi;
	if((dx < 0) && (dy > 0))
		theta += pi;
	if((dx == 0) && (dy < 0))
		theta += pi;
		
		
	float dist_sqrd =  dx*dx + dy*dy;
	
	float force = 300/dist_sqrd + GRAV;
	if (force > 2) force = 2;
	
	//If the_owner and the afflicted_sprite are too close, and they are too close to the wall,
	//problems can result b/c the source of the gravity is smashed into the wall
	if (sqrt(dist_sqrd) < 90)	// Too close
	{
		 if((the_owner->position.h < 50) || (the_owner->position.h > (GAME_WIDTH - 50)))
		 	force = .1;
		  if((the_owner->position.v < 100) || (the_owner->position.h > (GAME_HEIGHT - 50)))
		 	force = .1;
	}
	
	
	afflicted_sprite->acceleration.h += force * cos(theta);
	afflicted_sprite->acceleration.v += force * sin(theta);
	
	sprite::do_behavior();
}
