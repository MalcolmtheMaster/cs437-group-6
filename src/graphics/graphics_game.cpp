#include <entities/skeleton.h>
#include <entities/gold.h>
#include "graphics/graphics_game.h"
#include "views/player_view_game.h"
#include "macros.h"
#include "EntityManager.h"
#include "Animations/Animation.h"
#include "entities/Projectile.h"

#define IDX_BOUND_X   ((WINDOW_WIDTH / (2 * CELL_SIZE * ZOOM_SCALAR)) + 1)
#define IDX_BOUND_Y   ((WINDOW_HEIGHT / (2 * CELL_SIZE * ZOOM_SCALAR)) + 1)

#define VISIBLE_RANGE_X     (WINDOW_WIDTH / (2 * ZOOM_SCALAR) + 100)
#define VISIBLE_RANGE_Y     (WINDOW_HEIGHT / (2 * ZOOM_SCALAR) + 100)

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

    sf::View camera;
    camera.reset(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    Vector2D playerPos = view->get_state().get_level().get_player().get_position();
    camera.zoom(1 / ZOOM_SCALAR);
    camera.setCenter(playerPos.x, playerPos.y);
    target.setView(camera);
    target.clear(sf::Color::Black);
    drawLevel(target, states);

    float x;
    float y;
    float size;
    EntityType type;

    //TODO This entire loop can probably ultimately be removed entirely
    for (auto &i : EntityManager::Instance()->getEntites())
    {
        auto ent = i.second;
        x = ent->get_position().x;
        y = ent->get_position().y;
        size = ent->get_size();
        type = ent->getEntityType();

        sf::CircleShape circle(size);
        circle.setFillColor(sf::Color(0, 0, 0, 0));
        if (type == Projectile::entityType)
            circle.setFillColor(sf::Color(255, 140, 0)), drawProjectileMotionBlur(target, states, circle, ent->trail);
        if (type == Gold::entityType)
            circle.setFillColor(sf::Color(255, 255, 0, 125));
        circle.setOrigin(sf::Vector2f(size, size));
        circle.setPosition(x, y);
        target.draw(circle, states);
    }

    drawSprites(target, states);
    drawUI(target, states);
}

void GameGraphics::drawSprites(sf::RenderTarget &target, sf::RenderStates states) const
{
    float x, y;
    Vector2D playerPos = view->get_state().get_level().get_player().get_position();
    std::priority_queue<sf::Sprite*, std::vector<sf::Sprite*>, ComparatorY> spriteQueue;

    // Add the sprites within visible range in Y-order
    for (auto &i : spriteManager.getAnimations())
    {
        x = i.second->getSprite().getPosition().x;
        y = i.second->getSprite().getPosition().y;
        if (x >= playerPos.x - VISIBLE_RANGE_X && x <= playerPos.x + VISIBLE_RANGE_X &&
            y >= playerPos.y - VISIBLE_RANGE_Y && y <= playerPos.y + VISIBLE_RANGE_Y)
        {
            spriteQueue.push(&i.second->getSprite());
        }
    }

    // Draw the all the sprites
    while (!spriteQueue.empty())
    {
        auto sprite = spriteQueue.top();
        target.draw(*sprite, states);
        spriteQueue.pop();
    }
}


void GameGraphics::drawUI(sf::RenderTarget &target, sf::RenderStates states) const {

    sf::View camera;
    camera.reset(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    Vector2D playerPos = view->get_state().get_level().get_player().get_position();
    float x = playerPos.x;
    float y = playerPos.y;
    camera.setCenter(playerPos.x, playerPos.y);
    target.setView(camera);

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

    if (view->getMapMode()) drawMap(target, states);
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

void GameGraphics::drawMap(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    //TODO this can be heavily optimized, theoretically the tiles one must draw should only need to be computed once
    //TODO unless we want to have a "visited" or "seen" kind of cell attribute?
    //TODO also, there's got to be a better way to do the camera for everything
    sf::View camera;
    camera.reset(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    Vector2D playerPos = view->get_state().get_level().get_player().get_position();
    camera.zoom(3);
    camera.setCenter(playerPos.x, playerPos.y);
    target.setView(camera);
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    rect.setFillColor(sf::Color(128, 128, 128, 96));
    auto map = view->get_state().get_level().get_map();
    for (int m = 1; m < map.get_height() - 1; ++m)
    {
        for (int n = 1; n < map.get_width() - 1; ++n)
        {
            auto type = map.get_cell(m, n).get_cell_type();
            if (type == FLOOR || type == ORNAMENT || type == EXIT)
            {
                rect.setPosition(n * CELL_SIZE, m * CELL_SIZE);
                target.draw(rect, states);
            }
        }
    }
}

void GameGraphics::update(float delta)
{
    spriteManager.updateAnimations(delta);
}
