#ifndef CSCI437_SKELETON_H
#define CSCI437_SKELETON_H

#include "entity.h"
#include "event.h"

#define SKELETON_SIZE  10
#define SKELETON_SPEED 50

class Skeleton : public Entity
{

public:
    Skeleton(float x, float y);
    void move(Vector2D &dir, float delta) override;
    void HandleEvent(Event *event) override;

private:

};


#endif //CSCI437_SKELETON_H
