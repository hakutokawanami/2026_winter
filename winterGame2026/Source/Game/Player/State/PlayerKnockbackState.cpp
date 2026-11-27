#include "PlayerKnockbackState.h"
#include"PlayerDownState.h"

void PlayerKnockbackState::Enter()
{
	m_animationController.Play(Player::kKnockbackAnimName,false,15.0f,1.0f);
	m_animationController.RootLocked();
}

void PlayerKnockbackState::Update()
{
	if (m_animationController.IsEnd())
	{
		m_player.ChangeState(std::make_unique<PlayerDownState>(m_player));
	}
}

void PlayerKnockbackState::Exit()
{
}
