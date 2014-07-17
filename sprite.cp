/* Spiked 2.0
	Chuck Groom, 1997
	
	The sprite class serves as the parent class for all sprites. It handles all the nitty-gritty
	stuff of sprite behavior, animation, and iterface with the Sprite Animation Toolkit. */
	
#include "sprite.h"
#include "SAT.h"
#include "global.h"
#include "stdlib.h"

//This blank procedure is used by the SAT
pascal void empty_proc(Sprite * s) 
{}


//----------------------------------------------
// Constructor: Set values, create the SAT sprite
//----------------------------------------------
sprite::sprite(fPoint p, sprite_manager * s_m)
{
	the_sprite_manager = s_m;
	position.h = p.h;
	position.v = p.v;
	
	//Default settings
	alive = TRUE;
	mass = 16000;
	velocity.h = velocity.v = 0;
	num_of_faces = 0;
	cur_face = 0;
	collide_radius = draw_radius = 0;
	myFaces = NULL;	
	thrust_disable = 0;
	
	//Assume sprite can't be hit
	collideable = FALSE;
	
	// Initialize the SAT package's sprite. 
	theSprite = SATNewSprite(0, (int) position.h - draw_radius, (int) position.v - draw_radius, &empty_proc);
	if (theSprite ==  NULL)
		exit(1);
	theSprite->task = empty_proc;
}


//----------------------------------------------
// De-allocate, kill the sprite
//----------------------------------------------
sprite::~sprite()
{
	delete myFaces;
	if (theSprite) 
	{
		theSprite->task = nil;	//Tells SAT that this sprite is very dead
		SATRun(TRUE);
		theSprite = NULL;
		
	}
}



//----------------------------------------------
// All sprites invoke their own do_behavior functions, then cycle them up to the
// parent class. This is the highest part of the heirarchy, and it places the sprite in
// the correct position on the screen.
//----------------------------------------------
void sprite::do_behavior(void)
{
	if (alive) 
	{
		if (thrust_disable)		//For use by players who have been twisted
			thrust_disable--;
		
		//Set the correct frame of animation
		theSprite->face = GlobalData->GetFace(myFaces[cur_face]);
		
		//Place the sprite correctly on the screen
		theSprite->position.h = (int) (position.h - draw_radius);
		theSprite->position.v = (int) (position.v - draw_radius);
	}
	else
		theSprite->face = GlobalData->GetFace(0);	//erase from screen
}


//----------------------------------------------
// Is the circle defined by p and r within the sprite?
//----------------------------------------------
Boolean sprite::circle_in_sprite(fPoint p, int r)
{
	float total_radius = collide_radius + r;
	float x_dif = position.h - p.h;
	float y_dif = position.v - p.v;
	
	if ((sqrt (x_dif*x_dif + y_dif*y_dif)) <= total_radius)
		return TRUE;
	return FALSE;
}




//----------------------------------------------
/*Each sprite uses different faces. This has the hard-wired IDs of the faces used by each sprite.
	While hard-wiring isn't an elegant solution, it is appropriate for a single-use situation, in
	the same way that coding  "t" means a "sword" in NetHack would be OK.*/
//----------------------------------------------
void sprite::load_faces(void)
{
	switch(myType)
	{
		case SPIKE:
			num_of_faces = 29;	//resources 5 to 33, inclusive
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 5 + ii;
			break;
		case BRICK:
			num_of_faces = 17;	//resources 132 to 148, inclusive
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 132 + ii;
			break;
		case GREEN_PLAYER:
			num_of_faces = 60;	//resources 36 to 65 and 190 to 219, inclusive
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < 30; ii++)
				myFaces[ii] = 36 + ii;
			for (int kk = 0; kk < 30; kk++)
				myFaces[kk + 30] = 190 + kk;
			break;
		case RED_PLAYER:
			num_of_faces = 60;	//resources 158 to 187 and 221 to 250, inclusive
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < 30; ii++)
				myFaces[ii] = 158 + ii;
			for (int kk = 0; kk < 30; kk++)
				myFaces[kk + 30] = 221 + kk;
			break;
		case TWISTER:
			num_of_faces = 14;	//resources 76 to 89, inclusive
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 76 + ii;
			break;
		case GIFT:
			num_of_faces = 6;	//resources 91 to 96, inclusive
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 91 + ii;
			break;
		case CANNON_BALL:
			num_of_faces = 1;	//resource 90
			myFaces = new int[num_of_faces];
			myFaces[0] = 90;
			break;
		case NEW_BUTTON:
			num_of_faces = 2;
			myFaces = new int[num_of_faces];
			myFaces[0] = 1; 
			myFaces[1] = 2;
			break;
		case PREF_BUTTON:
			num_of_faces = 2;
			myFaces = new int[num_of_faces];
			myFaces[0] = 3; 
			myFaces[1] = 4;
			break;
		case ABOUT_BUTTON:
			num_of_faces = 2;
			myFaces = new int[num_of_faces];
			myFaces[0] = 279; 
			myFaces[1] = 280;
			break;			
		case PLAYER_ITEM:
			num_of_faces = 6;
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)	//CICNs# 110-115
				myFaces[ii] = 110 + ii;
			break;	
		case LIFE:
			num_of_faces = 6;
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)	//CICNs# 96-101
				myFaces[ii] = 96 + ii;
			break;	
		case RIFT:
			num_of_faces = 15;						//CICNs# 35, 118-131
			myFaces = new int[num_of_faces];
			myFaces[0] = 35;
			
			for (int ii = 0; ii < num_of_faces - 1; ii++)
				myFaces[ii + 1] = 118 + ii;
			break;
		case EXPLOSION:
			num_of_faces = 9;						//CICNs# 35, 102-109
			myFaces = new int[num_of_faces];
			myFaces[0] = 35;
			for (int ii = 0; ii < num_of_faces - 1; ii++)
				myFaces[ii + 1] = 102 + ii;
			break;
		case GO_INTRO_BUTTON:
			num_of_faces = 1;						//CICNs# 157
			myFaces = new int[num_of_faces];
			myFaces[0] = 157;
			break;
		case GAME_KEY_BUTTON:
			num_of_faces = 8;						//CICNs# 149-156
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 149 + ii;
			break;
		case BULLET:
			num_of_faces = 5;						//CICNs# 68-72
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 68 + ii;
			break;
		case TWISTED:
			num_of_faces = 8;						//CICNs# 253-60
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 253 + ii;
			break;
		case GRAVITRONED:
			num_of_faces = 6;						//CICNs# 261-66
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 261 + ii;
			break;
		case GRAVITRON:
			num_of_faces = 12;						//CICNs# 267-78
			myFaces = new int[num_of_faces];
			for (int ii = 0; ii < num_of_faces; ii++)
				myFaces[ii] = 267 + ii;
			break;
		default:
			break;
	}
}
