#ifndef CSCI437_PLAYER_H
#define CSCI437_PLAYER_H

#include <memory>
#include "event.h"
#include "entity.h"
#include "Projectile.h"
#include "listener.h"

#define PLAYER_SIZE  15

class Player: public Entity
{

private:
    int health = 20;
    int mana;
    //std::shared_ptr<PlayerData> PlayerData;

public:
		Player(float x, float y);
    ~Player();
    static const EntityType entityType;
    const EntityType& getEntityType() const override {return entityType;}
    void update(double delta) {};
    void attack(Vector2D &dir, float delta);

    //used to prevent more than one projectile firing each frame (assuming 60fps)
    //NOTE THIS CAN BE CHANGED TO SIMULATE DIFFERENT WEAPON SPEED
    float delta_threshold = (float)(10 / 60.0);
    float delta_sum;

};

#endif //CSCI437_PLAYER_H