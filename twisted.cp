/* Spiked 2.0
	Chuck Groom, 1997
	
	A sprite hit by a twister has a nimbus floating around it.*/
	
#include "sprite.h"

twisted::twisted(fPoint p, sprite_manager * s_m, sprite * other_sprite) : sprite(p, s_m)
{
	myType = TWISTED;
	load_faces();
	aflicted_sprite = other_sprite;
	cur_face = 0;
	count_up = TRUE;
	draw_radius = 22;
}


void twisted::do_behavior(void)
{
	//count up, then down the number of faces
	if (count_up)
	{
		cur_face++;
		if (cur_face == num_of_faces)
		{
			count_up = FALSE;
			cur_face--;
		}
	}
	else
	{
		cur_face--;
		if (cur_face == -1)
		{
			count_up = TRUE;
			cur_face++;
		}
	}
	
	//Conditions for folding up: other sprite doesn't exist,
	//the other sprite has been de-allocated messily, or the other sprite no longer is twisted.
	if((!aflicted_sprite) || (aflicted_sprite->alive == FALSE))
		alive = FALSE;
	
	if((aflicted_sprite->position.h < 0) || (aflicted_sprite->position.v < 0)
		|| (aflicted_sprite->position.h > GAME_WIDTH) || (aflicted_sprite->position.v > GAME_HEIGHT))
		alive = FALSE;
	
	if (aflicted_sprite->thrust_disable < 5)
		alive = FALSE;
		
	position.h = aflicted_sprite->position.h;
	position.v = aflicted_sprite->position.v;
	
	sprite::do_behavior();
}
