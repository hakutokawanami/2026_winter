#pragma once
#include"CameraStateBase.h"

/// <summary>
/// 通常の対戦中のカメラ
/// </summary>
class CameraBattleState : public CameraStateBase
{
public:
	CameraBattleState(BattleCamera& camera);

	void Update()override;
};
