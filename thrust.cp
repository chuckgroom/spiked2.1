/* Spiked 2.0
	Chuck Groom, 1997
	
	The thrust sprite lingers behind the player.*/
	
#include "sprite.h"
#include "math.h"

thrust::thrust(fPoint p, sprite_manager * s_m, fPoint *pPos, float *pTh) : sprite(p, s_m)
{
	myType = PLAYER_THRUST;
	load_faces();
	cur_face = 0;
	draw_radius = 5;
	active = FALSE;
	
	pPlayerPos = pPos;
	pPlayerTheta = pTh;
}



void thrust::do_behavior(void)
{
	if(active)
	{
		cur_face = 1;
		active = FALSE;
		position.h = pPlayerPos->h - 10*cos(*pPlayerTheta + .104);
		position.v = pPlayerPos->v - 10*sin(*pPlayerTheta + .104);
	}
	else
		cur_face = 0;
	
	sprite::do_behavior();
}



void thrust::set_condition(Boolean is_thrust)
{
	active = is_thrust;
}

