/*	Spiked 2.0, Chuck Groom 1997

	There are ten keys used to control Spiked. The prefs file keeps track of which key 
	corresponds to which control. There can be only a one-to-one mapping of a key to 
	a control.
	
	There is a data file, "spiked.pref," that stores the last settings. When the program
	is opened, the prefs are created, and the pref file is read in. Then, when the program
	terminates the current prefs are saved to disk. */

#include "prefs.h"
#include <fstream.h>

//--------------------------------------
//	Read the prefs from disk; if no pref file exists, use the default settings.
//--------------------------------------
prefs::prefs()
{
	ifstream pref_stream("spiked.pref");
	if (!pref_stream)
	{
		//use hardwired prefs:
		Game_Keys[0] = 'a';
		Game_Keys[1] = 'd';
		Game_Keys[2] = 'w';
		Game_Keys[3] = 's';
		Game_Keys[4] = 'q';
		Game_Keys[5] = 'j';
		Game_Keys[6] = 'l';
		Game_Keys[7] = 'i';
		Game_Keys[8] = 'k';
		Game_Keys[9] = 'u';
		return;
	}
	
	for (int counter = 0; counter < 10; counter++)
	{
		pref_stream >> Game_Keys[counter];
	}
	
	pref_stream.close();
}


//--------------------------------------
//	Write the prefs to disk when the program quits.
//--------------------------------------
prefs::~prefs()
{
	ofstream pref_stream("spiked.pref");
	
	for (int counter = 0; counter < 10; counter++)
	{
		pref_stream << Game_Keys[counter];
		pref_stream << endl;
	}
	
	pref_stream.close();
}


//--------------------------------------
//	The pref filter -- what command (KEY_TYPE) does a char signify, if any?
//  Note that the KEY_TYPE enumerations are (0, 1...7), so the index of the Game_Keys[]
//  array corresponds to a KEY_TYPE.
//--------------------------------------
KEY_TYPE prefs::interpret_key(char the_char)
{
	for (int counter = 0; counter < 10; counter++)
	{
		if(Game_Keys[counter] == the_char)
			return (KEY_TYPE) counter;
	}
	return ZILCH;
}


//--------------------------------------
//	Change the key corresponding to a KEY_TYPE, if that is a unique key.
//--------------------------------------
void prefs::set_command(KEY_TYPE the_key, char the_char)
{
	//must be a unique character
	for (int counter = 0; counter < 10; counter++)
	{
		if(Game_Keys[counter] == the_char)
			return;
	}
	
	Game_Keys[(int) the_key] = the_char;
}
