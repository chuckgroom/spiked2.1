/* Spiked 2.0
	Chuck Groom, 1997
	
	A bullet sprite. Each player can fire a bullet..*/
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"


bullet::bullet(fPoint p, fPoint v, sprite_manager * s_m, float theta) : mobile_sprite(p, s_m)
{
	myType = BULLET;
	load_faces();
	cur_face = 0;
	draw_radius = 4; 
	collide_radius = 4;
	mass = BULLET_MASS;
	
	max_velocity = 26;
	normal_max_velocity = 15;
	countdown = 10; 	//How many frames the bullet will stay on the screen
	
	//The bullet moves in the direction the player is pointed
	velocity.h = normal_max_velocity * cos (theta);
	velocity.v = normal_max_velocity * sin (theta);
	set_velocity(normal_max_velocity);
	
	//Make sure the bullet stays out of the player's way
	position.h += 1.7*velocity.h;
	position.v += 1.7*velocity.v;
	
	float player_vel_magnitude = sqrt((v.h * v.h) + (v.v * v.v));
	float player_theta = vector_to_angle(v);
	
	//If player is zooming forward, pump up the bullet's speed.
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


void bullet::do_behavior(void)
{
	countdown--;
	
	if (cur_face || (countdown < 0))
		cur_face++;

	if(cur_face == 4)
		alive = FALSE;
	
	cur_face = cur_face%num_of_faces;
		
	mobile_sprite::do_behavior();
}



void bullet::do_collision(sprite * other_sprite)
{
	GlobalData->PlaySound(HIT_NORMAL);
	collideable = FALSE;	//can't collide more than once
	cur_face++;
	mobile_sprite::do_collision(other_sprite);	//Pass the collision event to superclass
}


