#include "Entity.h"

Entity::Entity	(int ID, char mapSymbol, std::string name, Info::Attributes attr)
				:	MyObject(ID, mapSymbol, MyObject::ENTITY, false, name), 
					attributes(attr), actionsLeft(attr.speed), hpRemaining(attr.maxHP),
					ground(NULL)
{
}

Entity::Entity	(const Entity* temp)
				: MyObject(temp->ID, temp->mapSymbol, temp->group, temp->isPassable, temp->name), 
				attributes(temp->attributes), actionsLeft(temp->actionsLeft), 
				hpRemaining(temp->attributes.maxHP), ground(temp->ground)
{
}

Entity::~Entity() {
	if(ground != NULL) {
		delete ground;
	}
}

bool Entity::move(LevelMap& levelMap, int newY, int newX)
{
	// Check if entity moved
	if(y == newY && x == newX) {
		return true;
	}
	
	// Check for collision
	if(!levelMap[newY][newX]->getPassable()) {
		// MyObject on new coordinates not passable. No move.
		return false;
	}
	
	// Update game map
		// Old position of this Entity is swapped for the MyObject that Entity stood on
		ground->addToMap(levelMap, y, x, false);
		transferItems(ground , this, attributes.invSpace);
		
		// Entity is moved to new position on the map
		this->addToMap(levelMap, newY, newX, false);
		
	
	checkGround();
	
	if(!inventory.empty()) {
		std::cerr << this->name << " [" << y << "/ " << x << "]" << std::endl;
		for(Item* curr : inventory) {
			std::cerr << curr->getName() << ",";
		}
		std::cerr << std::endl;
	}
		
	return true;
}

bool Entity::alive() const {
	if(attributes.maxHP <= 0) {
		return false;
	}
	
	return true;
}

void Entity::addToMap(LevelMap& levelMap, int y, int x, bool removeFormer) {
	this->ground = levelMap[y][x];
	this->MyObject::addToMap(levelMap, y, x, removeFormer);
}

void Entity::checkGround() {
	transferItems(this, ground);
}

bool Entity::findTarget(LevelMap& levelMap, Direction direction, Entity*& target, 
		const int yPos, const int xPos) {
	int addY = 0;
	int addX = 0;
	int currRange = attributes.range;
	
	switch(direction) {
		case UP:
			addY = -1;
			break;
		case DOWN:
			addY = 1;
			break;
		case LEFT:
			addX = -1;
			break;
		case RIGHT:
			addX = 1;
			break;
	}
	
	int currX = xPos + addX;
	int currY = yPos + addY;
	
	do {
		
		// Check bounds
		if(currY < 0 || currY >= (signed) levelMap.size() || currX < 0 
				|| currX >= (signed) levelMap[currY].size()) {
			return false;
		}
		
		MyObject* currObject = levelMap[currY][currX];
		
		// If not passable static object found
		if(currObject->getGroup() == STATIC && !currObject->getPassable()) {
			return false;
		}
		
		// Entity found, potential target
		if(currObject->getGroup() == ENTITY) {
			target = dynamic_cast<Entity*> (currObject);
			return true;
		}
		
		currX += addX;
		currY += addY;
		currRange--;	
	} while(currRange > 0);
	
	return false;
}

void Entity::isAttacked(const Entity* attacker, MsgBox* msgBox) {
	Info::Attributes attr = attacker->attributes;
	attributes.maxHP -= attr.attackDmg;
	
	std::stringstream ss;
	ss << attacker->name << " hit " << name << " for " << attr.attackDmg << " HP.";
	msgBox->addMsg(ss.str().c_str());
}

void Entity::attack(Entity* target, MsgBox* msgBox) {
	target->isAttacked(this, msgBox);
	actionsLeft--;
}

bool Entity::hasActionsLeft() {
	if(actionsLeft <= 0) {
		actionsLeft = attributes.speed;
		return false;
	}
	
	return true;
}

std::string Entity::getInfo() const {
	std::stringstream ss;
	ss << name << ": " << attributes;
	return ss.str();
}

void Entity::transferItems(MyObject* destination, MyObject* source, const int invSize) {
	InvList& destInv = destination->getInventory();
	InvList& srcInv = source->getInventory();
	
	if(invSize == -1) {
		// Transfer all Items
		destInv.merge(srcInv);
	} else {
		// Transfer exceeding items
		for(int i = 0; i < ( (signed) srcInv.size() - invSize); i++) {
			Item* curr = srcInv.back();
			srcInv.pop_back();
			destInv.push_back(curr);
		}
	}
}
