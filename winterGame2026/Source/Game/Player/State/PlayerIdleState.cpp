#include "PlayerIdleState.h"
#include"Game/Battle/BattleManager.h"
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
		if (m_player.GetBattleManager()->IsOpponentOnRight(m_player.GetPlayerID()))
		{

		}
		else
		{

		}
	}
}

void PlayerIdleState::Exit()
{
}
