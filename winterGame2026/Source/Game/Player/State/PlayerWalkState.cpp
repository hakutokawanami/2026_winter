#include "PlayerWalkState.h"
#include"PlayerIdleState.h"
#include"PlayerAttackState.h"
#include"PlayerGrabState.h"
#include"Game/Battle/AttackData.h"

namespace
{
	constexpr float kSpeed = 5.0f;
}

void PlayerWalkState::Enter()
{
}

void PlayerWalkState::Update()
{
	//入力方向と移動方向の表を作る
	static constexpr std::pair<StickDirection, PlayerWalkState::Type>walkTable[] =
	{
		{ StickDirection::right,PlayerWalkState::Type::forward },
		{ StickDirection::left,PlayerWalkState::Type::back },
		{ StickDirection::downRight,PlayerWalkState::Type::forward },
		{ StickDirection::downLeft,PlayerWalkState::Type::back }
	};

	const StickDirection stick = GetStickDirection();

	for (auto [dir,type] : walkTable)
	{
		//倒されている方向をチェック
		if (stick == ToScreenDirection(dir))
		{
			m_type = type;
			break;
		}
	}

	//毎フレーム向きを更新
	Vector3 toOpponent = m_battleManager->GetOpponentDirection(m_player.GetPlayerID());
	Vector3 side = m_battleManager->GetForwardDirection();

	switch (m_type)
	{
	case Type::forward:
		m_direction = toOpponent;
		m_animationController.Play(Player::kWalkForwardAnimName);
		break;
	case Type::back:
		m_direction = -toOpponent;
		m_animationController.Play(Player::kWalkBackAnimName);
		break;
	}

	//攻撃
	if (IsTriggered("Y"))
	{
		m_player.ChangeState(std::make_unique<PlayerAttackState>(m_player, AttackTable::GetInstance().Get("LungeJab")));
		return;
	}

	//攻撃
	if (IsTriggered("X"))
	{
		m_player.ChangeState(std::make_unique<PlayerAttackState>(m_player, AttackTable::GetInstance().Get("Jab")));
		return;
	}

	//攻撃
	if (IsTriggered("A"))
	{
		m_player.ChangeState(std::make_unique<PlayerAttackState>(m_player, AttackTable::GetInstance().Get("Kick")));
		return;
	}

	//掴み
	if (IsTriggered("B"))
	{
		m_player.ChangeState(std::make_unique<PlayerGrabState>(m_player, AttackTable::GetInstance().Get("Grab")));
		return;
	}

	//ニュートラルに入っていたらIdleへ
	if (stick == StickDirection::neutral ||
		stick == StickDirection::up ||
		stick == StickDirection::down  )
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
		return;
	}
	//移動
	m_player.GetTransform().Translate(m_direction * kSpeed);
}

void PlayerWalkState::Exit()
{
}
