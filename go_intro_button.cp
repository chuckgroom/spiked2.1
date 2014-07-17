/* Spiked 2.0
	Chuck Groom, 1997
	
	A go-intro button sprite. */
	
#include "sprite.h"
#include "SAT.h"
#include "global.h"
#include "EventArbitrator.h"


go_intro_button::go_intro_button(fPoint p, sprite_manager * s_m, Event_Stack * q) : button_sprite(p, s_m, q)
{
	myType = GO_INTRO_BUTTON;
	load_faces();
	cur_face = 0;
	draw_radius = collide_radius = 29;
}


void go_intro_button::do_behavior()
{
	if(button_mode != 0)
	{
		//if the button has been pressed, create a go to intro_arena event
		theStack->push (new Game_Event(Gintro_arena));
	}
	button_sprite::do_behavior();
}


void go_intro_button::do_clicked(void)
{
	button_mode = 1;
}

