#include <entities/skeleton.h>
#include <entities/gold.h>
#include "graphics/graphics_game.h"
#include "views/player_view_game.h"
#include "macros.h"
#include "Animation.h"
#include "entities/Projectile.h"

#define IDX_BOUND_X   ((WINDOW_WIDTH / (2 * CELL_SIZE * ZOOM_SCALAR)) + 1)
#define IDX_BOUND_Y   ((WINDOW_HEIGHT / (2 * CELL_SIZE * ZOOM_SCALAR)) + 1)

GameGraphics::GameGraphics(GameView *view) : Graphics(), view(view) {
    storeLevel();
    this->view = view;

    // get all necessary resources from resource manager
    font = resources.GetFont("old_school");
    std::shared_ptr<rapidxml::xml_document<>> doc = resources.GetXMLDoc("text");
    buffer = resources.GetXMLBuffer("text");
    root_node = (*doc).first_node("Root")->first_node("UI");
    tileTexture = resources.GetTexture("tileset");
}

void GameGraphics::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// This must always be the first line of every draw method
	states.transform *= getTransform();

	drawLevel(target, states);

	float x;
	float y;
	float size;
	EntityType type;
	for (auto &ent : EntityManager::Instance()->getEntites())
	{
		x = ent->get_position().x;
		y = ent->get_position().y;
		size = ent->get_size();
		type = ent->getEntityType();

		sf::CircleShape circle(size);
		sf::RectangleShape rect;
		circle.setFillColor(sf::Color(0,0,0,0));
		sf::Sprite sprite;

		if (type == Skeleton::entityType) sprite = view->animation_skeleton.getSprite();
		if (type == Player::entityType) rect.setFillColor(sf::Color(0, 255, 0, 125));
		if (type == Player::entityType) sprite = view->animation_player.getSprite();
		if (type == Projectile::entityType)
            circle.setFillColor(sf::Color(255, 140, 0)), drawProjectileMotionBlur(target, states, circle, ent->trail);
    if (type == Gold::entityType) circle.setFillColor(sf::Color(255, 255, 0));
		rect.setSize( sf::Vector2f(size , size));
		rect.setOrigin(sf::Vector2f(size/2.0, size/2.0));
		circle.setOrigin(sf::Vector2f(size, size));
		sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width /2.0, sprite.getLocalBounds().height /2.0 + 15.0 ));
		sprite.setPosition(x,y);
		circle.setPosition(x, y);
		rect.setPosition(x,y);

		target.draw(sprite, states);
		if ( circle.getFillColor() != sf::Color(0,0,0,0))
			target.draw(circle, states);
		//target.draw(rect, states);

	}

  x = EntityManager::Instance()->getPlayer()->get_position().x;
  y = EntityManager::Instance()->getPlayer()->get_position().y;
	drawUI(target, states, x, y);
}


void GameGraphics::drawUI(sf::RenderTarget &target, sf::RenderStates states, float x, float y) const {

    sf::Sprite sprite;
    sprite.setTexture(resources.GetTexture("fog"));
    sprite.setPosition(sf::Vector2f(x - WINDOW_WIDTH / 2.f, y - WINDOW_HEIGHT / 2.f));
    //target.draw(sprite, states); // Uncomment me to see a nice effect

    //TODO set size of bar to match player'sactual health/mana
    // updates hpBar
    sf::RectangleShape hpBar, manaBar;
    hpBar.setFillColor(sf::Color(255, 0, 0, 190));
    hpBar.setSize(sf::Vector2f(2.5f * 100, 30));
    hpBar.setPosition(sf::Vector2f(x - WINDOW_WIDTH / 2.f + 10, y - WINDOW_HEIGHT / 2.f + 10));
    target.draw(hpBar, states);
    //updates mana bar
    manaBar.setFillColor(sf::Color(0, 255, 0, 190));
    manaBar.setSize(sf::Vector2f(2.5f * 100, 30));
    manaBar.setPosition(sf::Vector2f(x - WINDOW_WIDTH / 2.f + 10, y - WINDOW_HEIGHT / 2.f + 50));
    target.draw(manaBar, states);

    // update and draw text for health and mana, must be drawn after the corresponding bars
    sf::Text hpText, manaText;
    hpText = prepareText("health", font);
    //note that y position of text is relative to bar size but x position is fixed so that bar can change width without moving text
    hpText.setPosition(x - WINDOW_WIDTH / 2.f + 135, hpBar.getPosition().y + hpBar.getLocalBounds().height / 2.f);
    manaText = prepareText("mana", font);
    manaText.setPosition(x - WINDOW_WIDTH / 2.f + 135, manaBar.getPosition().y + manaBar.getLocalBounds().height / 2.f);
    target.draw(hpText, states);
    target.draw(manaText, states);
}

void GameGraphics::drawLevel(sf::RenderTarget &target, sf::RenderStates states) const {
    // This must always be the first line of every draw method
    states.transform *= getTransform();
    states.texture = &tileTexture;

    target.draw(vertices, states);

}


/*
	Stores all rectangle shapes for the level in a vector so that they can be quickly drawn later
*/
void GameGraphics::storeLevel() {
    tile_map.SetTexture();
    tile_map.PopulateVertexArray(view->get_state().get_level().get_map(), 2);
    vertices = tile_map.GetVertices();
}


void GameGraphics::drawProjectileMotionBlur(sf::RenderTarget &target, sf::RenderStates states, sf::CircleShape circle,
                                            std::vector<Vector2D> &trail) const {
    float size = circle.getRadius();

    //draws all balls in trail except 0th since it is used to store main ball's location
    for (int i = 1; i < 5; i++) {
        sf::CircleShape trail_shape(size);
        trail_shape.setOrigin(size, size);
        trail_shape.setFillColor(sf::Color(255, 0, 0, 175 - 30 * i));
        trail_shape.setPosition(trail[i].x, trail[i].y);
        target.draw(trail_shape, states);
    }

}