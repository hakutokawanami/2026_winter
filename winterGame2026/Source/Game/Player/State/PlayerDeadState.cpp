#include "PlayerDeadState.h"

void PlayerDeadState::Enter()
{
	m_animationController.Play(Player::kDeadAnimName, false,15.0f,0.7f);
}

void PlayerDeadState::Update()
{
	
}

void PlayerDeadState::Exit()
{
}
