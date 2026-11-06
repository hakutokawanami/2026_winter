#include "PlayerGrabState.h"
#include"PlayerIdleState.h"
#include"Game/Battle/HitBoxSet.h"

namespace
{
	//アニメーションのブレンドにかけるフレーム数
	constexpr float kGrabBlendTime = 10.0f;
}

PlayerGrabState::PlayerGrabState(Player& player, const AttackDef& attack):
	PlayerStateBase(player),
	m_attack(attack),
	m_frame(0)
{
}

void PlayerGrabState::Enter()
{
	//アニメション再生
	if (!m_attack.animName.empty())
	{
		m_animationController.Play(m_attack.animName, false, m_attack.blendTime, m_attack.animSpeed);
	}
}

void PlayerGrabState::Update()
{
	m_frame++;
	
	const AttackData& data = m_attack.data;
	auto& hitBoxes = m_player.GetHitBoxes();
	//発生フレームになったら攻撃判定を出す
	if (m_frame == data.startUp)
	{
		hitBoxes.StartThrowAttack(m_attack.GetBox(), data, m_battleManager->IssueAttackInstanceID());
	}

	//持続が終わったら攻撃判定を消す
	if (m_frame == data.startUp + data.active)
	{
		hitBoxes.EndThrowAttack();
	}

	//硬直が終わったら Idle に戻る
	if (m_frame >= m_attack.GetTotalFrame())
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
		return;
	}
}

void PlayerGrabState::Exit()
{
}
