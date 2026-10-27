#include "PlayerAttackState.h"
#include"PlayerIdleState.h"
#include"Game/Battle/AttackData.h"
#include"Game/Battle/HitBoxSet.h"

PlayerAttackState::PlayerAttackState(Player& player, const AttackDef& attack):
	PlayerStateBase(player),
	m_attack(attack),
	m_frame(0)
{
}

void PlayerAttackState::Enter()
{
	//アニメション再生
	if (!m_attack.animName.empty())
	{
		m_animationController.Play(m_attack.animName, false, m_attack.blendTime, m_attack.animSpeed);
	}
}

void PlayerAttackState::Update()
{
	++m_frame;

	const AttackData& data = m_attack.data;
	auto& hitBoxes = m_player.GetHitBoxes();
	//発生フレームになったら攻撃判定を出す
	if (m_frame == data.startUp)
	{
		hitBoxes.StartAttack(m_attack.GetBox(), data, m_battleManager->IssueAttackInstanceID());
	}

	//持続が終わったら攻撃判定を消す
	if (m_frame == data.startUp + data.active)
	{
		hitBoxes.EndAttack();
	}

	//硬直が終わったら Idle に戻る
	if (m_frame >= m_attack.GetTotalFrame())
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
		return;
	}
}

void PlayerAttackState::Exit()
{
	//攻撃判定が残らないようにする
	m_player.GetHitBoxes().EndAttack();
}
