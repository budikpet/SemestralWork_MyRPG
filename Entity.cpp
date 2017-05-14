#include "Entity.h"
#include "Info.h"

Entity::Entity	(int ID, char mapSymbol, int health, int speed, 
				int attackDmg, int attackSpeed, int range)
				: MyObject(ID, mapSymbol, MyObject::ENTITY, false), health(health), 
				speed(speed), attackDmg(attackDmg), attackSpeed(attackSpeed), range(range)
{
}

Entity::Entity	(const Entity* temp)
				: MyObject(temp->ID, temp->mapSymbol, temp->group, temp->isPassable), 
				health(temp->health), speed(temp->speed), attackDmg(temp->attackDmg), 
				attackSpeed(temp->attackSpeed), range(temp->range)
{
}

bool Entity::move(std::vector< std::vector<MyObject*> >& vect_levelMap, int newY, int newX)
{
	int oldX = x;
	int oldY = y;
	
	MyObject* destination = vect_levelMap[newY][newX];
	// Check if entity moved
	if(y == newY && x == newX) {
		return true;
	}
	
	// Check for collision
	if(!destination->getPassable()) {
		// MyObject on new coordinates not passable. No move.
		return false;
	}
	
	// Update game map
	MyObject* currStandsOn = Info::getMyObject(entityStandsOn.first, entityStandsOn.second);
	currStandsOn->setX(oldX);
	currStandsOn->setY(oldY);
	
	std::cerr << "Dest/ CurrSt: (" << destination->getMapSymbol()
			<< ") / (" << currStandsOn->getMapSymbol() << ") " << std::endl;
	
	//this->addToMap(vect_levelMap, newY, newX);
	std::cerr << "PreCurr: " << vect_levelMap[oldY][oldX]->getMapSymbol() << std::endl;
	
	//currStandsOn->addToMap(vect_levelMap, oldY, oldX);
	vect_levelMap[oldY][oldX] = currStandsOn;
	
	std::cerr << "PostCurr: " << vect_levelMap[oldY][oldX]->getMapSymbol() << "\n\n";
	
	std::cerr << "PreThis: " << vect_levelMap[newY][newX]->getMapSymbol() << std::endl;
	this->MyObject::addToMap(vect_levelMap, newY, newX);
	std::cerr << "PostThis: " << vect_levelMap[newY][newX]->getMapSymbol() << std::endl;
	
	
	// Update entity coordinates
	x = newX;
	y = newY;
		
	return true;
}

bool Entity::move(std::vector< std::vector<MyObject*> >& vect_levelMap, int pressedKey) {
	return true;
}
	
void Entity::attack(Entity* target)
{
	
}

void Entity::isAttacked(int damage)
{
	
}

void Entity::addToMap(std::vector<std::vector<MyObject*> >& vect_levelMap, int y, int x) {
	MyObject* curr = vect_levelMap[y][x];
	this->entityStandsOn = std::make_pair(curr->getGroup(), curr->getID());
	this->MyObject::addToMap(vect_levelMap, y, x);
	delete curr;
}