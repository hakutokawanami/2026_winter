#include "CameraWinState.h"
#include"Game/Camera/BattleCamera.h"
#include"Game/Player/Player.h"

CameraWinState::CameraWinState(BattleCamera& camera, Player* winner) :
	CameraStateBase(camera),
	m_winner(winner),
	m_frame(0)
{
}

void CameraWinState::Enter()
{
	m_frame = 0;
}

void CameraWinState::Update()
{
	m_frame++;

	//TODO:勝利の演出(m_winnerを正面から映す など)
	//今は仮で通常のカメラと同じ位置
	Vector3 player1Pos = m_camera.GetPlayer1()->GetTransform().GetPosition();
	Vector3 player2Pos = m_camera.GetPlayer2()->GetTransform().GetPosition();
	m_camera.CalculateSideView(player1Pos, player2Pos);
}
