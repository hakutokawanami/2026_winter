#include "CameraBattleState.h"
#include"Game/Camera/BattleCamera.h"
#include"Game/Player/Player.h"

CameraBattleState::CameraBattleState(BattleCamera& camera) :
	CameraStateBase(camera)
{
}

void CameraBattleState::Update()
{
	//2人の位置から、横から映す位置と注視点を計算する
	Vector3 player1Pos = m_camera.GetPlayer1()->GetTransform().GetPosition();
	Vector3 player2Pos = m_camera.GetPlayer2()->GetTransform().GetPosition();
	m_camera.CalculateSideView(player1Pos, player2Pos);
}
