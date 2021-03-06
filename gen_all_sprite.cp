/* Spiked 2.0
	Chuck Groom, 1997
	
	The gen_all_sprite is a generic display sprite that just animates itself. */
	
#include "sprite.h"

gen_all_sprite::gen_all_sprite(fPoint p, sprite_manager * s_m, SPRITE_TYPE s_t) : sprite(p, s_m)
{
	myType = s_t;
	load_faces();
	
	//Default settings:
	simple_loop = 	TRUE;
	count_up = 		TRUE;
	frame_pause = 	2;
	frame_pause_count = 0;
	first_frame_wait =	0;
	first_frame_count =	0;
	
	//Modify defaults depending on the sprite type
	switch (myType)
	{
		case TWISTER:
			simple_loop = 	FALSE;	//loops back and forth
			break;
		case BRICK:
			frame_pause += 		random(4);
			break;
		case GREEN_PLAYER:
			count_up = 		FALSE;	//loop in CCW direction
			break;
		case EXPLOSION:
			frame_pause = 		3;	
			first_frame_wait = 	30;
			break;
		case RIFT:
			frame_pause = 		3;	
			first_frame_wait = 	20;
			break;
		default:
			break;
	}
	

	
	cur_face = 0;
	draw_radius = collide_radius = 0;	//since it isn't supposed to interact with the world at all.
}




void gen_all_sprite::do_behavior(void)
{
	//Check to see if we should skip this frame of animation
	if(frame_pause)
	{
		frame_pause_count++;
		frame_pause_count = frame_pause_count % frame_pause;
		
		if(frame_pause_count != 0)
		{
			return;
		}
	} 
	
	
	//Check to see if we should pause at the first frame of animation
	if((cur_face == 0) && (first_frame_wait))
	{
		if(first_frame_count != first_frame_wait)
		{
			first_frame_count++;
			return;
		}
		first_frame_count = 0;
	}
	
	
	//Move the current face up or down
	if(count_up)
		cur_face++;
	else
		cur_face--;
	
	
	if(simple_loop)
	{
		if(cur_face < 0)
			cur_face = num_of_faces - 1;
		
		cur_face = cur_face%num_of_faces;
	}
	else 	//we're looping back and forth
	{
		if(cur_face < 0)
		{
			cur_face = 0;
			count_up = TRUE;
		}
		if (cur_face > num_of_faces - 1)
		{
			cur_face = num_of_faces - 1;
			count_up = FALSE;
		}
	}
	
	
	
	
	sprite::do_behavior();
}
