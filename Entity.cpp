#include "Entity.h"

int Entity::lastID = 0;


Entity::Entity	(char mapSymbol, int health, int speed, 
				int attackDmg, int attackSpeed)
				: MyObject(lastID, mapSymbol, MyObject::ENTITY), health(health), 
				speed(speed), attackDmg(attackDmg), attackSpeed(attackSpeed)
{
	lastID++;
}

void Entity::move(int newX, int newY)
{
	
}
	
void Entity::attack(Entity* target)
{
	
}

void Entity::isAttacked(int damage)
{
	
}