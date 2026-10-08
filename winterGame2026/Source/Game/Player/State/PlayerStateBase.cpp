#include "PlayerStateBase.h"

PlayerStateBase::PlayerStateBase(Player& player):
	m_player(player),
	m_animationController(player.GetAnimationController()),
	m_battleManager(player.GetBattleManager())
{
}
