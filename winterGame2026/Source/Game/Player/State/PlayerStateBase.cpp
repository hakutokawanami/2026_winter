#include "PlayerStateBase.h"
#include"Engine/Input/Button.h"
#include"Engine/Input/InputBuffer.h"
#include"Engine/Input/IInputSource.h"
#include"Engine/Input/CommandInput.h"
#include"PlayerAttackState.h"
#include"Game/Battle/AttackData.h"

namespace
{
	constexpr int kCommandBufferFrame = 10;
}

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

bool PlayerStateBase::IsCommandInput(std::string_view command, int maxFrame) const
{
	if (!m_player.CanControl())
	{
		return false;
	}

	return m_player.GetCommandInput().IsInput(command, maxFrame);
}

bool PlayerStateBase::TryCommandMove()
{
	// 技を出すかの処理
	auto tryMove = [&](std::string_view command, int maxFrame, Button button, const char* attack)
		{
			// コマンドとボタンの判定
			if (!IsCommandInput(command, maxFrame) || !IsBufferd(button, kCommandBufferFrame))
			{
				return false;
			}
			// 履歴を消す
			m_player.GetCommandInput().Clear();
			// 技を出す
			m_player.ChangeState(std::make_unique<PlayerAttackState>(m_player, AttackTable::GetInstance().Get(attack)));
			return true;
		};

	// 優先順位を決めて試していく
	if (tryMove("236", 15, Button::Y, "RushPunch")) return true;

	return false;
}