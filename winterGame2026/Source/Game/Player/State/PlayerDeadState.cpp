#include "PlayerDeadState.h"

void PlayerDeadState::Enter()
{
	m_animationController.Play(Player::kDeadAnimName, false);
}

void PlayerDeadState::Update()
{
	
}

void PlayerDeadState::Exit()
{
}
