// File "gamma.c" - Source for Altering the Gamma Tables of GDevices
//   Last updated 9/24/95, MJS
 
// * ****************************************************************************** *
//
//	This is the Source Code for the Gamma Utils Library file. Use this to build
//		new functionality into the library or make an A4-based library. 
//	See the header file "gamma.h" for much more information. -- MJS
//
// * ****************************************************************************** *

#include <GestaltEqu.h>
#include <Quickdraw.h>
#include <Traps.h>
#include <Video.h>
#include "gamma.h"

long			gammaUtilsInstalled;
GammaTrackerHdl	gammaTables;

// * ****************************************************************************** *
// * ****************************************************************************** *

pascal Boolean IsGammaAvailable() {
	GDHandle theGDevice;

	if (NGetTrapAddress(kGetDeviceListTrapNum, ToolTrap) ==
			NGetTrapAddress(_Unimplemented, ToolTrap)) return(FALSE);
	
	for(theGDevice = GetDeviceList(); theGDevice; theGDevice = GetNextDevice(theGDevice)) {
		if (TestDeviceAttribute(theGDevice, screenDevice) && 
				TestDeviceAttribute(theGDevice, noDriver)) return(FALSE);
		if ((*theGDevice)->gdType == fixedType) return(FALSE);
		}
		
	return(TRUE);
	}

// * ****************************************************************************** *
// * ****************************************************************************** *

pascal Boolean IsOneGammaAvailable(GDHandle theGDevice) {
	
	if (NGetTrapAddress(kGetDeviceListTrapNum, ToolTrap) ==
			NGetTrapAddress(_Unimplemented, ToolTrap)) return(FALSE);
	
	if (TestDeviceAttribute(theGDevice, screenDevice) && 
			TestDeviceAttribute(theGDevice, noDriver)) return(FALSE);
	
	if ((*theGDevice)->gdType == fixedType) return(FALSE);
	
	return(TRUE);
	}

// * ****************************************************************************** *
// * ****************************************************************************** *

pascal OSErr SetupGammaTools() {
	short err=0;
	GammaTrackerHdl tempHdl;
	GammaTblPtr	masterGTable;
	GDHandle theGDevice;

	if (gammaUtilsInstalled == kGammaUtilsSig) return(-1);
	
	gammaTables = 0;
	gammaUtilsInstalled = kGammaUtilsSig;
	for(theGDevice = GetDeviceList(); theGDevice; theGDevice = GetNextDevice(theGDevice)) {
		if (err = GetDevGammaTable(theGDevice, &masterGTable)) return(err);
		
		tempHdl = (GammaTrackerHdl) NewHandle(sizeof(GammaTracker));
		if (tempHdl == 0) return(err = MemError());
		
		(*tempHdl)->size = sizeof(GammaTbl) + masterGTable->gFormulaSize +
				(masterGTable->gChanCnt * masterGTable->gDataCnt * masterGTable->gDataWidth / 8);
		(*tempHdl)->dataOffset = masterGTable->gFormulaSize;
		(*tempHdl)->theGDevice = theGDevice;
		
		(*tempHdl)->saved = (GammaTblHandle) NewHandle((*tempHdl)->size);
		if ((*tempHdl)->saved == 0) return(err = MemError());
		(*tempHdl)->hacked = (GammaTblHandle) NewHandle((*tempHdl)->size);
		if ((*tempHdl)->hacked == 0) return(err = MemError());
	
		BlockMove((Ptr) masterGTable, (Ptr) *(*tempHdl)->saved, (*tempHdl)->size);
		
		(*tempHdl)->next = gammaTables;
		gammaTables = tempHdl;
		}

	return(0);
	}

// * ****************************************************************************** *
// * ****************************************************************************** *

pascal OSErr DoGammaFade(short percent) {
	short err=0;
	register long size, i, theNum;
	GammaTrackerHdl tempHdl;
	unsigned char *dataPtr;

	if (gammaUtilsInstalled != kGammaUtilsSig) return(-1); 
	
	for(tempHdl = gammaTables; tempHdl; tempHdl = (*tempHdl)->next) {
	
		BlockMove((Ptr) *(*tempHdl)->saved, (Ptr) *(*tempHdl)->hacked, (*tempHdl)->size);
		dataPtr = (unsigned char *) (*(*tempHdl)->hacked)->gFormulaData + (*tempHdl)->dataOffset;
		size = (*(*tempHdl)->hacked)->gChanCnt * (*(*tempHdl)->hacked)->gDataCnt;
		
		for(i=0; i < size; i++) {
			theNum = dataPtr[i];
			theNum = (theNum * percent) / 100;
			dataPtr[i] = theNum;
			}
		
		if (err = SetDevGammaTable((*tempHdl)->theGDevice, (*tempHdl)->hacked))
			return(err);
		}
		
	return(0);
	}

// * ****************************************************************************** *
// * ****************************************************************************** *

pascal OSErr DoOneGammaFade(GDHandle theGDevice, short percent) {
	short err=0;
	register long size, i, theNum;
	GammaTrackerHdl tempHdl;
	unsigned char *dataPtr;

	if (gammaUtilsInstalled != kGammaUtilsSig) return(-1); 
	for(tempHdl = gammaTables; tempHdl && (theGDevice != (*tempHdl)->theGDevice);
			tempHdl = (*tempHdl)->next);
	if (! tempHdl) return(err = -1);

	BlockMove((Ptr) *(*tempHdl)->saved, (Ptr) *(*tempHdl)->hacked, (*tempHdl)->size);
	dataPtr = (unsigned char *) (*(*tempHdl)->hacked)->gFormulaData + (*tempHdl)->dataOffset;
	size = (*(*tempHdl)->hacked)->gChanCnt * (*(*tempHdl)->hacked)->gDataCnt;
	
	for(i=0; i < size; i++) {
		theNum = dataPtr[i];
		theNum = (theNum * percent) / 100;
		dataPtr[i] = theNum;
		}
	
	err = SetDevGammaTable((*tempHdl)->theGDevice, (*tempHdl)->hacked);
	
	return(err);
	}

// * ****************************************************************************** *
// * ****************************************************************************** *

pascal OSErr DisposeGammaTools() {
	GammaTrackerHdl tempHdl, nextHdl;

	if (gammaUtilsInstalled != kGammaUtilsSig) return(-1); 
	for(tempHdl = gammaTables; tempHdl; tempHdl = nextHdl) {
		nextHdl = (*tempHdl)->next;
		DisposeHandle((Handle) (*tempHdl)->saved);
		DisposeHandle((Handle) (*tempHdl)->hacked);
		DisposeHandle((Handle) tempHdl);
		}
		
	gammaUtilsInstalled = 0;
	return(0);
	}

// * ****************************************************************************** *
// * ****************************************************************************** *

pascal OSErr GetDevGammaTable(GDHandle theGDevice, GammaTblPtr *theTable) {
	short err=0;
	CntrlParam  *myCPB;
 
	((long *) theTable)[0] = 0;
 
	if (IsOneGammaAvailable(theGDevice) == 0) return(-1);
			
	if ((myCPB = (CntrlParam *) NewPtrClear(sizeof(CntrlParam))) == 0) return(MemError());
	myCPB->csCode = cscGetGamma;
	myCPB->ioCRefNum = (*theGDevice)->gdRefNum;
	((GammaTblPtr **) myCPB->csParam)[0] = theTable;
	err = PBStatus((ParmBlkPtr) myCPB, 0);

	DisposePtr((Ptr) myCPB);
	return(err);
	}

// * ****************************************************************************** *
// * ****************************************************************************** *

pascal OSErr SetDevGammaTable(GDHandle theGDevice, GammaTblPtr *theTable) {
	CntrlParam *myCPB;
	short err=0;
	CTabHandle cTab;
	GDHandle saveGDevice;
 
	if (IsOneGammaAvailable(theGDevice) == 0) return(-1);

	if ((myCPB = (CntrlParam *) NewPtrClear(sizeof(CntrlParam))) == 0) return(MemError());
	myCPB->csCode = cscSetGamma;
	myCPB->ioCRefNum = (*theGDevice)->gdRefNum;
	((GammaTblPtr **) myCPB->csParam)[0] = theTable;
	err = PBControl((ParmBlkPtr) myCPB, 0);
 
	if (err == 0) {
		saveGDevice = GetGDevice();
		SetGDevice(theGDevice);
 		cTab = (*(*theGDevice)->gdPMap)->pmTable;
		SetEntries (0, (*cTab)->ctSize, (*cTab)->ctTable);
		SetGDevice(saveGDevice);
		}

	DisposePtr((Ptr) myCPB);
	return (err);
	}
