#ifndef CSCI437_ENTITY_H
#define CSCI437_ENTITY_H

#define ENTITYID int

struct Position
{
    float x;
    float y;
};

struct Health
{
    int x; 
};

enum Direction
{
    NONE,
    NORTH,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST,
};

class Entity
{
    ENTITYID m_Id;


public:
    //Entity();
    Entity(float x, float y, float size);

    ~Entity();
    virtual const ENTITYID GetEntityID() const { return this->m_Id; }

    void set_position(float x, float y);
    void set_position(Position new_pos);
    Position get_position() {return pos;}
    Position get_old_position() {return pos_old;}
    float get_size() {return size;}
    void move(Direction dir, float delta); // TODO maybe encapsulate into a "Moveable" component?
    void correct_position(float delta);

    void takedamage(int damage);

protected:
    Position pos{};
    float size;
    int damage;
    Position pos_old{};       // TODO maybe encapsulate into a "Moveable" component?

};

#endif //CSCI437_ENTITY_H
