/* Spiked 2.0
	Chuck Groom, 1997
	
	A green player sprite. */
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"


green_p::green_p(fPoint p, sprite_manager * s_m, p_item * p_i) : player(p, s_m, p_i)
{
	myType = GREEN_PLAYER;
	load_faces();
}

