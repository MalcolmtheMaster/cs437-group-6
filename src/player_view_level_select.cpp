#include <engine.h>
#include "graphics_level_select.h"
#include "player_view_level_select.h"

LevelSelectView::LevelSelectView(GameLogic *state, sf::RenderWindow *App) : PlayerView(state, App)
{
  selected_level = 0;
  graphics = std::make_shared<LevelSelectGraphics>(this);
  App->setView(App->getDefaultView());

	nodes[0] = {298, 403, 1, 10};
	nodes[1] = {47, 321, 2, 10};
	nodes[2] = {718, 442, 3, 10};
	nodes[3] = {526, 320, 4, 10};
	nodes[4] = {358, 223, 5, 10};
	nodes[5] = {317, 84, 6, 10};
	nodes[6] = {622, 103, 7, 10};
}

void LevelSelectView::process_input(float delta)
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) Engine::getInstance().set_mode(MODE_MENU);
}

void LevelSelectView::handle_event(sf::Event event)
{
	if (event.type == sf::Event::EventType::MouseButtonPressed)
  {
		selected_level = clicked_node(event.mouseButton.x, event.mouseButton.y);
	}
  else if (event.type == sf::Event::EventType::MouseButtonReleased)
  {
    if (selected_level == clicked_node(event.mouseButton.x, event.mouseButton.y))
    {
      printf("Level %d selected\n", selected_level);
      Engine::getInstance().set_mode(MODE_PLAY);
    }
  }
}

void LevelSelectView::update(float delta)
{
  //Process input
  sf::Event event;
  while (app->pollEvent(event))
  {
    handle_event(event);
  }
  process_input(delta);
}

void LevelSelectView::draw()
{
  app->clear(sf::Color::Black);
  app->draw(*graphics);
  app->display();
}

int LevelSelectView::clicked_node(int mouseX, int mouseY)
{
	int i;
	int dx;
	int dy;
	int hypo;
	for (i = 0; i < NUMBER_OF_LEVELS; ++i)
	{
		Node node = get_node(i);
		hypo = node.size;
		dx = mouseX - node.x;
		dy = mouseY - node.y;
		if (hypo * hypo > dx * dx + dy * dy) return i + 1;
	}
	return 0;
}
