/*  Spiked 2.0
	Chuck Groom, 1997

	Sprites are the heart of Spiked. Each sprite unit, while self-animating by the
	Sprite Animation Toolkit (SAT) still has its own unique behavior. Some sprites have
	related behaviors, so it is appropriate to set them up in heirarchical classes. 
	
	Sprites take care of the tasks of how the game behaves. Each sprite is an autonomous unit
	that interacts with other sprites in the Arena. 
	
	The heirarchy of sprites (as described in this header) is:
	[an asterix * means that this class has instances of it]
	
	sprite
		*p_item
		*life
		*thrust
		mobile_sprite
			player
				*green_p (green player)
				*red_p   (red player)
			*spike
			*brick
			*twister
			*cannon_ball
			*gift
			*rift
			*bullet
		button sprite
			*new_button
			*pref_button
			*about_button
			*go_intro_button
			*game_key_button
		*explosion                        */



#ifndef SPRITE_H
#define SPRITE_H




#include "sprite_manager.h"
#include "SAT.h"
#include "math.h"
#include "global.h"
#include "EventArbitrator.h"



pascal void empty_proc(Sprite *);


//The kinds of sprites
enum SPRITE_TYPE {	SPIKE,
					BRICK,
					GREEN_PLAYER,
					RED_PLAYER,
					TWISTER,
					GIFT,
					CANNON_BALL,
					BULLET,
					NEW_BUTTON,
					PREF_BUTTON,
					ABOUT_BUTTON,
					PLAYER_ITEM,
					LIFE,
					RIFT,
					EXPLOSION,
					GO_INTRO_BUTTON,
					GAME_KEY_BUTTON,
					TWISTED,
					GRAVITRONED,
					GRAVITRON,
					NO_SPRITE };

//The actions a player can do				
enum S_ACTION	{	NOTHING = 0,
					TURN_LEFT,
					TURN_RIGHT,
					THRUST,
					ITEM,
					CHANGE_ITEM };
				

//The kinds of gifts that can be on the screen, or that a player can carry				
enum GIFT_TYPE {  	LIFE_GIFT = 0,
				 	CANNON_GIFT,
				 	TWISTER_GIFT,
				 	SPEED_GIFT,
				 	GRAVITRON_GIFT,
				 	NO_GIFT };
				 





//-----------------------------------------------
// All sprites have class sprite as a superclass (or super-super...). The
// sprite class handles animation, sprite existance, etc.
//-----------------------------------------------				
class sprite
{
	private:
		SpritePtr theSprite;		//The SAT handle to the sprite that appears on screen
		int *myFaces;				//a list of all faces
	protected:
		int num_of_faces;			//the number of total faces available
		int cur_face;				//the current face index
		void load_faces (void); 	//determine which faces this sprite may use.
		class sprite_manager * the_sprite_manager;	//The list 'o sprites
	public:
		sprite(fPoint, sprite_manager *);
		~sprite();
		
		//The mechanics of sprite behavior and existances:
		SPRITE_TYPE myType;		//The kind of sprite
		fPoint position;		//H and V coordinates of position
		fPoint velocity;		//H and V components of velocity
		float draw_radius;		//How many pixels in radius the sprite is
		float collide_radius;	//How many pixels from the center counts as a "hit"
		float mass;				//Units of mass
		Boolean alive;			//Is the sprite alive? If this is ever false, the sprite is deleted
		Boolean collideable;
		
		int thrust_disable; 	//How many frames a player's thrust will be disabled
		
		virtual void do_behavior(void);	
		Boolean circle_in_sprite(fPoint, int);
		virtual Boolean collide(sprite *) {return FALSE;}		//default
		
		//The virtual functions used in derived classes:
		virtual void do_collision(sprite *) {;}
		virtual void do_clicked(void) {;}
		virtual void do_sprite_action(S_ACTION) {;}
		virtual GIFT_TYPE read_gift(void) { return NO_GIFT;}	//default
};




//--------------------------------------------------------
// In the about games section, there are sprites that simply display what a sprite
// looks like, but has no behavior other than simple animation. This is the gen_all_sprite
// class. 
//--------------------------------------------------------
class gen_all_sprite : public sprite
{	
	protected:
		Boolean simple_loop; 	//Does the sprite's frames loop, i.e. 1,2,3,4,1,2,3,4,1...
		Boolean count_up;		//if simple_loop == false, then we need to know if the animation frames are counting up or down
		int frame_pause;		//The pause, in frames, between the next animation iteration
		int frame_pause_count;	//A counter for the frame_pause, if there is one.
		int first_frame_wait;	//How many frames the sprite should hold on the first frame of animation
		int first_frame_count;	//A counter for the frame wait
	public:
		gen_all_sprite(fPoint, sprite_manager *, SPRITE_TYPE);
		virtual void do_behavior(void);
};





//--------------------------------------------------------
// The display for each player's number of lives is a sprite.
//--------------------------------------------------------
class p_life : public sprite
{	
	protected:
		int *lives;		// A pointer to the player's number of lives
	public:
		p_life(fPoint, sprite_manager *, int*);
		virtual void do_behavior(void);
};



//--------------------------------------------------------
// Each player can carry a gift. This is shown by the icon shown in each player's
// p_item display sprite.
//--------------------------------------------------------
class p_item : public sprite
{	
	private:
		GIFT_TYPE 	item;			//What kind of item the player is currently using
		int *lives;					// A pointer to the player's number of lives
		Boolean itemsArray[6];
		int wait_cycle;
		
	public:
		p_item(fPoint, sprite_manager *, int*);
		virtual void do_behavior(void);
		virtual GIFT_TYPE use_item(void);
		
		void cycle_item(void);
		void recieve_item(GIFT_TYPE g_t);
};





//------------------------------------------------------
//Mobile Sprites are sprites that move around the screen, sprites that can collide and blow up.
//--------------------------------------------------------
class mobile_sprite : public sprite
{
	protected:
		float max_velocity;
		float normal_max_velocity;
		void set_velocity(float);
		float vector_to_angle(fPoint);
	public:
		mobile_sprite(fPoint, sprite_manager *);
		
		fPoint acceleration;
		
		virtual void do_collision(sprite *);
		virtual Boolean collide(sprite *);
		virtual void do_behavior(void);
};



//--------------------------------------------------------
// All players can shoot bullets -- very useful.
//---------------------------------------------------
class bullet : public mobile_sprite
{	
	private:
		int countdown;
	public:
		bullet(fPoint, fPoint, sprite_manager *, float);
		virtual void do_collision(sprite *);
		virtual void do_behavior(void);
};





//------------------------------------------------------
//	The players are mobile sprites that can rotate around and are under keyboard control
//------------------------------------------------------
class player: public mobile_sprite
{
	protected:
		float theta; 	//Angle of the sprite
		p_item *my_gift_holder;
		int wait_use_item;			//The number of frames you must wait until you can use an item.
		Boolean use_engine;
	public:
		player(fPoint, sprite_manager *, p_item *);
		virtual void do_sprite_action(S_ACTION);
		virtual void do_behavior(void);
		virtual void do_collision(sprite *);
};


//------------------------------------------------------
// The green player
//------------------------------------------------------
class green_p : public player
{	
	public:
		green_p(fPoint, sprite_manager *, p_item *);
};

//------------------------------------------------------
//The red player
//------------------------------------------------------
class red_p : public player
{	
	public:
		red_p(fPoint, sprite_manager *, p_item *);
};





//--------------------------------------------------------
// The spike is a mobile sprite that sits in wait for hapless players, bricks, etc.
//--------------------------------------------------------
class spike : public mobile_sprite
{	
	public:
		spike(fPoint, sprite_manager *);
		virtual void do_behavior(void);
};


//--------------------------------------------------------
// The brick (aka rock) is a mobile sprite that sits there, idly rotating, until it is bumped.
//---------------------------------------------------
class brick : public mobile_sprite
{	
	private:
		int frame_pause;	//The number of frames between a change in the sprite's face
		int frame_count;
		int health;			//Bricks can take health # of hits before they die (~10)
	public:
		brick(fPoint, sprite_manager *);
		virtual void do_collision(sprite *);
		virtual void do_behavior(void);
};



//--------------------------------------------------------
// The twister is a weapon that a player shoots to snare anything in the twister's path.
//---------------------------------------------------
class twister : public mobile_sprite
{	
	private:
		Boolean count_up;	//Used for animation purposes
	public:
		twister(fPoint, fPoint, sprite_manager *, float);
		virtual void do_collision(sprite *);
		virtual void do_behavior(void);
};


//--------------------------------------------------------
// The gravitron pulls its target towards the player who makes it.
//---------------------------------------------------
class gravitron : public mobile_sprite
{	
	private:
		player * the_owner;
	public:
		gravitron(fPoint, fPoint, sprite_manager *, float, player *);
		virtual void do_collision(sprite *);
		virtual void do_behavior(void);
};


		
//--------------------------------------------------------
// Another weapon is the cannonball, which a player uses to blast his/her way through things.
//---------------------------------------------------
class cannon_ball : public mobile_sprite
{	
	public:
		cannon_ball(fPoint, fPoint, sprite_manager *, float);
		virtual void do_collision(sprite *);
};


	
	
//--------------------------------------------------------
// Gifts are goodies that float about, waiting to be picked up.	
//---------------------------------------------------	 
class gift : public mobile_sprite
{	
	protected:
		GIFT_TYPE my_gift;		//The kind of gift
	public:
		gift(fPoint, sprite_manager *, GIFT_TYPE);
		virtual void do_behavior(void);
		virtual void do_collision(sprite *);
		virtual GIFT_TYPE read_gift(void) { return my_gift;}	//reader for my_gift
};




//--------------------------------------------------------
// Every so often, a rift in space opens up and a new sprite
// pops out when the rift closes.
//--------------------------------------------------------
class rift : public mobile_sprite
{	
	private:
		int cur_frame;					//use to slow down the animation.
		void do_genesis();				//Make a new sprite
	public:
		rift(fPoint, sprite_manager *);
		virtual void do_behavior(void);
};







//--------------------------------------------------------
//	Another kind of sprite is the button sprite, which is a not-motile sprite
//  that reacts when it recieves a do_click functions.
//--------------------------------------------------------
class button_sprite : public sprite
{
	protected:
		int button_mode;			//Used by sprite to register the fact that it's been clicked.
		Event_Stack * theStack;		//Has access to Event_Stack so that it can create new events.
	public:
		button_sprite(fPoint, sprite_manager *, Event_Stack *);
		virtual void do_clicked(void) {;}
		virtual void do_behavior(void);
};

//--------------------------------------------------------
// The new_button generates a Gnew_game event when clicked
//--------------------------------------------------------
class new_button : public button_sprite
{
	public:
		new_button(fPoint, sprite_manager *, Event_Stack *);
		virtual void do_clicked(void);
		virtual void do_behavior(void);
};

//--------------------------------------------------------
// The pref_button generates a Gpref event when clicked
//--------------------------------------------------------
class pref_button : public button_sprite
{
	public:
		pref_button(fPoint, sprite_manager *, Event_Stack *);
		virtual void do_clicked(void);
		virtual void do_behavior(void);
};

//--------------------------------------------------------
// The about_button generates a Gabout event when clicked
//--------------------------------------------------------
class about_button : public button_sprite
{
	public:
		about_button(fPoint, sprite_manager *, Event_Stack *);
		virtual void do_clicked(void);
		virtual void do_behavior(void);
};


//--------------------------------------------------------
// The go_intro_button generates a Gintro_arena event when clicked
//--------------------------------------------------------
class go_intro_button : public button_sprite
{
	public:
		go_intro_button(fPoint, sprite_manager *, Event_Stack *);
		virtual void do_clicked(void);
		virtual void do_behavior(void);
};

//--------------------------------------------------------
// The game_key_button is a button that, with a one-to-one correspondance, represents
// one of the eight KEY_TYPE preference keys. This button allows for the section, and 
// change of, one of the game prefs.
//--------------------------------------------------------
class game_key_button : public button_sprite
{
	private: 
		game_key_button ** key_buttons;	//All ten such game_key_buttons
		char the_char;					//The character that this button represents
		KEY_TYPE my_key;				//The KEY_TYPE this button represents
	public:
		game_key_button(fPoint, sprite_manager *, Event_Stack *, game_key_button **, KEY_TYPE, char);
		
		KEY_TYPE get_key_type(void) 	{return my_key;}
		void set_char (char);			
		void set_button_mode(int mode) 		{button_mode = mode;}
		virtual void do_clicked(void);
		virtual void do_behavior(void);
};





//--------------------------------------------------------
// Once something blows up, it leaves an explosion that lingers around a bit.
//--------------------------------------------------------
class explosion : public sprite
{	
	public:
		explosion(fPoint, sprite_manager *);
		virtual void do_behavior(void);
};

//--------------------------------------------------------
// A sprite caught in a twister has a numbus floating around it.
//--------------------------------------------------------
class twisted : public sprite
{	
	private:
		sprite * aflicted_sprite;
		Boolean count_up;
	public:
		twisted(fPoint, sprite_manager *, sprite *);
		virtual void do_behavior(void);
};

//--------------------------------------------------------
// Ditto for a gravitron
//--------------------------------------------------------
class gravitroned : public sprite
{	
	private:
		player *the_owner;
		mobile_sprite *afflicted_sprite;
		int life_time;
	public:
		gravitroned(fPoint, sprite_manager *, player *, mobile_sprite *);
		virtual void do_behavior(void);
};


#endif
