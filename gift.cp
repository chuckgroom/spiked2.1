/* Spiked 2.0
	Chuck Groom, 1997
	
	A gift is a sprite that can deliever goodies to a player. Although it moves, it doesn't
	count as a mobile sprite because it dies when it hits the edge of the screen.
	It is derived from the moblie sprite so that collisions are handled correctly. */
	
#include "sprite.h"
#include "global.h"


gift::gift(fPoint p, sprite_manager * s_m, GIFT_TYPE gt) : mobile_sprite(p, s_m)
{
	myType = GIFT;
	my_gift = gt;
	load_faces();
	cur_face = my_gift;
	draw_radius = 16;
	collide_radius = 12;
	mass = 1;
}



void gift::do_behavior(void)
{
	velocity.h += acceleration.h;
	velocity.v += acceleration.v;
	
	position.h += velocity.h;
	position.v += velocity.v;
	
	acceleration.h = 0;
	acceleration.v = 0;
	
	//If off of edge of screen, eliminate gift.
	if ((position.h - draw_radius) < 0)
		alive = FALSE;
	if ((position.h + draw_radius) > GAME_WIDTH)
		alive = FALSE;
	if ((position.v - draw_radius) < 52)
		alive = FALSE;
	if ((position.v + draw_radius) > GAME_HEIGHT)
		alive = FALSE;
	
	//Short-ciruit the mobile_sprite's behavior
	sprite::do_behavior();
}



void gift::do_collision(sprite * other_sprite)
{
	switch (other_sprite->myType)
	{
		case SPIKE:
		case BULLET:
		case CANNON_BALL:
			alive = 0;	//This sprite shall die
			the_sprite_manager->add_sprite(new explosion(position, the_sprite_manager));
			GlobalData->PlaySound(NORMAL_EXPLOSION);
			break;
		case RED_PLAYER:
		case GREEN_PLAYER:
			alive = 0;
			GlobalData->PlaySound(GOT_GIFT);
			break;
	}
		
	mobile_sprite::do_collision(other_sprite);	//Pass the collision event to superclass
}
