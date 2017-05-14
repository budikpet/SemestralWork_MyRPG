#include "Levels.h"

Levels::Levels() : currTurn(PLAYER), currState(INIT) {
	srand(time(NULL));
	// Get player from ConfigClass
	player = Info::getPlayer();
	
	gameScreen = new GameScreen(player);
}

Levels::~Levels() {
	delete gameScreen;
	vect_enemiesInLevel.clear();
	//TODO Mustn't delete player when going to new level
	
	for(unsigned int y = 0; y < vect_levelMap.size(); y++) {
		for(unsigned int x = 0; x < vect_levelMap[y].size(); x++) {
			MyObject* curr = vect_levelMap[y][x];
			
			// Remove everything but player
			if(curr->getGroup() != player->getGroup() || curr->getID() != player->getID()) {
				delete curr;
			}
		}
		vect_levelMap[y].clear();
	}
	vect_levelMap.clear();
	delete player;
}

void Levels::update() {	
	switch(currState) {
		case(INIT):
			loadLevel();
			currState = INGAME;
			break;
		case(INGAME):
			ingameUpdate();
			break;
	}
	
}

void Levels::paint() {
	gameScreen->paint(vect_levelMap);
}

void Levels::loadLevel() {
	// Used for adding other MyObjects (enemies, items), randomly
	std::vector<MyObject*> vect_floors;
	//TODO Read map from file instead of hardcoding it
	int h = 30;
	int w = 50;
	
	// Read map from file
	for(int y = 0; y < h; y++) {
		std::vector<MyObject*> row;
		for(int x = 0; x < w; x++) {
			MyObject* tmp;
			if(y == 0 || y == h -1 || x == 0 || x == w - 1) {
				tmp = Info::getMyObject('#');
			} else {
				// Floor ' '
				tmp = Info::getMyObject('.');
				vect_floors.push_back(tmp);
			}
			row.push_back(tmp);
			tmp->addToMap(vect_levelMap, y, x);
		}
		vect_levelMap.push_back(row);
	}
	
	// Add other MyObjects
	addRandomObjects(vect_floors);	
}

void Levels::addRandomObjects(std::vector<MyObject*>& vect_floors) {
	MyObject* curr;
	int ranPos = rand() % vect_floors.size();
	int ranNum = rand() % Info::maxEnemiesPerLevel + 1;
		
	// Add player to random position
	curr = getFloor(vect_floors, ranPos);
	player->addToMap(vect_levelMap, curr->getY(), curr->getX());
	
	// Add door to random position
	MyObject* door = Info::getMyObject(MyObject::STATIC, Info::ID_Door);
	ranPos = rand() % vect_floors.size();
	curr = getFloor(vect_floors, ranPos);
	door->addToMap(vect_levelMap, curr->getY(), curr->getX());
	
	// Add random number of random enemies	
	//std::cerr << "Num of Enemies added: " << ranNum << std::endl;
	for(int i = 0; i < ranNum || vect_floors.size() == 0; i++) {
		// Get random enemy
		Enemy* enemy = dynamic_cast<Enemy*> (Info::getMyObject(MyObject::ENTITY));
		ranPos = rand() % vect_floors.size();
		curr = getFloor(vect_floors, ranPos);
		
		vect_enemiesInLevel.push_back(enemy);
		enemy->addToMap(vect_levelMap, curr->getY(), curr->getX());
		//std::cerr << "Enemy: " << enemy->getID() << "/ " << enemy->getMapSymbol() << std::endl;
	}
	
}

MyObject* Levels::getFloor(std::vector<MyObject*> vect_floors, int index) {
	vect_floors.erase( vect_floors.begin() + index );
	return vect_floors[index];
}


void Levels::ingameUpdate() {
	switch(currTurn) {
		case(PLAYER):
			// If player moved, enemy has turn
			if( player->move(vect_levelMap, UserInput::getPressedKey()) ) {
				currTurn = ENEMY;
			
				// Enemies turn, we don't have to wait for input
				nodelay(stdscr, true);
			}
			
			break;
		case(ENEMY):
			currTurn = PLAYER;
			
			for(Enemy* curr : vect_enemiesInLevel) {
				curr->AI_update(vect_levelMap, player->getY(), player->getX());
			}
			
			// Players turn, we have to wait for input
			nodelay(stdscr, false);
			break;
	}
	
	gameScreen->update();
}
