/* Spiked 2.0 Chuck Groom
	I use an external library to fade the screen to black. The fader class is a little tool to
	keep track of the current intensity, 0 = black, 100 = bright. */

#include "fades.h"
#include "gamma.h"


//--------------------------------------
//	Constuctor: set-up, make sure screen starts at full intensity
//--------------------------------------
fader::fader()
{
	intensity = 100;
	SetupGammaTools();
}

//--------------------------------------
//	Clean-up
//--------------------------------------
fader::~fader()
{
	fade_in();
	DisposeGammaTools();
}


//--------------------------------------
//	Fade out if the screen is bright
//--------------------------------------
void fader::fade_out(void) 
{
	if(intensity == 100)
		for(; intensity >= 0; intensity -= 5) 
			DoGammaFade(intensity);
}

//--------------------------------------
//	Fade in the screen if it is anything less than full intensity.
//--------------------------------------
void fader::fade_in(void) 
{
	if(intensity < 100)
		for(; intensity <= 100; intensity += 5) 
			DoGammaFade(intensity);
	intensity = 100;
}
