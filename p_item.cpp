/* Spiked 2.0
	Chuck Groom, 1997
	
	A sprite that displays the player's current icon*/
	
#include "sprite.h"

p_item::p_item(fPoint p, sprite_manager * s_m, int * ll) : sprite(p, s_m)
{
	myType = PLAYER_ITEM;
	load_faces();
	cur_face = 0;
	draw_radius = collide_radius = 0;
	
	item = NO_GIFT;
	lives = ll;
	
	wait_cycle = 0;
	
	//Clean out the item array
	for (int ii = 0; ii < 6; ii++)
	{
		itemsArray[ii] = FALSE;
	}
	itemsArray[NO_GIFT] = TRUE;	//B/c it is always true
}


void p_item::do_behavior(void)
{
	//We may have to wait a few frames between cycling between items.
	if(wait_cycle)
		wait_cycle--;
	// There is a one-to-one correspondence between the #s of this sprite's faces and the gift
	// it bears.
	cur_face = (int) item;
	sprite::do_behavior();
}



GIFT_TYPE p_item::use_item(void)
{
	GIFT_TYPE ret_val = item;
	
	itemsArray[item] = FALSE;
	itemsArray[NO_GIFT] = TRUE;
	item = NO_GIFT;
	
	return ret_val;
}

	

void p_item::recieve_item(GIFT_TYPE g_t)
{
	if (g_t == LIFE_GIFT) 
	{
		*lives += 1;	//increment the player's lives
		return;
	}
	
	itemsArray[g_t] = TRUE;
	item = g_t;
}



void p_item::cycle_item(void)
{
	//b/c they may be holding down the key...
	if(wait_cycle)
		return;
		
	//Otherwise, it is OK to change the item	
	int index = item;
	do 	{
		index++;
		index = index%6;
	} while (!itemsArray[index]);
	
	item = (GIFT_TYPE) index;
	
	wait_cycle = 3;	//Wait a little bit before cycling again
}
