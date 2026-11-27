#include "CameraRoundStartState.h"
#include"Game/Camera/BattleCamera.h"
#include"Game/Player/Player.h"

namespace
{
	//演出の長さ(フレーム)
	constexpr int kDuration = 120;
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

	//TODO:試合開始の演出(ステージを回り込んでから2人を映す など)
	//今は仮で通常のカメラと同じ位置
	Vector3 player1Pos = m_camera.GetPlayer1()->GetTransform().GetPosition();
	Vector3 player2Pos = m_camera.GetPlayer2()->GetTransform().GetPosition();
	m_camera.CalculateSideView(player1Pos, player2Pos);
}

bool CameraRoundStartState::IsFinished() const
{
	return m_frame >= kDuration;
}
