#ifndef CSCI437_CELL_H
#define CSCI437_CELL_H

#include <vector>
#include <memory>
#include "entity.h"

class Cell
{

public:
    Cell(char ch) : data(ch) {}
    void insert_entity(std::shared_ptr<Entity> &entity);
    void clear_entities();
    std::vector<std::shared_ptr<Entity>> get_entities() {return entities;}
    char get_data() {return data;}
    bool is_occupied() {return !entities.empty();}

private:
    std::vector<std::shared_ptr<Entity>> entities;
    char data;

};

#endif //CSCI437_CELL_H
