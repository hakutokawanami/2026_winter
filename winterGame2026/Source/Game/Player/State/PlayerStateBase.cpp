#include "PlayerStateBase.h"

PlayerStateBase::PlayerStateBase(Player& player, AnimationController& anim):
	m_player(player),
	m_animationController(anim)
{
}
