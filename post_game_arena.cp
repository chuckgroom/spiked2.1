/*	Spiked 2.0
	Chuck Groom
	
	The post-game display shows a picture of either the red or the green ship winning, depending
	on the victor, of course.  */

#include "arena.h"
#include "global.h"
#include "time.h"


//------------------------------------
//	When created, the about_arena draws the about pict, and creates a button to
//  return to the main screen. In addition, there are many small sprites that just flit about,
//  animating themselves.
//------------------------------------
post_game_arena::post_game_arena(Event_Stack * q) : arena(q)
{
	GlobalTool->fade_out();
	GlobalTool->menus_intro_mode();
	
	game_result the_result = GlobalData->read_result();
	wait_frames = 0;
	switch (the_result)		//If something interesting happend
	{
		case RED_WON:
			GlobalTool->set_background(RED_WON_PICT);
			GlobalTool->set_background(RED_WON_PICT);
			break;
		case GREEN_WON:
			GlobalTool->set_background(GREEN_WON_PICT);
			GlobalTool->set_background(GREEN_WON_PICT);
			break;
		case NO_WIN:
			wait_frames = GAME_RESULT_PAUSE;
			break;
	}
	
	GlobalData->write_result(NO_WIN);

	
	// Make the screen all nice and ready
	SATRedraw();
	SATRun(TRUE);
	
	GlobalTool->fade_in();
	
	switch (the_result)		//Make a sound
	{
		case RED_WON:
			GlobalData->PlaySound(HAHA);
			break;
		case GREEN_WON:
			GlobalData->PlaySound(YEAH);
			break;
		case NO_WIN:
			break;
	}
}


// ------------------------------------------------------------------------------
// The loop that runs until the number of frames defined in the globals is used up.
// -------------------------------------------------------------------------------
void post_game_arena::run_frame(void)
{
	long L = TickCount();
	
	if (wait_frames < GAME_RESULT_PAUSE)
	{
		
		SATRun(TRUE);
		wait_frames++;
		
		//Speed limit: Slow the animation down
		while (L > (TickCount() - SPEED_LIMIT))
			;
	}
	else
	{	
		//Go back to the intro screen
		GlobalTool->fade_out();
		theStack->push (new Game_Event(Gintro_arena));
	}
}
