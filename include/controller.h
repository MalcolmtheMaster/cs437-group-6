#ifndef CSCI437_CONTROLLER_H
#define CSCI437_CONTROLLER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "game_logic.h"

/**
 * Abstract base class for all controllers.
 */
class Controller
{
public:
    explicit Controller(GameLogic *state) : state(state) {};
    virtual ~Controller() = default;
    virtual void process_input(float delta, sf::Vector2f mouse_pos) = 0;
    virtual void handle_event(sf::Event event) = 0;

protected:
    GameLogic *state;
};

#endif //CSCI437_CONTROLLER_H
