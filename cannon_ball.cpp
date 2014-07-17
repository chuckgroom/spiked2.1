/* Spiked 2.0
	Chuck Groom, 1997
	
	A cannon_ball sprite. This sprite materializes at a high velocity and plows forward.*/
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"


cannon_ball::cannon_ball(fPoint p, fPoint v, sprite_manager * s_m, float theta) : mobile_sprite(p, s_m)
{
	myType = CANNON_BALL;
	load_faces();
	cur_face = 0;
	draw_radius = 6; 
	collide_radius = 6;
	mass = CANNONBALL_MASS;
	
	max_velocity = 30;
	normal_max_velocity = 16;


	//The cannonball moves in the direction the player is pointed
	velocity.h = normal_max_velocity * cos (theta);
	velocity.v = normal_max_velocity * sin (theta);
	set_velocity(normal_max_velocity);
	
	//Make sure the cannonball stays out of the player's way
	position.h += 2*velocity.h;
	position.v += 2*velocity.v;
	
	float player_vel_magnitude = sqrt((v.h * v.h) + (v.v * v.v));
	float player_theta = vector_to_angle(v);
	
	//If player is zooming forward, pump up the cannonball's speed.
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




void cannon_ball::do_collision(sprite * other_sprite)
{
	if((other_sprite->myType == SPIKE) || 	(other_sprite->myType == BRICK))		//This sprite will die
	{
		alive = 0;
		the_sprite_manager->add_sprite(new explosion(position, the_sprite_manager));
	}
	
	if (other_sprite->myType == SPIKE)
		GlobalData->PlaySound(HIT_SPIKE);
	else
		GlobalData->PlaySound(HIT_CANNONBALL);
		
		
	mobile_sprite::do_collision(other_sprite);	//Pass the collision event to superclass
}


