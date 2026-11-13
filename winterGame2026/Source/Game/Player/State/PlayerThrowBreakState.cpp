#include "PlayerThrowBreakState.h"
#include"PlayerIdleState.h"

namespace
{
	constexpr float kSpeed = 10.0f;
	//ノックバックするフレーム数
	constexpr int kKnockbackFrame = 20;
}

PlayerThrowBreakState::PlayerThrowBreakState(Player& player, bool attacker):
	PlayerStateBase(player),
	m_isAttacker(attacker),
	m_backDir(),
	m_frame(0)
{
}

void PlayerThrowBreakState::Enter()
{
	if (m_isAttacker)
	{
		m_animationController.Play(Player::kThrowBreakAttackerAnimName,false);
	}
	else
	{
		m_animationController.Play(Player::kThrowBreakDefenderAnimName, false);
	}

	m_backDir = -m_battleManager->GetOpponentDirection(m_player.GetPlayerID());
}

void PlayerThrowBreakState::Update()
{
	m_frame++;
	if (m_frame <= kKnockbackFrame)
	{
		m_player.GetTransform().Translate(m_backDir * kSpeed);
	}

	if (m_animationController.IsEnd())
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
	}
}

void PlayerThrowBreakState::Exit()
{
}
