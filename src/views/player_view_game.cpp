#include "views/player_view_game.h"
#include "graphics/graphics_game.h"
#include "macros.h"
#include "engine.h"

GameView::GameView(GameLogic *state, sf::RenderWindow *App) : PlayerView(state, App) {
    graphics = std::make_shared<GameGraphics>(this);
    if (MUSIC && music.getCurrentTrack() != "vanquisher") {
        music.stopMusic();
        music.setMusic("vanquisher");
        music.setVolume(35.0);
        music.playMusic();
    }

    state->get_player_data().set_l_cooldown(45/60.f);
    state->get_player_data().set_l_mana_cost(5);
    state->get_level().get_player().set_player_data(&(state->get_player_data()));
}

GameView::~GameView() {

}

void GameView::process_input(float delta) {
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
//        state->reset();
//      //Engine::Instance().switch_mode(MODE_MENU);
//        //state->reset_level_factory();
//    }
    if (!state->is_paused())
    {
        // TODO check that game has started (not in menu)
        int x_dir = 0, y_dir = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) y_dir++;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) y_dir--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) x_dir--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) x_dir++;

        if (x_dir != 0 || y_dir != 0) {
            auto dir = VEC_NONE;
            if (x_dir == 1 && y_dir == 1) dir = VEC_NORTHEAST;
            else if (x_dir == 1 && y_dir == 0) dir = VEC_EAST;
            else if (x_dir == 1 && y_dir == -1) dir = VEC_SOUTHEAST;
            else if (x_dir == 0 && y_dir == 1) dir = VEC_NORTH;
            else if (x_dir == 0 && y_dir == -1) dir = VEC_SOUTH;
            else if (x_dir == -1 && y_dir == 1) dir = VEC_NORTHWEST;
            else if (x_dir == -1 && y_dir == 0) dir = VEC_WEST;
            else if (x_dir == -1 && y_dir == -1) dir = VEC_SOUTHWEST;
            state->get_level().get_player().move(dir, delta);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            sf::Vector2f mpos = (*app).mapPixelToCoords(sf::Mouse::getPosition(*app));
            Vector2D mouse_pos = Vector2D(mpos.x, mpos.y);
            Vector2D direction = mouse_pos - state->get_level().get_player().get_position();
            state->get_level().get_player().r_attack(direction, delta);
        } else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f mpos = (*app).mapPixelToCoords(sf::Mouse::getPosition(*app));
            Vector2D mouse_pos = Vector2D(mpos.x, mpos.y);
            Vector2D direction = mouse_pos - state->get_level().get_player().get_position();
            state->get_level().get_player().l_attack(direction, delta);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
            state->get_level().get_player().set_speed(122);
        } else
            state->get_level().get_player().set_speed(85);

    }
}

void GameView::handle_event(sf::Event event) {

    if (event.type == sf::Event::Closed) Engine::Instance().shutdown();
    else if (event.key.code == sf::Keyboard::M && event.type == sf::Event::KeyReleased) map_mode = !map_mode;
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Tab) Engine::Instance().switch_mode(MODE_INVENTORY);
        //TODO DELETE BELOW, ONLY INTENDED FOR DEMO (AND FUN)
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::L)
    {
        state->get_player_data().set_l_cooldown(1/60.f);
        state->get_player_data().set_l_mana_cost(0.05);
        state->get_level().get_player().set_player_data(&(state->get_player_data()));
    }
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G) {
        state->get_player_data().add_gold(1000);
        state->get_level().get_player().set_player_data(&(state->get_player_data()));
    }
    else if (event.key.code == sf::Keyboard::P && event.type == sf::Event::KeyReleased) state->toggle_pause();
    else if (event.key.code == sf::Keyboard::V && event.type == sf::Event::KeyReleased) Engine::Instance().switch_mode(MODE_SHOP);

}

void GameView::update(float delta) {
    sf::Event event;
    while (app->pollEvent(event)) {
        handle_event(event);
        common_handle_event_tasks(event);
    }

    // Process input
    process_input(delta);
}

void GameView::draw(float delta) {
    // initializes the camera
    graphics->update(delta);
    app->draw(*graphics);
    app->display();
}
