#include "PlayerGetUpState.h"
#include"PlayerIdleState.h"
#include"PlayerAttackState.h"
#include"Game/Battle/AttackData.h"

PlayerGetUpState::PlayerGetUpState(Player& player, GetUp getUp):
	PlayerStateBase(player),
	m_getUp(getUp)
{
}

void PlayerGetUpState::Enter()
{
	switch (m_getUp)
	{
	case GetUp::normal:
		m_animationController.Play(Player::kGettingUpAnimName, false, 0.0f, 1.0f);
		break;

	case GetUp::attack:
		m_player.ChangeState(std::make_unique<PlayerAttackState>(m_player, AttackTable::GetInstance().Get("GetUpAttack")));
		return;
		break;
	case GetUp::rollBack:
		m_animationController.Play(Player::kGettingUpRollBackAnimName, false, 0.0f, 1.0f);
		break;
	}

	m_animationController.RootLocked();
}

void PlayerGetUpState::Update()
{
	if (m_animationController.IsEnd())
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
	}
}

void PlayerGetUpState::Exit()
{
}
