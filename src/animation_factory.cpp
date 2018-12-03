#include <unordered_map>
#include <entities/Player.h>
#include "Animations/BloodAnimation.h"
#include "Animations/PlayerAnimation.h"
#include "Animations/SkeletonAnimation.h"
#include "Animations/GhostAnimation.h"
#include "animation_factory.h"
#include "entities/entity.h"

using namespace AnimationFactory;

/**
 * Function pointer to a createInstance<Animation> function
 */
typedef std::shared_ptr<Animation> (*Factory)(Entity &);

/**
 * Animation creation method
 *
 * @tparam E:     Entity type
 * @tparam V:     Corresponding animation type
 * @param entity: Reference to the entity of type E
 * @return        Pointer to the newly created animation
 */
template<class E, class A>
std::shared_ptr<Animation> createInstance(E &entity) {
    return std::make_shared<A>(entity);
}

/**
 * Animation creation method
 *
 * @param entity: Pointer to the entity
 * @return:       Pointer to the newly created animation, or null if no animation exists for entity type
 */
std::shared_ptr<Animation> AnimationFactory::createAnimation(Entity *entity) {
    /**
     * The EntityType > AnimationType mapping.
     * ADD AN ENTRY WITH AN ENTITY'S CORRESPONDING VIEW TO AUTOMATE VIEW CREATION
     */
    static const std::unordered_map<EntityType, Factory> map{
            {Blood::entityType,    (Factory) createInstance<Blood, BloodAnimation>},
            {Player::entityType,   (Factory) createInstance<Player, PlayerAnimation>},
            {Skeleton::entityType, (Factory) createInstance<Skeleton, SkeletonAnimation>},
            {Ghost::entityType,    (Factory) createInstance<Ghost, GhostAnimation>},
    };

    std::shared_ptr<Animation> animation = nullptr;
    auto i = map.find(entity->getEntityType());
    if (i != map.end()) {
        Factory f = i->second;
        animation = f(*entity);
    }
    return animation;
}
