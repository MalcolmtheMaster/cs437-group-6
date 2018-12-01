
#include "Animations/PlayerAnimation.h"

PlayerAnimation::PlayerAnimation(Player &entity) : Animation(), player(&entity) {
    texture = resources.GetTexture("playerTexture");
    //sprite.setTextureRect(uvRect);
    imageCount = sf::Vector2u(13, 21);
    switchTime = 3/60.f;
    totalTime = 0.0f;
    currentImage.x = 0;
    currentImage.y = 10;
    uvRect.width = texture.getSize().x / imageCount.x;
    uvRect.height = texture.getSize().y / imageCount.y;
    uvRect.left = currentImage.x * uvRect.width;
    uvRect.top = currentImage.y * uvRect.height;
    sprite.setTexture(texture);
    sprite.setTextureRect(uvRect);
    sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2.0, sprite.getLocalBounds().height / 2.0 + 15.0 ));
    sprite.setPosition(player->get_position().x, player->get_position().y);
}
PlayerAnimation::~PlayerAnimation(){}

void PlayerAnimation::update(float delta){
    //imageCount is the specific value for the number of animation
    //for a certain movement base on the sprite sheet
    unsigned int imageCount = 9;
    Vector2D dir = player->get_position() - player->get_old_position();

    if (dir.y < 0)
        currentImage.y = 8;
    else if (dir.y > 0)
        currentImage.y = 10;
    else if (dir.x < 0)
        currentImage.y = 9;
    else if (dir.x > 0)
        currentImage.y = 11;

    this->imageCount.x = imageCount;
    totalTime += delta;

    if ( totalTime >= switchTime){
        totalTime = 0.0f;
        currentImage.x++;
        if (currentImage.x >= this->imageCount.x){
            currentImage.x =0;
        }
        uvRect.left = currentImage.x * uvRect.width;
        uvRect.top = currentImage.y * uvRect.height;
    }

    sprite.setTextureRect(uvRect);
    sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2 + 15));
    sprite.setPosition(player->get_position().x, player->get_position().y);
}