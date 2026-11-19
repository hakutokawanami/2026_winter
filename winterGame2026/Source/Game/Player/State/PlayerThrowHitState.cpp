#include "PlayerThrowHitState.h"
#include"PlayerIdleState.h"
#include"PlayerGetUpState.h"
#include"PlayerDeadState.h"
#include"PlayerDownState.h"

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
	m_animationController.RootLocked();
}

void PlayerThrowHitState::Update()
{
	m_frame++;

	if (m_frame == AttackTable::GetInstance().Get("Grab").takeDamageFrame)
	{
		m_player.TakeDamage(m_attack);
	}

	if (m_player.GetHp() > 0)
	{
		if (m_animationController.IsEnd())
		{
			m_player.ChangeState(std::make_unique<PlayerDownState>(m_player));
		}
	}
}

void PlayerThrowHitState::Exit()
{
}
