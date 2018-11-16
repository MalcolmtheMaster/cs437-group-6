#include "view_skeleton.h"

SkeletonView::SkeletonView(GameLogic *state, Entity &entity) : View(state)
{
  skeleton = (Skeleton*)&entity;
}

void SkeletonView::update(float delta)
{
  Vector2D dir = state->get_level().get_player()->get_position() - skeleton->get_position();
  float hypo = state->get_level().get_player()->get_size() + skeleton->get_size();
  if (hypo * hypo < dir.x * dir.x + dir.y * dir.y)
  {
    skeleton->move(dir, delta);
  }
}
