#ifndef CSCI437_PLAYER_VIEW_H
#define CSCI437_PLAYER_VIEW_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "game_logic.h"
#include "view.h"

/**
 * Abstract base class for all the player views, to include functionality for SFML input, graphics, and audio.
 */
class Graphics;

class PlayerView : public View
{

public:
    PlayerView(GameLogic *state, sf::RenderWindow *App) : View(state), app(App) {}
    virtual void draw() = 0;

protected:
    virtual void process_input(float delta) = 0;
    virtual void handle_event(sf::Event event) = 0;
    sf::RenderWindow *app;
    std::shared_ptr<Graphics> graphics;

};

#endif //CSCI437_PLAYER_VIEW_H
