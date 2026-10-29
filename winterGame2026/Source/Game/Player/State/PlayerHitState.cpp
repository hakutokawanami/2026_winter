#include "PlayerHitState.h"

PlayerHitState::PlayerHitState(Player& player, const AttackData& attack):
	PlayerStateBase(player),
	m_attack(attack),
	m_frame(0),
	m_knockbackDir({})
{
}

void PlayerHitState::Enter()
{
}

void PlayerHitState::Update()
{
}

void PlayerHitState::Exit()
{
}
