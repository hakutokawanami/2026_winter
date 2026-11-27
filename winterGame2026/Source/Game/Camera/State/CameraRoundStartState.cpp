#include "CameraRoundStartState.h"
#include"Game/Camera/BattleCamera.h"
#include"Game/Player/Player.h"

namespace
{
	//演出の長さ(フレーム)
	constexpr int kDuration = 120;
	//最初のターゲットの高さ
	constexpr float kStartTargetY = 500.0f;
}

CameraRoundStartState::CameraRoundStartState(BattleCamera& camera) :
	CameraStateBase(camera),
	m_frame(0)
{
}

void CameraRoundStartState::Enter()
{
	m_frame = 0;
}

void CameraRoundStartState::Update()
{
	m_frame++;

	Vector3 p1Pos = m_camera.GetPlayer1()->GetTransform().GetPosition();
	Vector3 p2Pos = m_camera.GetPlayer2()->GetTransform().GetPosition();
	//中央
	Vector3 center = (p1Pos + p2Pos) * 0.5f;
	//演出の時間で行う
	float rate = m_frame / kDuration;
	if (rate > 1.0f)
	{
		rate = 1.0f;
	}
	//演出中に動く距離
	const float kMoveLength = kStartTargetY - BattleCamera::kCameraHeightOffset;
	//高さを計算
	float y = kStartTargetY - kMoveLength * rate;
	//ターゲット
	Vector3 target = { center.x,y,center.z };
	//プレイヤー同士の方向
	Vector3 playerToPlayerDirection = p2Pos - p1Pos;
	//カメラの方向を計算
	Vector3 cameraOffsetDirection = playerToPlayerDirection.Cross(Vector3::Up()).Normalized();
	//カメラの距離を計算
	float cameraDistance = playerToPlayerDirection.Length() * BattleCamera::kDistanceRate;

	//カメラの距離の最小値と最大値を制限
	if (cameraDistance < BattleCamera::kCameraDistanceMin)
	{
		cameraDistance = BattleCamera::kCameraDistanceMin;
	}
	else if (cameraDistance > BattleCamera::kCameraDistanceMax)
	{
		cameraDistance = BattleCamera::kCameraDistanceMax;
	}

	//カメラの位置を計算
	
}

bool CameraRoundStartState::IsFinished() const
{
	return m_frame >= kDuration;
}
