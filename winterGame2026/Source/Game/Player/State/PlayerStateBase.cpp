#include "PlayerStateBase.h"
#include"Engine/Input/Button.h"
#include"Engine/Input/InputBuffer.h"
#include"Engine/Input/IInputSource.h"

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

bool PlayerStateBase::IsTriggered(Button button)const
{
	if (!m_player.CanControl())
	{
		return false;
	}

	return m_player.GetInput().IsTriggerd(button);
}

StickDirection PlayerStateBase::GetStickDirection()const
{
	if (!m_player.CanControl())
	{
		return StickDirection::neutral;
	}

	return m_player.GetInput().GetStickDirection();
}

bool PlayerStateBase::IsStickDoubleTap(StickDirection dir) const
{
	if (!m_player.CanControl())
	{
		return false;
	}

	return m_player.GetInput().IsStickDoubleTap(dir);
}

bool PlayerStateBase::IsBufferd(const Button button,int frame) const
{
	if (!m_player.CanControl())
	{
		return false;
	}

	return m_player.GetInputBuffer().IsBuffered(button, frame);
}
