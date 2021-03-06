#ifndef CSCI437_WEAPON_H
#define CSCI437_WEAPON_H

#include "events/event.h"
#include "vector2d.h"

class Weapon
{

    Weapon();
    ~Weapon();

private: 
    int maxRange;
    int maxDamage;

public:

    void set_maxDamage( int maxD);
        
    void set_maxRange( int maxR);

    void HandleEvents( Event* event);

    void attack(Vector2D &dir, float delta);

};


#endif //CSCI437_WEAPON_H