#include "BattleCamera.h"
#include "Game/Player/Player.h"
#include<cmath>

namespace
{
	// カメラの高さ
	constexpr float kCameraHeightOffset = 100.0f; 
	// カメラのオフセット
	constexpr float kCameraDistanceOffset = 3000.0f; 
}

BattleCamera::BattleCamera():
	m_cameraState(CameraState::Battle)
{
}

BattleCamera::~BattleCamera()
{
}

void BattleCamera::Init()
{
}

void BattleCamera::Init(std::shared_ptr<Player>player1, std::shared_ptr<Player>player2)
{
	m_player1 = player1;
	m_player2 = player2;

	CalculateCameraPositionAndTarget();
	SetCameraPositionAndTarget();
	Apply();
}

void BattleCamera::Update()
{
	CalculateCameraPositionAndTarget();
	LerpCamera();
	Apply();

#ifdef _DEBUG
	// デバッグ用にカメラの位置とターゲットを表示
	DrawFormatString(10, 10, 0xffffff, L"Camera Position: (%.2f, %.2f, %.2f)", m_position.x, m_position.y, m_position.z);
	DrawFormatString(10, 30, 0xffffff, L"Camera Target: (%.2f, %.2f, %.2f)", m_target.x, m_target.y, m_target.z);
#endif // _DEBUG

}

void BattleCamera::SetPlayers(std::shared_ptr<Player>player1, std::shared_ptr<Player>player2)
{
	m_player1 = player1;
	m_player2 = player2;
}

void BattleCamera::CalculateCameraPositionAndTarget()
{
	// プレイヤーの位置を取得してカメラのターゲットを設定
	Vector3 player1Pos = m_player1.lock()->GetTransform().GetPosition();
	Vector3 player2Pos = m_player2.lock()->GetTransform().GetPosition();
	m_calculatedTarget = (player1Pos + player2Pos) * 0.5f;
	//プレイヤー間の距離を計算
	Vector3 playerToPlayerDirection = player2Pos - player1Pos;
	playerToPlayerDirection.y = 0.0f; // 水平方向の距離のみを考慮
	//カメラの方向を計算
	Vector3 cameraOffsetDirection = playerToPlayerDirection.Cross(Vector3::Up()).Normalized();
	//カメラの距離を計算
	m_calculatedPosition = m_calculatedTarget - cameraOffsetDirection * kCameraDistanceOffset;
	//カメラの高さを設定
	m_calculatedTarget.y = kCameraHeightOffset;
	m_calculatedPosition.y = kCameraHeightOffset;
}

void BattleCamera::LerpCamera()
{
	//lerpでカメラの位置とターゲットを滑らかに更新
	m_target.x = std::lerp(m_target.x, m_calculatedTarget.x, 0.1f);
	m_target.y = std::lerp(m_target.y, m_calculatedTarget.y, 0.1f);
	m_target.z = std::lerp(m_target.z, m_calculatedTarget.z, 0.1f);
	m_position.x = std::lerp(m_position.x, m_calculatedPosition.x, 0.1f);
	m_position.y = std::lerp(m_position.y, m_calculatedPosition.y, 0.1f);
	m_position.z = std::lerp(m_position.z, m_calculatedPosition.z, 0.1f);
}

void BattleCamera::SetCameraPositionAndTarget()
{
	m_position = m_calculatedPosition;
	m_target = m_calculatedTarget;
}

void BattleCamera::Apply()
{
	// カメラの位置とターゲットをDxLibに設定
	SetCameraPositionAndTarget_UpVecY(m_position, m_target);
}
