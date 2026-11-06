#include "PlayerGuardHitState.h"
#include"PlayerIdleState.h"

namespace
{
	//アニメーションのブレンドにかけるフレーム数
	constexpr float kGuardHitBlendTime = 10.0f;
}

PlayerGuardHitState::PlayerGuardHitState(Player& player, const AttackData& attack):
	PlayerStateBase(player),
	m_attack(attack),
	m_frame(0),
	m_knockbackDir({})
{
}

void PlayerGuardHitState::Enter()
{
	m_animationController.Play(Player::kGuardHitAnimName, false, kGuardHitBlendTime, 0.5f, true);
	//のけぞり時間ぴったりでアニメが終わる速さにする
	if (m_attack.guardStun > 0)
	{
		m_animationController.SetSpeed(m_animationController.GetTotalTime() / m_attack.guardStun);
	}
	//相手と反対方向に吹き飛ぶ
	m_knockbackDir = -m_battleManager->GetOpponentDirection(m_player.GetPlayerID());
}

void PlayerGuardHitState::Update()
{
	m_frame++;

	if(m_frame >= m_attack.guardStun)
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
	}
}

void PlayerGuardHitState::Exit()
{
}
