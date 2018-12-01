#ifndef CSCI437_ANIMATION_H
#define CSCI437_ANIMATION_H

#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "vector2d.h"


class Animation 
{
    public:
        sf::Vector2u imageCount;
        sf::Vector2u currentImage;

        float totalTime;
        float switchTime;
        sf::IntRect uvRect;
        sf::Sprite sprite;
        sf::Texture texture;

    public:
        Animation();
        ~Animation();
        void initSprite(sf::Texture &texture, sf::Vector2u imageCount, float switchTime);
        virtual void update(float delta) = 0;
        //void Update(Vector2D dir, int imageCount, float delta);
        sf::Sprite &getSprite(){ return sprite;}
};

//global access to resource manager
extern ResourceManager resources;

#endif //CSCI437_ANIMATION_H