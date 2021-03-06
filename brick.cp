/* Spiked 2.0
	Chuck Groom, 1997
	
	A brick sprite. This should have been called a rock sprite. */
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"


brick::brick(fPoint p, sprite_manager * s_m) : mobile_sprite(p, s_m)
{
	myType = BRICK;
	load_faces();
	cur_face = random(num_of_faces);	
	frame_pause = random(2) + 1;	//How fast the brick rotates
	frame_count = 0;
	draw_radius = collide_radius = 20;
	mass = 50;
	health = 4;
}


//-------------------------------------------
// Rotate the brick, according to its frame_pause speed
//-------------------------------------------
void brick::do_behavior(void)
{
	//animate
	frame_count++;
	frame_count = frame_count % frame_pause;
	 
	if (frame_count == 0)
	{
		cur_face++;
		cur_face = cur_face % num_of_faces;
	}
	
	if (health <= 0) 	//if this sprite is toast
	{
		alive = 0;
		the_sprite_manager->add_sprite(new explosion(position, the_sprite_manager));	//blow into pieces
	}
	mobile_sprite::do_behavior();
}



void brick::do_collision(sprite * other_sprite)
{
	if(other_sprite->myType == SPIKE)		//This sprite will die
	{
		alive = 0;
		GlobalData->PlaySound(NORMAL_EXPLOSION);
		the_sprite_manager->add_sprite(new explosion(position, the_sprite_manager));	//blow into pieces
	}
	else
		GlobalData->PlaySound(HIT_NORMAL);
	
	if(other_sprite->myType == CANNON_BALL)		//Blows up if hit by a cannon ball
	{
		GlobalData->PlaySound(NORMAL_EXPLOSION);
		health -= 20;
	}
		
	if(other_sprite->myType == BULLET)			//Hurt by bullets
	{
		if (--health == 0)
			GlobalData->PlaySound(NORMAL_EXPLOSION);
	}
	
	
	mobile_sprite::do_collision(other_sprite);	//Pass the collision event to superclass
}


