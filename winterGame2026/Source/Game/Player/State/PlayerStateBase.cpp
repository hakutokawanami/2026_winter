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

bool PlayerStateBase::IsBackInput() const
{
	const auto direction = GetStickDirection();
	return direction == ToScreenDirection(StickDirection::left) || 
		direction == ToScreenDirection(StickDirection::downLeft);
}

bool PlayerStateBase::IsGuardInput() const
{
	return IsBackInput();
}

bool PlayerStateBase::IsTriggered(const char* button)const
{
	if (!m_player.CanControl())
	{
		return false;
	}

	return InputManager::GetInstance().IsTriggered(m_player.GetPlayerID(), button);
}

StickDirection PlayerStateBase::GetStickDirection()const
{
	if (!m_player.CanControl())
	{
		return StickDirection::neutral;
	}

	return InputManager::GetInstance().GetStickDirection(m_player.GetPlayerID());
}

bool PlayerStateBase::IsStickDoubleTap(StickDirection dir) const
{
	if (!m_player.CanControl())
	{
		return false;
	}

	return InputManager::GetInstance().IsStickDoubleTapInput(m_player.GetPlayerID(), dir);
}
