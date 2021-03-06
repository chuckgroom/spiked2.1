/* Spiked 2.0
	Chuck Groom, 1997
	
	Explosions are the simplist of sprites. They cycle through their frames, then die.*/
	
#include "sprite.h"

explosion::explosion(fPoint p, sprite_manager * s_m) : sprite(p, s_m)
{
	myType = EXPLOSION;
	load_faces();
	cur_face = 0;
	draw_radius = 20;
}


void explosion::do_behavior(void)
{
	cur_face++;
	if (cur_face == num_of_faces)
	{
		cur_face--;
		alive = FALSE; 	//Explosion is done -- erase sprite.
	}
	sprite::do_behavior();
}
