/* Spiked 2.0
	Chuck Groom, 1997
	
	A rift is an explosion that, when it subsides, gives a gift or brick (rock) to the world.*/
	
#include "sprite.h"
#include "global.h"


rift::rift(fPoint p, sprite_manager * s_m) : mobile_sprite(p, s_m)
{
	myType = RIFT;
	load_faces();
	cur_face = 0;
	cur_frame = 0;
	draw_radius = 30;
	collide_radius = 10;
	velocity.h = velocity.v = .05;	//just in case
	mass = 1000; //quite heavy
	
	GlobalData->PlaySound(NORMAL_EXPLOSION);
}


//------------------------------------
// Slowly animate the rift.
//------------------------------------
void rift::do_behavior(void)
{
	cur_frame++;
	cur_face = (int) cur_frame/2;
	if ((cur_face == num_of_faces) && alive)
	{
		cur_face--;
		alive = FALSE;
		do_genesis();	//As it dies, make a new sprite.
	}
	mobile_sprite::do_behavior();
}


//------------------------------------
//Genesis: the rift creates a new sprite out of its ashes.
//------------------------------------
void rift::do_genesis(void)
{
	
	sprite *the_sprite = NULL;
	
	//What kind of a sprite do we make? A brick or a gift?
	if (random(100) < 100*PROB_BRICK)	//If it is a brick. PROB_BRICK is a global const.
	{
		the_sprite = new brick(position, the_sprite_manager);
	}
	else	//make a gift, according to the different probabilites of different gifts
	{
		int gift_num = random(100);
		GIFT_TYPE g_t = CANNON_GIFT;	//Default			 	
				 	
		//Very nasty if...else if.... etc tree
		if (gift_num < 100*LIFE_GIFT_PROBABILITY)
			g_t = LIFE_GIFT;
		else if (gift_num < 100*(LIFE_GIFT_PROBABILITY + CANNONBALL_GIFT_PROBABILITY))
			g_t = CANNON_GIFT;
		else if (gift_num < 100*(LIFE_GIFT_PROBABILITY + CANNONBALL_GIFT_PROBABILITY + TWISTER_GIFT_PROBABILITY))
			g_t = TWISTER_GIFT;
		else if (gift_num < 100*(LIFE_GIFT_PROBABILITY + CANNONBALL_GIFT_PROBABILITY + TWISTER_GIFT_PROBABILITY + SPEED_GIFT_PROBABILITY))
			g_t = SPEED_GIFT;
		else if (gift_num < 100*(LIFE_GIFT_PROBABILITY + CANNONBALL_GIFT_PROBABILITY + TWISTER_GIFT_PROBABILITY + SPEED_GIFT_PROBABILITY + GRAVITRON_GIFT_PROBABILITY))
			g_t = GRAVITRON_GIFT;
	
		the_sprite = new gift(position, the_sprite_manager, g_t);
	}
	
	if(!the_sprite)
		exit(1);
		
	
	//Choose a good velocity, based on the the new sprite's position. Point it towards the center of the screen.
	if(position.h < (GAME_WIDTH/2))
		the_sprite->velocity.h = random(4)/4 + 1;
	else
		the_sprite->velocity.h = -(random(4)/4 + 1);
		
	if(position.v < (GAME_HEIGHT/2))
		the_sprite->velocity.v = random(4)/4 + 1;
	else
		the_sprite->velocity.v = -(random(4)/4 + 1);
	
	
	the_sprite_manager->add_sprite_in_region(the_sprite);	
}
