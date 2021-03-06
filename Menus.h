/* 	Spiked 2.0, 1997 Chuck Groom

	Menus.h: The menu system and associated constants. */
	
#ifndef MENU_H
#define MENU_H

#include "EventArbitrator.h"

//Menu system constants:
const int AppleMenuID = 100;
const int FileMenuID = 128;
const int SoundVolumeMenuID = 130;

//Enumerated keys refering to the number of the item in each menu.
const int AAbout = 1;
enum FileMenuItem {FNew = 1, FEnd, FPref, FSound, FZilch, FQuit};
enum SoundVolumeMenuType {Zero = 1, One, Two, Three, Four, Five, Six, Seven};
		
		
//-----------------------------------------------------
//The Game_Menu class catches menu events and responds appropriatly.
//-----------------------------------------------------
class Game_Menu
{
	private:
		MenuHandle	appleMenu, fileMenu, soundvolumeMenu;
		
		Game_Event * DoAppleMenu (short);
		Game_Event * DoFileMenu (short);
		void DoSoundVolumeMenu (short);
		void OnlyChecktheSoundItem(short int);
		
		short Start_Sound_Vol;
		
		char * 	end_game_str;
	public:
		Game_Menu();
		~Game_Menu(); 
		Game_Event * DoMenuChoice (long menuChoice);
		void menus_intro_mode (void);
		void menus_game_mode (void);
};

#endif
