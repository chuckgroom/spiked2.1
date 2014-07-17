/* Spiked 2.0
	Chuck Groom, 1997
	
	A red player sprite. */
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"


red_p::red_p(fPoint p, sprite_manager * s_m, p_item * p_i) : player(p, s_m, p_i)
{
	myType = RED_PLAYER;
	load_faces();
}
