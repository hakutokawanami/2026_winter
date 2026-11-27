#include "CameraThrowState.h"
#include"Game/Camera/BattleCamera.h"
#include"Game/Player/Player.h"

CameraThrowState::CameraThrowState(BattleCamera& camera) :
	CameraStateBase(camera)
{
}

void CameraThrowState::Update()
{
	//今は通常のカメラと同じ計算
	//(投げ専用の動きを付けるときは、ここでm_thrownPlayerを使って位置を補正する)
	Vector3 player1Pos = m_camera.GetPlayer1()->GetTransform().GetPosition();
	Vector3 player2Pos = m_camera.GetPlayer2()->GetTransform().GetPosition();
	m_camera.CalculateSideView(player1Pos, player2Pos);
}
