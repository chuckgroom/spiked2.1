/* Chuck Groom, Spiked 2.0 1997
	Although "good" code should use as few globals as possible, in some classes it's warrented.
	
	For Spiked, I decided that several data structures and common functions should be global.
	One of these data structures is that all-imporant GData(), which is sort of a storage tank
	of a lot of the data that determines how the game will play. This includes the actual sprite
	graphic maps, the prefs, etc.
	
	The GTool is a container of the useful tools used in Spiked, including the fader class. */

#include "Menus.h"
#include "global.h"
#include "fades.h"
#include "SAT.h"
#include "stdlib.h"
#include "math.h"
#include <fstream.h>
#include <stdlib.h>

GData * GlobalData;
GTool * GlobalTool;

//--------------------------------------
//	Your basic randomizer
//--------------------------------------
int random (int n) 
{
	return (abs(rand()))%n;
}



//--------------------------------------
// Global, but not constant, data declaration:
//------------------------------------------
//The amount of acceration per thrust
float ACCEL_RATIO = .9;

//The gravitational pull by a gravitron
float GRAV = .4;

//The constant for decellearation of a sprite that exceeds its max. normal velocity.
float DEACCEL = .6;

//The speed limit. 60/Speed limit = frames per second of animation
long SPEED_LIMIT = 2;	//30 FPS

//The time, in frames, between games that the screen pauses
int GAME_RESULT_PAUSE = 60;


//Genesis constants: After a rift has opened, what things are made?
int FREQ_NEW_SPRITE = 250;    	//The average time, in frames, that it takes for a new sprite to pop up if there are 5 other sprites on the screen
float PROB_BRICK = 0.30; 		//The prob a new sprite is a brick
int NORMAL_NUM_SPRITE = 13;		//The optimal number of sprites to have on screen.




//Sprite behavior constants
int TWISTED_FRAMES = 250;		//How long a player is twisted up
int GRAVITRONED_FRAMES = 250;	//How many frames a sprite it gravitroned

int PLAYER_MAX_VELOCITY = 25;			//Top speed limit
int PLAYER_NORMAL_MAX_VELOCITY = 7;		//Normal speed limit
int PLAYER_MASS = 10;					//Player's mass


int BULLET_MASS = 1;		//The bullet's mass
int CANNONBALL_MASS = 30;	//The cannonball's mass

//Gift Probabilites
float LIFE_GIFT_PROBABILITY = .2;
float CANNONBALL_GIFT_PROBABILITY = .2;
float TWISTER_GIFT_PROBABILITY = .2;
float SPEED_GIFT_PROBABILITY = .2;
float GRAVITRON_GIFT_PROBABILITY = .2;










//--------------------------------------
//	The GData class: store global data about... everything.
//--------------------------------------
GData::GData()
{
	num_of_faces = 0;			//How many cicn resources are loaded
	menu_showing = TRUE;		//Yes, the menu is showing
	theFaces = NULL;			//Initialize the data storage class for sprite cicn resources
	
	//Clear sound array
	for (int ii = 0; ii < NUM_SOUNDS; ii++)
		the_sounds[ii] = NULL;
		
	LoadFaces();				//Read all the cicn's from disk. This takes a while
	LoadSounds();				// Read in the sounds. This will take little while, too.
	LoadPhysics();				//Read in the physics constants.
	the_result = NO_WIN;		//There has been no game played, so there has been no winner yet.
	
	//When a player wins, one of two strings is displayed. These must be Pascal strings.
	green_won_str = C2PStr("The green player won!\0");
	red_won_str = C2PStr("The red player won!\0");
}

//--------------------------------------
//	De-allocate! Throw out resources!
//--------------------------------------
GData::~GData()
{
	for (int counter = 0; counter < num_of_faces; counter++)
		SATDisposeFace(theFaces[counter]);
}


/*--------------------------------------
 	The sprite faces -- animation frames -- are stored as 'cicn' resources in the 
 	resource fork. They are numbered 0, 1, 2,....up to the MAX_CICN_RES number. 
 	
 	This grabs the faces from the resource fork, and maintains a fairly large index
 	to these resources, for use by the sprites.
//--------------------------------------*/
void GData::LoadFaces()
{
	theFaces = new FacePtr[MAX_CICN_RES];
	
	for (int counter = 0; counter < MAX_CICN_RES + 1; counter++)
	{
		theFaces[counter] = SATGetFace(counter);
		DrawStatusBar(counter);
	}
	
}



//Read in the phsyics off the disk, if possible.
void GData::LoadPhysics()
{
 	float the_data[NUM_PHYSICS_CONSTANTS];
 	
	ifstream  phys_data("Spiked Constants");
	if (!phys_data) //if file doesn't exist, we'll just use defaults
		return;
	
	float holder;
	for (int counter = 0; !phys_data.eof() && (counter < NUM_PHYSICS_CONSTANTS); counter++)
		phys_data >> the_data[counter];
	phys_data.close();

	
	//Time to decipher!
	ACCEL_RATIO = the_data[0];
	GRAV = the_data[1];
	DEACCEL = the_data[2];
	SPEED_LIMIT = (long) the_data[3];
	GAME_RESULT_PAUSE = (int) the_data[4];
	FREQ_NEW_SPRITE = (int) the_data[5];
	PROB_BRICK = the_data[6];
	NORMAL_NUM_SPRITE = (int) the_data[7];
	TWISTED_FRAMES = (int) the_data[8];
	GRAVITRONED_FRAMES = (int) the_data[9];
	PLAYER_MAX_VELOCITY = (int) the_data[10];
	PLAYER_NORMAL_MAX_VELOCITY = (int) the_data[11];
	PLAYER_MASS = (int) the_data[12];
	BULLET_MASS = (int) the_data[13];
	CANNONBALL_MASS = (int) the_data[14];
	LIFE_GIFT_PROBABILITY = the_data[15];
 	CANNONBALL_GIFT_PROBABILITY = the_data[16];
	TWISTER_GIFT_PROBABILITY = the_data[17];
	SPEED_GIFT_PROBABILITY = the_data[18];
	GRAVITRON_GIFT_PROBABILITY = the_data[19];
}





void GData::LoadSounds(void)
{
	//Read the sounds in from disk
	for (int counter = 0; counter < NUM_SOUNDS; counter++)
	{
		the_sounds[counter] = SATGetSound(counter + 100);
		DrawStatusBar(counter + MAX_CICN_RES);
	}
}






//As we load, we want to display how well the game has loaded so far.
// This reflects both the number of sounds as well as the number of CICNs loaded.
void GData::DrawStatusBar(int CICN_LOADED)
{
	float percent_loaded = ((float) CICN_LOADED)/((float) MAX_CICN_RES + NUM_SOUNDS);
	int line_len = percent_loaded * 513;	//The loading line is 513 pixels wide, max.
	
	SATSetPort (&gSAT.backScreen);
	
	RGBColor fore;
	//Light purple
	fore.red = 39321;
	fore.green = 0;
	fore.blue = 26214;
	RGBForeColor(&fore);
	
	PenSize (1,12);
	MoveTo(63, 295);
	Line(line_len, 0);
	
	//3-d look: dark purple leader bar 
	fore.red = 13107;
	fore.green = 0;
	fore.blue = 13107;
	RGBForeColor(&fore);
	
	PenSize (1,10);
	MoveTo(63 + line_len, 296);
	Line(2,0);
	
	
	Rect changedRect;
	changedRect.top = 295;
	changedRect.left = 63;
	changedRect.bottom = 310;
	changedRect.right = 630;
	SATBackChanged (&changedRect);
	
	SATRun(TRUE);
}




//Play the sound, setting flags where appropriate
void GData::PlaySound(int the_sound_id)
{
	Handle the_sound_handle = the_sounds[the_sound_id - 100];
	if (the_sound_handle == NULL)
		return;
		
	int priority = 1;
	Boolean can_wait = FALSE;
	
	//Special sounds
	switch (the_sound_id)
	{
		case NORMAL_EXPLOSION:
			priority = 10;
			break;
		case PLAYERS_HIT:
			priority = 7;
			break;
		case PLAYER_DEATH:
			priority = 15;
			break;
		case HIT_TWISTER:
			priority = 3;
			can_wait = TRUE;
			break;
		case HIT_GRAVITRON:
			priority = 3;
			can_wait = TRUE;
			break;
		case SPIKED_INTRO:
			priority = 15;
			can_wait = TRUE;
			break;
			
	}
	
	SATSoundPlay(the_sound_handle, priority, can_wait);
}














//--------------------------------------
//	Change the background to the pict resource of id bkgnd_id.
//--------------------------------------
void GTool::set_background(int bkgnd_id)
{
	Rect	gameArea;
	SetRect(&gameArea, 0, 0, GAME_WIDTH, GAME_HEIGHT);		
	SATSetPortBackScreen();
	SATGetandDrawPICTResInRect(bkgnd_id, &gameArea);
	SATBackChanged(&gameArea); 
	SATSetPortScreen();
}

//--------------------------------------
//	When we have a coordinate of a point on the screen, how do we know where this point
//  falls in the animation window? We use the offset variables, which are determined by
//  this function.
//--------------------------------------
void GTool::set_offset(void)
{
	SATPort thePort;
	SATGetPort(&thePort);
	GlobalData->v_offset = (*thePort.port).portRect.top;
	//Adjust down because of menu
	GlobalData->v_offset -= 20;
	GlobalData->h_offset = (*thePort.port).portRect.left;
}


	
