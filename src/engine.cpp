#include <SFML/Window/Event.hpp>
#include <iostream>
#include <chrono>
#include <view_menu.h>
#include "graphics_example.h"
#include "graphics_game.h"
#include "view_game.h"
#include "engine.h"
#include "macros.h"
#include "ResourceManager.h"

Engine::Engine()
{

};

Engine *Engine::GameEngine()
{
  static Engine instance;
  return &instance;
}

Engine::~Engine()
{
  delete state;
}

void Engine::init(sf::RenderWindow *app)
{
  App = app;
  curr_game_mode = MODE_MENU;

  // Load fonts and audio
  // Initialize game state, graphics, sound, and controllers here
  state = new GameLogic();

  //loads necessary resources to the resource manager
  resources.LoadFont("old_school", "../data/Old-School-Adventures.ttf");
  resources.LoadXML("text", "../data/game-text.xml");

  curr_player_view = std::make_shared<MenuView>(state, App);

  views.push_back(curr_player_view);

  //starts clock
  time.restart();
}

/**
 * Update all the game views.
 */
void Engine::update_views(float delta)
{
  for (const auto &view : views)
  {
    view->update(delta);
  }
}

/**
 * Update the game state.
 */
void Engine::update_state(float delta)
{
  switch_mode();
  if (curr_game_mode == MODE_PLAY)
  {
    if (!state->is_paused()) state->update_state();
  }
  if (state->shutdown()) App->close();
}

void Engine::update_graphics()
{
  curr_player_view->draw();
}

/**
 * Update elapsed time to regulate main game loop.
 */
float Engine::clock()
{
	return time.restart().asSeconds() * GAME_CLOCK_SCALER;
}

/*
	Maintains the vectors storing Views and Controllers, adds/removes mode specific views/controllers
	when the game mode is changed.
*/
void Engine::set_mode(GameMode mode)
{
  curr_game_mode = mode;
}

void Engine::switch_mode()
{
  static GameMode old_mode = curr_game_mode;

  if (curr_game_mode != old_mode)
  {
    old_mode = curr_game_mode;

    //places the primary controller and view for the mode at the 0th index of each vector
    switch (curr_game_mode)
    {
      case MODE_MENU:
        curr_player_view = std::make_shared<MenuView>(state, App);
        break;
      case MODE_LEVEL_SELECT:
        break;
      case MODE_SHOP:
        break;
      case MODE_PLAY:
        curr_player_view = std::make_shared<GameView>(state, App);
        break;
    }
    views[0] = curr_player_view;
  }
}
