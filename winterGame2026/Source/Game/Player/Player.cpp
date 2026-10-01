#include "Player.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Input/InputManager.h"
#include<DxLib.h>
namespace
{
	//プレイヤーの初期位置
	constexpr Vector3 kPlayer1InitPos = { -100.0f,0.0f,0.0f };
	constexpr Vector3 kPlayer2InitPos = { 100.0f,0.0f,0.0f };
}

Player::Player(PlayerID id, int modelHandle):
	m_playerID(id),
	m_modelHandle(-1)
{
	//モデル複製
	m_modelHandle = MV1DuplicateModel(modelHandle);
	//アニメーションコントローラー生成
//	m_animationController = std::make_unique<AnimationController>(m_modelHandle);
	//アニメーション登録
//	m_animationController->AddAnimation(PlayerAnim::idle);
//	m_animationController->AddAnimation(PlayerAnim::walk);
	//初期アニメーション再生
//	m_animationController->Play(PlayerAnim::idle, true, 15.0f);
}

Player::~Player()
{
}

void Player::Init()
{
	if(m_playerID == PlayerID::P1)
	{
		// 1Pの初期化処理
	//	m_transform.SetPosition(kPlayer1InitPos);
	//	m_transform.SetRotate(Quaternion::AngleAxis(0.0f, Vector3::Up()));
	//	MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());
	}
	else
	{
		// 2Pの初期化処理
		m_transform.SetPosition(kPlayer2InitPos);
		m_transform.SetRotate(Quaternion::AngleAxis(180.0f, Vector3::Up()));
		m_transform.SetScale({0.2f, 0.2f, 0.2f});
		MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());
	}
}

void Player::End()
{
}

void Player::Update()
{
	auto& input = InputManager::GetInstance();
	
//	m_animationController->Update();


#ifdef _DEBUG
	if (input.IsTriggered(m_playerID, "A"))
	{
		// Aボタンが押されたときの処理
		printfDx(L"Player %d: A button pressed\n", static_cast<int>(m_playerID));
	}
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
