#include "Game.h"

Game::Game() : currState(MAIN_MENU) {
	// Init Ncurses
	initscr();
	keypad(stdscr,true);
	curs_set(0);				// Hide cursor
	noecho();					// Do not print out pressed keys
	nodelay(stdscr, true);		// Do not wait for input when getch()
	
	Handler::init();
	
	// TODO Load Main Menu
	ChoiceVect choices;
	choices.push_back( std::make_pair("New Game", NEW_LEVELS) );
	choices.push_back( std::make_pair("Exit application", EXIT) );
	mainMenu.setChoices(choices);
	mainMenu.setHead("MAIN MENU");
}

Game::~Game() {
	endwin();
}

void Game::update() {
	int tmp;
	UserInput::update();
	info.update();
	
	switch(currState) {
		case(MAIN_MENU):
			tmp = mainMenu.update();
			if(tmp != -1) {
				// New state was chosen
				currState = (GameState) tmp;
				//nodelay(stdscr, true);
			} else {
				//nodelay(stdscr, false);
			}
			
			break;
		case(NEW_LEVELS):
			levels = new Levels();
			currState = LEVELS;
			break;
		case(LEVELS):
			levels->update();
			if(levels->getLevelState() == levels->EXIT) {
				currState = LEVELS_ENDED;
			}
			break;
		case(LEVELS_ENDED):
			delete levels;
			currState = MAIN_MENU;
			break;
		case(EXIT):
			break;
	}
	
}

void Game::paint() {
	clear();
	
	switch(currState) {
		case(MAIN_MENU):
			mainMenu.paint();
			break;
		case(LEVELS): 
			levels->paint();
			break;
		case(EXIT):
			break;
	}
	
	refresh();
}

bool Game::isRunning() const {
	return currState != GameState::EXIT;
}