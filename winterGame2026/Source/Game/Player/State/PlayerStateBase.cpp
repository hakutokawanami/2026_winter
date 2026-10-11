#include "PlayerStateBase.h"

PlayerStateBase::PlayerStateBase(Player& player):
	m_player(player),
	m_animationController(player.GetAnimationController()),
	m_battleManager(player.GetBattleManager())
{
}

StickDirection PlayerStateBase::ToScreenDirection(StickDirection direction) const
{
	return m_battleManager->IsOpponentOnRight(m_player.GetPlayerID()) ? direction : Mirror(direction);
}
