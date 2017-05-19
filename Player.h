#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "UserInput.h"

class Player : public Entity {
public:
	Player(int ID, char mapSymbol, std::string name, Info::Attributes attr);
	Player(const Player* temp);
	
	virtual void paint(Screen* screen, const int y, const int x) override;
		
	/**
	 * Checks ground for items and for doors to next level;
	 */
	void checkGround() override;
	
	bool getDoorFound();
	
	void setName(std::string name);
	
	void prepareToNextLevel();
	
	void update(LevelMap& levelMap, MsgBox* msgBox);
	
	virtual MyObject* clone() const override;
	
private:
	Direction currDirection;
	bool doorFound;
};

#endif /* PLAYER_H */

