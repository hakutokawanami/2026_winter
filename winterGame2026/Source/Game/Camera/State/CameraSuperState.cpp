#include "CameraSuperState.h"
#include"Game/Camera/BattleCamera.h"
#include"Game/Player/Player.h"

namespace
{
	//演出の長さ(フレーム)
	constexpr int kDuration = 60;
}

CameraSuperState::CameraSuperState(BattleCamera& camera, Player* user) :
	CameraStateBase(camera),
	m_user(user),
	m_frame(0)
{
}

void CameraSuperState::Enter()
{
	m_frame = 0;
}

void CameraSuperState::Update()
{
	m_frame++;

	//TODO:超必殺技の演出(m_userの顔に寄る など)
	//今は仮で通常のカメラと同じ位置
	Vector3 player1Pos = m_camera.GetPlayer1()->GetTransform().GetPosition();
	Vector3 player2Pos = m_camera.GetPlayer2()->GetTransform().GetPosition();
	m_camera.CalculateSideView(player1Pos, player2Pos);
}

bool CameraSuperState::IsFinished() const
{
	return m_frame >= kDuration;
}
