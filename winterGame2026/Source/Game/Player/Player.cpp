#include "Player.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Input/InputManager.h"
#include"Game/Battle/BattleManager.h"
#include<DxLib.h>
namespace
{
	//プレイヤーの初期位置
	constexpr Vector3 kPlayer1InitPos = { -200.0f,0.0f,0.0f };
	constexpr Vector3 kPlayer2InitPos = { 200.0f,0.0f,0.0f };
	//プレイヤーのスケール
	constexpr float kPlayer1Scale = 2.057f;
	constexpr float kPlayer2Scale = 0.222f;
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
	auto& input = InputManager::GetInstance();

	Vector2 leftStickInput = input.GetLeftStick(m_playerID);

	Vector3 moveDirection = m_battleManager->GetRightDirection() * leftStickInput.x;

	moveDirection += m_battleManager->GetForwardDirection() * leftStickInput.y;

	m_transform.Translate(moveDirection * kPlayerWalkSpeed);

	OpponentRotate();

	ApplyDxLibMatrix();

	m_animationController->Update();

#ifdef _DEBUG
	if (input.IsTriggered(m_playerID, "A"))
	{
		// Aボタンが押されたときの処理
		printfDx(L"Player %d: A button triggered\n", static_cast<int>(m_playerID) + 1);
	}

	DrawFormatString(10, 210 + static_cast<int>(m_playerID) * 20, 0xffffff, L"Player %d Left Stick: (%.2f, %.2f)", static_cast<int>(m_playerID) + 1, leftStickInput.x, leftStickInput.y);

	DrawFormatString(10, 250 + static_cast<int>(m_playerID) * 20, 0xffffff, L"Player %d Move Direction: (%.2f, %.2f, %.2f)", static_cast<int>(m_playerID) + 1, moveDirection.x, moveDirection.y, moveDirection.z);

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
