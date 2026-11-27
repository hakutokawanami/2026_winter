#include "BattleCamera.h"
#include "Game/Player/Player.h"
#include "State/CameraStateBase.h"
#include "State/CameraBattleState.h"
#include "State/CameraThrowState.h"
#include<cmath>

BattleCamera::BattleCamera():
	m_player1(nullptr),
	m_player2(nullptr),
	m_shakePower(0.0f),
	m_shakeTime(0),
	m_shakeOffset({})
{
	m_state = std::make_unique<CameraBattleState>(*this);
}

BattleCamera::~BattleCamera()
{
}

void BattleCamera::Init()
{
}

void BattleCamera::Init(Player* player1, Player*player2)
{
	m_player1 = player1;
	m_player2 = player2;

	m_state->Update();
	SetCameraPositionAndTarget();
	Apply();
}

void BattleCamera::Update()
{
	//ステートで行きたい位置と注視点を計算する
	m_state->Update();
	ShakeUpdate();
	LerpCamera();
	Apply();

#ifdef _DEBUG
	// デバッグ用にカメラの位置とターゲットを表示
	DrawFormatString(10, 400, 0xffffff, L"Camera Position: (%.2f, %.2f, %.2f)", m_position.x, m_position.y, m_position.z);
	DrawFormatString(10, 420, 0xffffff, L"Camera Target: (%.2f, %.2f, %.2f)", m_target.x, m_target.y, m_target.z);
#endif // _DEBUG

}

void BattleCamera::SetPlayers(Player* player1, Player* player2)
{
	m_player1 = player1;
	m_player2 = player2;
}

void BattleCamera::ChangeState(std::unique_ptr<CameraStateBase> newState)
{
	if (m_state)
	{
		m_state->Exit();
	}

	m_state = std::move(newState);

	m_state->Enter();
}

void BattleCamera::CalculateSideView(const Vector3& player1Pos, const Vector3& player2Pos)
{
	// プレイヤーの位置からカメラのターゲットを設定
	m_calculatedTarget = (player1Pos + player2Pos) * 0.5f;
	//プレイヤー間の距離を計算
	Vector3 playerToPlayerDirection = player2Pos - player1Pos;
	playerToPlayerDirection.y = 0.0f; // 水平方向の距離のみを考慮
	//カメラの方向を計算
	Vector3 cameraOffsetDirection = playerToPlayerDirection.Cross(Vector3::Up()).Normalized();
	//カメラの距離を計算
	float cameraDistance = playerToPlayerDirection.Length() * kDistanceRate;
	//カメラの距離の最小値と最大値を制限
	if (cameraDistance < kCameraDistanceMin)
	{
		cameraDistance = kCameraDistanceMin;
	}
	else if (cameraDistance > kCameraDistanceMax)
	{
		cameraDistance = kCameraDistanceMax;
	}
	//カメラの位置を計算
	m_calculatedPosition = m_calculatedTarget - cameraOffsetDirection * cameraDistance;
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
	SetCameraPositionAndTarget_UpVecY(m_position + m_shakeOffset, m_target + m_shakeOffset);
	// カメラの向きに合わせて、毎フレーム（BattleCamera の更新のあとなど）
	Vector3 dir = m_target - m_position;
	// 少し上から当てる
	dir.y -= dir.Length() / 2;
	SetLightDirection(VNorm(dir));
}

void BattleCamera::ShakeStart(float power, int time)
{
	m_shakePower = power;
	m_shakeTime = time;
}

void BattleCamera::ShakeUpdate()
{
	if (m_shakeTime > 0)
	{
		m_shakeTime--;
		m_shakeOffset.x = (rand() / (float)RAND_MAX - 0.5f) * 2.0f * m_shakePower;
		m_shakeOffset.y = (rand() / (float)RAND_MAX - 0.5f) * 2.0f * m_shakePower;
		m_shakeOffset.z = (rand() / (float)RAND_MAX - 0.5f) * 2.0f * m_shakePower;
	}
	else
	{
		m_shakeOffset = {};
	}
}
