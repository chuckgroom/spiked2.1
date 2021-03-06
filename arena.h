/*	Spiked 2.0
	Chuck Groom
		
		The arena header. The arena's are the classes that handle the sprite behavior in an 
		environment. The heirarchy is:
		
		Superclass: Arena
		Derived classes: intro_arena, pref_arena, game_arena. */


#ifndef ARENA_H
#define ARENA_H

#include "EventArbitrator.h"
#include "sprite_manager.h"

//--------------------------------------------------------------------------
class arena
{
	protected:
		Game_Event * theEvent;	// One particular event
		Event_Stack * theStack;	// The event manager
	public:
		arena(Event_Stack * q);
		class sprite_manager * s_manager;
		arena();
		~arena();
		virtual void run_frame(void) = 0;
};


//--------------------------------------------------------------------------
class intro_arena : public arena
{
	private:
		sprite * new_button_sprite;
		sprite * pref_button_sprite;
		sprite * about_button_sprite;
	public:
		intro_arena(Event_Stack * q);
		~intro_arena() {delete new_button_sprite; delete pref_button_sprite; }
		
		intro_arena();
		virtual void run_frame();
};




//--------------------------------------------------------------------------
class game_arena : public arena
{
	private:
		sprite * green_player, * red_player, * green_item,* red_item, * spike_sprite;
		p_life * g_life, * r_life;
		int green_lives, red_lives;
		
		Point 	last_mouse_point;
		int 	mouse_hide_count;	//The number of frames until we hide the mouse
		
		void prepare_round();
		void populate();
		void rift_create();

		void maintain_mouse();
	public:
		game_arena(Event_Stack * q);
		~game_arena();
		virtual void run_frame();
};




//--------------------------------------------------------------------------
class pref_arena : public arena
{
	private:
		sprite * go_intro_button_sprite;
		game_key_button * key_buttons[10],  * selected_key_button;
	public:
		pref_arena(Event_Stack * q);
		virtual void run_frame();
};



//--------------------------------------------------------------------------
class about_arena : public arena
{
	private:
		sprite * go_intro_button_sprite;
	public:
		about_arena(Event_Stack * q);
		virtual void run_frame();
};


//--------------------------------------------------------------------------
class post_game_arena : public arena
{
	private:
		int wait_frames;
	public:
		post_game_arena(Event_Stack * q);
		virtual void run_frame();
};



#endif
