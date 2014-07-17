/* Chuck Groom
	Spiked 2.0
	1997
	
	sprite_manager maintains a list of current sprites. It does a lot of useful things --
	add a sprite, delete a sprite, check for collisions, that sort of thing. */

#ifndef SMAN_H
#define SMAN_H

#include "sprite.h"
#include "global.h"

class sprite_manager
{
	private:
		class sprite ** sprite_list;		//an array of pointers to sprites
		int top;							//The next open slot of the array
		int sprite_index(sprite *);			//search for a sprite -- return its index
	public:
		sprite_manager();
		~sprite_manager();
		
		void add_sprite(sprite *);
		void delete_sprite(sprite *);
		void add_sprite_in_region(sprite *);
		
		void stop_action(void); 			// Stop everyone from moving
		void run_behaviors(void);			// Run do_behavior of all sprites in the list
		void collision_check(void);			// Handles collisions
		sprite * sprite_in_range(fPoint, int);
		int num_of_sprites (void) {return top;}
};

#endif
