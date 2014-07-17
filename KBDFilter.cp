/* Chuck Groom, Spiked 2.0

	The Keyboard filter: turns the fact that a key is down ==> game event.
	
   Macintosh KeyDown events aren't a very useful way to monitor which keys have been pressed
   becuase that only records ONE key being down. How do we deal with many keys being pressed 
   at once?

	The answer is that we read in the keyboard directly. The KeyMap data structure is four longs. 
	Each bit of these longs corresponds to a key on the keyboard which is down. I read the entire
	keymap. 
	
	However, the next question becomes, how do we tell which bit of the KeyMap corresponds to which
	char? To solve this, I generated a conversion table, "Spiked.dat" which stores a list of the
	bit numbers (i.e. 010000000000 => 2 ) and the char it represents. When initialized, the 
	keyboard_filter reads in this data from disc. */


#include <fstream.h>
#include <stdlib.h>
#include "KBDFilter.h"
#include "EventArbitrator.h"

//--------------------------------------
//	Constructor: read in the key_map -> char conversion for the keyboard.
//--------------------------------------
keyboard_filter::keyboard_filter()
{
	//read in the int -> char map of the keyboard:
	ifstream key_data("spiked.dat");
	if (!key_data) //if file doesn't exist
	{
		//Crash and burn
		exit(1);
		return;
	}

	
	int char_num, char_code;
	for (int counter = 0; !key_data.eof() && (counter < 150); counter++)
	{
		key_data >> char_num;
		key_data >> char_code;
		
		keyboard_map[counter][0] =  char_num;
		keyboard_map[counter][1] =  char_code;
	}
	key_data.close();
}


//--------------------------------------
//	For each key is that pressed, generate a Game_Event and push it onto the stack.
//--------------------------------------
void keyboard_filter::run_filter(class Event_Stack *stack)
{
	KeyMap km;
	GetKeys(km);
	Point where;
	where.h = where.v = 0;
	for (int counter = 0; counter < 128; counter++)
	{
		if(GetBit(&km, counter)) 
			stack->push (new Game_Event(GkeyDown, where, (char) lookup_code(counter)));
	}
}


//--------------------------------------
//	Are any keys down at all? Since the key_map is four longs, if these longs all equal zero,
//  the answer is no.
//--------------------------------------
Boolean keyboard_filter::key_is_down(void)
{
	KeyMap km;
	GetKeys(km);
	return (km[0] || km[1] || km[2] || km[3]);
}
