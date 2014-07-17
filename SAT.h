/* #include file for the C interface to SAT */

/* Comment out under TC5: */
/*#include <Color.h>*/
/*#include <WindowMgr.h>*/
/*#include <DialogMgr.h>*/

// Will it work with C++ if I do like this?
#pragma once
#ifdef _cplusplus
extern "C" {
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef nil
#define nil 0L
#endif

	typedef struct SATPort {
		GrafPtr		port;
		GDHandle	device;
		Ptr				rows;
		Rect			bounds;
		Ptr				baseAddr;
		short			rowBytes;
	} SATPort;

	typedef struct Face {
		char			*colorData;
		short			resNum;
		BitMap			iconMask;
		short			rowBytes; /* Note! No flags in the highest bits!*/
		struct face		*next;
		RgnHandle		maskRgn;
		Ptr	rows, maskRows;
  pascal void (*redrawProc)(struct Face *, short);	//Called on depth changes, not needed if the face is loaded fro a cicn}
		} Face, *FacePtr;

	typedef struct Sprite {
/* Variables that you should change as appropriate */
		short			kind;
		Point			position;
		Rect			hotRect, hotRect2;	// Only change hotRect, SAT handles hotRect2
		FacePtr			face;
		pascal void		(*task) (struct Sprite *);
		pascal void		(*hitTask) (struct Sprite *, struct Sprite *);
		pascal void		(*destructTask) (struct Sprite *);
		RgnHandle		clip; /* Clip region to be used when this sprite is drawn. */
/* SAT variables that you shouldn't change: */
		Point			oldpos;
		struct Sprite	*next, *prev;
		Rect			r, oldr;
		FacePtr		oldFace;			//Used by RunSAT2
		Boolean		dirty;		//Used by RunSAT2
/* Variables for free use by the application: */
		short		layer;
		Point			speed;
		short		mode;
		Ptr			appPtr;
		long			appLong;
		} Sprite, *SpritePtr;

typedef struct{
		short	patternType; 	/* 1 = Pattern, PatHandle, 2 = PixPat, PixPatHandle */
		PixPatHandle	thePat; 		/* or PatHandle */
		}	SATPattern, *SATPatPtr, **SATPatHandle;

#define	UpdatePtr	Ptr

typedef struct {
		SATPort		wind;
		short			offSizeH, offSizeV;
		SATPort		offScreen, backScreen;
		long			ox, oy;
		short			pict, bwpict;
		Boolean		fitThePICTs;
		short			sorting, collision, searchWidth;
//		GDHandle		device;
		PixMapHandle	screen;
		Rect			bounds;
		short			initDepth;
		pascal Boolean	(*synchHook) ();
		SpritePtr		sRoot;
		UpdatePtr		updateRoot;
		Boolean		anyMonsters;

		CGrafPtr		ditherOff;
		GDHandle		ditherOffGD;
		SATPort			iconPort;
		SATPort			iconPort2;
		GrafPtr			bwIconPort;

/* Environment-independent globals: */
		FacePtr			faceRoot;	/* Face list */
		Boolean			colorFlag; /* Is this Mac color capable? */
/*Blitters! You should not change these fields yourself. (Possibly for installing a blitter in your program code.)*/
		ProcPtr	rectBlit1, maskBlit1;				//{Supplied - this is either RBlt/MBlt 0 or 1}
		ProcPtr	rectBlit2, maskBlit2;				//{Will probably never be used}
		ProcPtr	rectBlit4, maskBlit4;				//{Supplied}
		ProcPtr	rectBlit8, maskBlit8;				//{Supplied}
		ProcPtr	rectBlit16, maskBlit16;			//{Not supplied}
		ProcPtr	rectBlit32, maskBlit32;			//{Not supplied}
		ProcPtr	curRectBlit, curMaskBlit;		//{Currently selected fast blitter}
		Rect		screenLocalDrawingArea;
		} SATglobalsRec;

/* Constants for SATConfigure */
	enum{kVPositionSort=0, kLayerSort, kNoSort};
	enum{kKindCollision=0, kForwardCollision, kBackwardCollision, kNoCollision, kForwardOneCollision};

// Bug/problem in CodeWarrior! gSAT is declared as gSAT in my code, and the docs,
// but the lib seems to name it GSAT. :-(
// Temporary fix: (68k only???)
#if GENERATINGPOWERPC
#else
#define	gSAT	GSAT
#endif

/* Global variables */
	extern SATglobalsRec	gSAT;	/* Most globals */
	extern pascal void		(*gSoundErrorProc)(OSErr); /* Sound errors reported here */

/* SAT routines */

	typedef pascal void (*TaskPtr)(SpritePtr);
	typedef pascal Boolean (*SynchPtr)(void);
	typedef pascal void (*EmergencyPtr)(void);

/* Initialization */
	pascal void SATConfigure(Boolean, short, short, short);
	pascal void SATInit (short, short, short, short);
	pascal void SATCustomInit (short, short, Rect *, WindowPtr, GDHandle, Boolean, Boolean, Boolean, Boolean, Boolean);
/* Maintainance, background manipulation etc. */
	pascal Boolean SATDepthChangeTest(void);
	pascal void SATDrawPICTs (short, short);
	pascal void SATRedraw(void);
/* Drawing */
	pascal void SATPlotFace(FacePtr, SATPort*, Point, Boolean); /*Draw a Face (icon).*/
	pascal void SATPlotFaceToScreen(FacePtr, Point, Boolean);
	pascal void SATCopyBits(SATPort*, SATPort*, GDHandle, Rect*, Rect*, Boolean);
	pascal void SATCopyBitsToScreen(SATPort*, Rect*, Rect*, Boolean);
	pascal void SATBackChanged (Rect *); /*Tell SAT about changes in BackScreen*/
/* SetPort replacements */
	pascal void SATGetPort (SATPort*);
	pascal void SATSetPort (SATPort*);
	pascal void SATSetPortOffScreen(void); /*Use before Quick-Drawing on offScreen*/
	pascal void SATSetPortBackScreen(void); /*Use before Quick-Drawing on BackScreen*/
	pascal void SATSetPortScreen(void); /*Use to restore after drawing off/backscreen*/
/* Sprite handling */
	pascal FacePtr SATGetFace (short);
	pascal void SATDisposeFace (FacePtr);
	pascal SpritePtr SATNewSprite (short,short,short, TaskPtr); // (*void)(SpritePtr));
	pascal SpritePtr SATNewSpriteAfter (SpritePtr,short,short,short, TaskPtr);
	pascal void SATKillSprite (SpritePtr);
/* Animating */
	pascal void SATRun (Boolean);
	pascal void	SATRun2 (Boolean); //Alternate routine for allowing resting sprites}
/* Advanced special-purpose calls */
	pascal void SATInstallSynch (SynchPtr);
	pascal void SATInstallEmergency (EmergencyPtr);
	pascal void SATSetSpriteRecSize (long);
	pascal void SATSkip(void);
	pascal void SATKill(void); /*Dispose of offscreen buffers to allow re-init*/
	pascal void SATWindMoved(void);			/* For programs with moveable animation window and fast animation */
/* Offscreen - use only if you need an *extra* offscreen buffer. These calls are likely to change in the future!*/
	pascal void SATMakeOffscreen (SATPort*, Rect*);	/*Make offscreen buffer in current screen depth and CLUT.*/
	pascal void SATDisposeOffScreen (SATPort*);		/*Get rid of offscreen*/
	pascal OSErr CreateOffScreen (Rect*, short, CTabHandle, CGrafPtr*, GDHandle*); /*From Principia Offscreen*/
	pascal void DisposeOffScreen (CGrafPtr, GDHandle);			/*From Principia Offscreen*/
/* Face manipulation */
	pascal void SATSetPortMask (FacePtr);
	pascal void SATSetPortFace (FacePtr);
	pascal void SATSetPortFace2 (FacePtr);
	pascal FacePtr SATNewFace (Rect*);
	pascal void SATChangedFace (FacePtr);

/* Most common old names, uncomment for compatibility with old programs */
// #define ConfigureSAT		SATConfigure
// #define InitSAT			SATInit
// #define CustomInitSAT	SATCustomInit
// #define KillSAT			SATKill
// #define RunSAT			SATRun
// #define SkipSAT			SATSkip
// #define GetFace			SATGetFace
// #define DisposeFace		SATDisposeFace
// #define NewSprite		SATNewSprite
// #define NewSpriteAfter	SATNewSpriteAfter
// #define KillSprite		SATKillSprite
// #define NewFace			SATNewFace
// #define ChangedFace		SATChangedFace
// #define PeekOffscreen	SATRedraw

/* New procedures, EXPERIMENTAL, intended for making a C++ interface */
	pascal SpritePtr SATNewSpritePP(SpritePtr, Ptr, short, short, short, TaskPtr);
	pascal void SATCopySprite(SpritePtr, SpritePtr);
	pascal FacePtr SATNewFacePP(Rect*, Ptr);
	pascal FacePtr SATGetFacePP (short, Ptr);
	pascal void SATCopyFace(FacePtr, FacePtr);
	pascal void SATDisposeFacePP (FacePtr);

/* Cicn utilities */
	pascal CIconHandle SATGetCicn (short);
	pascal void SATPlotCicn (CIconHandle, GrafPtr, GDHandle, Rect*);
	pascal void SATDisposeCicn (CIconHandle);

/*Utilities*/
	pascal void SATSetStrings (Str255, Str255, Str255, Str255, Str255, Str255, Str255, Str255);
	pascal Boolean SATTrapAvailable (short);
	pascal void SATDrawInt (short);
	pascal void SATDrawLong (long);
	pascal short SATRand (short);
	pascal short SATRand10(void);
	pascal short SATRand100(void);
	pascal void SATReportStr (Str255);
	pascal Boolean SATQuestionStr(Str255);
	pascal void CheckNoMem (Ptr); /*If the Ptr is nil, out of memory emergency exit*/
	pascal short SATFakeAlert (Str255, Str255, Str255, Str255, short, short, short, Str255, Str255, Str255);
	pascal void SATSetMouse (Point);
	pascal void SATInitToolbox(void);
	pascal void SATGetVersion(Str255 versionString);
/* Pattern utilities */
	pascal void SATPenPat (SATPatHandle);
	pascal void SATBackPat (SATPatHandle);
	pascal SATPatHandle SATGetPat (short);
	pascal void SATDisposePat (SATPatHandle);
/* Menu bar utilities */
	pascal void SATShowMBar(WindowPtr);
	pascal void SATHideMBar(WindowPtr);
/* PICT utilities */
	pascal void SATGetandDrawPICTRes (short id);
	pascal void SATGetandDrawPICTResInRect (short id, Rect *frame);
	pascal void SATGetandCenterPICTResInRect (short id, Rect *frame);
/*Sound*/
	pascal void SATInitSound(void);			/* No longer necessary */
	pascal void SATSoundPlay (Handle, short, Boolean);	/* Play sounds with priority handling */
	pascal void SATSoundShutup(void);		/*Silence, dispose of sound channel*/
	pascal void SATSoundEvents(void);		/* Call this once in a while when not calling RunSAT often */
	pascal Boolean SATSoundDone(void);	/* Any sound going on?*/
	pascal Handle SATGetSound (short);
	pascal Handle SATGetNamedSound (Str255);
	pascal void SATDisposeSound (Handle);
	pascal void SATSoundOn(void);
	pascal void SATSoundOff(void);
/* Multi-channel sound routines */
	pascal short SATSoundInitChannels (short);
	pascal Boolean SATSoundDoneChannel (short);
	pascal void SATSoundPlayChannel (Handle, short);
	pascal void SATSoundReserveChannel (short, Boolean);
	pascal void SATSoundShutupChannel (short);
	pascal void SATPreloadChannels(void);
/* Experimental, likely to be renamed/removed/changed: */
	pascal void SATSoundPlay2 (Handle, short, Boolean,Boolean);
	pascal void SATSoundPlayEasy (Handle, Boolean);
/* More multi-channel: */
	pascal int SATGetNumChannels(void);
	pascal Ptr SATGetChannel (int);
/* Customization */
	pascal void SATSetSoundInitParams (long);


// Will it work with C++ if I do like this?
#ifdef _cplusplus
}
#endif

