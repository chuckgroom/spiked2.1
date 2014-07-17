/* Spiked 2.0
	Chuck Groom
	
	The pref class stores the associtation betwen a KEY_TYPE and the char which represents it.
	It reads this data from disk, and when it is destructed it writes this data to disk.
*/

#ifndef PREF_H
#define PREF_H

enum KEY_TYPE {	GREEN_LEFT = 0,
				GREEN_RIGHT,
				GREEN_THRUST,
				GREEN_ITEM,
				GREEN_CHANGE_ITEM,
				RED_LEFT,
				RED_RIGHT,
				RED_THRUST,
				RED_ITEM,
				RED_CHANGE_ITEM,
				ZILCH };

class prefs
{
	private:
		char Game_Keys[10];	//A list of the chars. Game_Key[n] refers to char related to the nth KEY_TYPE
	public:
		prefs();
		~prefs();
		KEY_TYPE interpret_key (char);
		void set_command(KEY_TYPE the_key, char the_char);
		char command_is_key(KEY_TYPE the_key) {return Game_Keys[the_key];}
};


#endif
