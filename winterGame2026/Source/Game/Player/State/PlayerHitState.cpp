#include "PlayerHitState.h"
#include"PlayerIdleState.h"

namespace
{
	//このフレーム数以上のけぞる技は大きいのけぞりにする
	constexpr int kHeavyHitStun = 40;
	//ノックバックするフレーム数
	constexpr int kKnockbackFrame = 8;
	//ノックバックの最初の速さ
	constexpr float kKnockbackSpeed = 6.0f;
	//アニメーションのブレンドにかけるフレーム数
	constexpr float kHitBlendTime = 10.0f;
}

PlayerHitState::PlayerHitState(Player& player, const AttackData& attack):
	PlayerStateBase(player),
	m_attack(attack),
	m_frame(0),
	m_knockbackDir({})
{
}

void PlayerHitState::Enter()
{
	//のけぞる時間からアニメーションを選ぶ
	const wchar_t* anim = (m_attack.hitStun >= kHeavyHitStun) ? Player::kHitHeadHeavyAnimName : Player::kHitHeadLightAnimName;
	//アニメーションを最初から再生
	m_animationController.Play(anim, false, kHitBlendTime, 0.5f, true);
	//のけぞり時間ぴったりでアニメが終わる速さにする
	if (m_attack.hitStun > 0)
	{
		m_animationController.SetSpeed(m_animationController.GetTotalTime() / m_attack.hitStun);
	}
	//相手と反対方向に吹き飛ぶ
	m_knockbackDir = -m_battleManager->GetOpponentDirection(m_player.GetPlayerID());
}

void PlayerHitState::Update()
{
	m_frame++;

	//最初の数フレームだけ、だんだん遅くしながら下がる
	if (m_frame <= kKnockbackFrame)
	{
		float rate = 1.0f - static_cast<float>(m_frame - 1) / kKnockbackFrame;
		m_player.GetTransform().Translate(m_knockbackDir * (kKnockbackSpeed * rate));
	}

	if(m_frame >= m_attack.hitStun)
	{
		m_knockbackDir = {};
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
		return;
	}
}

void PlayerHitState::Exit()
{
}
