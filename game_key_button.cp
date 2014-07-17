/* Spiked 2.0
	Chuck Groom, 1997
	
	A game_key_button  sprite. This sprite -- and there are ten of them on the screen --
	displays the char which represents the KEY_TYPE action. Each button keeps track of the
	other 7 so it can de-active them. When a new char is selected, the button is responsible
	for drawing it on the screen. */
	
#include "sprite.h"
#include "SAT.h"
#include "global.h"
#include "EventArbitrator.h"


game_key_button::game_key_button(fPoint p, sprite_manager * s_m, Event_Stack * q, game_key_button ** k_b, KEY_TYPE k, char c) : button_sprite(p, s_m, q)
{
	myType = GAME_KEY_BUTTON;
	my_key = k;
	set_char(c);
	key_buttons = k_b;
	load_faces();
	cur_face = 0;
	draw_radius = collide_radius = 11;
}



//-------------------------------------------
// If the button is clicked, its mode is set to 1 and all the other's modes are set to 0.
// This means that this button's animation is turned on, and all the others are turned off.
//-------------------------------------------
void game_key_button::do_clicked(void)
{	
	//first of all, tell all the other key_buttons to de-animate by setting their modes to 0
	for (int ii = 0; ii < 10; ii++)
		key_buttons[ii]->set_button_mode(0);
	
	//Turn on this button's animation
	button_mode = 1;
}




//-------------------------------------------
// If the button is clicked, its mode is 1 and it will be animated.
//-------------------------------------------
void game_key_button::do_behavior()
{
	if(button_mode != 0)
	{
		cur_face++;
		cur_face = (cur_face % (num_of_faces - 1)) + 1;	//cycle through the frames, skipping the first one which is empty.
	}
	else
	{
		cur_face = 0;
	}
	
	button_sprite::do_behavior();
}






//------------------------------------------------------
// When the character that the button represents it changed, this change must be drawn on the screen.
//-------------------------------------------------------
void game_key_button::set_char (char c)
{
	the_char = c;
	char small_string[3];	//make a small string to print the char to screen.
	small_string[0] = the_char;
	small_string[1] =  ' ';
	small_string[2] =  '\0';
	
	//Special case characters: the arrow keys. Display "lf, rt, up, dw" depending on which arrow.
	switch(the_char)
	{
		case '': //left arrow
			small_string[0] = 'L'; small_string[1] = 'f'; 
			break;
		case '': //right arrow
			small_string[0] = 'R'; small_string[1] = 't'; 
			break;
		case '': //up arrow
			small_string[0] = 'U'; small_string[1] = 'p';
			break;
		case '': //down arrow
			small_string[0] = 'D'; small_string[1] = 'n';
			break;
	}
	
	
	//Set the graphics port to backscreen
	SATSetPortBackScreen();
	
	//The region we are drawing in:
	Rect theArea;
	SetRect(&theArea, position.h - 10, position.v - 10, position.h + 10, position.v + 10);
	
	//Make the color for erasing stuff white
	RGBColor the_color;
	the_color.red = the_color.green = the_color.blue = 65535;
	RGBBackColor(&the_color);
	
	//Erase the region.
	EraseRect(&theArea);
	
	//Draw the string in the region.
	TextSize(9);
	MoveTo(position.h - 6, position.v + 4);
	DrawString(C2PStr(small_string));
	
	//Tell SAT that the background has been modified.
	SATBackChanged(&theArea);
	
	//Reset the graphics port
	SATSetPortScreen();
}
