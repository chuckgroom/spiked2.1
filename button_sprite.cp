/* Spiked 2.0
	Chuck Groom, 1997
	
	A button sprite. This is the parent class for all buttons. */
	
#include "sprite.h"
#include "SAT.h"
#include "global.h"
#include "EventArbitrator.h"

button_sprite::button_sprite(fPoint p, sprite_manager * s_m, Event_Stack * q) : sprite(p, s_m)
{
	theStack = q;
	button_mode = 0;
}


void button_sprite::do_behavior(void)
{
	sprite::do_behavior();
}
