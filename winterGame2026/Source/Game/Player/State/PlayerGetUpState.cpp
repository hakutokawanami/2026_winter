#include "PlayerGetUpState.h"
#include"PlayerIdleState.h"

void PlayerGetUpState::Enter()
{
	m_animationController.Play(Player::kGettingUpAnimName);
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
