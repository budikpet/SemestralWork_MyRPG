/* 
 * File:   Game.h
 * Author: petr
 *
 * Created on May 8, 2017, 4:53 PM
 */

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <ncurses.h>
#include <list>
#include "Levels.h"
#include "Info.h"
#include "UserInput.h"
#include "Menu.h"

#define ChoiceList std::vector< std::pair<std::string, int> >

/**
 * Class represents the whole game.
 * It handles all updates and paints.
 */
class Game {
public:	
	/**
	 * All different major game states.
	 */
	enum GameState {
		MAIN_MENU,
		GAME,
		EXIT
	};
	
	Game();
	
	~Game();
	
	/**
	 * Updates whole game.
	 */
	void update();
	
	/**
	 * Paints whole game.
	 */
	void paint();
	
	/**
	 * @return true if game is still running.
	 */
	bool isRunning() const;
	
private:
	GameState currState;
	Menu mainMenu;
	Info configs;
	Levels* levels;
	
	
};

#endif /* GAME_H */

