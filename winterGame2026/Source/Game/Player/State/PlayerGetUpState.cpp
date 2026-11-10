#include "PlayerGetUpState.h"
#include"PlayerIdleState.h"

void PlayerGetUpState::Enter()
{
	m_animationController.Play(Player::kGettingUpAnimName, false, 0.0f, 1.0f);
}

void PlayerGetUpState::Update()
{
	if (m_animationController.IsEnd())
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
	}
}

void PlayerGetUpState::Exit()
{
}
