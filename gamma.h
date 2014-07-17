// File "gamma.h" - Header for Altering the Gamma Tables of GDevices

// * ****************************************************************************** *
//
//	This library is intended as a general tool for manipulating the Gamma Tables
//		of Graphics Devices, to ramp them up or down in order to achieve smooth
//		screen fades. The source is included for programmers who want to convert
//		the library to A4-based, but it is not commented for public consumption.
//	The library defines 2 globals to save state data, but the entire Table 
//		manipulation is performed with unlocked handles to be easy on your heap.
//		The typical memory chunk is about 600 bytes for a 13" Monitor in 8-bit 
//		depth, or about 1700 bytes for one in 24-bit color. Usage will vary.
//	Of course, the Classic Mac cannot use Gamma Fades, only Mac II or later machines
//		with attached monitors. (I don't know about the Color Classic tho!). Also,
//		GDevice manipulation needs to follow InitGraf() & InitWindows() calls.
//	Please use the listed functions to see if you can use this code before you set
//		it up. As usual, this stuff is not warranteed, guaranteed, or anything --
//		use it at your own risk. It is not Apple-recommended for anything, but it
//		worked for me, so there!
//	This library is made possible by the hard work of Matt Slot (fprefect@umich.edu)
//		and Matt Mora (mxmora@apple.com), and all the others who have made
//		suggestions and bug reports. Your feedback is appreciated  -- really!
//	
//	
//		Written:	12/17/92, Matt Slot, 			
//		
//		Updated:	 3/13/93, MJS	(v1.1)											
//						�>	Updated the GammaAvail calls to be more honest.
//							Actually check to see if Grafix Devices are supported
//							on this machine w/o using Gestalt. Also, used the 
//							std. Toolbox calls to test the GDevice attributes.
//						�>	Removed extraneous calls to lock handles in several
//							locations.
//						�>	Fixed bug in DoOneGammaFade which failed if the device
//							could not be found in the list.
//						�>	Changed function prototypes to be more intuitive.
//						�>	Updated the descriptions in header file.
//						�>  Thanks to David Phillip Oster, oster@well.sf.ca.us,
//							for his numerous suggestions and criticisms. :)
//		
//		Updated:	 11/9/93, MJS	(v1.1.1)									
//						�>	Fixed incompatibility with EvenBetterBusError...
//							OK, it was an obscure bug (dereferencing once too
//							often), but didnt seem to break except with EBBE.
//		
//		Updated:	 11/9/93, MJS	(v1.1.2)									
//						�>	Left a Debugger() in the posted application.
//	
//		Updated:	 6/29/95, MJS	(v1.1.3)									
//						�>	Updated IsGammaAvailable() routines to check for
//							Fixed CLUT devices... especially powerbooks.
//	
//		Updated:	 9/24/95, MJS	(v1.2)									
//						�>	Declared each function as Pascal
//						�>	Renamed some variable and structs
//						�>	Converted source and tester to Metrowerks 6
//						�>	Bundled MJS's C and MXM's Pascal projects
//	
//		Oh yeah, this stuff is free to anyone interested in it.
//
// * ****************************************************************************** *

//	A quick signature
#define kGammaUtilsSig	'GAMA'

//	To help check for compatibility
#define kGetDeviceListTrapNum		0xAA29

// * ****************************************************************************** *

//	Internal data storage
typedef struct GammaTracker {
	short size, dataOffset;
	GammaTblHandle saved, hacked;
	GDHandle theGDevice;
	struct GammaTracker **next;
	} GammaTracker, *GammaTrackerPtr, **GammaTrackerHdl;
	
// * ****************************************************************************** *
// * ****************************************************************************** *
// Function Prototypes

pascal Boolean IsGammaAvailable(void);
pascal Boolean IsOneGammaAvailable(GDHandle theGDevice);

//	These routines help you determine whether you can use the Gamma Table Utils
//		on the current machine. The first checks all attached monitors, and the 
//		second just checks the indicated monitor. Each returns TRUE if you can 
//		use the functions, or FALSE if you can't. � Note: Before calling any other
//		Gamma Table function below, use this function to see if you are allowed.

// * ****************************************************************************** *

pascal OSErr SetupGammaTools(void);
pascal OSErr DisposeGammaTools(void);

//	These routines must bracket any calls to the Gamma Table functions, perhaps
//		at the head and tail of your main(). The first sets up the data structures
//		necessary to save and restore the state of your monitors. The second
//		disposes of all the internal data structures, but does not reset the
//		monitors to their original states. Both return the error code if some
//		part failed. 

// * ****************************************************************************** *

pascal OSErr DoGammaFade(short percent);
pascal OSErr DoOneGammaFade(GDHandle theGDevice, short percent);

//	Use the first function to Fade each of your monitors to some percentage of their
//		initial brightness (100 = bright, 0 = dim). Repeatedly call this to ramp your
//		monitors up or down. The second function performs the same function, but only
//		for the specified monitor. Both return any applicable error codes.
//	Be sure to set up the necessary save-state data structures before you start by
//		calling the compatibility and initialization functions. 

// * ****************************************************************************** *

pascal OSErr GetDevGammaTable(GDHandle theGDevice, GammaTblPtr *theTable);
pascal OSErr SetDevGammaTable(GDHandle theGDevice, GammaTblPtr *theTable);

//	These routines are low-level interfaces to the device drivers for the monitors.
//		Use them at your own risk.
