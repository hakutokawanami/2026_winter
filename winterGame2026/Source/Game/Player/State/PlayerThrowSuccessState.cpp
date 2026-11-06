#include "PlayerThrowSuccessState.h"
#include"PlayerIdleState.h"
namespace
{
	//アニメーションのブレンドにかけるフレーム数
	constexpr float kThrowSuccessBlendTime = 10.0f;
}

PlayerThrowSuccessState::PlayerThrowSuccessState(Player& player, const AttackData& attack):
	PlayerStateBase(player),
	m_attack(attack),
	m_frame(0)
{
}

void PlayerThrowSuccessState::Enter()
{
	m_animationController.Play(Player::kThrowAnimName, false, kThrowSuccessBlendTime);
}

void PlayerThrowSuccessState::Update()
{
	m_frame++;

	if (m_animationController.IsEnd())
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
	}
}

void PlayerThrowSuccessState::Exit()
{
}
