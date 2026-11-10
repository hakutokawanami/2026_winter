#include "PlayerThrowHitState.h"
#include"PlayerIdleState.h"
#include"PlayerGetUpState.h"
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
	if (m_frame == AttackTable::GetInstance().Get("Grab").takeDamageFrame)
	{
		m_player.TakeDamage(m_attack);
	}

	if (m_animationController.IsEnd())
	{
		const int model = m_player.GetModelHandle();
		const int hipsFrame = MV1SearchFrame(model, L"mixamorig:Hips");
		const VECTOR before = MV1GetFramePosition(model, hipsFrame);

		m_animationController.Play(Player::kGettingUpAnimName, false, 0.0f, 1.0f);
		m_animationController.Update();
		const VECTOR after = MV1GetFramePosition(model, hipsFrame);

		Vector3 pos = m_player.GetTransform().GetPosition();
		pos.x += before.x - after.x;
		pos.z += before.z - after.z;
		m_player.GetTransform().SetPosition(pos);

		m_player.ChangeState(std::make_unique<PlayerGetUpState>(m_player));
	}
}

void PlayerThrowHitState::Exit()
{
}
