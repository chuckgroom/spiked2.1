/* Spiked 2.0
	Chuck Groom
	
	The keyboard filter class keeps track of the mapping between the keyboard and the char
	each keypress represents.  */
	
#ifndef KBDF
#define KBDF

#include "EventArbitrator.h"

class keyboard_filter
{
	private:
		int keyboard_map[150][2];	//The map of the key location, and the corresponding char.
		int GetBit(KeyMap *data, int offset)
			{
	       	 	int d = ((unsigned char *)data)[offset >> 3];
	        	return (d & (1 << (offset & 7))) != 0;
			}
			
		int lookup_code (int the_code)
			{
				char result = '\0';
				for (int ii = 0; ii < 150; ii++)
				{
					if (keyboard_map[ii][1] == the_code)
						result = keyboard_map[ii][0];
				}
				return result;
			}
	public:
		keyboard_filter();
		void run_filter(class Event_Stack *stack);
		Boolean key_is_down(void);
};


#endif
