#include "EntityManager.h"
#include "entities/Projectile.h"
#include "entities/skeleton.h"
#include "entities/blood.h"

Skeleton::Skeleton(float x, float y) : Enemy(x, y, SKELETON_SIZE) {
    obstructible = true;
    hostile = true;
    defence = 1;

    //uses normal skeleton by default
    setType("skeleton-white");

    EventManager::Instance().registerListener(EventCollision::eventType, this, & Skeleton::handleCollision);
}

void Skeleton::handleCollision(const EventCollision &event)
{
    if (event.getSelf().id == id && event.getOther().getEntityType() == Player::entityType)
    {
        event.getOther().take_damage(damage);
    }
}

void Skeleton::updateAttributes() {
    rapidxml::xml_node<> *main_node = root_node->first_node(type.c_str());
    health = std::stoi(main_node->first_node("health")->value());
    speed = std::stoi(main_node->first_node("speed")->value());
    damage = std::stoi(main_node->first_node("contact-damage")->value());
    aggro_dist = std::stoi(main_node->first_node("aggro-dist")->value());
}

void Skeleton::setType(std::string param_type) {
    type.assign(param_type);
    updateAttributes();
}

void Skeleton::die()
{
    EntityManager::Instance().removeEntity(id);
    EntityManager::Instance().createEntity<Blood>(pos.x, pos.y);
}
