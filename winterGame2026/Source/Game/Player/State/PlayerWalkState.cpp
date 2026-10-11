#include "PlayerWalkState.h"

PlayerWalkState::PlayerWalkState(Player& player, Type type):
	PlayerStateBase(player),
	m_type(type),
	m_direction({})
{
}

void PlayerWalkState::Enter()
{
}

void PlayerWalkState::Update()
{
}

void PlayerWalkState::Exit()
{
}
