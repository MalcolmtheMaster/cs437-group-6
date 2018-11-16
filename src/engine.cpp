#include <SFML/Window/Event.hpp>
#include <iostream>
#include <chrono>
#include "view_skeleton.h"
#include "player_view_menu.h"
#include "player_view_game.h"
#include "player_view_level_select.h"
#include "engine.h"
#include "macros.h"
#include "view_manager.h"

Engine &Engine::getInstance()
{
  static Engine instance;
  return instance;
}

void Engine::init(sf::RenderWindow *app)
{
  //loads necessary resources to the resource manager
  resources.LoadFont("old_school", "../data/Old-School-Adventures.ttf");
  resources.LoadXML("text", "../data/game-text.xml");
  resources.LoadTexture("map", "../data/map.png");

  App = app;
  curr_game_mode = MODE_MENU;
  state = GameLogic();
  ViewManager::Instance()->init(&state);
  curr_player_view = std::make_shared<MenuView>(&state, App);
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
  for (auto &view : ViewManager::Instance()->get_views())
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
    if (!state.is_paused()) state.update_state();
  }
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
    views.clear();

    //places the primary controller and view for the mode at the 0th index of each vector
    switch (curr_game_mode)
    {
      case MODE_MENU:
        curr_player_view = std::make_shared<MenuView>(&state, App);
        views.push_back(curr_player_view);
        break;
      case MODE_LEVEL_SELECT:
        curr_player_view = std::make_shared<LevelSelectView>(&state, App);
	      views.push_back(curr_player_view);
	      break;
      case MODE_SHOP:
        break;
      case MODE_PLAY:
        state.create_new_level(AGENT_BASED);
        curr_player_view = std::make_shared<GameView>(&state, App);
        views.push_back(curr_player_view);
        generate_views();
        break;
    }
  }
}


void Engine::generate_views()
{
  /*
  for (const auto &entity : state.get_level().get_entities())
  {
    std::shared_ptr<View> view;
    if (entity->get_type() == TYPE_SKELETON)
    {
      view = std::make_shared<SkeletonView>(&state, *entity);
      views.push_back(view);
    }
  }*/
}
