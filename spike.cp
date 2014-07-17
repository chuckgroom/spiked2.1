/* Spiked 2.0
	Chuck Groom, 1997
	
	The spike. It is the big, nasty, pointy thing. All it does is sit there, quietly animating.*/
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"


spike::spike(fPoint p, sprite_manager * s_m) : mobile_sprite(p, s_m)
{
	myType = SPIKE;
	load_faces();
	cur_face = 0;
	draw_radius = 30;
	collide_radius = 27;
	mass = 4000; //very heavy
}


void spike::do_behavior(void)
{
	//Cycle through frames of animation.
	cur_face++;
	cur_face = cur_face % num_of_faces;
	mobile_sprite::do_behavior();
}


