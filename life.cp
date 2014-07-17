/* Spiked 2.0
	Chuck Groom, 1997
	
	A sprite that displays the number of lives for each player.*/
	
#include "sprite.h"
#include "math.h"

p_life::p_life(fPoint p, sprite_manager * s_m, int * ll) : sprite(p, s_m)
{
	myType = LIFE;
	load_faces();
	cur_face = 0;
	draw_radius = collide_radius = 0;
	lives = ll;
}


// The face 0 means 1 life, face 1 means 2 lives, ... and face 5 means all lives >= 5.
void p_life::do_behavior(void)
{
	if (*lives > 5)
		cur_face = 5;
	else
		cur_face = (*lives - 1) % num_of_faces;
	
	if(cur_face < 0)
		cur_face = 0;
		
	sprite::do_behavior();
}
