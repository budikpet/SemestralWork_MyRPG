#include "Levels.h"

Levels::Levels() {
	srand(time(NULL));
	// Get player from ConfigClass
	player = ConfigClass::getPlayer();
	fillMap();
	gameScreen = new GameScreen(player);
}

Levels::~Levels() {
	delete gameScreen;
	vect_enemiesInLevel.clear();
	//TODO Mustn't delete player when going to new level
	
	for(unsigned int y = 0; y < vect_gameMap.size(); y++) {
		for(unsigned int x = 0; x < vect_gameMap[y].size(); x++) {
			delete vect_gameMap[y][x];
		}
		vect_gameMap[y].clear();
	}
	vect_gameMap.clear();
}

void Levels::update() {
	player->move(vect_gameMap, UserInput::getPressedKey());
	
	gameScreen->update();
}

void Levels::paint() {
	gameScreen->paint(vect_gameMap);
}

void Levels::fillMap() {
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
				tmp = ConfigClass::getMyObject('#');
			} else {
				// Floor ' '
				tmp = ConfigClass::getMyObject(' ');
				vect_floors.push_back(tmp);
			}
			row.push_back(tmp);
			tmp->addToMap(y, x);
		}
		vect_gameMap.push_back(row);
	}
	
	// Add other MyObjects
	addRandomObjects(vect_floors);
}

void Levels::addRandomObjects(std::vector<MyObject*>& vect_floors) {
	int ranPos = rand() % vect_floors.size();
	int ranNum = rand() % ConfigClass::maxEnemiesPerLevel + 1;
		
	// Add player to random position
	this->addToMap(vect_floors, ranPos, player);
	
	// Add random number of random enemies	
	//std::cerr << "Num of Enemies added: " << ranNum << std::endl;
	for(int i = 0; i < ranNum || vect_floors.size() == 0; i++) {
		// Get random enemy
		Entity* enemy = dynamic_cast<Entity*> (ConfigClass::getMyObject(MyObject::ENTITY));
		ranPos = rand() % vect_floors.size();
		
		vect_enemiesInLevel.push_back(enemy);
		this->addToMap(vect_floors, ranPos, enemy);
		//std::cerr << "Enemy: " << enemy->getID() << "/ " << enemy->getMapSymbol() << std::endl;
	}
	
}

void Levels::addToMap(std::vector<MyObject*>& floors, int index, MyObject* newObject) {
	MyObject* curr = floors[index];
	
	// Add new object to old object's coordinates
	vect_gameMap[curr->getY()][curr->getX()] = newObject;
	newObject->addToMap(curr->getY(), curr->getX());
	
	// Remove old floor
	floors.erase(floors.begin() + index);
	delete curr;
}