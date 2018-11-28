#include "skeleton.h"
#include "skeleton.h"
#include "EventManager.h"

Skeleton::Skeleton(float x, float y) : Entity(x, y, SKELETON_SIZE)
{
  speed = SKELETON_SPEED;
  type = TYPE_SKELETON;
  obstructible = true;
}
