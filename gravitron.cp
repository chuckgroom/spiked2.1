/* Spiked 2.0
	Chuck Groom, 1997
	
	A gravitron sprite. This sprite ties any sprite toward the sprite of origin. */
	
#include "sprite.h"
#include "math.h"
#include "global.h"


gravitron::gravitron(fPoint p, fPoint v, sprite_manager * s_m, float theta, player * the_player) : mobile_sprite(p, s_m)
{
	myType = GRAVITRON;
	load_faces();

	the_owner = the_player;
	
	cur_face = 0;
	draw_radius = 14;
	collide_radius = 10;
	mass = 1;
	max_velocity = 30;
	normal_max_velocity = 15;
	
	//The gravitron moves in the direction the player is pointed
	velocity.h = normal_max_velocity * cos (theta);
	velocity.v = normal_max_velocity * sin (theta);
	set_velocity(normal_max_velocity);
	
	//Make sure the gravitron stays out of the player's way
	position.h += 2*velocity.h;
	position.v += 2*velocity.v;
	
	float player_vel_magnitude = sqrt((v.h * v.h) + (v.v * v.v));
	float player_theta = vector_to_angle(v);
	
	//If gravitron is zooming forward, pump up the bullet's speed.
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
// Animate this puppy
//-------------------------------------------
void gravitron::do_behavior(void)
{	
	cur_face++;
	cur_face = cur_face % num_of_faces;
	
	mobile_sprite::do_behavior();
}



//-------------------------------------------
// A gravitron attracts the other_sprite to the player by creating an attracting field between the two.
//-------------------------------------------
void gravitron::do_collision(sprite * other_sprite)
{
	alive = FALSE;
	
	if((player *) other_sprite == the_owner)	//can't attach owner to him/her self, can we?
		return;
		
	the_sprite_manager->add_sprite_in_region(new gravitroned(position, the_sprite_manager, the_owner, (mobile_sprite*) other_sprite));
	
	if(other_sprite->myType == SPIKE)	
		GlobalData->PlaySound(OH_DAMN);		//It really sucks when a spike chases you, hence the sound.
	else
		GlobalData->PlaySound(HIT_GRAVITRON);
	
	mobile_sprite::do_collision(other_sprite);	//Pass the collision event to superclass
}


