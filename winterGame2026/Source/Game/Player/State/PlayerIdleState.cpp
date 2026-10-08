#include "PlayerIdleState.h"
#include"Game/Battle/BattleManager.h"
#include"PlayerStepState.h"
void PlayerIdleState::Enter()
{
	m_animationController.Play(Player::kIdleAnimName);
}

void PlayerIdleState::Update()
{
	auto& input = InputManager::GetInstance();
	Vector2 leftStickInput = input.GetLeftStick(m_player.GetPlayerID());

	if (input.IsStickDoubleTapInput(m_player.GetPlayerID(), StickDirection::left))
	{
		if (m_battleManager->IsOpponentOnRight(m_player.GetPlayerID()))
		{
			m_player.ChangeState(std::make_unique<PlayerStepState>(m_player, PlayerStepState::Type::back));
		}
		else
		{
			m_player.ChangeState(std::make_unique<PlayerStepState>(m_player, PlayerStepState::Type::forward));
		}
	}
}

void PlayerIdleState::Exit()
{
}
