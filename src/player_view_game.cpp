#include "player_view_game.h"
#include "graphics_game.h"
#include "macros.h"
#include "engine.h"
#include "Player.h"

GameView::GameView(GameLogic *state, sf::RenderWindow *App) : PlayerView(state, App)
{
  graphics = std::make_shared<GameGraphics>(this);
}

void GameView::process_input(float delta)
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		EventManager::Instance()->ClearEvents();
		for (const auto &e : state->get_level().get_entities())
		{
			EventManager::Instance()->UnregisterAll(e.get());
		}
		Engine::getInstance().set_mode(MODE_MENU);
	}

	// TODO check that game has started (not in menu)


	int x_dir = 0, y_dir = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))	y_dir++;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))	y_dir--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))	x_dir--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))	x_dir++;

  if (x_dir != 0 || y_dir != 0)
  {
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
}

void GameView::handle_event(sf::Event event)
{
  if (event.type == sf::Event::Closed) app->close();
}

void GameView::update(float delta)
{
  sf::Event event;
  while (app->pollEvent(event))
  {
    handle_event(event);
  }

  // Process input
  process_input(delta);
}

void GameView::draw()
{
  // initializes the camera
  // TODO send some of this to the constructor?
  sf::View camera;
  camera.reset(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
  Position playerPos = state->get_level().get_player().get_position();
  camera.setCenter(playerPos.x, playerPos.y);

  app->setView(camera);
  app->clear(sf::Color::Black);
  app->draw(*graphics);
  app->display();
}
