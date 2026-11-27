#include "CameraKOState.h"
#include"Game/Camera/BattleCamera.h"
#include"Game/Player/Player.h"

namespace
{
	//演出の長さ(フレーム)
	constexpr int kDuration = 90;
}

CameraKOState::CameraKOState(BattleCamera& camera, Player* loser) :
	CameraStateBase(camera),
	m_loser(loser),
	m_frame(0)
{
}

void CameraKOState::Enter()
{
	m_frame = 0;
}

void CameraKOState::Update()
{
	m_frame++;

	//TODO:KOの演出(m_loserに寄る など)
	//今は仮で通常のカメラと同じ位置
	Vector3 player1Pos = m_camera.GetPlayer1()->GetTransform().GetPosition();
	Vector3 player2Pos = m_camera.GetPlayer2()->GetTransform().GetPosition();
	m_camera.CalculateSideView(player1Pos, player2Pos);
}

bool CameraKOState::IsFinished() const
{
	return m_frame >= kDuration;
}
