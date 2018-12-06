#include "views/player_view_menu.h"
#include "graphics/graphics_menu.h"
#include "engine.h"
#include "macros.h"

MenuView::MenuView(GameLogic *state, sf::RenderWindow *App) : PlayerView(state, App) {
    screenIndex = 0;
    selectionIndex = 0;
    graphics = std::make_shared<MenuGraphics>(this);
    App->setView(App->getDefaultView());
    if (MUSIC) {
        music.stopMusic();
        music.setMusic("takecourage_noper");
        music.setVolume(35.0);
        music.playMusic();
    }
}
void MenuView::process_input(float delta)
{

}


void MenuView::handle_event(sf::Event event)
{
    //gets mode object
    //ensures menu mode selection index is within proper range
    sf::Vector2f mouse_pos = (*app).mapPixelToCoords(sf::Mouse::getPosition(*app));

    if (screenIndex == 0 && (WINDOW_WIDTH / 6.f) < mouse_pos.x && mouse_pos.x < (5 * WINDOW_WIDTH / 6.f)) {
        if (275 < mouse_pos.y && mouse_pos.y <= 325)
            selectionIndex = 0;
        else if (325 < mouse_pos.y && mouse_pos.y <= 375)
            selectionIndex = 1;
        else if (375 < mouse_pos.y && mouse_pos.y <= 425)
            selectionIndex = 2;
    }
    //ensures menu mode selection index is within proper range
    /*since this function is called inside the poll event loop, there is an OS delay which produces the
    desired cursor movement on the menu*/
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        selectionIndex = (selectionIndex + NUM_MENU_BUTTONS - 1) % NUM_MENU_BUTTONS;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        selectionIndex = (selectionIndex + 1) % NUM_MENU_BUTTONS;


    //KeyReleased so that the next screen doesn't immediately think confirm has been hit
    if ((event.key.code == sf::Keyboard::E && event.type == sf::Event::KeyReleased) ||
        (event.mouseButton.button == sf::Mouse::Left && event.type == sf::Event::MouseButtonReleased)) {
        //changes the game mode when player selects play
        if (makeSelection())
            Engine::Instance().switch_mode(MODE_STORY);
    }


    if ((event.key.code == sf::Keyboard::Q && event.type == sf::Event::KeyReleased) ||
        (event.mouseButton.button == sf::Mouse::Right && event.type == sf::Event::MouseButtonReleased))
        goBack();

}

void MenuView::update(float delta) {
    // Process input
    sf::Event event;
    while (app->pollEvent(event)) {
        handle_event(event);
        common_handle_event_tasks(event);
    }
    process_input(delta);
}

bool MenuView::makeSelection() {
    if (screenIndex == 0) //title screen
    {
        if (selectionIndex == 0) //play game has been selected
            return true;
        else if (selectionIndex == 1) //controls have been selected
            screenIndex = 1;
        else                            //about game has been selected
            screenIndex = 2;
    }

    return false;
}

/*
	called by Menu Controller, allows player to go back to an earlier menu screen
*/
void MenuView::goBack() {
    if (screenIndex != 0)
        screenIndex = 0;

}

void MenuView::draw(float delta) {
    app->clear(sf::Color::Black);
    app->draw(*graphics);
    app->display();
}
