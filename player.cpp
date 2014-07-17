/* Spiked 2.0
	Chuck Groom, 1997
	
	The player sprite: the parent class for the players. */
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"

//----------------------------------------------
// Players have an angle (theta) that they are turned. Players also keep track of the
// items (p_item) they own.
//----------------------------------------------
player::player(fPoint p, sprite_manager * s_m, p_item * p_i) : mobile_sprite(p, s_m)
{
	my_gift_holder = p_i;
	draw_radius = 22;
    collide_radius = 15;
    max_velocity = PLAYER_MAX_VELOCITY;
    normal_max_velocity = PLAYER_NORMAL_MAX_VELOCITY;
    theta = 0;
    cur_face = 0;
    mass = PLAYER_MASS;
    wait_use_item = 0;
    use_engine = FALSE;
}


//----------------------------------------------
// The player's behavior is to correctly turn according to their angle measure
//----------------------------------------------
void player::do_behavior(void)
{
	cur_face = (int) ((theta/(2*pi)) * 30);
	cur_face = cur_face % 30;
	
	if (use_engine)	//light up the engine
	{
		cur_face += 30;
		use_engine = FALSE;
	}
	
	if(wait_use_item)
		wait_use_item--;
		
	mobile_sprite::do_behavior();
}



//----------------------------------------------
// The game_arena can feed the player an S_ACTION, which can be turn left, right, thurst, or use item.
//----------------------------------------------
 void player::do_sprite_action(S_ACTION the_action)
 {
 	switch (the_action)
 	{
 		case THRUST:
 			if (!thrust_disable) 
 			{
 				//Apply accereration in the direction that the player is pointed.
	 			acceleration.v += ACCEL_RATIO * sin(theta);
	 			acceleration.h += ACCEL_RATIO * cos(theta);
	 			
	 			use_engine = TRUE;
	 		}
 			break;
 		case TURN_LEFT:
 			theta -= (4*pi)/num_of_faces;
 			if (theta < 0) 
 				theta += 2*pi;
 			break;
 		case TURN_RIGHT:
 			theta += (4*pi)/num_of_faces;
 			if (theta > 2*pi) 
 				theta -= 2*pi;
 			break;
 		case CHANGE_ITEM:
 			my_gift_holder->cycle_item();
 			break;
 		case ITEM:
 			if(wait_use_item == 0)
 			{
 				wait_use_item = 10;	//Pause for 10 frames before using another item
 				
	 			switch (my_gift_holder->use_item())
	 			{
	 				case CANNON_GIFT:	//Fire a cannonball!
	 					the_sprite_manager->add_sprite_in_region(new cannon_ball(position, velocity, the_sprite_manager, theta));
	 					GlobalData->PlaySound(FIRE_CANNONBALL);
	 					break;
	 				case TWISTER_GIFT:	//Fire a twister!!
	 					the_sprite_manager->add_sprite_in_region(new twister(position, velocity, the_sprite_manager, theta));
	 					GlobalData->PlaySound(FIRE_TWISTER);
	 					break;
	 				case SPEED_GIFT:	//Speed forward!!
	 					velocity.h = cos(theta);
	 					velocity.v = sin(theta);
	 					set_velocity(max_velocity);
	 					GlobalData->PlaySound(FIRE_SPEED);
	 					break;
	 				case GRAVITRON_GIFT:	//Fire a gravitron!
	 					the_sprite_manager->add_sprite_in_region(new gravitron(position, velocity, the_sprite_manager, theta, this));
	 					GlobalData->PlaySound(FIRE_GRAVITRON);
	 					break;
	 				case NO_GIFT:
	 				default:
	 					GlobalData->PlaySound(FIRE_BULLET);
			 			the_sprite_manager->add_sprite_in_region(new bullet(position, velocity, the_sprite_manager, theta));
	 					break;
	 			}
 			}
 	}
 }



//------------------------------------
// When a player collides with a spike, is so dead. Also, players pick up the goodies from gifts.
//------------------------------------
void player::do_collision(sprite * other_sprite)
{
	if(other_sprite->myType == SPIKE)		//This sprite will blow into little bits and pieces.
	{
		alive = 0;
		the_sprite_manager->add_sprite(new explosion(position, the_sprite_manager));
		GlobalData->PlaySound(PLAYER_DEATH);
	}
	
	if(other_sprite->myType == GIFT)		//Grab the goodies!
		my_gift_holder->recieve_item(((gift *)other_sprite)->read_gift());

	if((other_sprite->myType == RED_PLAYER) || (other_sprite->myType == GREEN_PLAYER))
		GlobalData->PlaySound(PLAYERS_HIT);
		
		
	mobile_sprite::do_collision(other_sprite);	//Pass the collision event to superclass
}


