#include "Player.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Input/InputManager.h"
#include"Game/Battle/BattleManager.h"
#include"State/PlayerStateBase.h"
#include"State/PlayerIdleState.h"
#include<cassert>
#include<DxLib.h>
namespace
{
	//プレイヤーの初期位置
	constexpr Vector3 kPlayer1InitPos = { -200.0f,0.0f,0.0f };
	constexpr Vector3 kPlayer2InitPos = { 200.0f,0.0f,0.0f };
	//プレイヤーのスケール
	constexpr float kPlayer1Scale = 2.057f;
	constexpr float kPlayer2Scale = 0.252f;
	//プレイヤーの歩く速度
	constexpr float kPlayerWalkSpeed = 5.0f;
}

Player::Player(PlayerID id, int modelHandle, BattleManager* battleManager) :
	m_playerID(id),
	m_modelHandle(-1),
	m_battleManager(battleManager)
{
	//モデル複製
	m_modelHandle = MV1DuplicateModel(modelHandle);
	//アニメーションコントローラー生成
	m_animationController = std::make_unique<AnimationController>(m_modelHandle);
	//アニメーション登録
	m_animationController->AddAnimation(kIdleAnimName);
	m_animationController->AddAnimation(kWalkAnimName);
	//初期アニメーション再生
	m_animationController->Play(kIdleAnimName, true);

	//ステート初期化
	m_state = std::make_unique<PlayerIdleState>(*this);
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
	if (m_playerID == PlayerID::P1)
	{
		// 1Pの初期化処理
		m_transform.SetPosition(kPlayer1InitPos);
		m_transform.SetScale({ kPlayer1Scale, kPlayer1Scale, kPlayer1Scale });
		MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());
	}
	else
	{
		// 2Pの初期化処理
		m_transform.SetPosition(kPlayer2InitPos);
		m_transform.SetScale({ kPlayer2Scale, kPlayer2Scale, kPlayer2Scale });
		MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());
	}
}

void Player::End()
{
}

void Player::Update()
{
	assert(m_state);

	m_state->Update();

	OpponentRotate();

	ApplyDxLibMatrix();

	m_animationController->Update();

#ifdef _DEBUG
	auto& input = InputManager::GetInstance();
	StickDirection direction = input.GetStickDirection(m_playerID);

	const int x = 100 + static_cast<int>(m_playerID) * 200;
	const int y = 500;

	const int normalColor = GetColor(255, 255, 255);
	const int activeColor = GetColor(255, 0, 0);

	// 現在の方向と一致しているか
	auto GetColorByDirection =
		[&](StickDirection target)
		{
			return direction == target ? activeColor : normalColor;
		};

	// 上段
	DrawFormatString(
		x, y,
		GetColorByDirection(StickDirection::upLeft),
		L"↖"
	);

	DrawFormatString(
		x + 20, y,
		GetColorByDirection(StickDirection::up),
		L"↑"
	);

	DrawFormatString(
		x + 40, y,
		GetColorByDirection(StickDirection::upRight),
		L"↗"
	);

	// 中段
	DrawFormatString(
		x, y + 20,
		GetColorByDirection(StickDirection::left),
		L"←"
	);

	DrawFormatString(
		x + 20, y + 20,
		GetColorByDirection(StickDirection::neutral),
		L"○"
	);

	DrawFormatString(
		x + 40, y + 20,
		GetColorByDirection(StickDirection::right),
		L"→"
	);

	// 下段
	DrawFormatString(
		x, y + 40,
		GetColorByDirection(StickDirection::downLeft),
		L"↙"
	);

	DrawFormatString(
		x + 20, y + 40,
		GetColorByDirection(StickDirection::down),
		L"↓"
	);

	DrawFormatString(
		x + 40, y + 40,
		GetColorByDirection(StickDirection::downRight),
		L"↘"
	);

#endif // _DEBUG
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	//プレイヤーのポジションのデバッグ表示
	DrawFormatString(10, 50 + static_cast<int>(m_playerID) * 20, 0xffffff, L"Player %d Position: (%.2f, %.2f, %.2f)", static_cast<int>(m_playerID), m_transform.GetPosition().x, m_transform.GetPosition().y, m_transform.GetPosition().z);
#endif // _DEBUG

}

void Player::ChangeState(std::unique_ptr<PlayerStateBase> newState)
{
	if (m_state)
	{
		m_state->Exit();
	}

	m_state = std::move(newState);

	m_state->Enter();
}

void Player::OpponentRotate()
{
	m_transform.SetRotate(
		Quaternion::LookRotation(m_battleManager->GetOpponentDirection(m_playerID),
			Vector3::Up()) * Quaternion::AngleAxis(DX_PI_F, Vector3::Up()));
}

void Player::ApplyDxLibMatrix()
{
	MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());
}
