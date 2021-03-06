/*	Spiked 2.0 Chuck Groom
	Menu management system */
	
#include "Menus.h"
#include <stdlib.h>
#include <Sound.h>
#include "SAT.h"
#include "string.h"
//-------------------------------------------------------------
//	Create all the menus at startup
//-------------------------------------------------------------
Game_Menu::Game_Menu()
{
	appleMenu = GetMenu(AppleMenuID);
	fileMenu = GetMenu(FileMenuID);
	soundvolumeMenu = GetMenu(SoundVolumeMenuID);

	InsertMenu(appleMenu, 0);
	InsertMenu(fileMenu, 0);
	InsertMenu(soundvolumeMenu, -1);
	
	AddResMenu(appleMenu, 'DRVR');

	//Get the starting sound volume
	Start_Sound_Vol = 0;
	GetSoundVol(&Start_Sound_Vol);
	OnlyChecktheSoundItem(Start_Sound_Vol + 1);

	end_game_str = new char[strlen("Are you sure you want to end the game?")+2];
	strcpy(end_game_str, "Are you sure you want to end the game?");
	C2PStr(end_game_str);

	DrawMenuBar();
}

//--------------------------------------------------------------
//  The closing of the menus indicates the close of the game. 
//  Since the menu system is responsible for the volume, the volume must be reset
//  when the game closes.
//--------------------------------------------------------------
Game_Menu::~Game_Menu()
{
 	SetSoundVol(Start_Sound_Vol);
}

//--------------------------------------------------------------  
//DoMenuChoice
//--------------------------------------------------------------
Game_Event * Game_Menu::DoMenuChoice (long menuChoice)
{
	if (menuChoice == 0)
		return NULL;
	
	short theMenu, theItem;
	
	theMenu = HiWord(menuChoice);
	theItem = LoWord(menuChoice);

	HiliteMenu(theItem);

	switch (theMenu)
	{
		case AppleMenuID:
			HiliteMenu(0);
			return DoAppleMenu(theItem);
		break;
		
		case FileMenuID:
			HiliteMenu(0);
			return DoFileMenu(theItem);
		break;
		
		case SoundVolumeMenuID:
			DoSoundVolumeMenu(theItem);
		break;
	}
	
	HiliteMenu(0);

	return NULL;
}


//--------------------------------------------------------------  
//DoAppleMenu
//--------------------------------------------------------------  
Game_Event * Game_Menu::DoAppleMenu (short theItem)
{
	Str255		daName;
	GrafPtr		wasPort;
	short		daNumber;
	
	switch (theItem)
	{
		case AAbout:
			return new Game_Event(Gabout_arena);
			break;
		
		default:
			GetItem(appleMenu, theItem, daName);
			GetPort(&wasPort);
			daNumber = OpenDeskAcc(daName);
			SetPort((GrafPtr)wasPort);
			break;
	}

	return NULL;
}


//--------------------------------------------------------------  
//DoFileMenu -- Create new games, end games, or quit events.
//--------------------------------------------------------------  
Game_Event * Game_Menu::DoFileMenu (short theItem)
{
	switch (theItem)
	{
		case FNew:
			return new Game_Event(Gplay_arena);
			break;
		
		case FEnd:
			ShowCursor();		//Make SURE the mouse is visible.
			if (SATQuestionStr((unsigned char *)end_game_str))
			{
				SATRedraw();
				return new Game_Event(Gintro_arena);
			}
			SATRedraw();
			break;

		case FPref:
			return new Game_Event(Gpref_arena);
			break;

		case FZilch:
			//Do nothing since this is the little grey menu bar
			break;
		
		case FQuit:
			return new Game_Event(Gquit);
			break;
	}
	return NULL;
}


//--------------------------------------------------------------  
//DoSoundVolumeMenu
//Set the volumes of the sound. This is a submenu of the options menu.
//--------------------------------------------------------------  
void Game_Menu::DoSoundVolumeMenu (short theItem)
{
	OnlyChecktheSoundItem(theItem);
	SetSoundVol(theItem - 1);
}


//--------------------------------------------------------------  
//OnlyChecktheSoundItem
//Put a check mark next to the current sound volume, and only the sound volume
//--------------------------------------------------------------  
void Game_Menu::OnlyChecktheSoundItem(short int theItem)
{
	for (int counter = 0; counter <= 8; counter++)
		CheckItem(soundvolumeMenu, counter, counter == theItem);
}


//--------------------------------------------------------------  
//Set the new game, prefs, and about menus to disabled when a game is being played.
//--------------------------------------------------------------  
void Game_Menu::menus_intro_mode(void)
{
	EnableItem(appleMenu, AAbout);

	EnableItem(fileMenu, FNew);
	DisableItem(fileMenu, FEnd);
	EnableItem(fileMenu, FPref);
}

//--------------------------------------------------------------  
//Set the new game, prefs, and about menus to disabled when a game is being played.
//--------------------------------------------------------------  
void Game_Menu::menus_game_mode(void)
{
	DisableItem(appleMenu, AAbout);

	DisableItem(fileMenu, FNew);
	EnableItem(fileMenu, FEnd);
	DisableItem(fileMenu, FPref);
}

