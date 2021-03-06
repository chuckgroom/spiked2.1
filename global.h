/* Chuck Groom, Spiked 2.0  1997 
	global.h is a header file of all the constants and single-instance classes that are used.
	
	This is appropriate data to make global, because it reflects constanst that all parts of the
	game need access to, but are not appropriate to throw into a storage cesspool. */
	
#ifndef CONST_H
#define CONST_H

#include "fades.h"
#include "SAT.h"
#include "prefs.h"
#include "stdlib.h"
#include "Menus.h"




//---------------------------------------------------
//Define my own random function
//----------------------------------------------------
int random (int n);



//---------------------------------------------------
//CONSTANTS
//----------------------------------------------------

//The four background resource IDs
enum BACKGROUND_PICTS {DEF_PICT = 128, INTRO_PICT, PREF_PICT, GAME_PICT, RED_WON_PICT, GREEN_WON_PICT, ABOUT_PICT};
enum game_result	{RED_WON, GREEN_WON, NO_WIN};
enum SOUND_ID { NORMAL_EXPLOSION = 100, HIT_SPIKE, FIRE_BULLET, FIRE_CANNONBALL, PLAYERS_HIT, 
				PLAYER_DEATH, FIRE_SPEED, HIT_NORMAL, GOT_GIFT, HIT_TWISTER, HIT_CANNONBALL, 
				FIRE_GRAVITRON, HIT_GRAVITRON, SPIKED_INTRO, FIRE_TWISTER, OH_DAMN, HAHA, YEAH};
				
const int NUM_SOUNDS = 18;
						

//Dimensions of the window
const int GAME_WIDTH = 640;
const int GAME_HEIGHT = 460;

//Maximum number of sprites allowed on screen.
const int MAX_SPRITES = 150;

//The maximum resource number of the CICN resources used to animate the game
const int MAX_CICN_RES = 282;


//How many constants are read from disk?
const int NUM_PHYSICS_CONSTANTS = 20;
//---------------------------------------------------
// The following global variables reflect the default game settings. The real settings are read in from
// disk, from the file "Spiked Constants". For a description of these constants, refer to "global.cp"
//----------------------------------------------------


extern float ACCEL_RATIO;
extern float GRAV;
extern float DEACCEL;
extern long SPEED_LIMIT;
extern int GAME_RESULT_PAUSE;
extern int FREQ_NEW_SPRITE;
extern float PROB_BRICK;
extern int NORMAL_NUM_SPRITE;
extern int TWISTED_FRAMES;
extern int GRAVITRONED_FRAMES;
extern int PLAYER_MAX_VELOCITY;
extern int PLAYER_NORMAL_MAX_VELOCITY;
extern int PLAYER_MASS;
extern int BULLET_MASS;
extern int CANNONBALL_MASS;
extern float LIFE_GIFT_PROBABILITY;
extern float CANNONBALL_GIFT_PROBABILITY;
extern float TWISTER_GIFT_PROBABILITY;
extern float SPEED_GIFT_PROBABILITY;
extern float GRAVITRON_GIFT_PROBABILITY;


//---------------------------------------------------------
// End of constants
//---------------------------------------------------------










//-----------------------------------------------------
// Data structure-- fPoint: a floating-point point.
//----------------------------------------------------
struct fPoint
{
	float h;
	float v;
};







//---------------------------------------------------
// Global data: That data which is used by all parts of the game, but which requires special
// access (ie data that isn't constant, but should be global).
//---------------------------------------------------
class GData						
{
	private:
		int num_of_faces;		// How many frames total used by the game.
		FacePtr *theFaces;		// The cicn's used by the game. These are the animation frames for sprites.
		void LoadFaces(void);	// Load all the animation cells.
		void LoadPhysics(void);	// Load phun data from disk
		void LoadSounds(void);	// Read in them sounds
		game_result the_result;	// Who won the last round?
		
		Handle the_sounds[NUM_SOUNDS];	// Holds the sound resources
		
		//Draw a status bar of how well the game has loaded.
		void 		DrawStatusBar(int);
	public:
  		GData();
  		~GData();
  		
  		int v_offset;
  		int h_offset;
  		
  		Boolean menu_showing;
  		
  		prefs the_prefs;
  		
  		void PlaySound(int);
  		
		FacePtr GetFace(int index) { if ((index < MAX_CICN_RES) && (index >= 0))
										return theFaces[index];
									else return theFaces[35];}
		void write_result (game_result r) {the_result = r;}
		game_result read_result (void) {return the_result;}
		
		unsigned char * red_won_str, * green_won_str;
};






//---------------------------------------------------
//Global tool: Those special functions that are universally accessable, but need special care.
//---------------------------------------------------
class GTool						
{
	private:
		fader *			the_fader;
		Game_Menu	*	the_game_menus;	
		
	public:
		GTool(Game_Menu *gm) 	{	the_game_menus = 	gm;
								 	the_fader = 		new fader;	}	
		
		void fade_in(void)  	{the_fader->fade_in();}
		void fade_out(void)  	{the_fader->fade_out();}
		
		void set_background(int);
		void set_offset(void);
		
		//Pass on the command to set the game menus so that the appropriate menus are dimmed
		void menus_intro_mode(void) 	{the_game_menus->menus_intro_mode();}	
		void menus_game_mode(void) 		{the_game_menus->menus_game_mode();}	
};





//-----------------------------------------------------
// Global instances of the GData, GTool classes
extern GData * GlobalData;
extern GTool * GlobalTool;



#endif
