#include "PlayerStepState.h"
#include"PlayerIdleState.h"
namespace
{
	struct StepParam
	{
		//全体フレーム
		int frame;
		//移動距離
		float distance;
	};

	constexpr StepParam kStepParams[] =
	{
		{16,250.0f},//forward
		{18,200.0f},//back
		{14,150.0f},//sideIn
		{14,150.0f},//sideOut
	};

	const StepParam& GetStepParam(PlayerStepState::Type type)
	{
		return kStepParams[static_cast<int>(type)];
	}
}

PlayerStepState::PlayerStepState(Player& player, Type type):
	PlayerStateBase(player),
	m_type(type),
	m_direction({}),
	m_frame(0)
{
}

void PlayerStepState::Enter()
{
	//アニメション再生
	switch (m_type)
	{
	case Type::forward:
		m_animationController.Play(Player::kStepForwardAnimName);
		break;
	case Type::back:
		m_animationController.Play(Player::kStepBackAnimName);
		break;
	case Type::sideIn:
		m_animationController.Play(Player::kStepSideInAnimName);
		break;
	case Type::sideOut:
		m_animationController.Play(Player::kStepSideOutAnimName);
		break;
	}
}

void PlayerStepState::Update()
{
	//毎フレーム向きを更新
	Vector3 toOpponent = m_battleManager->GetOpponentDirection(m_player.GetPlayerID());
	Vector3 side = m_battleManager->GetForwardDirection();

	switch (m_type)
	{
	case Type::forward:
		m_direction = toOpponent;
		break;
	case Type::back:
		m_direction = -toOpponent;
		break;
	case Type::sideIn:
		m_direction = side;
		break;
	case Type::sideOut:
		m_direction = -side;
		break;
	}

	//ステップのパラメータ取得
	const auto& param = GetStepParam(m_type);
	//移動量計算
	float speed = param.distance / param.frame;
	m_player.GetTransform().Translate(m_direction * speed);
	//ステップが終わったら遷移
	if (++m_frame >= param.frame)
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
	}
}

void PlayerStepState::Exit()
{
}
