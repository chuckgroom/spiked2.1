/* Spiked 2.0
	Chuck Groom, 1997
	
	A twister sprite. This stops any sprite it touches in its tracks, and prevents a player
	from acclerating for a little while. Very nasty weapon. */
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"


twister::twister(fPoint p, fPoint v, sprite_manager * s_m, float theta) : mobile_sprite(p, s_m)
{
	myType = TWISTER;
	load_faces();
	count_up = TRUE;
	cur_face = 0;
	draw_radius = 15;
	collide_radius = 11;
	mass = .1;
	
	max_velocity = 30;
	normal_max_velocity = 15;
	
	//The twister moves in the direction the player is pointed
	velocity.h = normal_max_velocity * cos (theta);
	velocity.v = normal_max_velocity * sin (theta);
	set_velocity(normal_max_velocity);
	
	//Make sure the twister stays out of the player's way
	position.h += 2*velocity.h;
	position.v += 2*velocity.v;
	
	float player_vel_magnitude = sqrt((v.h * v.h) + (v.v * v.v));
	float player_theta = vector_to_angle(v);
	
	//If player is zooming forward, pump up the twister's speed.
	if((player_vel_magnitude > normal_max_velocity) && (fabs(theta- player_theta) < pi/2))
	{
		if(player_vel_magnitude > max_velocity)
			set_velocity(max_velocity);
		else
			set_velocity(player_vel_magnitude + 1);
		
		position.h += v.h;
		position.v += v.v;
	}
}



//-------------------------------------------
// Cycle through the animations. The twister counts up (0, 1, 2... num_of_faces), then 
// counts down (num_of_faces, num_of_faces - 1... 0) for its animation.
//-------------------------------------------
void twister::do_behavior(void)
{	
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
	
	mobile_sprite::do_behavior();
}



//-------------------------------------------
// A twister always dies whenever it hits something.
//-------------------------------------------
void twister::do_collision(sprite * other_sprite)
{
	alive = FALSE;
	other_sprite->thrust_disable = TWISTED_FRAMES;
	other_sprite->velocity.h = other_sprite->velocity.v = 0;
	
	the_sprite_manager->add_sprite_in_region(new twisted(position, the_sprite_manager, other_sprite));
	GlobalData->PlaySound(HIT_TWISTER);
	
	mobile_sprite::do_collision(other_sprite);	//Pass the collision event to superclass
}


