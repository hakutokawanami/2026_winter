#include "PlayerIdleState.h"
#include"PlayerStepState.h"
#include"PlayerWalkState.h"
#include"PlayerAttackState.h"
#include"Game/Battle/AttackData.h"
void PlayerIdleState::Enter()
{
	m_animationController.Play(Player::kIdleAnimName);
}

void PlayerIdleState::Update()
{
	//入力方向とステップ方向の表を作る
	static constexpr std::pair<StickDirection, PlayerStepState::Type>stepTable[] =
	{
		{StickDirection::right,PlayerStepState::Type::forward},
		{StickDirection::left,PlayerStepState::Type::back},
		{StickDirection::up,PlayerStepState::Type::sideIn},
		{StickDirection::down,PlayerStepState::Type::sideOut},
	};

	//左右移動用の入力方向の表を作る
	static constexpr StickDirection dirTable[] =
	{
		StickDirection::right,
		StickDirection::left,
	};

	auto& input = InputManager::GetInstance();

	//攻撃
	if (input.IsTriggered(m_player.GetPlayerID(), "Y"))
	{
		m_player.ChangeState(std::make_unique<PlayerAttackState>(m_player, AttackTable::GetInstance().Get("LungeJab")));
		return;
	}

	//攻撃
	if (input.IsTriggered(m_player.GetPlayerID(), "X"))
	{
		m_player.ChangeState(std::make_unique<PlayerAttackState>(m_player, AttackTable::GetInstance().Get("Jab")));
		return;
	}

	//どの入力か探す
	for (auto [dir, type] : stepTable)
	{
		//2回連続で倒されていたら
		if (input.IsStickDoubleTapInput(m_player.GetPlayerID(), ToScreenDirection(dir)))
		{
			//対応する方向にステップ
			m_player.ChangeState(std::make_unique<PlayerStepState>(m_player, type));
			return;
		}
	}

	for (auto dir : dirTable)
	{
		//倒されている方向をチェック
		if (input.GetStickDirection(m_player.GetPlayerID()) == ToScreenDirection(dir))
		{
			//移動
			m_player.ChangeState(std::make_unique<PlayerWalkState>(m_player));
			return;
		}
	}
}

void PlayerIdleState::Exit()
{
}
