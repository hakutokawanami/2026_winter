#include "PlayerWalkState.h"
#include"PlayerIdleState.h"
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
		{StickDirection::right,PlayerWalkState::Type::forward},
		{StickDirection::left,PlayerWalkState::Type::back}
	};

	auto& input = InputManager::GetInstance();

	for (auto [dir,type] : walkTable)
	{
		//倒されている方向をチェック
		if (input.GetStickDirection(m_player.GetPlayerID()) == ToScreenDirection(dir))
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

	//ニュートラルに入っていたらIdleへ
	if (input.GetStickDirection(m_player.GetPlayerID()) == StickDirection::neutral)
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
