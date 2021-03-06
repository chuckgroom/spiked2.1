/* Spiked 2.0
	Chuck Groom, 1997
	
	An about game button sprite. */
	
#include "sprite.h"
#include "SAT.h"
#include "global.h"
#include "EventArbitrator.h"


about_button::about_button(fPoint p, sprite_manager * s_m, Event_Stack * q) : button_sprite(p, s_m, q)
{
	myType = ABOUT_BUTTON;
	load_faces();
	cur_face = 0;
	draw_radius = collide_radius = 29;
}


//--------------------------------
// If the button has been clicked, its button_mode != 0, and it has a different face to indicate
// that it has been clicked. 
//--------------------------------
void about_button::do_behavior()
{
	if(button_mode != 0)
	{
		cur_face = 1;
		button_mode++;
		if(button_mode == 5)
		{
			//if the button has been pressed long enough, then create a new_game event
			theStack->push (new Game_Event(Gabout_arena));
		}
		
	}
	button_sprite::do_behavior();
}



void about_button::do_clicked(void)
{
	button_mode = 1;
	cur_face = 1;
}

