#ifndef CSCI437_GOLD_H
#define CSCI437_GOLD_H

#include "entities/entity.h"
#include "events/event_gold_collection.h"
#include "events/event_collision.h"

#define GOLD_SIZE 10

class Gold : public Entity
{

public:
    Gold(float x, float y);
    static const EntityType entityType;
    const EntityType& getEntityType() const override {return entityType;}
    void move(Vector2D &dir, float delta) override {}

private:
    void handleGoldCollection(const EventGoldCollection &event);
    void handleCollision(const EventCollision &event);

};


#endif //CSCI437_GOLD_H
