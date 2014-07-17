/* Spiked 2.0
	Chuck Groom

	The fader class header. Used for fading in/out of black */

#ifndef FADE_H
#define FADE_H
 
class fader
{
	private: 
		int intensity;
	public:
		fader();
		~fader();
		void fade_out(void);
		void fade_in(void);
};

#endif
