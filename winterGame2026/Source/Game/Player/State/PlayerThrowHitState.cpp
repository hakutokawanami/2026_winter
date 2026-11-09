#include "PlayerThrowHitState.h"
#include"PlayerIdleState.h"

namespace
{
	//アニメーションのブレンドにかけるフレーム数
	constexpr float kThrowBlendTime = 10.0f;
}

PlayerThrowHitState::PlayerThrowHitState(Player& player, const AttackData& attack):
	PlayerStateBase(player),
	m_attack(attack),
	m_frame(0)
{
}

void PlayerThrowHitState::Enter()
{
	m_animationController.Play(Player::kThrownAnimName, false, kThrowBlendTime);
}

void PlayerThrowHitState::Update()
{
	m_frame++;

	if (m_animationController.IsEnd())
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
	}
}

void PlayerThrowHitState::Exit()
{
}
