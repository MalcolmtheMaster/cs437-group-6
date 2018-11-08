#ifndef CSCI437_PLAYER_H
#define CSCI437_PLAYER_H

#include "event.h"
#include "entity.h"
#include "listener.h"

class Player: public Entity, public Listener
{

private:
		int health;
		int mana;

public:
		Player(float x, float y, float size);

		~Player();
		void update(double deltaMs) {};

		void move(Direction dir, float delta);
		void animate() {};
		void HandleEvent(Event* event);

}; 

#endif //CSCI437_PLAYER_H